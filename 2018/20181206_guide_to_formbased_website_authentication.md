# 【译】Form-based authentication for websites

stackoverflow上有一篇关于网站表单认证的guide，觉得很有用，这里翻译一下。

[原文](https://stackoverflow.com/questions/549/the-definitive-guide-to-form-based-website-authentication)


## Part 1，如何登陆

* 直接使用HTTPS
    *   就算不愿意维护证书，也不应该自己实现加密机制，应该强制使用HTTPS。
    *   自己实现加密机制看起来很厉害，也很“合理”，但仍然依赖于安全的信道连接，否则还是会面临中间人攻击的危险。


* 永远不要明文存储密码，使用Salt+Hash
    * 如果很多用户使用同一个密码，并且其中一个用户被hack了，系统会直接面临类似装库的危险。
    * 可以为每个用户生成一个Salt值，并且应该保持变化。

* 尽可能把Session数据存在server，不要直接给用户，并且开启HTTPOnly和Secure选项
    * 只给用户一个session key，尽量减小数据长度对报文传输的影响
    * Secure选项限制cookie只能用在HTTPS连接中
    * HTTPOnly选项限制cookie只能用在HTML中，从而避免XSS攻击（跨站脚本被执行，脚本里窃取cookie）

## Part 2，登录态维护

* 不要使用持久性登录态