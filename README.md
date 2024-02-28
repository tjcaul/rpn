# RPN

A Reverse Polish Notation calculator using double precision and supporting most
standard scientific calculator operations.

Expressions can be passed as shell arguments, in which case they will be
interpreted separately, with each result printed on a new line.
There is also an interactive mode, which allows the user to evaluate expressions
in the context of a single stack.

## Compilation

Simply compile with
```gcc -o rpn *.c```

## Running

Get help on usage:
```
$ rpn --help
Usage: rpn ...
```

Evaluate `1 + 2` (note the quotes; necessary to express a single expression):
```
$ rpn '1 2 +'
3
```

Evaluate `pi^2` and `log(1.1)`:
```
$ rpn 'pi 2 ^' '1.1 log'
9.8696
0.0953102
```

Enter interactive mode (note `print` to show the state of the stack):
```
$ rpn
>>> 1 1 +
2
>>> ^C
rpn $ ./rpn        
>>> 1 1 +
2
>>> 3 *
6
>>> e
2.71828
>>> print
.--------------.
[       2.7183 ]
[            6 ]
'--------------'
>>> %
0.563436
>>> print
.--------------.
[      0.56344 ]
'--------------'
>>> quit
```
