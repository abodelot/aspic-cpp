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

    else end false if null true

They cannot be used as identifiers.

### Types

* int
* float
* string
* boolean
* null
* built-in function

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
