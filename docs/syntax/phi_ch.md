# Phi Script

在 PhiScript 中，有几个基本的理念：

1. 一切皆是表达式
2. 一切皆是引用
3. ~~一切皆是左结合（已知缺陷）~~

## 表达式

表达式由运算分量和运算符组成。

### 一元表达式

语法：

> _operator_ operand

已有的运算符：

| 运算符 | 解释     |
| ------ | -------- |
| -      | 单目减法 |
| !      | 逻辑取反 |
| not    | 逻辑取反 |
| ~      | 按位取反 |
| @      | 复制     |
| @@     | 深度复制 |
| ++     | 前缀自增 |
| --     | 前缀自减 |

### 二元表达式

语法：

> operand1 _operator_ operand2

对于表达式：`a op b`

| 运算符 | 解释       | 结果      |
| ------ | ---------- | --------- |
| +      | 值加法     | a+b       |
| -      | 值减法     | a-b       |
| \*     | 值乘法     | a\*b      |
| /      | 值除法     | a/b       |
| %      | 值求模     | a%b       |
| \*\*   | 值求幂     | pow(a, b) |
| &      | 按位与     | a&b       |
| \|     | 按位与     | a\|b      |
| ^      | 按异或     | a^b       |
| <<     | 按位左移   | a<<b      |
| >>     | 按位右移   | a>>b      |
| &&     | 逻辑与     | a&&b      |
| and    | 逻辑与     | a&&b      |
| \|\|   | 逻辑或     | a\|\|b    |
| or     | 逻辑或     | a\|\|b    |
| =      | 赋值       | b         |
| <      | 值小于     | a<b       |
| <=     | 值小于等于 | a<=b      |
| >      | 值大于     | a>b       |
| >=     | 值大于等于 | a>=b      |
| ==     | 值等于     | a==b      |
| !=     | 值不等     | a!=b      |

### 块表达式

语义同其他语言。

块也是表达式，语法如下：

```phi
{
    statements
}
```

块的默认值为 Null。

### Eval 语法

可以通过使用`eval`关键字来设定块的值，语法：`eval expr;`。  
它的作用是将当前块的值设置为 expr 的值。

### 语句表达式

语句也是表达式，也就是说，**语句也是有值的**。

#### if 语句

语义同其他语言。

if 语句的语法如下：

```phi
if (condition)
    expr
```

if 语句值为 expr 的值。

#### if else 语句

语义同其他语言。

if else 语句的语法如下（其中，condition，expr1，expr2 都是表达式）：

```phi
if (condition)
    expr1
else
    expr2
```

if else 语句的值为 expr1 或 expr2 的值。

由此，可以写出如下嵌套语句：

```phi
if (c1)
    e1
else if (c2)
    e2
else
    e3
```

注意，如果要对以上语句使用 eval，请在第二个 if 前添加 eval，具体如下：

```phi
if (c1)
    eval e1
else eval if (c2)
    eval e2
else
    eval e3
```

因为 if，else 嵌套的本质是以下的代码：

```phi
if (c1)
    e1
else
    if (c2)
        e2
    else
        e3
```

因此，如果想让第一个 else 的值为 e2 或者 e3，要在第二个 if 之前加上 eval，来使第一个 else 的值为 e2 或 e3，最终到达整个 if else 语句。

#### for 语句

语义同其他语言。

语法如下（其中，init，test，update，body 为表达式）：

```phi
for (init; test; update)
    body
```

for 语句的值为 body 的值。

#### for else 语句

for else 语义为当 for 语句未成功进入 body 时执行 elseBody，相当于如下语句：

```phi
{
    init
    if (test)
        for (init; test; update)
            body
    else
        elseBody
}
```

语法如下（其中，init，test，update，body, elseBody 为表达式）：

```phi
for (init; test; update)
    body
else
    elseBody
```

for else 语句的值为 body 或 elseBody 的值。

#### while 语句

语义同其他语言。

语法如下（其中，test，body 为表达式）：

```phi
while (test)
    body
```

while 语句的值为 body 的值。

#### while else 语句

while else 语义为当 while 语句未成功进入 body 时执行 elseBody，相当于如下语句：

```phi
if (test)
    while (test)
        body
else
    elseBody
```

语法如下（其中，test，body, elseBody 为表达式）：

```phi
while (test)
    body
else
    elseBody
```

while else 语句的值为 body 或 elseBody 的值。

#### Loop with tags

for、while 循环都可以带 tag，tag 可以是一个标识符，也可以是一个字符串，语法如下：

```
for: tag
while: tag
```

其余部分遵循其对应的语句语法，该语法仅仅是为其打下标签。

#### break | continue 语句

语义同其他语言。  
语法如下：

```
continue
break
```

可以带上标签，以作用于指定循环：

```
continue tag
break tag
```

#### 函数语句
语法（fn、func、function均为函数关键字， 以下统一用fn）：
```
fn func_name(arg1, arg2, arg3, ...)
    body
```
fn语句的值是其描述的函数，其中，func_name可省。

#### 函数闭包
语法：
```
fn[x1: e1, x2: e2, ...] (arg1, arg2, arg3, ...)
    body
```
该函数会将捕获列表（即`[]`中的内容）中的表达式作为函数的内部参数，其中，$x_n$是键，$e_n$是值， 也就是说，函数内部可以直接使用$x_n$作为变量。  
当$x_n$与$e_n$在表达式上完全相同且两边都是一个标识符时，可以直接简写为一个标识符，其语义也可以理解为捕获外部的指定变量。  
对于捕获列表中的$x_n$，还可以使用`this`标识符，按如下形式访问：`this.xxx`。


#### 函数递归
由于实现问题，函数内部环境与外部隔离，若要调用其他函数或者使用外部变量，需要使用函数闭包。
如果要调用自身以实现递归，请使用`this`标识符。
例如以下例子：
```phi
fn fib(n)
{
    if (n <= 2) return 1;
    return this(n - 1) + this(n - 2);
}
```


#### lambda表达式
lambda表达式是函数的简写形式，语法如下：
```phi
(arg1, arg2, arg3, ...) => body
```
当参数列表只有一个参数时，括号可省：
```phi
arg => body
```
当body是单个表达式时，会自动将body的值作为该函数顶点返回值，例如：
```phi
(a, b) => a + b
```
所得到的函数相当于如下函数：
```
fn (a, b)
    return a + b
```