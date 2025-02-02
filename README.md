<h1 align="center">moom</h1>
<p align="center">the MOOM programming language</p>

moom is an interpreted, esoretic programming language written in C\
It has python-like syntax

# Building

```bash
git clone https://github.com/danilaxila3/moom.git
cd moom
make
```

Then run `bin/moom <file>` to run a file

# Full syntax

```
moomLet var = value
var = value
var += value:float
var *= value:float
moomPrint("string", var...)
moomPrintNl("string", var...)
moomInput(var)
```

That's all there is\
No comment support btw

# Example (Calculator)

```
moomLet x = 0
moomLet y = 0

moomPrint("Enter x: ")
moomInput(x)
moomPrint("Enter y: ")
moomInput(y)

moomLet z = x
moomLet w = x

z += y
w *= y

moomPrintNl("x + y is ", z)
moomPrintNl("x * y is ", w)
```
