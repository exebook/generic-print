# generic-print
Convenient generic `print()` for **C** inspired by **Python**/**JavaScript** and other high-level languages.

> *Still using `printf("%i\n", result)` for debugging?*

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
print(byte, ch);
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

Tested on **Ubuntu** `20.04` `x86_64`, **GCC** `5.1+`, **Clang** `3.4.1+`, `Intel C 17.0+`, **TinyC** `0.9.27`.

Tested on **macOS** `11.2` `arm64` with **Apple Clang** `12.0.0`.

May **not** work on `32`-bit systems. Does **not** work with **Microsoft C Compiler**.

There is a scary looking macro code, what it compiles to? It is basically optimized out. See what `GCC -O1` has done to `print(42, 43)`, note that all type information bolied down to `0x00840084`, which is type `4` (alias for `int`), size `8` bytes.
```asm
xor     eax, eax
mov     ecx, 43
mov     edx, 42
lea     rsi, [rsp+12]
mov     edi, 2
mov     DWORD PTR [rsp+12], 0x00840084
call    __print_func
```

