# φPhiScript

This document provides an overview of how to use the φPhiScript programming language.

## Contents

- [φPhiScript](#φphiscript)
  - [Contents](#contents)
  - [Basic Usage](#basic-usage)
    - [Importing Modules](#importing-modules)
      - [Usage of `import`](#usage-of-import)
      - [Example of `import`](#example-of-import)
    - [Pause](#pause)
    - [Variables Syntax](#variables-syntax)
    - [Operators Syntax](#operators-syntax)
      - [Usage of Operators](#usage-of-operators)
      - [Example of Operators](#example-of-operators)
    - [Output Syntax](#output-syntax)
      - [Usage of `io.print()`](#usage-of-ioprint)
      - [Example of `io.print()`](#example-of-ioprint)
    - [Input Syntax](#input-syntax)
      - [Usage of `io.input()`](#usage-of-ioinput)
      - [Example of `io.input()`](#example-of-ioinput)
      - [Usage of `io.get_number()`](#usage-of-ioget_number)
      - [Example of `io.get_number()`](#example-of-ioget_number)
    - [If-else Statements](#if-else-statements)
      - [Usage of `if-else`](#usage-of-if-else)
      - [Example of `if-else`](#example-of-if-else)
    - [For-else Statements](#for-else-statements)
      - [Usage of `for-else`](#usage-of-for-else)
      - [Example of `for-else`](#example-of-for-else)
    - [While-else Statements](#while-else-statements)
      - [Usage of `while-else`](#usage-of-while-else)
      - [Example of `while-else`](#example-of-while-else)
    - [Break \& Continue Statements](#break--continue-statements)
      - [Usage of `break` and `continue`](#usage-of-break-and-continue)
      - [Example of `break` and `continue`](#example-of-break-and-continue)
    - [List Syntax](#list-syntax)
      - [Usage of List Syntax](#usage-of-list-syntax)
      - [Example of List Syntax](#example-of-list-syntax)
  - [Modules](#modules)

## Basic Usage

### Importing Modules

#### Usage of `import`

```phi
import <module name>;
```

#### Example of `import`

```phi
import io;  // import input/output module
```

### Pause

Use `os.system("pause")` to pause the program. This function is defined in the `os` module.

```phi
import os;
os.system("pause");
```

### Variables Syntax

```phi
var <variable name> = <value>;
```

Variables are declared with the `var` keyword. The value of the variable can be any number, text, or boolean.

### Operators Syntax

There are **39** operators in PhiScript!

```cpp
//Operators defined in PhiScript
static const search_container operators{
    "+", "-", "*", "/", "%", "**", "-", "~", "|", "&", "^", "@", "@@", "<<", ">>", "==", "!=", "<", "<=", ">", ">=", "&&", "||", "!", "^^", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", ".", "..", "..."
};
```

#### Usage of Operators

```phi
<var/value> <operator> <var/value>
```

#### Example of Operators

```phi
import io;
import os;
var a = 10;
var b = 20;
var c = a + b;
io.print(c);
os.system("pause");
```

### Output Syntax

#### Usage of `io.print()`

First, import the `io` module. Then, use the `io.print(<text or number>)` to print a text or a number, similar to the `print()` function in Python.

```phi
import io;
io.print(<text or number>);
```

#### Example of `io.print()`

```phi
// This file is at examples/hello_world.phi
import io;
import os;
io.print("Hello, World!");
os.system("pause");
```

### Input Syntax

Similar to `io.print()`, you must import the `io` module first. There are only two ways to input values in PhiScript.

#### Usage of `io.input()`

```phi
import io;
var x = io.input(<prompt>);
```

#### Example of `io.input()`

```phi
import io;
var x = io.input("Whats your name?");
io.print(x);
```

#### Usage of `io.get_number()`

```phi
import io;
var x = io.get_number(<prompt>, <output after input a invalid number>);
```

`io.get_number()` is similar to this Python code:

```python
while(True):
    try:
        x = int(input("<prompt>"))
        break
    except ValueError:
        print("<output after input a invalid number>")
```

#### Example of `io.get_number()`

```phi
import io;
var x = io.input("How old are you:", "Invalid input:");
io.print(x);
```

### If-else Statements

#### Usage of `if-else`

```phi
if (){
    <code>
}
else if (){
    <code>
}
else{
    <code>
}
//If the corresponding code block has only one line, you can omit curly braces
```

#### Example of `if-else`

```phi
import io;
import os;
var age = io.get_number("What's your age?", "Wrong input! Try again:");
if (age < 0)
    io.print("Invalid input!");
else if (age < 18)
    io.print("You can't drink!");
else
    io.print("You can drink!");
os.system("pause");
```

### For-else Statements

#### Usage of `for-else`

```phi
for (<var> = <start>; <condition>; <next>){
    <code>
}
else{
    <code>
}
//Else can be omitted
```

#### Example of `for-else`

```phi
import io;
import os;
for (var i = 0; i < 10; i += 1){
    io.print(i);
}
else{
    io.print("i is greater than 10!");
}
os.system("pause");
//output:
//0
//1
//2
//3
//4
//5
//6
//7
//8
//9
```

```phi
import io;
import os;
for (var i = 10; i < 10; i += 1){
    io.print(i);
}
else{
    io.print("i is greater than 10!");
}
os.system("pause");
//output:"i is greater than 10!"
```

### While-else Statements

#### Usage of `while-else`

```phi
while (<condition>){
    <code>
}
else{
    <code>
}
//Else can be omitted
```

#### Example of `while-else`

```phi
import io;
import os;
var x = io.get_number("Please input a number:", "Invalid input!");
while (x > 0){
    io.print(x);
    x = x - 1;
}
else{
    io.print("X = 0!");
}
os.system("pause");
```

### Break & Continue Statements

#### Usage of `break` and `continue`

```phi
<some loop>{
    <code>
    if (<condition>){
        break;
    }
}
<some loop>{
    if (<condition>){
        continue;//at the beginning
    }
    <code>
}
```

#### Example of `break` and `continue`

```phi
import io;
import os;
for (var i = 0; i < 10; i += 1){
    if(i % 2 == 0){
        continue;
    }
    if (i == 5){
        break;
    }
    io.print(i);
}
//output:
//1
//3
```

### List Syntax

#### Usage of List Syntax

```phi
var <variable name> = [ <value>, <value>, ... ];
```

#### Example of List Syntax

```phi
import io;
import os;

var n = io.get_number("Please input a number：", "Please input a positive number:");
var fib = [];
fib.push_back(1);
fib.push_back(1);
for (var i = 2; i <= n; i += 1)
    fib.push_back(fib[i - 1] + fib[i - 2]);
for (var i = 0; i < n; i += 1)
    io.print(fib[i]);
os.system("pause");
```

## Modules

In progress...
