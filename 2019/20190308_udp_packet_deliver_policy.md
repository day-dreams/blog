# Linux内核：UDP报文的分发策略

- [Linux内核：UDP报文的分发策略](#linux内核udp报文的分发策略)
  - [前言](#前言)
  - [现象：socket收到额外的数据包](#现象socket收到额外的数据包)
  - [回顾：DNS报文如何从网卡分发到进程](#回顾dns报文如何从网卡分发到进程)
  - [探究：Linux 2.6.11.1的UDP包分发策略](#探究linux-26111的udp包分发策略)
  - [探究：Linux 4.4.0的UDP包分发策略](#探究linux-440的udp包分发策略)
  - [分发策略的本质](#分发策略的本质)
  - [结论：udp socket为什么会收到额外的数据包](#结论udp-socket为什么会收到额外的数据包)


## 前言

我本科在实验室里负责DNS探测相关工作，曾高度依赖`select()`调用，来提高DNS探测效率。我发现一个很有趣的现象：一个udp socket，发送100个DNS查询包，有可能接收到**超过**100个DNS响应包。

这个问题曾给我造成很大的困扰，这次打算好好研究一番，要弄清楚：UDP报文有着怎样的分发策略，又是如何产生上面那个问题？

一方面，这个问题和代码上下文强相关；另一方面，这个问题直接指向Linux内核细节，所以很难在google上找到满意的解释。最佳的解决办法是：阅读源代码 + 重复实验。

## 现象：socket收到额外的数据包

在我的DNS探测逻辑中，关键步骤如下：
```
1. 建立一个udp socket
2. 遍历查询集合
   2.1. 组DNS查询报文
   2.2. 调用sendto()发送到目的地址
   2.3. 回到2.1
3. 设定一个超时时间，调用select()，陷入阻塞
   3.1. 如果在select超时时间内，socket没有发生可读事件，认为所有DNS报文已经到达，进入4
   3.2. 如果有可读事件发生，则读取报文，翻入报文暂存缓冲区
4. 关闭socket，处理报文暂存缓冲区，执行业务逻辑
```

按照我之前的一般设想，发送一个DNS查询，要么收到一个响应，要么没有，不可能收到两个。但就是有些DNS服务器不同寻常，比如`www.hitwh.edu.cn`的NS服务器，一边向它发A记录查询，一边用tcpdump抓包，经常能看到这样的输出：
```
$ sudo tcpdump  port 53 and host 222.194.15.6
15:45:01.709691 IP 10.245.146.42.43642 > dns1.hitwh.edu.cn.domain: 22908+ A? www.hitwh.edu.cn. (34)
15:45:01.711434 IP dns1.hitwh.edu.cn.domain > 10.245.146.42.43642: 22908* 2/2/2 CNAME www1.hitwh.edu.cn., A 202.102.144.56 (139)
15:45:01.711471 IP dns1.hitwh.edu.cn.domain > 10.245.146.42.43642: 22908* 2/2/2 CNAME www1.hitwh.edu.cn., A 202.102.144.56 (139)
15:45:02.714208 IP 10.245.146.42.39882 > dns1.hitwh.edu.cn.domain: 29711+ A? www.hitwh.edu.cn. (34)
15:45:02.715881 IP dns1.hitwh.edu.cn.domain > 10.245.146.42.39882: 29711* 2/2/2 CNAME www1.hitwh.edu.cn., A 202.102.144.56 (139)
15:45:02.716010 IP dns1.hitwh.edu.cn.domain > 10.245.146.42.39882: 29711* 2/2/2 CNAME www1.hitwh.edu.cn., A 202.102.144.56 (139)
```

没错，这个DNS给一个查询回了两次响应，并且两次响应没有任何差别，连端口都是一样的。我不知道RFC对这种现象有没有约束条款，但我认为是不符合情理的。这种类型的DNS，会直接引发我的DNS探测逻辑出现异常：响应包数量大于请求包。

试想：
* 第一次探测，使用一个socket发送1000个包，由于不可预期的网络拥塞，只收到990个响应包
* 第二次探测，使用新的socket，又发送1000个包，由于网络环境突然变好（或者DNS服务器给某些请求发送了多个响应包），收到了1010个响应包

这真是让人头大。不过我们可以推测出一个结论：上述例子中，对于DNS包的分发，这两个socket似乎没有什么实质性差别；并且，第一次发出去查询，其响应可能被第二次探测使用的socket接收到了。这是为什么呢？

## 回顾：DNS报文如何从网卡分发到进程

TCP/IP协议栈告诉我们：链路层负责相邻设备之间的定位，网络层负责远程主机之间的定位，传输层负责远程主机之间的进程定位。

Linux内核源码告诉我们，进程使用socket fd来操作`struct sock`，进而操作网络协议栈进行收发报文，

我们遇到的问题在于传输层：UDP给socket分发的报文数量不太对，可能出现把一个socket的报文分发给另一个socket的现象。

根据美团点评的技术博客：[Redis 高负载下的中断优化](https://tech.meituan.com/2018/03/16/redis-high-concurrency-optimization.html)，报文经过硬中断和软中断，最终被`netif_receive_skb`传递给网络层和传输层协议。其中，传输层(这里是UDP)通过socket哈希表查询，把报文分发到socket。

经过软中断后，报文形成了`struct sk_buff`结构体，udp协议栈开始根据报文的源地址和目的地址，为它寻找对应的socket，

我们需要直接去看相应的内核源码(大概在net/ipv4/udp.c中)，来分析UDP的分发策略。考虑到内核版本问题，我们选择2.6.11（相关资料非常多）和 4.4.0（ubuntu 16.04的内核版本）进行分析。

## 探究：Linux 2.6.11.1的UDP包分发策略


这个版本的分发策略是基于打分的：根据`源地址`、`目的地址`、`源端口`、`目的端口`、`目的网卡`五项指标，udp协议为每个socket打分，socket每有一项指标和报文符合就得到一定分值，如果某个socket达到了9分，也就是5项全中，就由这个socket去处理这份报文。

如果没有任何一个socket能够5项全中呢？那就挑选得分最高的socket去处理；如果所有socket得分都为0(所有指标都不匹配)，那么没有socket能够获得报文，也就是udp报文错误。


[linux 2.6.11源码,net/ipv4/udp.c](https://elixir.bootlin.com/linux/v2.6.11/source/net/ipv4/udp.c#L222)
```c
/* UDP is nearly always wildcards out the wazoo, it makes no sense to try
 * harder than this. -DaveM
 */
 static struct sock* udp_v4_lookup_longway(u32 saddr, u16 sport,u32 daddr, u16 dport, int dif) { 
	struct sock *sk, *result = NULL;
	struct hlist_node *node;
	unsigned short hnum = ntohs(dport);
	int badness = -1;

	sk_for_each(sk, node, &udp_hash[hnum & (UDP_HTABLE_SIZE - 1)]) {
		struct inet_sock *inet = inet_sk(sk);

		if (inet->num == hnum && !ipv6_only_sock(sk)) {
			int score = (sk->sk_family == PF_INET ? 1 : 0);
			if (inet->rcv_saddr) {
				if (inet->rcv_saddr != daddr)
					continue;
				score+=2;
			}
			if (inet->daddr) {
				if (inet->daddr != saddr)
					continue;
				score+=2;
			}
			if (inet->dport) {
				if (inet->dport != sport)
					continue;
				score+=2;
			}
			if (sk->sk_bound_dev_if) {
				if (sk->sk_bound_dev_if != dif)
					continue;
				score+=2;
			}
			if(score == 9) {
				result = sk;
				break;
			} else if(score > badness) {
				result = sk;
				badness = score;
			}
		}
	}
	return result;
}

```


## 探究：Linux 4.4.0的UDP包分发策略

4.16.0版本的分发策略还是基于打分，不过比2.6.11更复杂：
* 网络接口指标
  * 引入主从网络接口（这个特性暂时没有不做研究）
  * 不仅仅考虑报文的目标接口，还考虑从目标接口
* udp socket哈希表
  * 不同于2.6.11，4.16.0里的udp哈希表，有两个哈希槽
  * 查询时需要兼顾两个槽
  * 简要概括一下，如果主槽里的sock不超过10，那么副槽一定没有sock，只用查主槽；如果主槽sock大于10，那么先查sock相对少的那个槽，再去查相对多的槽
  * 查询hash表时，仍然是遍历打分取得分最高
* SO_REUSEPORT
  * 4.16.0考虑了这个SO_REUSEPORT选项
  * 如果设置了这个选项，可能存在多个socket使用同一个端口，它们的得分相同
  * 为了避免总是在哈希表中位置靠前的那个socket获得报文，内核根据某种策略计算hash值，尽可能公平地选取一个socket

(ps: 有一行注释，跨越2个大版本还在内核里，这大概是内核开发者们的永垂不朽？)

[linux 4.6.0源码,net/ipv4/udp.c](https://elixir.bootlin.com/linux/v4.16/source/net/ipv4/udp.c#L469)

```c
/* UDP is nearly always wildcards out the wazoo, it makes no sense to try
 * harder than this. -DaveM
 */
struct sock *__udp4_lib_lookup(struct net *net, __be32 saddr,
		__be16 sport, __be32 daddr, __be16 dport, int dif,
		int sdif, struct udp_table *udptable, struct sk_buff *skb)
{
	struct sock *sk, *result;
	unsigned short hnum = ntohs(dport);
	unsigned int hash2, slot2, slot = udp_hashfn(net, hnum, udptable->mask);
	struct udp_hslot *hslot2, *hslot = &udptable->hash[slot];
	bool exact_dif = udp_lib_exact_dif_match(net, skb);
	int score, badness;
	u32 hash = 0;

	if (hslot->count > 10) {
		hash2 = ipv4_portaddr_hash(net, daddr, hnum);
		slot2 = hash2 & udptable->mask;
		hslot2 = &udptable->hash2[slot2];
		if (hslot->count < hslot2->count)
			goto begin;

		result = udp4_lib_lookup2(net, saddr, sport,
					  daddr, hnum, dif, sdif,
					  exact_dif, hslot2, skb);
		if (!result) {
			unsigned int old_slot2 = slot2;
			hash2 = ipv4_portaddr_hash(net, htonl(INADDR_ANY), hnum);
			slot2 = hash2 & udptable->mask;
			/* avoid searching the same slot again. */
			if (unlikely(slot2 == old_slot2))
				return result;

			hslot2 = &udptable->hash2[slot2];
			if (hslot->count < hslot2->count)
				goto begin;

			result = udp4_lib_lookup2(net, saddr, sport,
						  daddr, hnum, dif, sdif,
						  exact_dif, hslot2, skb);
		}
		return result;
	}
begin:
	result = NULL;
	badness = 0;
	sk_for_each_rcu(sk, &hslot->head) {
		score = compute_score(sk, net, saddr, sport,
				      daddr, hnum, dif, sdif, exact_dif);
		if (score > badness) {
			if (sk->sk_reuseport) {
				hash = udp_ehashfn(net, daddr, hnum,
						   saddr, sport);
				result = reuseport_select_sock(sk, hash, skb,
							sizeof(struct udphdr));
				if (result)
					return result;
			}
			result = sk;
			badness = score;
		}
	}
	return result;
}
```

## 分发策略的本质

可以看到，经历了2个大版本，内核对于UDP报文分发，采用的策略大致相同，其本质是：基于端口、地址、网卡进行打分，选契合度最高的socket进行报文分发。不同于机器学习各种天花乱坠的拟合方法，这种加权打分显得格外朴素。

想起当初一个面试官问我的问题：内核中epoll为什么使用基树作为存储结构，而不是hash表。在udp协议栈，其存储sock的结构又成了hash表。主要是查询时间和内存空间开销的取舍：
* udp协议栈属于系统层次，需要频繁搜索sock，并且整个系统的udp sock数量有限，所以使用hash表是最佳选择
* epoll属于进程层次，使用频次没有udp协议栈高，但epoll管理的sock总数胜于udp协议栈，更加注重空间开销而不是时间，所以选用了树结构。

## 结论：udp socket为什么会收到额外的数据包

首先，这个“额外”并不准确。UDP并没有“一发一收”的说法，所以不算是额外的数据包。

其次，udp报文的分发策略只取决于地址、端口、网卡，至于报文是否被分发到相同的socket实例或者是进程，这不是udp协议栈关心的重点。

最后，即使有SO_REUSEPORT的影响，UDP协议栈只会把报文分发到一个socket，而不是多个。

UDP报文的分发策略也说明：不管bind9重启多少次，只要监听的地址端口相同，当前的bind9实例，都有可能收到“本应发给先代实例”的报文.

