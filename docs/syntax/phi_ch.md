Phi Script

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

块也可以有值，语法如下：

```phi
{
    statements
}
```

### Eval 语法

可以通过使用`eval`关键字来设定块的值，语法：`eval expr;`。  
它的作用是将当前块的值设置为 expr 的值。

### 语句表达式

语句也是表达式，也就是说，**语句也是有值的**。

#### if 语句

if 语句的语法如下：

```phi
if (condition)
    expr
```

if 语句值为 expr 的值（如果 expr 中带 eval 语句 的话）

#### if else 语句

if else 语句的语法如下：

```phi
if (condition)
    expr1
else
    expr2
```

if else 语句的值为 expr1 或 expr2 的值（如果带有 eval 语句）

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

因此，如果想让第一个else的值为e2或者e3，要在第二个 if 之前加上eval，来使第一个else的值为e2或e3，最终到达整个if else语句。
