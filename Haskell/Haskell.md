## 变量类型

* 基本数据类型：Char、Bool、Int（32 位或 64 位）、Word（无符号整型，32 位或 64 位）Integer（不限长度的整型）、Float、Double、Rational（有理数）

```hs
‘a' :: Char
True :: Bool
42 :: Int
42 :: Word
9999999999999999999 :: Integer
3.14 :: Float
3.14 :: Double
3.14 :: Rational
```

* 可以在 GHCi 中使用 `:type` 或 `:t` 查看类型签名

```
Prelude> :t 'a'
'a' :: Char
Prelude> :t True
True :: Bool
Prelude> :t 42
42 :: Num p => p
Prelude> 9999999999999999999
9999999999999999999
Prelude> :t 9999999999999999999
9999999999999999999 :: Num p => p
Prelude> :t 3.14
3.14 :: Fractional p => p
Prelude> 3.14 :: Rational
157 % 50
```

* 复合数据类型：List、Tuple

```hs
[1, 2, 3] :: Num a => [a]
("hello", 42, False) :: Num b => ([Char], b, Bool)
```

* 小括号内有单个元素，其类型即为该元素类型

```hs
(42) :: Num p => p
```

* 小括号内无元素，其类型为特殊类型 ()

```hs
() :: ()
```

* 二元组相关操作

```hs
fst (1, 2) -- 1
snd (1, 2) -- 2
```

* String 类型本质是元素类型为 Char 的 List

```hs
"abc" :: [Char]
```

* 用 Range 构造 List

```hs
[1 .. 9] -- [1,2,3,4,5,6,7,8,9]
[9 .. 1] -- []
[1, 4 .. 9] -- [1,4,7]
[1, 0 .. 9] -- []
[9, 8 .. 1] -- [9,8,7,6,5,4,3,2,1]
[1 ..] -- [1,2,3,4...] 无穷
['a' .. 'z'] -- "abcdefghijklmnopqrstuvwxyz"
['A' .. 'Z'] -- "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
```

* List Comprehension

```hs
[x * x | x <- [1 .. 3]] -- [1,4,9]
[x | x <- [1 .. 5], x /= 3] -- [1,2,4,5] 注意 /= 是不等
[x | x <- [1 .. 9], odd x] -- [1,3,5,7,9]
[if x < 5 then 1 else 2 | x <- [1 .. 9], odd x] -- [1,1,2,2,2]
[x ++ show y | x <- ["a", "b", "c"], y <- [1, 2, 3]] -- ["a1","a2","a3","b1","b2","b3","c1","c2","c3"]
```

* [List 相关操作](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html)

```hs
length [1, 2, 3] -- 3
null [] -- True
null [1,2,3] -- False
replicate 5 1 -- [1,1,1,1,1]
head [1, 2, 3] -- 1
tail [1, 2, 3] -- [2,3]
last [1, 2, 3] -- 3
init [1, 2, 3] -- [1,2]
reverse [1, 2, 3] -- [3,2,1]
take 2 [1 .. 5] -- [1,2]
takeWhile odd [1, 3, 5, 6, 7, 8, 9] -- [1, 3, 5] 从首元素开始提取直到遇到第一个不满足条件的元素
drop 2 [1 .. 5] -- [3,4,5]
dropWhile odd [1, 3, 5, 6, 7, 8, 9] -- [6,7,8,9] 从首元素开始移除直到遇到第一个不满足条件的元素
span odd [1, 3, 5, 6, 7] -- ([1,3,5],[6,7,8,9]) -- takeWhile 和 dropWhile 的结果组成的元组
[1,2] ++ [3] ++ [4, 5, 6] -- [1,2,3,4,5,6]
concat [[1, 2], [3], [4, 5, 6]] -- [1,2,3,4,5,6]
zip [1, 2, 3] [4, 5, 6] -- [(1,4),(2,5),(3,6)]
zipWith (+) [1, 2, 3] [4, 5, 6] -- [5,7,9]
and [True, False, True] -- False
and [] -- True
or [True, False, True] -- True
or [] -- False
all odd [1, 3, 5] -- True
all odd [] -- True
any even [1, 3, 5] -- False
any even [] -- False
break odd [1, 2, 3] -- ([],[1,2,3]) 拆成两个列表组成的元组，从第一个满足条件的元素开始拆到第二个列表
break even [1, 2, 3] -- ([1],[2,3])
break (> 3) [1, 2, 3] -- ([1,2,3],[])
splitAt 3 "downdemo" -- ("dow","ndemo")
splitAt (-3) "downdemo" -- ("","downdemo")
splitAt (100) "downdemo" -- ("downdemo","")
elem 'o' "downdemo" -- True
filter odd [1 .. 5, 6] -- [1,3,5]
lines "a\nb\nc" -- ["a","b","c"]
lines "a\r\nb\nc" -- ["a\r","b","c"]
lines "a\nb\nc\n" -- ["a","b","c"]
lines "a\nb\nc\n\n" -- ["a","b","c",""]
lines "a\nb\nc\n\n\n" -- ["a","b","c","",""]
lines "\na\n\nb\nc" -- ["","a","","b","c"]
words "a    b  c" -- ["a","b","c"]
words "a \n\r b \n \t c \n" -- ["a","b","c"]
unwords ["hello", "haskell", "world"] -- "hello haskell world"
Data.List.isPrefixOf "down" "downdemo" -- True
Data.List.isSuffixOf "demo" "downdemo" -- True
Data.List.isInfixOf "nd" "downdemo" -- True
Data.List.sort [3, 1, 2] -- [1,2,3]
```

## 函数

* 创建 add.hs 如下，用 IO 开头的类型签名来表示不纯（impure）函数，`runghc add.hs` 运行

```hs
add :: Num a => a -> a -> a -- 实质是 (a -> (a -> a))，接受一个参数并返回一个函数，这个函数接受一个参数返回一个值
add a b = a + b

main :: IO ()
main = print (add 1 2)
```

* 也可以将 module 载入 GHCi，即可在 GHCi 中调用载入的 module 的函数

```hs
Prelude> :l add.hs
[1 of 1] Compiling Main             ( add.hs, interpreted )
Ok, one module loaded.
*Main> add 1 2
3
*Main> main
3
```

* lambda 函数

```hs
(\x y -> [x, y]) 1 2 -- [1,2]
(\(x : _) -> x) [1, 2, 3] -- 1
```

* drop 函数会对错误情况进行处理

```hs
drop 3 "abcde" -- "de"
drop 6 "abcde" -- ""
drop (-1) "abcde" -- "abcde"
```

* 实现与 drop 功能相同的函数

```hs
myDrop :: Int -> [a] -> [a]
myDrop n xs =
  if n <= 0 || null xs
    then xs
    else myDrop (n -1) (tail xs)
```

* 对元素为 Num 的 List，按下标取元素

```hs
indexAt :: (Num a) => Int -> [a] -> a
indexAt n xs =
  if n < 0 || n >= length xs
    then error ("cannot access index " ++ show n)
    else
      if n == 0
        then head xs
        else indexAt (n - 1) (tail xs)
```

* 对于多条件分支，可以用守卫表达式简化写法

```hs
indexAt :: (Num a) => Int -> [a] -> a
indexAt n xs
  | n < 0 || n >= length xs = error ("cannot access index " ++ show n)
  | n == 0 = head xs
  | otherwise = indexAt (n - 1) (tail xs) -- otherwise 其实是一个被绑定为值 True 的普通变量
```

* 也可以写成如下形式

```hs
indexAt :: (Num a) => Int -> [a] -> a
indexAt n xs | n < 0 || n >= length xs = error ("cannot access index " ++ show n)
indexAt 0 xs = head xs
indexAt n xs = indexAt (n - 1) (tail xs)
```

* 对于异常情况，也可以使用 Maybe 类型来包裹，它既可以表示有值（Just）也可以表示空缺（Nothing）

```hs
indexAt :: Int -> [a] -> Maybe a
indexAt n xs
  | n < 0 || n >= length xs = Nothing
  | n == 0 = Just (head xs)
  | otherwise = indexAt (n - 1) (tail xs)

main :: IO ()
main = print (indexAt 2 [0, 1, 2], indexAt (-1) [0, 1, 2]) -- (Just 2,Nothing)
```

* 可以用 case 表达式进行模式匹配来提取 Maybe 的值

```hs
indexAt :: Int -> [a] -> Maybe a
indexAt n xs
  | n < 0 || n >= length xs = Nothing
  | n == 0 = Just (head xs)
  | otherwise = indexAt (n - 1) (tail xs)

fromMaybe :: p -> Maybe p -> p
fromMaybe defval maybe =
  case maybe of
    Nothing -> defval
    Just val -> val

a :: Integer
a = fromMaybe (-1) (indexAt 2 [0, 1, 2])

b :: Integer
b = fromMaybe (-1) (indexAt (-1) [0, 1, 2])

main :: IO ()
main = print (a, b) -- (2,-1)
```

## 局部变量

* 通过 let 区块引入局部变量，注意一个 let 区块中的每个变量的左侧缩进必须对齐

```hs
myAbs :: (Ord p, Num p) => p -> p
myAbs a =
  let zero = 0
      x = a
      y = - a
   in if a >= zero
        then x
        else y -- 惰性求值，只有条件不满足时才会计算 y

main :: IO ()
main = print (myAbs 1, myAbs (-1)) -- (1,1)
```

* 也可以用 where 子句引入局部变量，同样必须对齐左侧缩进

```hs
myAbs :: (Ord p, Num p) => p -> p
myAbs a =
  if a >= zero
    then x
    else y -- 惰性求值，只有条件不满足时才会计算 y
  where
    zero = 0
    x = a
    y = - a

main :: IO ()
main = print (myAbs 1, myAbs (-1)) -- (1,1)
```

* 使用显式语法结构可以解除缩进的限制，其中需要加上分号，但这种语法通常不会被使用

```hs
myAbs :: (Ord p, Num p) => p -> p
myAbs a =
  if a >= zero
    then x
    else y
  where { zero = 0; x = a;
    y = - a
  }

main :: IO ()
main = print (myAbs 1, myAbs (-1)) -- (1,1)
```

* let 区块可以嵌套

```hs
foo :: Integer
foo =
  let a = 1
   in let b = 2
       in a + b

main :: IO ()
main = print foo -- 3
```

* 存在同名变量时，内部区块的变量会屏蔽外部区块的变量

```hs
foo :: Integer
foo =
  let a = 1
   in let a = 2
       in a

main :: IO ()
main = print foo -- 2
```

* 同理，函数参数也会被屏蔽

```hs
foo :: p -> [Char] -- 参数可以是任意类型，因为它被内部的变量屏蔽了
foo a =
  let a = "hello "
   in a ++ "world"

main :: IO ()
main = print (foo 123) -- "hello world"
```

## 定义类型

* 用 data 关键字定义类型，类型的首字母必须大写

```hs
data PersonInfo = Person String Int -- PersonInfo 称为类型构造器，Person 称为值构造器，用 String 来表示名字，Int 表示年龄
  deriving (Show) -- 由类型类 Show 自动派生后，PersonInfo 类型的值可以转换为字符串来打印

myInfo :: PersonInfo
myInfo = Person "downdemo" 12

main :: IO ()
main = print myInfo -- Person "downdemo" 12
```

* 也可以载入 GHCi 来看出类型

```
Prelude> :l Main.hs
[1 of 1] Compiling Main             ( Main.hs, interpreted )
Ok, one module loaded.
*Main> myInfo
Person "downdemo" 12
*Main> :info PersonInfo
data PersonInfo = Person String Int     -- Defined at Main.hs:1:1
instance [safe] Show PersonInfo -- Defined at Main.hs:2:13
*Main> :t Person
Person :: String -> Int -> PersonInfo
```

* 类型构造器和值构造器可以使用相同的名字，两者是独立的

```hs
data Person = Person String Int
  deriving (Show)

myInfo :: Person
myInfo = Person "downdemo" 12

main :: IO ()
main = print myInfo -- Person "downdemo" 12
```

## 类型别名

* 使用 type 来定义类型别名，类型别名的目的是提高可读性，定义的别名与与原来的类型是相同的

```hs
type Name = String

type Age = Int

data Person = Person Name Age
  deriving (Show)

myInfo :: Person
myInfo = Person "downdemo" 12

main :: IO ()
main = print myInfo -- Person "downdemo" 12
```

## 代数数据类型（Algebraic Data Type）

* Bool 是最常见的代数数据类型，一个代数类型可以有多个值构造器

```hs
data Bool = False | True
```

* 有多个值构造器时，这些值构造器通常称为备选（alternatives）或分支（case），同一类型的所有备选创建的值的类型都是相同的

```hs
type Name = String

type Age = Int

type Country = String

type City = String

data PersonInfo
  = Person Name Age
  | Address Country City
  | Boy
  | Girl
  deriving (Show)

myInfo :: PersonInfo
myInfo = Person "downdemo" 12

yourInfo :: PersonInfo
yourInfo = Address "China" "Shanghai"

hisInfo :: PersonInfo
hisInfo = Boy

main :: IO ()
main = print (myInfo, yourInfo, hisInfo) -- (Person "downdemo" 12,Address "China" "Shanghai",Boy)
```

## 模式匹配（Pattern Matching）

* [1, 2, 3] 实际上只是 (1:(2:(3:[]))) 的一种简单的表示方式，其中冒号用于构造列表

```
Prelude> (1:(2:(3:[])))
[1,2,3]
Prelude> 1:2:3:[]
[1,2,3]
```

* 构造一个模式匹配冒号的求和函数

```hs
mySum :: Num p => [p] -> p
mySum (x : xs) = x + mySum xs
mySum [] = 0

main :: IO ()
main = print (mySum [1, 2, 3]) -- 6
```

* mySum 的计算过程为

```
mySum [1, 2, 3]
-> mySum 1:2:3:[]
-> 1 + mySum 2:3:[]
-> 1 + 2 + mySum 3:[]
-> 1 + 2 + 3 + mySum []
-> 1 + 2 + 3 + 0
```

* 构造一个值时也会对这个值进行模式匹配

```hs
data Person = Person String Int

person :: Person
person = Person "downdemo" 12 -- 模式匹配 Person String Int，String 绑定为 "downdemo"，Int 绑定为 12
```

* 模式匹配的过程就像是逆转一个值的构造（construction）过程，因此它有时候也被称为解构（deconstruction），如果所有给定等式的模式都匹配失败，那么返回一个运行时错误

```hs
data Person = Person String Int

person :: Person
person = Person "downdemo" -- 错误：Couldn't match expected type ‘Person’
```

* As-pattern，如果输入值匹配 @ 右侧模式，则输入值绑定到 @ 左侧的变量中

```hs
myTails :: [a] -> [[a]]
myTails xs@(_ : xs') = xs : myTails xs'
myTails [] = []

main :: IO ()
main = print (myTails [1, 2, 3]) -- [[1,2,3],[2,3],[3]]
```

## 通配符模式匹配

* 如果在匹配模式时，不关心某个值类型，则可以用下划线来表示，这个下划线称为通配符

```hs
data Person = Person String Int

name :: Person -> String
name (Person name _) = name

age :: Person -> Int
age (Person _ age) = age

person :: Person
person = Person "downdemo" 12

main :: IO ()
main = print (name person, age person) -- ("downdemo",12)
```

* 为一个类型写一组匹配模式时，必须穷举所有可能情况

```hs
mySum :: Num p => [p] -> p
mySum (x : xs) = x + mySum xs

main :: IO ()
main = print (mySum [1, 2, 3]) -- Non-exhaustive patterns in function mySum
```

* 利用通配符即可匹配所有未列出的模式

```hs
mySum :: Num p => [p] -> p
mySum (x : xs) = x + mySum xs
mySum _ = 0

main :: IO ()
main = print (mySum [1, 2, 3]) -- 6
```

## 记录语法（Record Syntax）

* 为类型的每个成分写一个访问器函数十分枯燥乏味

```hs
data Person = Person String Int

name :: Person -> String
name (Person name _) = name

age :: Person -> Int
age (Person _ age) = age
```

* 有一种更简单的等价写法，在定义数据类型时即可定义每个成分的访问器函数

```hs
data Person = Person
  { name :: String,
    age :: Int
  }
  deriving (Show)

person :: Person
person = Person "downdemo" 12

main :: IO ()
main = print (name person, age person) -- ("downdemo",12)
```

* 这种语法称为记录语法，它还允许为每个字段设置值，并且不关心字段顺序。此外用记录语法定义类型时，会改变类型的打印格式

```hs
data Person = Person
  { name :: String,
    age :: Int
  }
  deriving (Show)

person :: Person
person =
  Person
    { age = 12,
      name = "downdemo"
    }

main :: IO ()
main = print person -- Person {name = "downdemo", age = 12}
```

## 参数化类型

* 列表元素可以是任何类型，这就是一种多态。参数化类型的多态性相当于 C++ 模板的静态多态。Prelude 提供的 Maybe 就是一种多态类型

```hs
data Maybe a = Just a | Nothing
```

* 在 GHCi 中查看其类型

```
Prelude> :t Just 123
Just 123 :: Num a => Maybe a
Prelude> :t Just "aaa"
Just "aaa" :: Maybe [Char]
Prelude> :t Nothing
Nothing :: Maybe a
```

## 递归类型

* 创建一个 List a 类型，用 Cons 来替代冒号构造器，Nil 替代空列表，它与内置列表是同构的

```hs
data List a
  = Cons a (List a)
  | Nil
  deriving (Show)

-- fromList :: [a] -> List a
-- fromList (x : xs) = Cons x (fromList xs)
-- fromList [] = Nil

-- 使用 foldr 简化上述写法
fromList :: Foldable t => t a -> List a
fromList = foldr Cons Nil

main :: IO ()
main = print (fromList [1, 2, 3]) -- Cons 1 (Cons 2 (Cons 3 Nil))
```

* 二叉树也是一种递归类型

```hs
data Tree a
  = Node a (Tree a) (Tree a)
  | Empty
  deriving (Show)

simpleTree :: Tree [Char]
simpleTree =
  Node
    "root"
    (Node "l" Empty Empty)
    (Node "r" Empty Empty)

main :: IO ()
main = print simpleTree -- Node "root" (Node "l" Empty Empty) (Node "r" Empty Empty)
```

## 高阶函数（Higher Order Function）

* 接受一个函数作为参数，或返回一个函数作为结果的函数，称为高阶函数

### map

* map 定义如下

```hs
map :: (t -> a) -> [t] -> [a]
map f (x : xs) = f x : map f xs
map _ [] = []
```

* map 将接受的函数作用到列表的每个元素上

```hs
map (1 +) [1, 2, 3] -- [2,3,4]
map (^ 2) [1, 2, 3] -- [1,4,9]
map (\x -> x * x) [1, 2, 3]  -- [1,4,9]
```

### filter

* filter 定义如下

```hs
filter :: (a -> Bool) -> [a] -> [a]
filter p [] = []
filter p (x : xs)
  | p x = x : filter p xs
  | otherwise = filter p xs
```

* filter 筛选列表中满足条件的元素

```hs
filter odd [1 .. 9] -- [1,3,5,7,9]
```

* 实现快速排序

```hs
quickSort :: Ord a => [a] -> [a]
quickSort [] = []
quickSort (x : xs) = l ++ [x] ++ r
  where
    l = quickSort (filter (<= x) xs)
    r = quickSort (filter (> x) xs)

main :: IO ()
main = print (quickSort "downdemo") -- "ddemnoow"
```

### foldl

* foldl 定义如下，它将一个初始值和列表的首元素传给 step 函数，接着将上一次 step 的结果和列表下一个元素继续传给 step，直到遍历所有元素

```hs
foldl :: (t1 -> t2 -> t1) -> t1 -> [t2] -> t1
foldl step zero (x : xs) = foldl step (step zero x) xs
foldl _ zero [] = zero
```

* 用 foldl 实现一个对列表所有元素求和的函数

```hs
mySum :: [Integer] -> Integer
mySum = foldl step zero
  where
    step = (+)
    zero = 0

main :: IO ()
main = print (mySum [1, 2, 3]) -- 6
-- foldl (+) 0 (1 : 2 : 3 : [])
-- foldl (+) 1 (2 : 3 : [])
-- foldl (+) 3 (3 : [])
-- foldl (+) 6 []
-- 6
```

* 更直观的计算过程

```hs
foldlList :: [Char]
foldlList = foldl (\x y -> concat ["(", x, "+", y, ")"]) "0" (map show [1 .. 4])

main :: IO ()
main = putStrLn foldlList -- ((((0+1)+2)+3)+4)
```

* foldl 执行过程中会展开为完整的表达式，这个在显式要求求值前会保存在块中，对于数值计算这样的廉价操作，块保存表达式所需的计算量比直接求表达式值的计算量还多
* GHC 对块中表达式的求值在内部栈中进行，这个栈的容量有限，如果块中表达式过大，foldl 就会执行失败
* Data.List 定义了 foldl' 函数，它与 foldl 的作用类似，但不会创建块，实际代码中不要使用 foldl，可以用 Data.List 的 foldl' 来替代
* 非惰性求值的表达式称为严格的（strict），fold' 就是 foldl 的严格版本，它使用 seq 函数来绕过非严格求值

```hs
foldl' :: (t -> a -> t) -> t -> [a] -> t
foldl' _ zero [] = zero
foldl' step zero (x : xs) =
  let new = step zero x
   in new `seq` foldl' step new xs
```

* seq 强制对第一个参数求值，然后返回第二个参数

```hs
seq :: a -> b -> b
```

### flodr

* foldr 定义如下，它从右侧开始折叠

```hs
foldr :: (t1 -> t2 -> t2) -> t2 -> [t1] -> t2
foldr step zero (x : xs) = step x (foldr step zero xs)
foldr _ zero [] = zero
```

* 用 foldr 实现一个对列表所有元素求和的函数

```hs
mySum :: [Integer] -> Integer
mySum = foldr step zero
  where
    step = (+)
    zero = 0

main :: IO ()
main = print (mySum [1, 2, 3]) -- 6
-- foldr (+) 0 (1 : 2 : 3 : [])
-- (+) 1 (foldr (+) 0 (2 : 3 : []))
-- (+) 1 ((+) 2 (foldr (+) 0 (3 : [])))
-- (+) 1 ((+) 2 ((+) 3 (foldr (+) 0 [])))
-- (+) 1 ((+) 2 ((+) 3 0))
-- (+) 1 ((+) 2 3)
-- (+) 1 5
-- 6
```

* 更直观的计算过程

```hs
foldrList :: [Char]
foldrList = foldr ((\x y -> concat ["(", x, "+", y, ")"]) . show) "0" [1 .. 4]

main :: IO ()
main = putStrLn foldrList -- (1+(2+(3+(4+0))))
```

* 用 foldr 实现 map

```hs
myMap :: Foldable t1 => (t2 -> a) -> t1 t2 -> [a]
myMap f xs = foldr step [] xs
  where
    step x xs = f x : xs
```

* 用 foldr 实现 filter

```hs
myFilter :: Foldable t => (a -> Bool) -> t a -> [a]
myFilter p xs = foldr step [] xs
  where
    step x ys
      | p x = x : ys
      | otherwise = ys
```

* 柯里化后如下

```hs
myFilter :: Foldable t => (a -> Bool) -> t a -> [a]
myFilter p = foldr step []
  where
    step x xs
      | p x = x : xs
      | otherwise = xs
```

* 用 foldr 实现 foldl

```hs
myFoldl :: Foldable t1 => (t2 -> t3 -> t2) -> t2 -> t1 t3 -> t2
myFoldl f z xs = foldr step id xs z
  where
    step x g a = g (f a x)
```

* 用 foldr 实现 id

```hs
myId :: Foldable t => t a -> [a]
myId = foldr (:) []

main :: IO ()
main = print (myId [1, 2, 3]) -- [1,2,3]
```

* 用 foldr 实现 ++

```hs
append :: Foldable t => t a -> [a] -> [a]
append xs ys = foldr (:) ys xs

main :: IO ()
main = print (append [1, 2, 3] [4, 5, 6]) -- [1,2,3,4,5,6]
```

## 组合函数

* 可以用点组合多个函数

```hs
mapFilter :: (a -> b) -> (a -> Bool) -> [a] -> [b]
mapFilter p q = map p . filter q

main :: IO ()
main = print (mapFilter (\x -> x * x) odd [1, 2, 3, 4, 5, 6]) -- [1,9,25]
```

* 相当于

```hs
compose :: (b -> c) -> (a -> b) -> a -> c
compose f g x = f (g x)

mapFilter :: (a -> b) -> (a -> Bool) -> [a] -> [b]
mapFilter p q = compose (map p) (filter q)

main :: IO ()
main = print (mapFilter (\x -> x * x) odd [1, 2, 3, 4, 5, 6]) -- [1,9,25]
```

## 类型类（Typeclass）

* 类型类定义了一系列函数的签名，这些函数的实现定义在实例中

```hs
class A a where -- A 是一个类型类
  isEqual :: a -> a -> Bool

data Color = Red | Blue | Green

instance A Color where --  定义基于 Color 的 A 的实例
  isEqual Red Red = True
  isEqual Blue Blue = True
  isEqual Green Green = True
  isEqual _ _ = False

main :: IO ()
main = print (isEqual Red Blue) -- False
```

* 如果可以从一个函数计算出其他函数，实现每个函数就显得十分冗余

```hs
class A a where
  isEqual :: a -> a -> Bool
  isNotEqual :: a -> a -> Bool

instance A Bool where
  isEqual True True = True
  isEqual False False = True
  isEqual _ _ = False
  isNotEqual True False = True
  isNotEqual False True = True
  isNotEqual _ _ = False

main :: IO ()
main = print (isNotEqual True False) -- True
```

* 对于这种情况，可以在类型类中提供函数的默认实现，这样在实例中定义一个函数就可以生成另一个

```hs
class A a where
  isEqual :: a -> a -> Bool
  isEqual x y = not (isNotEqual x y)

  isNotEqual :: a -> a -> Bool
  isNotEqual x y = not (isEqual x y)

instance A Bool where
  isEqual True True = True
  isEqual False False = True
  isEqual _ _ = False

main :: IO ()
main = print (isNotEqual True False) -- True
```

### [Eq](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-Eq.html#t:Eq)

* Eq 是相等类型类

```hs
data Color = Red | Blue | Green
  deriving (Eq)

main :: IO ()
main = print (Red == Blue) -- False
```

* Eq 定义为

```hs
class Eq a where
  (==) :: a -> a -> Bool
  (/=) :: a -> a -> Bool
  x == y = not (x /= y)
  x /= y = not (x == y)
  {-# MINIMAL (==) | (/=) #-}
```

* 有序类型类 [Ord](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-Ord.html) 是基于 Eq 的

```hs
class (Eq a) => Ord a where
  compare :: a -> a -> Ordering
  (<), (<=), (>), (>=) :: a -> a -> Bool
  max, min :: a -> a -> a
  compare x y =
    if x == y
      then EQ
      else
        if x <= y
          then LT
          else GT
  x < y = case compare x y of LT -> True; _ -> False
  x <= y = case compare x y of GT -> False; _ -> True
  x > y = case compare x y of GT -> True; _ -> False
  x >= y = case compare x y of LT -> False; _ -> True
  max x y = if x <= y then y else x
  min x y = if x <= y then x else y
  {-# MINIMAL compare | (<=) #-}
```

### [Show](https://hackage.haskell.org/package/base-4.15.0.0/docs/GHC-Show.html#t:Show)

* Show 类型类将值转换为字符串，show 是它的一个函数

```
Prelude> :t show
show :: Show a => a -> String
Prelude> show [1, 2, 3]
"[1,2,3]"
Prelude> show (1, 2, 3)
"(1,2,3)"
Prelude> putStrLn (show 1)
1
Prelude> putStrLn (show [1, 2, 3])
[1,2,3]
```

* String 类型也可以使用 show

```
Prelude> show "hello"
"\"hello\""
Prelude> putStrLn "\"hello\""
"hello"
Prelude> putStrLn "hello"
hello
```

* 定义 Show 的实例

```hs
data Color = Red | Blue | Green

instance Show Color where
  show Red = "Red"
  show Blue = "Blue"
  show Green = "Green"

main :: IO ()
main = print Red -- Red
```

* 对于简单的数据类型，可以用 deriving 自动派生

```hs
data Color = Red | Blue | Green
  deriving (Show)

main :: IO ()
main = print Red -- Red
```

* 可以手动指定不同于默认值的结果

```hs
data Color = Red | Blue | Green

instance Show Color where
  show Red = "Color Red"
  show Blue = "Color Blue"
  show Green = "Color Green"

main :: IO ()
main = print Red -- Color Red
```

* 可以用 newtype 包裹类型，它的目的是使用包裹类型，但隐藏包裹类型实现的类型类

```hs
newtype MyNumber = N Double
  deriving (Show) -- Double 实现了 Show 类型类，但 MyNumber 没有，需要自动派生

main :: IO ()
main = print (N 3.14) -- N 3.14
```

* 和 data 不同的是，newtype 只能有一个值构造器，且构造器只能有一个参数

```hs
data Color = Red | Blue | Green -- 多个值构造器
  deriving (Show)

newtype Person = P (String, Int) -- 只有一个值构造器，构造器只有一个元组类型的参数
  deriving (Show)

main :: IO ()
main = print (P ("downdemo", 12)) -- P ("downdemo",12)
```

### [Read](https://hackage.haskell.org/package/base-4.15.0.0/docs/GHC-Read.html)

* Read 类型将字符串解析为值，read 是它的一个函数

```
Prelude> :t read
read :: Read a => String -> a
```

* 由于 read 会返回任意类型值，因此需要显式指定返回类型，要注意指定的类型必须匹配解析结果

```
Prelude> read "3"
*** Exception: Prelude.read: no parse
Prelude> read "3" :: Int
3
Prelude> :t it
it :: Int
Prelude> read "3" :: Double
3.0
Prelude> :t it
it :: Double
Prelude> read "3.14" :: Int
*** Exception: Prelude.read: no parse
```

### [Functor](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-Functor.html)

* Functor 即函子，范畴间的一类映射，简单来说就是范畴间的同态

```hs
fmap :: Functor f => (a -> b) -> f a -> f b
```

* List 和 Maybe 都是 Functor

```hs
fmap (* 3) [1, 2, 3] -- [3,6,9]
fmap (* 3) (Just 3) -- Just 9
```

### [Applicative](https://hackage.haskell.org/package/base-4.15.0.0/docs/Control-Applicative.html)

* Applicative 即可应用函子

```hs
class Functor f => Applicative f where
    pure :: a -> f a
    (<*>) :: f (a -> b) -> f a -> f b
```

* List 和 Maybe 都是 Applicative

```hs
[(* 3)] <*> [1, 2, 3] -- [3,6,9]
[(* 3), (+ 1)] <*> [1, 2, 3] -- [3,6,9,2,3,4]
Just (* 3) <*> Just 1 -- Just 3
Just (+) <*> Just 1 <*> Just 2 -- Just 3
Just (+ 3) <*> Nothing -- Nothing
```

* 为了方便，Control.Applicative 会 export 一个函数 `<$>`

```hs
(<$>) :: (Functor f) => (a -> b) -> f a -> f b
f <$> x = fmap f x
```

* 使用 `<$>`

```hs
(* 3) <$> [1, 2, 3] -- [3,6,9]
(++) <$> ["A", "B", "C"] <*> ["a", "b", "c"] -- ["Aa","Ab","Ac","Ba","Bb","Bc","Ca","Cb","Cc"]
(* 3) <$> Just 1 -- Just 3
(+) <$> Just 1 <*> Just 2 -- Just 3
```

### [Monoid](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-Monoid.html)

* Monoid 即幺半群，指一个带有可结合二元运算和单位元的代数结构

```hs
class Semigroup a => Monoid a where
    mempty  :: a
    mappend :: a -> a -> a
    mappend = (<>)
    mconcat :: [a] -> a
    mconcat = foldr mappend mempty
```

* List 和 Maybe 都是 Monoid

```hs
[1, 2, 3] `mappend` [4, 5, 6] -- [1,2,3,4,5,6]
(<>) [1, 2, 3] [4, 5, 6] -- [1,2,3,4,5,6]
(<>) [1, 2, 3] mempty -- [1,2,3]
[1, 2] `mappend` [3, 4] `mappend` [5, 6] -- [1,2,3,4,5,6]
([1, 2] `mappend` [3, 4]) `mappend` [5, 6] -- [1,2,3,4,5,6]
[1, 2] `mappend` ([3, 4] `mappend` [5, 6]) -- [1,2,3,4,5,6]
mconcat [[1, 2], [3], [4, 5, 6]] -- [1,2,3,4,5,6]
Just "A" `mappend` Just "B" `mappend` Just "C" -- Just "ABC"
(Just "A" `mappend` Just "B") `mappend` Just "C" -- Just "ABC"
Just "A" `mappend` (Just "B" `mappend` Just "C") -- Just "ABC"
Just "A" `mappend` Nothing `mappend` Just "C" -- Just "AC"
```

### [Monad](https://hackage.haskell.org/package/base-4.15.0.0/docs/Control-Monad.html#t:Monad)

* Monad 即自函子范畴上的幺半群

```hs
class Applicative m => Monad m where
  (>>=) :: forall a b. m a -> (a -> m b) -> m b
  (>>) :: forall a b. m a -> m b -> m b
  m >> k = m >>= \_ -> k
  return :: a -> m a
  return = pure
```

* List 和 Maybe 都是 Monad

```hs
[1, 2, 3] >>= \x -> return (x * 3) -- [3,6,9]
[1, 2, 3] >>= \x -> [x, - x] -- [1,-1,2,-2,3,-3]
Just 1 >>= \x -> return (x * 3) -- Just 3
Just 1 >>= \x -> Just (x, - x) -- Just (1,-1)
```

* 串联多个 Monad

```hs
a :: (Monad m, Num a) => a -> m a
a x = return (x ^ 2)

b :: (Monad m, Num a) => a -> m a
b x = return (x + 1)

c :: (Monad m, Num a) => a -> m a
c x = return (x * 3)

main :: IO ()
main = print ([1, 2, 3] >>= a >>= b >>= c) -- [6,15,30]
```

* 对于多层嵌套

```hs
Just "A" >>= (\x -> Just "B" >>= (\y -> Just (x ++ y))) -- Just "AB"
```

* 可以用 do 表示法来简化

```hs
foo :: Maybe [Char]
foo = do
  x <- Just "A"
  y <- Just "B"
  Just (x ++ y)

main :: IO ()
main = print foo -- Just "AB"
```

## 编写 JSON 库

* 创建 SimpleJSON.hs 如下

```hs
module SimpleJSON
  ( JValue (..),
    isNull,
    getBool,
    getInt,
    getDouble,
    getString,
    getArray,
    getObject,
  )
where

data JValue
  = JNull
  | JBool Bool
  | JNumber Double
  | JString String
  | JArray [JValue]
  | JObject [(String, JValue)]
  deriving (Eq, Ord, Show)

isNull :: JValue -> Bool
isNull v = v == JNull

getBool :: JValue -> Maybe Bool
getBool (JBool b) = Just b
getBool _ = Nothing

getInt :: Integral a => JValue -> Maybe a
getInt (JNumber n) = Just (truncate n) -- truncate 返回浮点数的整数部分
getInt _ = Nothing

getDouble :: JValue -> Maybe Double
getDouble (JNumber n) = Just n
getDouble _ = Nothing

getString :: JValue -> Maybe String
getString (JString s) = Just s
getString _ = Nothing

getArray :: JValue -> Maybe [JValue]
getArray (JArray a) = Just a
getArray _ = Nothing

getObject :: JValue -> Maybe [(String, JValue)]
getObject (JObject o) = Just o
getObject _ = Nothing
```

* 载入 GHCi

```
Prelude> :l SimpleJSON
[1 of 1] Compiling SimpleJSON       ( SimpleJSON.hs, interpreted )
Ok, one module loaded.
*SimpleJSON> JNull
JNull
*SimpleJSON> isNull JNull
True
*SimpleJSON> JBool True
JBool True
*SimpleJSON> getBool (JBool True)
Just True
*SimpleJSON> JNumber 3.14
JNumber 3.14
*SimpleJSON> isNull (JNumber 3.14)
False
*SimpleJSON> getInt (JNumber 3.14)
Just 3
*SimpleJSON> getDouble (JNumber 3.14)
Just 3.14
*SimpleJSON> JString "downdemo"
JString "downdemo"
*SimpleJSON> getString (JString "downdemo")
Just "downdemo"
*SimpleJSON> getString (JNumber 3.14)
Nothing
*SimpleJSON> JArray [JString "C++", JString "Haskell"]
JArray [JString "C++",JString "Haskell"]
getArray (JArray [JString "C++", JString "Haskell"])
Just [JString "C++",JString "Haskell"]
*SimpleJSON> JObject [("name", JString "downdemo"), ("age", JNumber 12)]
JObject [("name",JString "downdemo"),("age",JNumber 12.0)]
*SimpleJSON> getObject (JObject [("name", JString "downdemo"), ("age", JNumber 12)])
Just [("name",JString "downdemo"),("age",JNumber 12.0)]
```

* 创建 PutJSON.hs 如下，用于打印 JSON 数据

```hs
module PutJSON where

import Data.List (intercalate)
import SimpleJSON (JValue (..))

renderJValue :: JValue -> String
renderJValue JNull = "null"
renderJValue (JBool True) = "true"
renderJValue (JBool False) = "false"
renderJValue (JNumber n) = show n
renderJValue (JString s) = show s
renderJValue (JArray a) = "[" ++ values a ++ "]"
  where
    values [] = ""
    values vs = intercalate ", " (map renderJValue vs)
renderJValue (JObject o) = "{" ++ pairs o ++ "}"
  where
    pairs [] = ""
    pairs ps = intercalate ", " (map renderPair ps) -- intercalate 将列表中的元素连接为字符串，以分隔符隔开
    renderPair (k, v) = show k ++ ": " ++ renderJValue v

putJValue :: JValue -> IO ()
putJValue v = putStrLn (renderJValue v)
```

* 载入 GHCi

```
Prelude> :l PutJSON
[1 of 2] Compiling SimpleJSON       ( SimpleJSON.hs, interpreted )
[2 of 2] Compiling PutJSON          ( PutJSON.hs, interpreted )
Ok, two modules loaded.
*PutJSON> putJValue JNull
null
*PutJSON> putJValue (JBool True)
true
*PutJSON> putJValue (JNumber 3.14)
3.14
*PutJSON> putJValue (JString "downdemo")
"downdemo"
*PutJSON> putJValue (JArray [JString "C++", JString "Haskell"])
["C++", "Haskell"]
*PutJSON> putJValue (JObject [("name", JString "downdemo"), ("age", JNumber 12)])
{"name": "downdemo", "age": 12.0}
```

* 为了能自定义打印 JSON 的格式，创建 PrettyJSON.hs 如下，它将 JValue 类型转换为 Doc 类型

```hs
module PrettyJSON
  ( renderJValue,
  )
where

import Prettify (Doc, double, series, string, text, (<>))
import SimpleJSON (JValue (..))
import Prelude hiding ((<>))

renderJValue :: JValue -> Doc
renderJValue JNull = text "null"
renderJValue (JBool True) = text "true"
renderJValue (JBool False) = text "false"
renderJValue (JNumber num) = double num
renderJValue (JString str) = string str
renderJValue (JArray ary) = series '[' ']' renderJValue ary
renderJValue (JObject obj) = series '{' '}' field obj
  where
    field (name, val) =
      string name
        <> text ": "
        <> renderJValue val
```

* 转换函数在 Prettify.hs 中实现

```hs
module Prettify where

import Data.Bits (shiftR, (.&.))
import Data.Char (ord)
import Numeric (showHex)
import Prelude hiding ((<>))

-- Doc 是树，Concat 和 Union 以两个 Doc 值构造内部节点，Line 表示一个换行，其他构造器构造叶子节点
data Doc
  = Empty
  | Char Char
  | Text String
  | Line
  | Concat Doc Doc
  | Union Doc Doc
  deriving (Show, Eq)

empty :: Doc
empty = Empty

char :: Char -> Doc
char = Char

text :: String -> Doc
text "" = Empty
text s = Text s

double :: Double -> Doc
double d = text (show d)

line :: Doc
line = Line

string :: String -> Doc
string = enclose '"' '"' . hcat . map oneChar

enclose :: Char -> Char -> Doc -> Doc
enclose left right x = char left <> x <> char right

(<>) :: Doc -> Doc -> Doc
Empty <> y = y
x <> Empty = x
x <> y = x `Concat` y

-- hcat 将多个 Doc 值连接起来
-- 类比于
-- concat :: [[a]] -> [a]
-- concat = foldr (++) []
hcat :: [Doc] -> Doc
hcat = fold (<>)

fold :: (Doc -> Doc -> Doc) -> [Doc] -> Doc
fold f = foldr f empty

oneChar :: Char -> Doc
oneChar c = case lookup c simpleEscapes of
  Just r -> text r
  Nothing
    | mustEscape c -> hexEscape c
    | otherwise -> char c
  where
    mustEscape c = c < ' ' || c == '\x7f' || c > '\xff'

-- 即 [('\b',"\\b"),('\n',"\\n"),('\f',"\\f"),('\r',"\\r"),('\t',"\\t"),('\\',"\\\\"),('"',"\\\""),('/',"\\/")]
simpleEscapes :: [(Char, String)]
simpleEscapes = zipWith ch "\b\n\f\r\t\\\"/" "bnfrt\\\"/"
  where
    ch a b = (a, ['\\', b])

hexEscape :: Char -> Doc
hexEscape c
  | d < 0x10000 = smallHex d
  | otherwise = astral (d - 0x10000)
  where
    d = ord c

-- smallHex 提供的4位数字编码仅能够表示 0xffff 范围之内的 Unicode 字符
smallHex :: Int -> Doc
smallHex x =
  text "\\u"
    <> text (replicate (4 - length h) '0')
    <> text h
  where
    h = showHex x "" -- showHex 定义于 Numeric，返回 x 的十六进制表示

-- 合法的 Unicode 字符范围可达 0x10ffff
astral :: Int -> Doc
astral n = smallHex (a + 0xd800) <> smallHex (b + 0xdc00)
  where
    a = (n `shiftR` 10) .&. 0x3ff -- shiftR 和 .&. 定义于 Data.Bits，前者把 n 右移 10 位，后者按位与
    b = n .&. 0x3ff

-- series 用于将 JArray 和 JObject 转化为 Doc
series :: Char -> Char -> (a -> Doc) -> [a] -> Doc
series open close f =
  enclose open close
    . fsep
    . punctuate (char ',')
    . map f

-- fsep 将多个 Doc 值拼接成一个
fsep :: [Doc] -> Doc
fsep = fold (</>)

(</>) :: Doc -> Doc -> Doc
x </> y = x <> softline <> y

-- softline 在行过长时插入一个新行，否则插入一个空格
softline :: Doc
softline = group line

group :: Doc -> Doc
group x = flatten x `Union` x

-- flatten 将 Line 替换为空格，把两行变成一行
flatten :: Doc -> Doc
flatten (x `Concat` y) = flatten x `Concat` flatten y
flatten Line = Char ' '
flatten (x `Union` _) = flatten x
flatten other = other

punctuate :: Doc -> [Doc] -> [Doc]
punctuate p [] = []
punctuate p [d] = [d]
punctuate p (d : ds) = (d <> p) : punctuate p ds

-- compact 提供一种紧凑转换
compact :: Doc -> String
compact x = transform [x]
  where
    transform [] = ""
    transform (d : ds) =
      case d of
        Empty -> transform ds
        Char c -> c : transform ds
        Text s -> s ++ transform ds
        Line -> '\n' : transform ds
        a `Concat` b -> transform (a : b : ds)
        _ `Union` b -> transform (b : ds)

-- pretty 比 compact 多了一个表示每行的最大宽度的参数，使打印结果更美观
pretty :: Int -> Doc -> String
pretty width x = best 0 [x]
  where
    best col (d : ds) = case d of
      Empty -> best col ds
      Char c -> c : best (col + 1) ds
      Text s -> s ++ best (col + length s) ds
      Line -> '\n' : best 0 ds
      a `Concat` b -> best col (a : b : ds)
      a `Union` b ->
        nicest
          col
          (best col (a : ds))
          (best col (b : ds))
    best _ _ = ""
    nicest col a b
      | (width - least) `fits` a = a
      | otherwise = b
      where
        least = min width col

-- fits 确定某一个已经被转换的 Doc 值能否放进给定宽度
fits :: Int -> String -> Bool
w `fits` _ | w < 0 = False
w `fits` "" = True
w `fits` ('\n' : _) = True
w `fits` (c : cs) = (w - 1) `fits` cs
```

* 创建 Main.hs 如下，运行 runhaskell Main.hs

```hs
module Main (main) where

import Prettify (Doc, compact, pretty)
import PrettyJSON (renderJValue)
import PutJSON (putJValue)
import SimpleJSON (JValue (JArray, JBool, JNumber, JObject, JString))

x :: JValue
x = JObject [("name", JString "downdemo"), ("age", JNumber 12), ("pl", JArray [JString "C++", JString "Haskell"])]

y :: Doc
y = PrettyJSON.renderJValue x

main :: IO ()
main = do
  print x
  putStrLn "------------------------------------"
  PutJSON.putJValue x
  putStrLn "------------------------------------"
  putStrLn (Prettify.compact y) -- 紧凑打印
  putStrLn "------------------------------------"
  putStrLn (Prettify.pretty 30 y) -- 每行不超过 30 个字符
  putStrLn "------------------------------------"
  putStrLn (Prettify.pretty 100 y)

{-
JObject [("name",JString "downdemo"),("age",JNumber 12.0),("pl",JArray [JString "C++",JString "Haskell"])]
------------------------------------
{"name": "downdemo", "age": 12.0, "pl": ["C++", "Haskell"]}
------------------------------------
{"name": "downdemo",
"age": 12.0,
"pl": ["C++",
"Haskell"
]
}
------------------------------------
{"name": "downdemo",
"age": 12.0, "pl": ["C++",
"Haskell" ] }
------------------------------------
{"name": "downdemo", "age": 12.0, "pl": ["C++", "Haskell" ] }
-}
```
