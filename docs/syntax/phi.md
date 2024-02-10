# Phi Script

In PhiScript, there are several basic ideas:

1. Everything is an expression
2. Everything is a quote
3. ~~Everything is left binding (known defect)~~

## expression

An expression consists of an operational component and an operator.

### unary expression

Syntax:

> _operator_ operand

Existing operators:

| The operator | explains                      |
| ------------ | ----------------------------- |
| -            | monocular subtraction         |
| !            | is the opposite of            |
| not          | is the inverse of             |
| ~            | is replaced by                |
| @            | copy                          |
| @@           | Deep copy                     |
| ++           | The prefix of is increased by |
| --           | The prefix of is reduced by   |

### binary expression

Syntax:

> operand1 _operator_ operand2

For expression: 'a op b'

| The   | operator                              | interprets the result of |
| ----- | ------------------------------------- | ------------------------ |
| +     | adds the value                        | a+b                      |
| -     | subtraction                           | a-b                      |
| \*    | Value multiplication                  | a\*b                     |
| /     | Value division                        | a/b                      |
| %     | evaluation module                     | a%b                      |
| \*\*  | raised to the power                   | pow(a, b)                |
| &     | in bits is associated with            | a&b                      |
| \|    | is in place with                      | a\|b                     |
| ^     | or                                    | a^b                      |
| <<    | Move to the left                      | a<<b                     |
| >>    | Click and move to the right           | a>>b                     |
| &&    | Logic and                             | a& b                     |
| and   | logic is the same as                  | a& b                     |
| \| \| | logic or                              | a\|\|b                   |
| or    | logic or                              | a\|\|b                   |
| =     | assigns                               | b                        |
| <     | is less than                          | a<b                      |
| <=    | is less than or equal to              | a<=b                     |
| >     | is greater than                       | a>b                      |
| >=    | The value is greater than or equal to | a>=b                     |
| ==    | The value                             | a==b                     |
| ! =   | not equal to                          | a!=b                     |

### block expression

Semantically similar to other languages.

Blocks are also expressions, and the syntax is as follows:

```phi
{
statements
}
```

The default value of the block is Null.

### Eval syntax

You can set the value of a block by using the 'eval' keyword, syntax: 'eval expr; '.
It sets the value of the current block to the value of expr.

### statement expression

Statements are also expressions, that is, ** statements are also valuable **.

#### if statement

Semantically similar to other languages.

The syntax of the if statement is as follows:

```phi
if (condition)
    expr
```

The if statement value is the value of expr.

#### if else statement

Semantically similar to other languages.

The syntax of the if else statement is as follows (where condition, expr1, expr2 are expressions) :

```phi
if (condition)
    expr1
else
    expr2
```

The value of the if else statement is the value of expr1 or expr2.

From this, you can write the following nested statement:

```phi
if (c1)
    e1
else if (c2)
    e2
else
    e3
```

Note that if you want to use eval on the above statement, add eval before the second if as follows:

```phi
if (c1)
    eval e1
else eval if (c2)
    eval e2
else
    eval e3
```

Because the essence of if, else nesting is the following code:

```phi
if (c1)
    e1
else
    if (c2)
        e2
    else
        e3
```

Therefore, if you want the first else to be e2 or e3, you add eval before the second if to make the first else be e2 or e3, and finally reach the entire if else statement.

#### for statement

Semantically similar to other languages.

The syntax is as follows (where init, test, update, body are expressions) :

```phi
for (init; test; update)
    body
```

The value of the for statement is the value of body.

#### for else statement

The for else semantics means that elseBody is executed when the for statement fails to enter the body, equivalent to the following statement:

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

The syntax is as follows (where init, test, update, body, elseBody are expressions) :

```phi
for (init; test; update)
    body
else
    elseBody
```

The value of the for else statement is the value of body or elseBody.

#### while statement

Semantically similar to other languages.

The syntax is as follows (where test and body are expressions) :

```phi
while (test)
    body
```

The value of the while statement is the value of the body.

#### while else statement

The while else semantics is to execute elseBody when the while statement fails to enter the body, which is equivalent to the following statement:

```phi
if (test)
    while (test)
        body
else
    elseBody
```

The syntax is as follows (where test, body, and elseBody are expressions) :

```phi
while (test)
    body
else
    elseBody
```

The value of the while else statement is the value of body or elseBody.

#### Loop with tags

Both for and while loops can carry a tag, which can be either an identifier or a string. The syntax is as follows:

```
for: tag
while: tag
```

The rest follows its corresponding statement syntax, which merely labels it.

#### break | continue statement

Semantically similar to other languages.
The syntax is as follows:

```
continue
break
```

Labels can be attached to apply to a specified loop:

```
continue tag
break tag
```

#### function statement

Syntax (fn, func, function are function keywords, the following unified fn) :

```
fn func_name(arg1, arg2, arg3, ...)
    body
```

The value of the fn statement is the function it describes, where func_name can be reduced.

#### function closure

Syntax:

```
fn[x1: e1, x2: e2, ...]  (arg1, arg2, arg3, ...)
    body
```

The function takes an expression from the capture list (i.e., the contents of '[]') as an internal argument to the function, where $x_n$ is the key and $e_n$ is the value, that is, $x_n$ can be used as a variable directly inside the function.  
When $x_n$ and $e_n$ are identical in expression and both sides are an identifier, it can be shortened directly to an identifier, and its semantics can also be understood as capturing external specified variables.  
For $x_n$ in the capture list, you can also use the 'this' identifier, accessed as follows:' this.xxx '.

#### function recursion

Due to implementation issues, the internal environment of a function is isolated from the outside, and to call other functions or use external variables, you need to use a function closure.  
If you want to call yourself to achieve recursion, use the 'this' identifier.  
Take the following example:

```phi
fn fib(n)
{
    if (n <= 2) return 1;
    return this(n - 1) + this(n - 2);
}
```

#### lambda expression

A lambda expression is a short form of a function with the following syntax:

```phi
(arg1, arg2, arg3, ...)  => body
```

When the argument list has only one argument, parentheses can be saved:

```phi
arg => body
```

When body is a single expression, the value of body is automatically returned as the vertex value of the function, for example:

```phi
(a, b) => a + b
```

The resulting function is equivalent to the following:

```
fn (a, b)
    return a + b
```

```

```
