# 我的PHP7最佳实践


* 异常处理
  * 如果可以，直接使用Excetion，不要return error code

* 减少foreach操作
  * 使用array_filter、array_map等操作来实现，代码会更简洁