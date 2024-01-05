# φPhiScript

This document can help you to use the φPhiScript.

## Contents

- [φPhiScript](#φphiscript)
  - [Contents](#contents)
  - [Basic Usage](#basic-usage)
    - [Importing modules](#importing-modules)
      - [Usage of `import`](#usage-of-import)
      - [Example of `import`](#example-of-import)
    - [Pause](#pause)
    - [Variables Synax](#variables-synax)
    - [Operators Synax](#operators-synax)
      - [Useage of Operators](#useage-of-operators)
      - [Example of Operators](#example-of-operators)
    - [Output Synax](#output-synax)
      - [Usage of `io.print()`](#usage-of-ioprint)
      - [Example of `io.print()`](#example-of-ioprint)
    - [Input Synax](#input-synax)
      - [Usage of `io.input()`](#usage-of-ioinput)
      - [Example of `io.input()`](#example-of-ioinput)
      - [Usage of `io.get_number()`](#usage-of-ioget_number)
      - [Example of `io.get_number()`](#example-of-ioget_number)
    - [If-else Statements](#if-else-statements)
      - [Usage of `if-else`](#usage-of-if-else)
      - [Example of `if-else`](#example-of-if-else)
    - [For-else Statements](#for-else-statements)

## Basic Usage

### Importing modules

#### Usage of `import`

```phi
import <module name>;
```

#### Example of `import`

```phi
import io;//import input/output module
```

### Pause

Please use `os.system("pause")` to pause the program.You must use it at the end of the program.Or the program will not be executed.This function is defined in the `os` module.

```phi
import os;
```

```phi
os.system("pause");
```

### Variables Synax

```phi
var <variable name> = <value>;
```

Variables are declared with the `var` keyword. The value of the variable can be any number, text, or boolean.

### Operators Synax

There are **39** operators in PhiScript!

```cpp
//Operators defined in PhiScript
static const search_container operators{
    "+",
    "-",
    "*",
    "/",
    "%",
    "**",
    "-",
    "~",
    "|",
    "&",
    "^",
    "@",
    "@@",
    "<<",
    ">>",
    "==",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
    "&&",
    "||",
    "!",
    "^^",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "&=",
    "|=",
    "^=",
    "<<=",
    ">>=",
    ".",
    "..",
    "..."
};
```

#### Useage of Operators

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

### Output Synax

#### Usage of `io.print()`

First, you had to import the `io` module. Then, you can use the `io.print(<text or number>)` to print a text or a number, It's like the `print()` function in Python.

```phi
import io;
io.print(<text or number>);
```

#### Example of `io.print()`

```phi
//This file is at examples/hello_world.phi
import io;
import os;
io.print("Hello, World!");
os.system("pause");
```

### Input Synax

Same as `io.print()`, you had to import the `io` module first. There are only two way to input values in PhiScript.

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

`io.get_number()` just like this Python code:

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
