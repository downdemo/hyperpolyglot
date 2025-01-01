## [数据库系统概念](https://www.db-book.com/)

* 超键、候选键、主键、外键、外键约束等等；
* 第二章 关系模型介绍，2.4 节 模式图中 [大学数据库模式表](https://www.db-book.com/db6/lab-dir/sample_tables-dir/index.html)
* 两种查询，一种是用关系代数运算，一种是用 SQL 语句
* 关系代数运算（σ 选择、π 投影、⋈ 自然连接、× 笛卡尔积、并集差集）
* 连接 nature join，排重 distinct，排序 order by，having、group by 按分组查询，查询函数，比如 avg 求平均值、sum 求和
* 嵌套子查询，with 字句
* 深入学习关系代数运算
* 设计范式 1NF、2NF、3NF、BCNF，函数依赖（部分依赖、完全依赖、传递依赖之类），BCNF 分解，正则覆盖
* 第 11 章 索引与散列，稠密索引和稀疏索引，B+ 树的插入和删除
* 触发器、事务等等

## 安装

* 安装过程中可能会要求为 root 用户设置密码

```sh
sudo apt install -y mysql-server
```

* 安装后会自动启动，检查是否开启 mysqld 端口

```sh
sudo netstat -antp | grep mysqld
tcp        0      0 127.0.0.1:3306          0.0.0.0:*               LISTEN      1193/mysqld
```

* 也可以通过如下命令检查已启动

```sh
systemctl status mysql

● mysql.service - MySQL Community Server
   Loaded: loaded (/lib/systemd/system/mysql.service; enabled; vendor preset: enabled)
   Active: active (running) since Wed 2021-08-25 14:58:44 CST; 1min 12s ago
  Process: 1172 ExecStart=/usr/sbin/mysqld --daemonize --pid-file=/run/mysqld/mysqld.pid(code=exited, status=0/SUCCESS)
  Process: 998 ExecStartPre=/usr/share/mysql/mysql-systemd-start pre (code=exited, status=0/SUCCESS)
 Main PID: 1193 (mysqld)
    Tasks: 27 (limit: 2308)
   CGroup: /system.slice/mysql.service
           └─1193 /usr/sbin/mysqld --daemonize --pid-file=/run/mysqld/mysqld.pid

Aug 25 14:58:43 ub18 systemd[1]: Starting MySQL Community Server...
Aug 25 14:58:44 ub18 systemd[1]: Started MySQL Community Server.
```

* 以 root 身份登录

```sh
sudo mysql

Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 3
Server version: 5.7.35-0ubuntu0.18.04.1 (Ubuntu)

Copyright (c) 2000, 2021, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql>
```

* 若安装时设置了密码，则需要通过用户名密码登录

```sh
mysql -u root -p
Enter password:
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 47
Server version: 5.5.62-0ubuntu0.14.04.1 (Ubuntu)

Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql>
```

## 初始化

* 登录后，查看数据库，默认会有几个系统数据库，不要修改这些数据库

```
mysql> SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sys                |
+--------------------+
4 rows in set (0.00 sec)
```

* 创建一个新的数据库

```
mysql> CREATE DATABASE IF NOT EXISTS test;
Query OK, 1 row affected (0.01 sec)

mysql> SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sys                |
| test               |
+--------------------+
5 rows in set (0.00 sec)
```

* 选择数据库

```
USE test;
```

* 显示数据库中所有表

```
mysql> SHOW TABLES;
Empty set (0.00 sec)
```

* 删除数据库

```
mysql> DROP DATABASE test;
Query OK, 0 rows affected (0.00 sec)
```

* 创建如下 `test.sql` 文件

```sql
-- 如果test数据库不存在，就创建test数据库：
CREATE DATABASE IF NOT EXISTS test;

-- 切换到test数据库
USE test;

-- 删除classes表和students表（如果存在）：
DROP TABLE IF EXISTS classes;
DROP TABLE IF EXISTS students;

-- 创建classes表：
CREATE TABLE classes (
    id BIGINT NOT NULL AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 创建students表：
CREATE TABLE students (
    id BIGINT NOT NULL AUTO_INCREMENT,
    class_id BIGINT NOT NULL,
    name VARCHAR(100) NOT NULL,
    gender VARCHAR(1) NOT NULL,
    score INT NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 插入classes记录：
INSERT INTO classes(id, name) VALUES (1, '一班');
INSERT INTO classes(id, name) VALUES (2, '二班');
INSERT INTO classes(id, name) VALUES (3, '三班');
INSERT INTO classes(id, name) VALUES (4, '四班');

-- 插入students记录：
INSERT INTO students (id, class_id, name, gender, score) VALUES (1, 1, '小明', 'M', 90);
INSERT INTO students (id, class_id, name, gender, score) VALUES (2, 1, '小红', 'F', 95);
INSERT INTO students (id, class_id, name, gender, score) VALUES (3, 1, '小军', 'M', 88);
INSERT INTO students (id, class_id, name, gender, score) VALUES (4, 1, '小米', 'F', 73);
INSERT INTO students (id, class_id, name, gender, score) VALUES (5, 2, '小白', 'F', 81);
INSERT INTO students (id, class_id, name, gender, score) VALUES (6, 2, '小兵', 'M', 55);
INSERT INTO students (id, class_id, name, gender, score) VALUES (7, 2, '小林', 'M', 85);
INSERT INTO students (id, class_id, name, gender, score) VALUES (8, 3, '小新', 'F', 91);
INSERT INTO students (id, class_id, name, gender, score) VALUES (9, 3, '小王', 'M', 89);
INSERT INTO students (id, class_id, name, gender, score) VALUES (10, 3, '小丽', 'F', 85);

-- OK:
SELECT 'ok' as 'result:';
```

* 导入 sql 文件

```sh
sudo mysql < test.sql
```

* 登录并查看创建的表

```
mysql> SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sys                |
| test               |
+--------------------+
5 rows in set (0.00 sec)

mysql> USE test;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> SHOW TABLES;
+----------------+
| Tables_in_test |
+----------------+
| classes        |
| students       |
+----------------+
2 rows in set (0.00 sec)

mysql> SELECT * FROM classes;
+----+--------+
| id | name   |
+----+--------+
|  1 | 一班   |
|  2 | 二班   |
|  3 | 三班   |
|  4 | 四班   |
+----+--------+
4 rows in set (0.00 sec)

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
10 rows in set (0.01 sec)
```

## 条件查询

```
mysql> SELECT * FROM students WHERE score >= 90 AND gender = 'F';
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  2 |        1 | 小红   | F      |    95 |
|  8 |        3 | 小新   | F      |    91 |
+----+----------+--------+--------+-------+
2 rows in set (0.00 sec)

mysql> SELECT * FROM students WHERE score >= 90 AND NOT gender = 'M';
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  2 |        1 | 小红   | F      |    95 |
|  8 |        3 | 小新   | F      |    91 |
+----+----------+--------+--------+-------+
2 rows in set (0.00 sec)
```

* NOT 相当于 `<>`

```
mysql> SELECT * FROM students WHERE NOT gender = 'M';
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  2 |        1 | 小红   | F      |    95 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  8 |        3 | 小新   | F      |    91 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
5 rows in set (0.00 sec)

mysql> SELECT * FROM students WHERE gender <> 'M';
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  2 |        1 | 小红   | F      |    95 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  8 |        3 | 小新   | F      |    91 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
5 rows in set (0.00 sec)

mysql> SELECT * FROM students WHERE gender != 'M';
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  2 |        1 | 小红   | F      |    95 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  8 |        3 | 小新   | F      |    91 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
5 rows in set (0.00 sec)
```

## 投影查询

```
mysql> SELECT id, score point, name FROM students WHERE score > 90;
+----+-------+--------+
| id | point | name   |
+----+-------+--------+
|  2 |    95 | 小红   |
|  8 |    91 | 小新   |
+----+-------+--------+
2 rows in set (0.00 sec)
```

* 重命名列名

```
mysql> SELECT id, score points, name FROM students;
+----+--------+--------+
| id | points | name   |
+----+--------+--------+
|  1 |     90 | 小明   |
|  2 |     95 | 小红   |
|  3 |     88 | 小军   |
|  4 |     73 | 小米   |
|  5 |     81 | 小白   |
|  6 |     55 | 小兵   |
|  7 |     85 | 小林   |
|  8 |     91 | 小新   |
|  9 |     89 | 小王   |
| 10 |     85 | 小丽   |
+----+--------+--------+
10 rows in set (0.00 sec)
```

## 排序

* 默认排序规则是 ASC，表示升序，可以省略

```
mysql> SELECT id, score point, name FROM students WHERE score > 80 ORDER BY score;
+----+-------+--------+
| id | point | name   |
+----+-------+--------+
|  5 |    81 | 小白   |
|  7 |    85 | 小林   |
| 10 |    85 | 小丽   |
|  3 |    88 | 小军   |
|  9 |    89 | 小王   |
|  1 |    90 | 小明   |
|  8 |    91 | 小新   |
|  2 |    95 | 小红   |
+----+-------+--------+
8 rows in set (0.00 sec)

mysql> SELECT id, score point, name FROM students WHERE score > 80 ORDER BY score ASC;
+----+-------+--------+
| id | point | name   |
+----+-------+--------+
|  5 |    81 | 小白   |
|  7 |    85 | 小林   |
| 10 |    85 | 小丽   |
|  3 |    88 | 小军   |
|  9 |    89 | 小王   |
|  1 |    90 | 小明   |
|  8 |    91 | 小新   |
|  2 |    95 | 小红   |
+----+-------+--------+
8 rows in set (0.00 sec)
```

* DESC 表示降序

```
mysql> SELECT id, score, name FROM students WHERE score > 80 ORDER BY score DESC;
+----+-------+--------+
| id | score | name   |
+----+-------+--------+
|  2 |    95 | 小红   |
|  8 |    91 | 小新   |
|  1 |    90 | 小明   |
|  9 |    89 | 小王   |
|  3 |    88 | 小军   |
|  7 |    85 | 小林   |
| 10 |    85 | 小丽   |
|  5 |    81 | 小白   |
+----+-------+--------+
8 rows in set (0.00 sec)
```

* 可以指定多个排序条件，若排序时值相等则继续使用下一个条件进行排序

```
mysql> SELECT id, score, name FROM students WHERE score > 80 ORDER BY score DESC, id DESC;
+----+-------+--------+
| id | score | name   |
+----+-------+--------+
|  2 |    95 | 小红   |
|  8 |    91 | 小新   |
|  1 |    90 | 小明   |
|  9 |    89 | 小王   |
|  3 |    88 | 小军   |
| 10 |    85 | 小丽   |
|  7 |    85 | 小林   |
|  5 |    81 | 小白   |
+----+-------+--------+
8 rows in set (0.00 sec)
```

* 排序时可以使用列的别名

```
mysql> SELECT id, score point, name FROM students WHERE score > 80 ORDER BY point;
+----+-------+--------+
| id | point | name   |
+----+-------+--------+
|  5 |    81 | 小白   |
|  7 |    85 | 小林   |
| 10 |    85 | 小丽   |
|  3 |    88 | 小军   |
|  9 |    89 | 小王   |
|  1 |    90 | 小明   |
|  8 |    91 | 小新   |
|  2 |    95 | 小红   |
+----+-------+--------+
8 rows in set (0.00 sec)
```

## 分页查询

* LIMIT 表示查询结果的行数，OFFSET 表示跳过的查询结果数

```
mysql> SELECT * FROM students ORDER BY score;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  6 |        2 | 小兵   | M      |    55 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  7 |        2 | 小林   | M      |    85 |
| 10 |        3 | 小丽   | F      |    85 |
|  3 |        1 | 小军   | M      |    88 |
|  9 |        3 | 小王   | M      |    89 |
|  1 |        1 | 小明   | M      |    90 |
|  8 |        3 | 小新   | F      |    91 |
|  2 |        1 | 小红   | F      |    95 |
+----+----------+--------+--------+-------+
10 rows in set (0.00 sec)

mysql> SELECT * FROM students ORDER BY score LIMIT 3;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  6 |        2 | 小兵   | M      |    55 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
+----+----------+--------+--------+-------+
3 rows in set (0.00 sec)

mysql> SELECT * FROM students ORDER BY score LIMIT 3 OFFSET 2;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  5 |        2 | 小白   | F      |    81 |
| 10 |        3 | 小丽   | F      |    85 |
|  7 |        2 | 小林   | M      |    85 |
+----+----------+--------+--------+-------+
3 rows in set (0.00 sec)

mysql> SELECT * FROM students ORDER BY score LIMIT 3 OFFSET 3;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
| 10 |        3 | 小丽   | F      |    85 |
|  7 |        2 | 小林   | M      |    85 |
|  3 |        1 | 小军   | M      |    88 |
+----+----------+--------+--------+-------+
3 rows in set (0.00 sec)

mysql> SELECT * FROM students ORDER BY score LIMIT 3 OFFSET 6;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  9 |        3 | 小王   | M      |    89 |
|  1 |        1 | 小明   | M      |    90 |
|  8 |        3 | 小新   | F      |    91 |
+----+----------+--------+--------+-------+
3 rows in set (0.00 sec)

mysql> SELECT * FROM students ORDER BY score LIMIT 3 OFFSET 9;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  2 |        1 | 小红   | F      |    95 |
+----+----------+--------+--------+-------+
1 row in set (0.00 sec)

mysql> SELECT * FROM students ORDER BY score LIMIT 3 OFFSET 100;
Empty set (0.00 sec)
```

## 聚合查询

```
mysql> SELECT COUNT(*) FROM students WHERE score > 90;
+----------+
| COUNT(*) |
+----------+
|        2 |
+----------+
1 row in set (0.00 sec)

mysql> SELECT AVG(score) FROM students;
+------------+
| AVG(score) |
+------------+
|    83.2000 |
+------------+
1 row in set (0.01 sec)

mysql> SELECT MAX(score) FROM students;
+------------+
| MAX(score) |
+------------+
|         95 |
+------------+
1 row in set (0.00 sec)

mysql> SELECT MIN(score) FROM students;
+------------+
| MIN(score) |
+------------+
|         55 |
+------------+
1 row in set (0.00 sec)
```

* 若查询匹配不到任何行，COUNT 返回 0，SUM、AVG、MAX、MIN 返回 NULL

```
mysql> SELECT COUNT(*) FROM students WHERE gender = 'X';
+----------+
| COUNT(*) |
+----------+
|        0 |
+----------+
1 row in set (0.00 sec)

mysql> SELECT AVG(score) FROM students WHERE gender = 'X';
+------------+
| AVG(score) |
+------------+
|       NULL |
+------------+
1 row in set (0.00 sec)
```

* 分组查询

```
mysql> SELECT class_id, COUNT(*), AVG(score) FROM students GROUP BY class_id;
+----------+----------+------------+
| class_id | COUNT(*) | AVG(score) |
+----------+----------+------------+
|        1 |        4 |    86.5000 |
|        2 |        3 |    73.6667 |
|        3 |        3 |    88.3333 |
+----------+----------+------------+
3 rows in set (0.01 sec)
```

* 对于同一分组，SELECT 的列不能有不同的值，否则会出错

```
mysql> SELECT name, class_id, COUNT(*) FROM students GROUP BY class_id;
ERROR 1055 (42000): Expression #1 of SELECT list is not in GROUP BY clause and
contains nonaggregated column 'test.students.name' which is not functionally dependent
on columns in GROUP BY clause; this is incompatible with sql_mode=only_full_group_by
```

* 如果按多个列分组，则相当于按列的笛卡尔积分组

```
mysql> SELECT class_id, gender, AVG(score) FROM students GROUP BY class_id, gender;
+----------+--------+------------+
| class_id | gender | AVG(score) |
+----------+--------+------------+
|        1 | F      |    84.0000 |
|        1 | M      |    89.0000 |
|        2 | F      |    81.0000 |
|        2 | M      |    70.0000 |
|        3 | F      |    88.0000 |
|        3 | M      |    89.0000 |
+----------+--------+------------+
6 rows in set (0.00 sec)
```

## 多表查询

* 查询多个表时，实际查询的是多个表的笛卡尔积

```
mysql> SELECT COUNT(*) FROM students;
+----------+
| COUNT(*) |
+----------+
|       10 |
+----------+
1 row in set (0.00 sec)

mysql> SELECT COUNT(*) FROM classes;
+----------+
| COUNT(*) |
+----------+
|        4 |
+----------+
1 row in set (0.00 sec)

mysql> SELECT COUNT(*) FROM students, classes;
+----------+
| COUNT(*) |
+----------+
|       40 |
+----------+
1 row in set (0.00 sec)

mysql> SELECT * FROM students, classes;
+----+----------+--------+--------+-------+----+--------+
| id | class_id | name   | gender | score | id | name   |
+----+----------+--------+--------+-------+----+--------+
|  1 |        1 | 小明   | M      |    90 |  1 | 一班   |
|  1 |        1 | 小明   | M      |    90 |  2 | 二班   |
|  1 |        1 | 小明   | M      |    90 |  3 | 三班   |
|  1 |        1 | 小明   | M      |    90 |  4 | 四班   |
|  2 |        1 | 小红   | F      |    95 |  1 | 一班   |
|  2 |        1 | 小红   | F      |    95 |  2 | 二班   |
|  2 |        1 | 小红   | F      |    95 |  3 | 三班   |
|  2 |        1 | 小红   | F      |    95 |  4 | 四班   |
|  3 |        1 | 小军   | M      |    88 |  1 | 一班   |
|  3 |        1 | 小军   | M      |    88 |  2 | 二班   |
|  3 |        1 | 小军   | M      |    88 |  3 | 三班   |
|  3 |        1 | 小军   | M      |    88 |  4 | 四班   |
|  4 |        1 | 小米   | F      |    73 |  1 | 一班   |
|  4 |        1 | 小米   | F      |    73 |  2 | 二班   |
|  4 |        1 | 小米   | F      |    73 |  3 | 三班   |
|  4 |        1 | 小米   | F      |    73 |  4 | 四班   |
|  5 |        2 | 小白   | F      |    81 |  1 | 一班   |
|  5 |        2 | 小白   | F      |    81 |  2 | 二班   |
|  5 |        2 | 小白   | F      |    81 |  3 | 三班   |
|  5 |        2 | 小白   | F      |    81 |  4 | 四班   |
|  6 |        2 | 小兵   | M      |    55 |  1 | 一班   |
|  6 |        2 | 小兵   | M      |    55 |  2 | 二班   |
|  6 |        2 | 小兵   | M      |    55 |  3 | 三班   |
|  6 |        2 | 小兵   | M      |    55 |  4 | 四班   |
|  7 |        2 | 小林   | M      |    85 |  1 | 一班   |
|  7 |        2 | 小林   | M      |    85 |  2 | 二班   |
|  7 |        2 | 小林   | M      |    85 |  3 | 三班   |
|  7 |        2 | 小林   | M      |    85 |  4 | 四班   |
|  8 |        3 | 小新   | F      |    91 |  1 | 一班   |
|  8 |        3 | 小新   | F      |    91 |  2 | 二班   |
|  8 |        3 | 小新   | F      |    91 |  3 | 三班   |
|  8 |        3 | 小新   | F      |    91 |  4 | 四班   |
|  9 |        3 | 小王   | M      |    89 |  1 | 一班   |
|  9 |        3 | 小王   | M      |    89 |  2 | 二班   |
|  9 |        3 | 小王   | M      |    89 |  3 | 三班   |
|  9 |        3 | 小王   | M      |    89 |  4 | 四班   |
| 10 |        3 | 小丽   | F      |    85 |  1 | 一班   |
| 10 |        3 | 小丽   | F      |    85 |  2 | 二班   |
| 10 |        3 | 小丽   | F      |    85 |  3 | 三班   |
| 10 |        3 | 小丽   | F      |    85 |  4 | 四班   |
+----+----------+--------+--------+-------+----+--------+
40 rows in set (0.00 sec)
```

* 多表查询时，会存在相同的列名，此时可以使用别名

```
mysql> SELECT s.id sid, s.name, s.gender, s.score, c.id cid, c.name cname FROM students s, classes c;
+-----+--------+--------+-------+-----+--------+
| sid | name   | gender | score | cid | cname  |
+-----+--------+--------+-------+-----+--------+
|   1 | 小明   | M      |    90 |   1 | 一班   |
|   1 | 小明   | M      |    90 |   2 | 二班   |
|   1 | 小明   | M      |    90 |   3 | 三班   |
|   1 | 小明   | M      |    90 |   4 | 四班   |
|   2 | 小红   | F      |    95 |   1 | 一班   |
|   2 | 小红   | F      |    95 |   2 | 二班   |
|   2 | 小红   | F      |    95 |   3 | 三班   |
|   2 | 小红   | F      |    95 |   4 | 四班   |
|   3 | 小军   | M      |    88 |   1 | 一班   |
|   3 | 小军   | M      |    88 |   2 | 二班   |
|   3 | 小军   | M      |    88 |   3 | 三班   |
|   3 | 小军   | M      |    88 |   4 | 四班   |
|   4 | 小米   | F      |    73 |   1 | 一班   |
|   4 | 小米   | F      |    73 |   2 | 二班   |
|   4 | 小米   | F      |    73 |   3 | 三班   |
|   4 | 小米   | F      |    73 |   4 | 四班   |
|   5 | 小白   | F      |    81 |   1 | 一班   |
|   5 | 小白   | F      |    81 |   2 | 二班   |
|   5 | 小白   | F      |    81 |   3 | 三班   |
|   5 | 小白   | F      |    81 |   4 | 四班   |
|   6 | 小兵   | M      |    55 |   1 | 一班   |
|   6 | 小兵   | M      |    55 |   2 | 二班   |
|   6 | 小兵   | M      |    55 |   3 | 三班   |
|   6 | 小兵   | M      |    55 |   4 | 四班   |
|   7 | 小林   | M      |    85 |   1 | 一班   |
|   7 | 小林   | M      |    85 |   2 | 二班   |
|   7 | 小林   | M      |    85 |   3 | 三班   |
|   7 | 小林   | M      |    85 |   4 | 四班   |
|   8 | 小新   | F      |    91 |   1 | 一班   |
|   8 | 小新   | F      |    91 |   2 | 二班   |
|   8 | 小新   | F      |    91 |   3 | 三班   |
|   8 | 小新   | F      |    91 |   4 | 四班   |
|   9 | 小王   | M      |    89 |   1 | 一班   |
|   9 | 小王   | M      |    89 |   2 | 二班   |
|   9 | 小王   | M      |    89 |   3 | 三班   |
|   9 | 小王   | M      |    89 |   4 | 四班   |
|  10 | 小丽   | F      |    85 |   1 | 一班   |
|  10 | 小丽   | F      |    85 |   2 | 二班   |
|  10 | 小丽   | F      |    85 |   3 | 三班   |
|  10 | 小丽   | F      |    85 |   4 | 四班   |
+-----+--------+--------+-------+-----+--------+
40 rows in set (0.00 sec)

mysql> SELECT s.id, s.name, s.score, c.id cid, c.name cname FROM students s, classes c WHERE s.score > 90;
+----+--------+-------+-----+--------+
| id | name   | score | cid | cname  |
+----+--------+-------+-----+--------+
|  2 | 小红   |    95 |   1 | 一班   |
|  8 | 小新   |    91 |   1 | 一班   |
|  2 | 小红   |    95 |   2 | 二班   |
|  8 | 小新   |    91 |   2 | 二班   |
|  2 | 小红   |    95 |   3 | 三班   |
|  8 | 小新   |    91 |   3 | 三班   |
|  2 | 小红   |    95 |   4 | 四班   |
|  8 | 小新   |    91 |   4 | 四班   |
+----+--------+-------+-----+--------+
8 rows in set (0.00 sec)
```

## 连接查询

* INNER JOIN 返回同时存在于两张表的行

```
mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
10 rows in set (0.01 sec)

mysql> SELECT * FROM classes;
+----+--------+
| id | name   |
+----+--------+
|  1 | 一班   |
|  2 | 二班   |
|  3 | 三班   |
|  4 | 四班   |
+----+--------+
4 rows in set (0.00 sec)

mysql> SELECT * FROM students s INNER JOIN classes c ON s.class_id = c.id;
+----+----------+--------+--------+-------+----+--------+
| id | class_id | name   | gender | score | id | name   |
+----+----------+--------+--------+-------+----+--------+
|  1 |        1 | 小明   | M      |    90 |  1 | 一班   |
|  2 |        1 | 小红   | F      |    95 |  1 | 一班   |
|  3 |        1 | 小军   | M      |    88 |  1 | 一班   |
|  4 |        1 | 小米   | F      |    73 |  1 | 一班   |
|  5 |        2 | 小白   | F      |    81 |  2 | 二班   |
|  6 |        2 | 小兵   | M      |    55 |  2 | 二班   |
|  7 |        2 | 小林   | M      |    85 |  2 | 二班   |
|  8 |        3 | 小新   | F      |    91 |  3 | 三班   |
|  9 |        3 | 小王   | M      |    89 |  3 | 三班   |
| 10 |        3 | 小丽   | F      |    85 |  3 | 三班   |
+----+----------+--------+--------+-------+----+--------+
10 rows in set (0.00 sec)

mysql> SELECT * FROM classes c INNER JOIN students s ON c.id = s.class_id;
+----+--------+----+----------+--------+--------+-------+
| id | name   | id | class_id | name   | gender | score |
+----+--------+----+----------+--------+--------+-------+
|  1 | 一班   |  1 |        1 | 小明   | M      |    90 |
|  1 | 一班   |  2 |        1 | 小红   | F      |    95 |
|  1 | 一班   |  3 |        1 | 小军   | M      |    88 |
|  1 | 一班   |  4 |        1 | 小米   | F      |    73 |
|  2 | 二班   |  5 |        2 | 小白   | F      |    81 |
|  2 | 二班   |  6 |        2 | 小兵   | M      |    55 |
|  2 | 二班   |  7 |        2 | 小林   | M      |    85 |
|  3 | 三班   |  8 |        3 | 小新   | F      |    91 |
|  3 | 三班   |  9 |        3 | 小王   | M      |    89 |
|  3 | 三班   | 10 |        3 | 小丽   | F      |    85 |
+----+--------+----+----------+--------+--------+-------+
10 rows in set (0.00 sec)
```

* RIGHT OUTER JOIN 返回右表都存在的行

```
mysql> SELECT * FROM students s RIGHT OUTER JOIN classes c ON s.class_id = c.id;
+------+----------+--------+--------+-------+----+--------+
| id   | class_id | name   | gender | score | id | name   |
+------+----------+--------+--------+-------+----+--------+
|    1 |        1 | 小明   | M      |    90 |  1 | 一班   |
|    2 |        1 | 小红   | F      |    95 |  1 | 一班   |
|    3 |        1 | 小军   | M      |    88 |  1 | 一班   |
|    4 |        1 | 小米   | F      |    73 |  1 | 一班   |
|    5 |        2 | 小白   | F      |    81 |  2 | 二班   |
|    6 |        2 | 小兵   | M      |    55 |  2 | 二班   |
|    7 |        2 | 小林   | M      |    85 |  2 | 二班   |
|    8 |        3 | 小新   | F      |    91 |  3 | 三班   |
|    9 |        3 | 小王   | M      |    89 |  3 | 三班   |
|   10 |        3 | 小丽   | F      |    85 |  3 | 三班   |
| NULL |     NULL | NULL   | NULL   |  NULL |  4 | 四班   |
+------+----------+--------+--------+-------+----+--------+
11 rows in set (0.00 sec)

mysql> SELECT * FROM classes c RIGHT OUTER JOIN students s ON c.id = s.class_id;
+------+--------+----+----------+--------+--------+-------+
| id   | name   | id | class_id | name   | gender | score |
+------+--------+----+----------+--------+--------+-------+
|    1 | 一班   |  1 |        1 | 小明   | M      |    90 |
|    1 | 一班   |  2 |        1 | 小红   | F      |    95 |
|    1 | 一班   |  3 |        1 | 小军   | M      |    88 |
|    1 | 一班   |  4 |        1 | 小米   | F      |    73 |
|    2 | 二班   |  5 |        2 | 小白   | F      |    81 |
|    2 | 二班   |  6 |        2 | 小兵   | M      |    55 |
|    2 | 二班   |  7 |        2 | 小林   | M      |    85 |
|    3 | 三班   |  8 |        3 | 小新   | F      |    91 |
|    3 | 三班   |  9 |        3 | 小王   | M      |    89 |
|    3 | 三班   | 10 |        3 | 小丽   | F      |    85 |
+------+--------+----+----------+--------+--------+-------+
10 rows in set (0.00 sec)
```

* LEFT OUTER JOIN 返回左表都存在的行

```
mysql> SELECT * FROM classes c LEFT OUTER JOIN students s ON c.id = s.class_id;
+----+--------+------+----------+--------+--------+-------+
| id | name   | id   | class_id | name   | gender | score |
+----+--------+------+----------+--------+--------+-------+
|  1 | 一班   |    1 |        1 | 小明   | M      |    90 |
|  1 | 一班   |    2 |        1 | 小红   | F      |    95 |
|  1 | 一班   |    3 |        1 | 小军   | M      |    88 |
|  1 | 一班   |    4 |        1 | 小米   | F      |    73 |
|  2 | 二班   |    5 |        2 | 小白   | F      |    81 |
|  2 | 二班   |    6 |        2 | 小兵   | M      |    55 |
|  2 | 二班   |    7 |        2 | 小林   | M      |    85 |
|  3 | 三班   |    8 |        3 | 小新   | F      |    91 |
|  3 | 三班   |    9 |        3 | 小王   | M      |    89 |
|  3 | 三班   |   10 |        3 | 小丽   | F      |    85 |
|  4 | 四班   | NULL |     NULL | NULL   | NULL   |  NULL |
+----+--------+------+----------+--------+--------+-------+
11 rows in set (0.00 sec)

mysql> SELECT * FROM students s LEFT OUTER JOIN classes c ON s.class_id = c.id;
+----+----------+--------+--------+-------+------+--------+
| id | class_id | name   | gender | score | id   | name   |
+----+----------+--------+--------+-------+------+--------+
|  1 |        1 | 小明   | M      |    90 |    1 | 一班   |
|  2 |        1 | 小红   | F      |    95 |    1 | 一班   |
|  3 |        1 | 小军   | M      |    88 |    1 | 一班   |
|  4 |        1 | 小米   | F      |    73 |    1 | 一班   |
|  5 |        2 | 小白   | F      |    81 |    2 | 二班   |
|  6 |        2 | 小兵   | M      |    55 |    2 | 二班   |
|  7 |        2 | 小林   | M      |    85 |    2 | 二班   |
|  8 |        3 | 小新   | F      |    91 |    3 | 三班   |
|  9 |        3 | 小王   | M      |    89 |    3 | 三班   |
| 10 |        3 | 小丽   | F      |    85 |    3 | 三班   |
+----+----------+--------+--------+-------+------+--------+
10 rows in set (0.00 sec)
```

* 使用 UNION 实现 FULL OUTER JOIN

```
mysql> SELECT * FROM students s LEFT OUTER JOIN classes c ON s.class_id = c.id 
    -> UNION
    -> SELECT * FROM students s RIGHT OUTER JOIN classes c ON s.class_id = c.id;
+------+----------+--------+--------+-------+------+--------+
| id   | class_id | name   | gender | score | id   | name   |
+------+----------+--------+--------+-------+------+--------+
|    1 |        1 | 小明   | M      |    90 |    1 | 一班   |
|    2 |        1 | 小红   | F      |    95 |    1 | 一班   |
|    3 |        1 | 小军   | M      |    88 |    1 | 一班   |
|    4 |        1 | 小米   | F      |    73 |    1 | 一班   |
|    5 |        2 | 小白   | F      |    81 |    2 | 二班   |
|    6 |        2 | 小兵   | M      |    55 |    2 | 二班   |
|    7 |        2 | 小林   | M      |    85 |    2 | 二班   |
|    8 |        3 | 小新   | F      |    91 |    3 | 三班   |
|    9 |        3 | 小王   | M      |    89 |    3 | 三班   |
|   10 |        3 | 小丽   | F      |    85 |    3 | 三班   |
| NULL |     NULL | NULL   | NULL   |  NULL |    4 | 四班   |
+------+----------+--------+--------+-------+------+--------+
11 rows in set (0.00 sec)
```

## 修改数据

```
mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
10 rows in set (0.00 sec)

mysql> INSERT INTO students (class_id, name, gender, score) VALUES (3, 'A', 'M', 90);
Query OK, 1 row affected (0.00 sec)

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
| 13 |        3 | A      | M      |    90 |
+----+----------+--------+--------+-------+
11 rows in set (0.00 sec)

mysql> INSERT INTO students (class_id, name, gender, score) VALUES
    -> (4, 'B', 'F', 85),
    -> (4, 'C', 'M', 95);
Query OK, 2 rows affected (0.00 sec)
Records: 2  Duplicates: 0  Warnings: 0

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
| 13 |        3 | A      | M      |    90 |
| 14 |        4 | B      | F      |    85 |
| 15 |        4 | C      | M      |    95 |
+----+----------+--------+--------+-------+
13 rows in set (0.00 sec)

mysql> UPDATE students SET name = 'D', score = score + 10 WHERE id = 13;
Query OK, 1 row affected (0.01 sec)
Rows matched: 1  Changed: 1  Warnings: 0

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
| 13 |        3 | D      | M      |   100 |
| 14 |        4 | B      | F      |    85 |
| 15 |        4 | C      | M      |    95 |
+----+----------+--------+--------+-------+
13 rows in set (0.00 sec)

mysql> DELETE FROM students WHERE id >= 13 AND id <= 15;
Query OK, 3 rows affected (0.01 sec)

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
10 rows in set (0.00 sec)
```

## 实用 SQL

* REPLACE 在主键存在时删除行再插入，否则直接插入

```
mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
10 rows in set (0.00 sec)

mysql> REPLACE INTO students (id, class_id, name, gender, score) VALUES (11, 1, '小明', 'M', 90);
Query OK, 1 row affected (0.01 sec)

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
| 11 |        1 | 小明   | M      |    90 |
+----+----------+--------+--------+-------+
11 rows in set (0.01 sec)

mysql> REPLACE INTO students (id, class_id, name, gender, score) VALUES (11, 1, '小明', 'M', 100);
Query OK, 2 rows affected (0.00 sec)

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
| 11 |        1 | 小明   | M      |   100 |
+----+----------+--------+--------+-------+
11 rows in set (0.00 sec)
```

* 当主键不存在时插入行，否则更新行

```
mysql> INSERT INTO students (id, class_id, name, gender, score) VALUES (11, 2, 'XXX', 'M', 100) ON DUPLICATE KEY UPDATE name = '小蓝', gender = 'M';
Query OK, 2 rows affected (0.00 sec)

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
| 11 |        1 | 小蓝   | M      |   100 |
+----+----------+--------+--------+-------+
11 rows in set (0.00 sec)

mysql> INSERT INTO students (id, class_id, name, gender, score) VALUES (12, 2, 'XXX', 'M', 100) ON DUPLICATE KEY UPDATE name = '小蓝', gender = 'F';
Query OK, 1 row affected (0.00 sec)

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
| 11 |        1 | 小蓝   | F      |   100 |
| 12 |        2 | XXX    | M      |   100 |
+----+----------+--------+--------+-------+
12 rows in set (0.00 sec)

mysql> DELETE FROM students WHERE id >= 11;
Query OK, 2 rows affected (0.00 sec)
```

* 当主键不存在时插入行，否则无操作

```
INSERT IGNORE INTO students (id, class_id, name, gender, score) VALUES (1, 2, 'XXX', 'M', 100);
Query OK, 0 rows affected, 1 warning (0.00 sec)
```

* 拷贝结果到新表

```
mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
10 rows in set (0.00 sec)

mysql> CREATE TABLE top_student SELECT * FROM students WHERE score >= 90;
Query OK, 3 rows affected (0.07 sec)
Records: 3  Duplicates: 0  Warnings: 0

mysql> SELECT * FROM top_student;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  8 |        3 | 小新   | F      |    91 |
+----+----------+--------+--------+-------+
3 rows in set (0.00 sec)

mysql> DROP TABLE top_student;
Query OK, 0 rows affected (0.04 sec)
```

* 查询结果插入到新表

```
mysql> CREATE TABLE class_avg_score (
    -> id BIGINT NOT NULL AUTO_INCREMENT,
    -> class_id BIGINT NOT NULL,
    -> avg_score DOUBLE NOT NULL,
    -> PRIMARY KEY (id)
    -> );
Query OK, 0 rows affected (0.06 sec)

mysql> INSERT INTO class_avg_score (class_id, avg_score) SELECT class_id, AVG(score) FROM students GROUP BY class_id;
Query OK, 3 rows affected (0.02 sec)
Records: 3  Duplicates: 0  Warnings: 0

mysql> SELECT * FROM class_avg_score;
+----+----------+--------------+
| id | class_id | avg_score    |
+----+----------+--------------+
|  1 |        1 |         86.5 |
|  2 |        2 | 73.666666666 |
|  3 |        3 | 88.333333333 |
+----+----------+--------------+
3 rows in set (0.00 sec)

mysql> DROP TABLE class_avg_score;
Query OK, 0 rows affected (0.02 sec)
```

## 表管理

* 查看表结构

```
mysql> DESC classes;
+-------+--------------+------+-----+---------+----------------+
| Field | Type         | Null | Key | Default | Extra          |
+-------+--------------+------+-----+---------+----------------+
| id    | bigint(20)   | NO   | PRI | NULL    | auto_increment |
| name  | varchar(100) | NO   |     | NULL    |                |
+-------+--------------+------+-----+---------+----------------+
2 rows in set (0.00 sec)

mysql> DESC students;
+----------+--------------+------+-----+---------+----------------+
| Field    | Type         | Null | Key | Default | Extra          |
+----------+--------------+------+-----+---------+----------------+
| id       | bigint(20)   | NO   | PRI | NULL    | auto_increment |
| class_id | bigint(20)   | NO   |     | NULL    |                |
| name     | varchar(100) | NO   |     | NULL    |                |
| gender   | varchar(1)   | NO   |     | NULL    |                |
| score    | int(11)      | NO   |     | NULL    |                |
+----------+--------------+------+-----+---------+----------------+
5 rows in set (0.01 sec)
```

* 查看建表语句

```
mysql> SHOW CREATE TABLE classes;
+---------+---------------------------------------------------------+
| Table   | Create Table                                            |
+---------+---------------------------------------------------------+
| classes | CREATE TABLE `classes` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 |
+---------+---------------------------------------------------------+
1 row in set (0.00 sec)

mysql> SHOW CREATE TABLE students;
+----------+--------------------------------------------------------+
| Table    | Create Table                                           |
+----------+--------------------------------------------------------+
| students | CREATE TABLE `students` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `class_id` bigint(20) NOT NULL,
  `name` varchar(100) NOT NULL,
  `gender` varchar(1) NOT NULL,
  `score` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 |
+----------+--------------------------------------------------------+
1 row in set (0.00 sec)
```

* 修改列

```
mysql> ALTER TABLE students ADD COLUMN birth VARCHAR(10) NOT NULL;
Query OK, 0 rows affected (0.19 sec)
Records: 0  Duplicates: 0  Warnings: 0

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+-------+
| id | class_id | name   | gender | score | birth |
+----+----------+--------+--------+-------+-------+
|  1 |        1 | 小明   | M      |    90 |       |
|  2 |        1 | 小红   | F      |    95 |       |
|  3 |        1 | 小军   | M      |    88 |       |
|  4 |        1 | 小米   | F      |    73 |       |
|  5 |        2 | 小白   | F      |    81 |       |
|  6 |        2 | 小兵   | M      |    55 |       |
|  7 |        2 | 小林   | M      |    85 |       |
|  8 |        3 | 小新   | F      |    91 |       |
|  9 |        3 | 小王   | M      |    89 |       |
| 10 |        3 | 小丽   | F      |    85 |       |
+----+----------+--------+--------+-------+-------+
10 rows in set (0.00 sec)

mysql> ALTER TABLE students CHANGE COLUMN birth birthday VARCHAR(20) NOT NULL;
Query OK, 0 rows affected (0.01 sec)
Records: 0  Duplicates: 0  Warnings: 0

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+----------+
| id | class_id | name   | gender | score | birthday |
+----+----------+--------+--------+-------+----------+
|  1 |        1 | 小明   | M      |    90 |          |
|  2 |        1 | 小红   | F      |    95 |          |
|  3 |        1 | 小军   | M      |    88 |          |
|  4 |        1 | 小米   | F      |    73 |          |
|  5 |        2 | 小白   | F      |    81 |          |
|  6 |        2 | 小兵   | M      |    55 |          |
|  7 |        2 | 小林   | M      |    85 |          |
|  8 |        3 | 小新   | F      |    91 |          |
|  9 |        3 | 小王   | M      |    89 |          |
| 10 |        3 | 小丽   | F      |    85 |          |
+----+----------+--------+--------+-------+----------+
10 rows in set (0.00 sec)

mysql> ALTER TABLE students DROP COLUMN birthday;
Query OK, 0 rows affected (0.15 sec)
Records: 0  Duplicates: 0  Warnings: 0

mysql> SELECT * FROM students;
+----+----------+--------+--------+-------+
| id | class_id | name   | gender | score |
+----+----------+--------+--------+-------+
|  1 |        1 | 小明   | M      |    90 |
|  2 |        1 | 小红   | F      |    95 |
|  3 |        1 | 小军   | M      |    88 |
|  4 |        1 | 小米   | F      |    73 |
|  5 |        2 | 小白   | F      |    81 |
|  6 |        2 | 小兵   | M      |    55 |
|  7 |        2 | 小林   | M      |    85 |
|  8 |        3 | 小新   | F      |    91 |
|  9 |        3 | 小王   | M      |    89 |
| 10 |        3 | 小丽   | F      |    85 |
+----+----------+--------+--------+-------+
10 rows in set (0.00 sec)
```

* 删除表

```
mysql> SHOW TABLES;
+----------------+
| Tables_in_test |
+----------------+
| classes        |
| students       |
+----------------+
2 rows in set (0.00 sec)

mysql> DROP TABLE students;
Query OK, 0 rows affected (0.03 sec)

mysql> SHOW TABLES;
+----------------+
| Tables_in_test |
+----------------+
| classes        |
+----------------+
1 row in set (0.00 sec)
```

* 删除数据库

```
mysql> SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sys                |
| test               |
+--------------------+
5 rows in set (0.00 sec)

mysql> DROP DATABASE test;
Query OK, 1 row affected (0.05 sec)

mysql> SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sys                |
+--------------------+
4 rows in set (0.00 sec)
```
