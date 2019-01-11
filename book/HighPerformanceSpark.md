# High Performance Spark,高性能Spark

[原书地址](http://shop.oreilly.com/product/0636920046967.do)o


## Chapter 2,How Spark Works


* 什么是**wide transformations** 和 **narrow transformations**
  * 区别在于transformations使用到的分区的依赖复杂性，如果分区之间依赖很复杂，就是wide transformations。这种transformations需要使用多个分区的数据，如果分区分散在不同的executor上，就会引发executor之间的数据交换，即shuffle。

* 什么是**shuffle**，由什么引起
  * 不同executor之间的大规模数据交换

* 执行实体的概念图是什么样？Driver、Cluster Manager、Worker Node、Executor、Partition的概念关系如何？
  * 有一张概念图，在page 21。

* Spark application、Job、Stage、Task、Executor之间的关系？
  * Application会有多个output操作，每个output操作之前有一系列transformations和actions。一个output操作会被规划为一个Job。
  * Job内部分为多个stage，stage内的transformation都是一些narrow transformations，如果出现wide transformations，会新开启一个stage。
  * 每个stage内都是一组相对快的transformations，因为它们不需要去其他executor拿数据。
  * stage将transformations分割成task，每个task具有固定的运算逻辑，作用于各个partition上。

* 如何计算Spark application最多可以并行运行多少个task？
  * MaxRunningTaskSize= ExecutorSize * ExecutorCoreNum
  * Spark把这个最大的task数值称为Slot Number。
  * 每个executor可以分配多个core（理解为CPU）。如果具有太多的分区，每个分区都需要被一个task计算，name总的task数量也会变多。如果task数量大于Slots Number，剩余的Task会陷入等待，等待被分配slot去执行。

















## 最具有代表性的问题


* join操作的本质是什么