# generic-print
Convenient generic `print()` for **C**.

Nice generic print inspired byt **Python**/**JavaScript** and other high-level languages.

```c
#include "print.h"
print("number:", 25, "fractional number:", 1.2345, "expression:", (2.0 + 5) / 3);
```

> output
> 
> ![output](https://raw.githubusercontent.com/exebook/generic-print/main/output-demo1.png)

Passing variables of various primitive types:
```c
char *s = "abc";
void *p = main;
long l = 1234567890123456789;
print("string:", s, "pointer:", p, "long:", l);
```
> output
> 
> ![output](https://raw.githubusercontent.com/exebook/generic-print/main/output-demo2.png)

Primitive array support:
```c
int x[] = { 1, 2, 3 };
char *args[] = { "gcc", "hello.c", "-o", "hello" };
print(x, args);
```
> output
> 
> ![output](https://raw.githubusercontent.com/exebook/generic-print/main/output-demo3.png)

Extra information for byte/char:
```c
unsigned char byte = 222;
char ch = 'A';
print(byte, ch)
```
> output
> 
> ![output](https://raw.githubusercontent.com/exebook/generic-print/main/output-demo4.png)

You can setup your own colors, arguments are: `(normal, number, string, hex, fractional)`, defaults are `(-1, 4, 1, 2, 5)`.
```c
__print_setup_colors(249,236,239,244,232);
```
Disabling colors completely:
```c
__print_enable_color = 0;
```

All code is in single no dependency header [`print.h`](https://github.com/exebook/generic-print/blob/main/print.h).

The implementation is based on builtins that check types, variadic macros abuse and variable array initializers.

Tested on **Ubuntu** `20.04` `x86_64`, **GCC** `9.3.0`, **Clang** `10.0.0-4`, **TinyC** `0.9.27`.

May **not** work on `32`-bit systems, was not tested with **Microsoft C Compiler**, was not tested on **Mac**.

