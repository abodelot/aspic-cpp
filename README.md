Aspic
=====

Aspic is an instruction interpreter written in C++.
The parser implementation is based upon the [Shunting-yard algorithm](http://en.wikipedia.org/wiki/Shunting-yard_algorithm).

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
