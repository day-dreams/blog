# Designing Data-Intensive Applications

《设计数据密集型应用》读书笔记。摘录一些经典的概念和问题，不做详细的记录，就当时一份提纲吧。


## Chapter 2,Data Models and Query Languages

* 

* ORM的定位是什么，它是不是一个尴尬的技术呢？

* ActiveRecord的核心思想是什么？
    * [Active Record Basics](https://guides.rubyonrails.org/active_record_basics.html)
* 除了ActiveRecord，还有哪些设计模式用于实现ORM？

* Hibernate是什么?
    * [Hibernate Getting Started Guide](http://docs.jboss.org/hibernate/orm/5.3/quickstart/html_single/)

* Java代码中常见的Bean是什么？
    * [StackOverflow:What is a JavaBean exactly?](https://stackoverflow.com/questions/3295496/what-is-a-javabean-exactly)

* 在join查询方面，关系型数据库和文档型数据库有何不同？

* 文档型数据库适合什么样的数据？

<!-- 易于表示一对多关系;自解释，彼此关系弱，不需要频繁互相引用、参考的数据  -->

* 文档型数据库的有哪些缺点？

<!-- 数据有可能重复，比如多个对象具有相同的属性，字段值相同;一般不支持事务;不方便join-->

* Schema-on-reade和Schema-on-write的区别？

    * [To Schema On Read or to Schema On Write, That is the Hadoop Data Lake Question](https://adaptivesystemsinc.com/blog/to-schema-on-read-or-to-schema-on-write-that-is-the-hadoop-data-lake-question/)
    * [Schema On Read vs. Schema On Write Explained](https://www.thomashenson.com/schema-read-vs-schema-write-explained/)

* MySQL执行Alter Table执行原理是什么样的？为什么要复制整张表？
