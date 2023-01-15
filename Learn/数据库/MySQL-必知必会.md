---
mytags: myblog
title: 使用 MySQL
date: 2020.03.22 18:41:23
categories: 数据库
tags: MySQL
---

### 使用 MySQL

  use 和 show 的基本使用

* 返回可用数据库的一个列表

  ```mysql
  show databases;
  ```
  
* 选择一个数据库

  ```mysql
  use db_name
  ```

* 返回当前选择的数据库内可用表的列表

  ```mysql
  show tables;
  ```

* 给出一个表名，它对每个字段返回一行，行中包含字段名、数据类型、是否允许 NULL、键信息默认值以及其他信息，`describe` 是等价的。

  ```mysql
  show columns from tbl_name;
  ```

  ```mysql
  describe tbl_name;
  ```

  

> 进一步了解 `show`
>
> ```mysql
> help show;
> ```

### 检索数据

  select 从一个或多个表中检索信息，为了使用 `select` 检索数据，必须至少给出两条信息 ---- 想选择什么，以及从什么地方选择

* 下面利用 `select` 语句从 `products` 表中检索一个名为 `prod_name` 的列

  ```mysql
  select prod_name from products;
  ```
  
* 检索多个列
  
    ```mysql
    select prod_id, prod_name, prod_price from products;
    ```
  
* 检索所有列

  ```mysql
  select * from products;
  ```

* 检索不同行，返回的多个行中存在有些行的值是重复的，为得到值不重复的行可使用 `distinct` 关键字

  ```mysql
  select distinct vend_id from products;
  ```

* 限制结果，下面 `limit 5` 指示了 MySQL 返回不多于 5 行

  ```mysql
  select prod_name from products limit 5;
  ```

  下面 `limit 5, 5` 指示了 MySQL 返回从行 5 开始的 5 行。第一个数为开始的位置，第二个数为要检索的行数。为了直观，可以等价地写成 `limit 5 offset 5`

  ```mysql
  select prod_name from products limit 5,5;
  ```

  ```mysql
  select prod_name from products limit 5 offset 5;
  ```

* 使用完全限定的列名

  ```mysql
  select products.prod_name from products;
  ```

  表名也可以完全限定

  ```mysql
  select products.prod_name from crashcourse.products;
  ```

  

### 排序检索数据

* 排序数据

  子句（clause) ：SQL 语句由子句构成，有些是必须的，有些是可选的。一个子句通常由一个关键字和所提供的数据组成。比如 `select` 语句的 `from` 子句，为了明确地对 `select` 语句检索出来的数据进行排序，可以使用 `order by` 子句，`order by` 取一个或多个列的名字，据此对输出进行排序。比如下面语句将数字和字母作为排序依据进行排序。

  ```mysql
  select prod_name from products order by prod_name;
  ```

* 按多个列排序，比如下面 sql 代码将首先按价格排序，然后再按名称排序，因为拥有同一个价格的产品可能有多个，在对这个多个进行进行名字排序

  ```mysql
  select prod_id, prod_price, prod_name from products order by prod_price, prod_name;
  ```

  

* 按指定方向排序，默认是 1-10, A-Z 升序排序的，为了进行降序排序，必须指定 `DESC` 关键字，`desc` 关键字只应用到直接位于其前面的列名。所以下面降序只生效于 `prod_price`

  ```mysql
  select prod_id, prod_price, prod_name from products order by prod_price desc, prod_name;
  ```

  想要在多个列进行降序排序，必须对每个列指定 `DESC`  关键字。

  > `ASC` 指明升序，但实际上，`ASC` 没有多大用处，因为升序是默认的。

* 例子：求一个列中最高或最低值

  使用 `order by` 和 `limit` 组合，能够找到一个列中最高或最低的值，下面例子显示如何找到最昂贵的物品的值

  ```mysql
  select prod_price from products order by prod_price desc limit 1;
  ```



> 在给出 `order by` 子句时，应当保证它位于 `from` 子句之后，如果使用 `limit` ，它必须位于 `order by`之后，使用子句次序不对将产生错误



### 过滤数据

主要通过 select 语句和 where 子句来指定搜索条件

* `where` 子句操作符

  | 操作符  |       说明       |
  | :-----: | :--------------: |
  |    =    |       等于       |
  |   <>    |      不等于      |
  |   !=    |      不等于      |
  |    <    |       小于       |
  |   <=    |     小于等于     |
  |    >    |       大于       |
  |   >=    |     大于等于     |
  | between | 在指定两个值之间 |

* 检查单个值，MySQL 执行匹配时默认不区分大小写，所以 'fuses' 与 'Fuses' 匹配

  ```mysql
  select prod_name, prod_price from products where prod_name = 'fuses';
  ```

  检查价格小于等于 10 美元的所有产品

  ```mysql
  select prod_name, prod_price from products where prod_price <= 10;
  ```

* 不匹配检查，比如下面例子列出不是由供应商 1003 制造的所有产品

  ```mysql
  select vend_id, prod_name from products where vend_id != 1003;
  ```

* 范围检查

  ```mysql
  select prod_name, prod_price from products where prod_price between 5 and 10;
  ```

* 空值检查

  一个列不包含值时，称其为空值 NULL，比如下面语句返回没有价格的所有产品

  ```mysql
  select prod_name from products where prod_price is null;
  ```

  

### 数据过滤

之前的数据过滤使用的都是单一的条件，为了进行更强的过滤控制，MySQL 允许出现多个 `where` 子句，这些子句可以两种方式使用：以 AND 子句的方式或以 OR 子句的方式

操作符（operator）用来联结或改变 `where` 中的子句的关键字，也称为逻辑操作符。

比如主句是 select ，where 是 select 的子句



* AND 操作符，比如下面 select 语句中的 where 子句包含了两个条件，并且用 and 关键字联结它们

  ```mysql
  select prod_id, prod_price, prod_name from products where vend_id = 1003 and prod_price <= 10;
  ```

  
