Aspic
=====

Aspic is an instruction interpreter written in C++.
The parser implementation is based upon the [Pratt parsing technique](https://en.wikipedia.org/wiki/Pratt_parser).

* Author: Alexandre Bodelot <alexandre.bodelot@gmail.com>
* Website: https://github.com/abodelot/aspic

## Setup

1. Install GNU readline library. On Debian/Ubuntu:

    ```
    sudo apt install libreadline-dev
    ```

2. Build with make:

    ```
    make
    ```

## Testing

Aspic is tested with its own `assert` function. Tests can be run with:

```
./tests/run.sh
```
