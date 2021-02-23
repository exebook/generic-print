# generic-print
Convenient generic `print()` for **C**.

Nice generic print inspired byt **Python**/**JavaScript** and other high-level languages.

All code is in single no dependency header `print.h` [raw download](https://raw.githubusercontent.com/exebook/generic-print/main/print.h).

```c
#include "print.h"
print("number:", 25, "fractional number:", 1.2345, "expression:", (2.0 + 5) / 3);
```
##### output

```c
number: 25 fractional number: 1.2345 expression: 2.33333
```

> ![Vintage screenshot taken with CoolRetroTerm](https://raw.githubusercontent.com/exebook/generic-print/main/screenshot.png)
> Vintage screenshot taken with CoolRetroTerm

Passing variables of various primitive types:
```c
char *s = "abc";
void *p = main;
long l = 1234567890123456789;
print("string:", s, "pointer:", p, "long:", l);
```
##### output
```c
string: "abc" pointer: 0x401745 long: 1234567890123456789
```

Primitive array support:
```c
int x[] = { 1, 2, 3 };
char *args[] = { "gcc", "hello.c", "-o", "hello" };
print(x, args);
```
##### output
```c
[1 2 3] ["gcc" "hello.c" "-o" "hello"]
```

Extra love for byte/char:
```c
unsigned char byte = 222;
char ch = 'A';
print(byte, ch)
```
##### output
```c
222<0xDE> 'A'65
```

You can setup your own colors, arguments are: `(normal, number, string, hex, fractional)`, defaults are `(-1, 4, 1, 2, 5)`.
```c
__print_setup_colors(249,236,239,244,232);
```
Disabling colors completely:
```c
__print_enable_color = 0;
```

The implementation is based on builtins that check types, variadic macros abuse and variable array initializers.

Tested on **Ubuntu** `20.04` `x86_64`, **GCC** `9.3.0`, **Clang** `10.0.0-4`, **TinyC** `0.9.27`.

May **not** work on `32`-bit systems, was not tested with **Microsoft C Compiler**, was not tested on **Mac**.



