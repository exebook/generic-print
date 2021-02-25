#include <stdarg.h>
#include <stdio.h>

int __print_enable_color = 1;

void __print_color(int a) {
	if (!__print_enable_color) return;
	if (a == -1) printf("\x1b(B\x1b[m");
	else printf("\x1b[38;5;%im", a);
}

#define __print_array(T, qual, color) \
	__print_color(__print_color_normal); \
	int max_len = 16; \
	int n = size/sizeof(T); \
	T *m = va_arg(v, T*); \
	printf("["); \
	__print_color(color); \
	for (int i = 0; i < (n < max_len ? n : max_len); i++) { \
		if (i > 0) printf(" "); \
		printf(qual, m[i]); \
	} \
	__print_color(__print_color_normal); \
	if (n > max_len) printf("..."); \
	printf("]");

int __print_color_normal = -1; // -1 means default terminal foreground color
int __print_color_number = 4;
int __print_color_string = 1;
int __print_color_hex = 2;
int __print_color_float = 5;

void __print_setup_colors(int normal, int number, int string, int hex, int fractional) {
	__print_color_string = string;
	__print_color_number = number;
	__print_color_hex = hex;
	__print_color_normal = normal;
	__print_color_float = fractional;
}

void __print_func (int count, unsigned short types[], ...) {
	va_list v;
	va_start(v, types);
	#ifdef __print_DEBUG
	printf("args[%i]: ", count);
	for (int i = 0; i < count; i++) {
		char type = types[i] & 0x1F;
		char size = types[i] >> 5;
		if (i > 0) printf(" ");
		printf("%i[%i]", type, size);
	}
	printf("\n");
	#endif // __print_DEBUG

	for (int i = 0; i < count; i++) {
		if (i > 0) printf(" ");
		char type = types[i] & 0x1F;
		char size = types[i] >> 5;
		if (type == 1) {
			__print_color(__print_color_float);
			double d = va_arg(v, double);
			printf("%'G", d);
		}
		else if (type == 2) {
			__print_color(__print_color_string);
			char c = va_arg(v, int);
			printf("'%c'", c); __print_color(__print_color_number);
			printf("%i", (int)c);
		}
		else if (type == 3) {
			__print_color(__print_color_number);
			char c = va_arg(v, int);
			printf("%i", (unsigned char)c);
			__print_color(__print_color_normal);
			printf("<");
			__print_color(__print_color_hex);
			printf("0x%X", (unsigned char)c);
			__print_color(__print_color_normal);
			printf(">");
		}
		else if (type == 4) {
			__print_color(__print_color_number);
			printf("%'i", va_arg(v, int));
		}
		else if (type == 5) {
			__print_color(__print_color_number);
			printf("%'u", va_arg(v, int));
		}
		else if (type == 6) {
			__print_color(__print_color_number);
			printf("%'li", va_arg(v, unsigned long));
		}
		else if (type == 7) {
			__print_color(__print_color_number);
			printf("%'lu", va_arg(v, long));
		}
		else if (type == 8) {
			__print_color(__print_color_string);
			printf("\"%s\"", va_arg(v, char*));
		}
		else if (type == 9) {
			__print_color(__print_color_normal);
			printf("%s", va_arg(v, char*));
		}
		else if (type == 10) {
			__print_color(__print_color_hex);
			printf("%p", va_arg(v, void*));
		}
		else if (type == 11) {
			__print_array(int, "%i", __print_color_number);
		}
		else if (type == 12) {
			__print_array(unsigned int, "%u", __print_color_number);
		}
		else if (type == 13) {
			__print_array(short, "%i", __print_color_number);
		}
		else if (type == 14) {
			__print_array(unsigned short, "%i", __print_color_number);
		}
		else if (type == 15) {
			__print_array(char*, "\"%s\"", __print_color_string);
		}
		else {
			printf("print: unsupported type (of size %i)\n", size); break;
		}
	}
	va_end(v);
	__print_color(-1);
	printf("\n");
}

#define __print_typeid(a) \
	__builtin_choose_expr(__print_is_type(a, double), 1, \
	__builtin_choose_expr(__print_is_type(a, float), 1, \
	__builtin_choose_expr(__print_is_type(a, char), 2, \
	__builtin_choose_expr(__print_is_type(a, unsigned char), 3, \
	__builtin_choose_expr(__print_is_type(a, int), 4, \
	__builtin_choose_expr(__print_is_type(a, unsigned int), 5, \
	__builtin_choose_expr(__print_is_type(a, long), 6, \
	__builtin_choose_expr(__print_is_type(a, unsigned long), 7, \
	__builtin_choose_expr(__print_is_type(a, short), 4, \
	__builtin_choose_expr(__print_is_type(a, char*), 8, \
	__builtin_choose_expr(__print_is_type(a, char[]), 9, \
	__builtin_choose_expr(__print_is_type(a, void*), 10, \
	__builtin_choose_expr(__print_is_type(a, int[]), 11, \
	__builtin_choose_expr(__print_is_type(a, unsigned int[]), 12, \
	__builtin_choose_expr(__print_is_type(a, short[]), 13, \
	__builtin_choose_expr(__print_is_type(a, unsigned short[]), 14, \
	__builtin_choose_expr(__print_is_type(a, char*[]), 15, \
	0)))))))))))))))))

#define __print_push(c,size,cont) (cont, *--_p = c | (size << 5))
#define __builtin_choose_expr __builtin_choose_expr
#define __print_is_type(a, t) __builtin_types_compatible_p(typeof(a), t)
#define __print_count_int(q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m,...) m
#define __print_count(a...)__print_count_int(a,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define __print_code(a, cont) __builtin_choose_expr(__print_is_type(a, void), 0, __print_push(__print_typeid(a), (sizeof(a)<(1<<16>>5)?sizeof(a):(1<<16>>5)-1), cont))

#define __print_types_int(q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m,...)__print_code(q,__print_code(w,__print_code(e,__print_code(r,__print_code(t,__print_code(y,__print_code(u,__print_code(i,__print_code(o,__print_code(p,__print_code(a,__print_code(s,__print_code(d,__print_code(f,__print_code(g,__print_code(h,__print_code(j,__print_code(k,__print_code(l,__print_code(z,__print_code(x,__print_code(c,__print_code(v,__print_code(b,__print_code(n,__print_code(m, 0))))))))))))))))))))))))))

#define __print_types(a...) __print_types_int(a, (void)0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)

#define print(a...)({ \
	int count = __print_count(a); \
	unsigned short stack[count], *_p = stack + count; \
	__print_types(a); \
	__print_func(count, _p, a); \
});

