# PHP5 tips摘录

好记性不如烂笔头。

- [PHP5 tips摘录](#php5-tips%E6%91%98%E5%BD%95)
    - [detail](#detail)
        - [类型约定type hint](#%E7%B1%BB%E5%9E%8B%E7%BA%A6%E5%AE%9Atype-hint)
        - [类型运算符instanceof](#%E7%B1%BB%E5%9E%8B%E8%BF%90%E7%AE%97%E7%AC%A6instanceof)
        - [静态延迟绑定](#%E9%9D%99%E6%80%81%E5%BB%B6%E8%BF%9F%E7%BB%91%E5%AE%9A)
        - [array_map和array_filter](#arraymap%E5%92%8Carrayfilter)
        - [批量赋值list()](#%E6%89%B9%E9%87%8F%E8%B5%8B%E5%80%BClist)
        - [PHP也存在深浅拷贝的问题](#php%E4%B9%9F%E5%AD%98%E5%9C%A8%E6%B7%B1%E6%B5%85%E6%8B%B7%E8%B4%9D%E7%9A%84%E9%97%AE%E9%A2%98)

## detail

### 类型约定type hint

PHP可以指定函数参数的类型
```php
 function smoke(Man $man){
         echo "man is smoking";
 }   
```

### 类型运算符instanceof

PHP是个很强的动态语言，对参数类型要求很弱。但还是提供了相关机制，满足我们队程序的静态要求，比如typehint和instanceof。

* instanceof 用于确定一个 PHP 变量是否属于某一类 class 的实例：

```php
class Something{

}

class Somethingelse{

}

$x=new Something;

var_dump($x instanceof Somethingelse);//false
var_dump($x instanceof Something);//true
```

### 静态延迟绑定

长久以来，self代表当前解析上下文中的类，this代表当前运行上下文的对象。如果我们有这样的需求，是不能满足的,因为在上下文中self代表BaseObject类：
```php
abstract class BaseObject{
    public static function create(){
        return new self();
    }
}

class A extends BaseObject{

}
class B extends BaseObject{

}

//PHP Fatal error:  Cannot instantiate abstract class BaseObject
new B();
```

如果要实现这种需求，可以使用static进行运行时动态绑定，即多态：

```php
abstract class BaseObject{
    public static function create(){
        return new static();
    }
}

class A extends BaseObject{

}
class B extends BaseObject{

}

//No Error
new B();
```

### array_map和array_filter

二者在array处理上有奇效。

```php
$a = array(1, 2, 3, 4, 5);

//输出偶数
print_r(array_filter($a, function ($item) {
    if ($item % 2 == 0) return true;
    else return false;
}));

//输出偶数；如果是奇数，转换后再输出
print_r(array_map(function ($item) {
    if ($item % 2 != 0) return $item * 2;
    else return $item;
}, $a));

```


### 批量赋值list()

一次给多个变量赋值。

```php
$num=array(1,2,3,5);

list($a,$b,,$c)=$num;

echo $a,$b,$c;

```

### PHP也存在深浅拷贝的问题

在 PHP 5，一个对象变量已经不再保存整个对象的值。只是保存一个标识符来访问真正的对象内容。 当对象作为参数传递，作为结果返回，或者赋值给另外一个变量，另外一个变量跟原来的不是引用的关系，他们具有相同的值，这个值是一个标识符，指向一个对象。简单来讲，就是指针。

即使是clone关键字，也受限于这个特性。所以如果要实现深拷贝，需要自己去实现__clone()拦截器方法，通过new操作符分配额外的对象。