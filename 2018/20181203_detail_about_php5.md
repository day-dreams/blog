# PHP5的tip摘录

好记性不如烂笔头。

- [PHP5的tip摘录](#php5%E7%9A%84tip%E6%91%98%E5%BD%95)
    - [detail](#detail)
        - [type hint](#type-hint)
        - [静态延迟绑定](#%E9%9D%99%E6%80%81%E5%BB%B6%E8%BF%9F%E7%BB%91%E5%AE%9A)
        - [array_map和array_filter](#arraymap%E5%92%8Carrayfilter)
        - [批量赋值list()](#%E6%89%B9%E9%87%8F%E8%B5%8B%E5%80%BClist)

## detail

### type hint

PHP可以指定函数参数的类型
```php
 function smoke(Man $man){
         echo "man is smoking";
 }   
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