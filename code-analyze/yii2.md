# yii2框架源码剖析

基于2.0.15.

[注释版代码](https://github.com/day-dreams/yii2/tree/mydoc-based-on-2.0.15)

* web模式下，action运行大致流程
  * 框架初始化web/Application对象
  * 调用Application->run()
  * 创建web/Request对象
  * 调用Application->handleRequest()
  * 使用UrlManager去解析url（这里很依赖正则表达式），获得`标准路由字符串`和`相关参数`
  * 调用Application->runAction
  * 创建`Contoller`和`Action`
  * 通过`call_user_func_array()`，调用具体的action函数，返回action函数中的result
  * 组装web/Response对象，调用web/Response->send()，向连接中写HTTP头部和body
  * 从Application->run()返回，入口脚本(项目代码下的`web/index.php`)执行完毕
  * 完事儿，整体来看没有特别的技术细节，只是一系列concept的依次调用