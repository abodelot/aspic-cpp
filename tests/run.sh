#!/bin/sh

echo "running with python"
python program_test.txt > out.python 2>&1

echo "running with aspic"
../aspic program_test.txt > out.aspic 2>&1
if diff out.python out.aspic > /dev/null; then
    echo "test succeed"
    rm out.python
    rm out.aspic

else
    echo "test failed"
    diff out.python out.aspic
fi

