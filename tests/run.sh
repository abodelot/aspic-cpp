#!/bin/sh

python program_test.txt > out.python 2>&1
../aspic program_test.txt > out.aspic 2>&1
if diff out.python out.aspic > /dev/null; then
	echo "test succeed"
else
	echo "test failed"
fi

rm out.python
rm out.aspic
