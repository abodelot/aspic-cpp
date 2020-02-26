Aspic
=====

Aspic is an expression interpreter written in C++.
The parser implementation is based upon the [Pratt parsing technique](https://en.wikipedia.org/wiki/Pratt_parser).

* Author: Alexandre Bodelot <alexandre.bodelot@gmail.com>
* Website: https://github.com/abodelot/aspic

## Build

1. Install GNU readline library. On Debian/Ubuntu:

    ```
    sudo apt install libreadline-dev
    ```

2. Build with make:

    ```
    make
    ```

## Usage

- Interactive mode: `./aspic`
- Load a file: `./aspic <path_to_file>`

## Testing

Aspic is tested with its own `assert` function. Tests can be run with:

```
./tests/run.sh
```

## Aspic Syntax

Aspic syntax is close to Ruby and Python.

All syntactic constructs may be separated by an arbitrary number of whitespace characters and comments.

The whitespace characters are space and tab. Newlines work as whitespace only when expressions obviously continues to the next line:

    a = 10 +
        20

    b = len(
        "hello world!"
    )

    c = [
        10,
        20,
        30
    ]

Aspic treats newlines as a significant end-of-expression only if the following conditions are met:

- Last token of the line is a literal value, an identifier, or a closing pair character (`)`, `]`, or `}`).
- Newline is not inside a pair of brackets (`()`, `[]`, or `{}`).

### Identifiers

Examples:

    foobar
    _foobaz
    foo_bar2

Aspic identifiers consist of letters (`A-Z`, `a-z`), decimal digits (`0-9`), and the underscore character (`_`), and begin with a letter or and underscore. There are no restrictions on the lengths of identifiers.

### Comments

Examples:

    # this is a comment line
    a += 1 # increment a

Aspic comments start with `#` character outside of a string and all following text until the end of the line.

### Reserved words

The reserved words are:

    elif else end false if null true while

They cannot be used as identifiers.

### Types

* int
* float
* string
* boolean (literals `true` and `false`)
* null (literal `null`)
* built-in function
* array
* hashmap

### Operators

Aspic defines the following operators:

| Syntax | Name | Right associative? |
|--------|------|--------------------|
| `[]`   | Subscript | |
| `()`   | Function call | |
| `!`    | Unary not | ✓ |
| `+`    | Unary plus | ✓ |
| `-`    | Unary minus | ✓ |
| `**`   | Pow | ✓ |
| `*`    | Multiplication | |
| `%`    | Modulo | |
| `/`    | Division | |
| `+`    | Addition | |
| `-`    | Subtraction | |
| `<`    | Less than | |
| `<=`   | Less than or equal | |
| `>`    | Greater than | |
| `>=`   | Greater than or equal | |
| `==`   | Equal | |
| `!=`   | Not equal | |
| `&&`   | Logical and | |
| `\|\|` | Logical or | |
| `=`    | Assignment | ✓ |
| `*=`   | Multiply and assign | ✓ |
| `/=`   | Divide and assign | ✓ |
| `%=`   | Modulo and assign | ✓ |
| `+=`   | Add and assign | ✓ |
| `-=`   | Subtract and assign | ✓ |

### Expressions

Aspic programs are sequence of expressions. Each expression are delimited by newlines.

#### Array expression

Syntax:

	`[' expr, ...`]'

Examples:

    []
	[1, 2, 3]
    ["abc", [3.14, 4.93]]

Returns a new array, which contains result of each expressions.

#### HashMap expression

Syntax:

    { expr: expr, ... }

Examples:

    {}
    {"a": 1, "b": 2}
    {123: ["hello", "world"]}

Returns a new hashmap object, which maps each key to corresponding value.

### Control structures

Control structures in Aspic are expressions too, and have some value.

#### if

`if` expressions are used for conditional execution.

Syntax:

    if expr
        expr...
    [elif expr
        expr...]...
    [else
        expr...]
    end

Examples:

    if x > 0
        print("strictly positive")
    end

    if foobar == foobaz
        do_a()
    elif foobar == barbaz
        do_b()
    else
        do_c()
    end

    x = if test
        do_a()
    else
        do_b()
    end

#### while

Executes body while condition expression returns true.

Syntax:

    while expr
        expr...
    end

Example:

    i = 0
    while i < 10
        print(i)
        i += 1
    end
