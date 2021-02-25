#include <stdarg.h>
#include <stdio.h>

int __print_enable_color = 1;

void __print_color(FILE *fh, int a) {
	if (!__print_enable_color) return;
	if (a == -1) fprintf(fh,"\x1b(B\x1b[m");
	else fprintf(fh,"\x1b[38;5;%im", a);
}

#define __print_type_expr(x) \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), double), 'd', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), float), 'd', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), char), 'c', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), unsigned char), 'b', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), int), 'i', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), unsigned int), 'u', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), long), 'l', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), unsigned long), 'L', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), short), 'i', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), char*), 's', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), char[]), 'S', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), void*), 'p', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), int[]), 'I', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), unsigned int[]), 'U', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), short[]), 'H', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), unsigned short[]), 'h', \
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(x), char*[]), 'C', \
	__builtin_choose_expr(sizeof(x) == sizeof(void*), 'p', \
	'?'))))))))))))))))))

#define __print_cast(x) (__print_type_expr(x) | (sizeof(x) << 8))

#define __print_array(T, __fh, qual, color) \
	__print_color(__fh, __print_color_normal); \
	int max_len = 16; \
	int n = size/sizeof(T); \
	T *m = va_arg(v, T*); \
	fprintf(__fh,"[");     \
	__print_color(__fh,color);                              \
	for (int i = 0; i < (n < max_len ? n : max_len); i++) { \
		if (i > 0) fprintf(__fh," ");                   \
		fprintf(__fh,qual, m[i]); \
	} \
	__print_color(__fh,__print_color_normal); \
	if (n > max_len) fprintf(__fh,"..."); \
	fprintf(__fh,"]");

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

void __print_func (FILE* __fh, int count, short types[], ...) {
	va_list v;
	va_start(v, types);
	#ifdef __print_DEBUG
	fprintf(__fh,"args[%i]: ", count);
	for (int i = 0; i < count; i++) {
		char type = types[i] & 0xFF;
		char size = types[i] >> 8;
		if (i > 0) fprintf(__fh," ");
		fprintf(__fh,"%c[%i]", type, size);
	}
	fprintf(__fh,"\n");
	#endif // __print_DEBUG

	for (int i = 0; i < count; i++) {
		if (i > 0) fprintf(__fh," ");
		char type = types[i] & 0xFF;
		char size = types[i] >> 8;
		if (type == 'd') {
			__print_color(__fh,__print_color_float);
			double d = va_arg(v, double);
			fprintf(__fh,"%'G", d);
		}
		else if (type == 'c') {
			__print_color(__fh,__print_color_string);
			char c = va_arg(v, int);
			fprintf(__fh,"'%c'", c); __print_color(__fh,__print_color_number);
			fprintf(__fh,"%i", (int)c);
		}
		else if (type == 'b') {
			__print_color(__fh,__print_color_number);
			char c = va_arg(v, int);
			fprintf(__fh,"%i", (unsigned char)c);
			__print_color(__fh,__print_color_normal);
			fprintf(__fh,"<");
			__print_color(__fh,__print_color_hex);
			fprintf(__fh,"0x%X", (unsigned char)c);
			__print_color(__fh,__print_color_normal);
			fprintf(__fh,">");
		}
		else if (type == 'i') {
			__print_color(__fh,__print_color_number);
			fprintf(__fh,"%'i", va_arg(v, int));
		}
		else if (type == 'u') {
			__print_color(__fh,__print_color_number);
			fprintf(__fh,"%'u", va_arg(v, int));
		}
		else if (type == 'l') {
			__print_color(__fh,__print_color_number);
			fprintf(__fh,"%'li", va_arg(v, unsigned long));
		}
		else if (type == 'L') {
			__print_color(__fh,__print_color_number);
			fprintf(__fh,"%'lu", va_arg(v, long));
		}
		else if (type == 's') {
			__print_color(__fh,__print_color_string);
			fprintf(__fh,"\"%s\"", va_arg(v, char*));
		}
		else if (type == 'S') {
			__print_color(__fh,__print_color_normal);
			fprintf(__fh,"%s", va_arg(v, char*));
		}
		else if (type == 'p') {
			__print_color(__fh,__print_color_hex);
			fprintf(__fh,"%p", va_arg(v, void*));
		}
		else if (type == 'I') {
			__print_array(int, __fh, "%i", __print_color_number);
		}
		else if (type == 'U') {
			__print_array(unsigned int, __fh, "%u", __print_color_number);
		}
		else if (type == 'H') {
			__print_array(short, __fh, "%i", __print_color_number);
		}
		else if (type == 'h') {
			__print_array(unsigned short, __fh, "%i", __print_color_number);
		}
		else if (type == 'C') {
			__print_array(char*, __fh, "\"%s\"", __print_color_string);
		}
		else {
			fprintf(stderr,"print: unsupported type (of size %i)\n", size); break;
		}
	}
	va_end(v);
	fprintf(__fh,"\n");
}

#define __print_cast_list(q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m,...) (short[]){\
	__print_cast(q),__print_cast(w),__print_cast(e),__print_cast(r),\
	__print_cast(t),__print_cast(y),__print_cast(u),__print_cast(i),\
	__print_cast(o),__print_cast(p),__print_cast(a),__print_cast(s),\
	__print_cast(d),__print_cast(f),__print_cast(g),__print_cast(h),\
	__print_cast(j),__print_cast(k),__print_cast(l),__print_cast(z),\
	__print_cast(x),__print_cast(c),__print_cast(v),__print_cast(b),\
	__print_cast(n),__print_cast(m)\
}
#define __print_count26(q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m,...) m

#define print(a...) \
	__print_func(stdout,__print_count26(a,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0), \
	__print_cast_list(a,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1), a);
#define fprint(__fh,a...)                                                 \
	__print_func(__fh,__print_count26(a,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0), \
	__print_cast_list(a,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1), a);
