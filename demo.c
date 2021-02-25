#define __print_DEBUG

#include "print.h"

int main() {
	// basic usage
	print(42); return 0;
	print("number:", 25, "fractional number:", 1.2345, "expression:", (2.0 + 5) / 3);

	// variables can be passed
	char *s = "abc";
	void *p = main;
	long l = 1234567890123456789;
	print("string:", s, "pointer:", p, "long:", l);

	// some basic C arrays are supported
	int x[] = { 1, 2, 3 };
	char *args[] = { "gcc", "hello.c", "-o", "hello" };
	print(x, args);

	// char/byte are handled with extra love
	unsigned char byte = 222;
	char ch = 'A';
	print(byte, ch)

	printf(stderr, "Warning:", byte, ch)

	// you can setup your own colors
	// arguments are: (normal, number, string, hex, fractional)
	// defaults are (-1, 4, 1, 2, 5)
	__print_setup_colors(249,236,239,244,232);

	// or disable colors completely
	__print_enable_color = 0;
}
