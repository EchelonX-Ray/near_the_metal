#include "clib.h"
#include "syscalls.h"

unsigned long dprintf(signed int fd, const char *format, ... );

unsigned long _dprintf(signed int fd, const char *format, unsigned long* gen_regs, long double* vec_regs, void* stack_params) {
	// Parse String
	//void* stack_ptr = stack_params;
	char buff[100];
	const char* s = format;
	unsigned int int_count = 0;
	unsigned int float_count = 0;
	//unsigned int long_double_count = 0;
	unsigned int i = 0;
	unsigned int mode = 0;
	unsigned int alt_form = 0;
	unsigned int zero_padding = 0;
	unsigned int signage = 0;
	unsigned int length = 0;
	unsigned int precision = 0;
	while (s[i] != 0) {
		if (mode == 1 || mode == 2) {
			if (s[i] == '%') {
				mode = 0;
				write(fd, "%", 1);
				s += i + 1;
				i = -1;
			} else if (s[i] == '.') {
				alt_form |= 4;
			} else if (s[i] == '-') {
				zero_padding |= 2;
			} else if (s[i] == '+') {
				signage |= 2;
			} else if (s[i] == ' ') {
				signage |= 1;
			} else if (s[i] == '0') {
				if (alt_form & 2) {
					if (alt_form & 4) {
						precision *= 10;
					} else {
						length *= 10;
					}
				} else {
					zero_padding |= 1;
				}
			} else if (s[i] > '0' && s[i] <= '9') {
				alt_form |= 2;
				if (alt_form & 4) {
					precision *= 10;
					precision += s[i] - '0';
				} else {
					length *= 10;
					length += s[i] - '0';
				}
			} else if (s[i] == '#') {
				alt_form |= 1;
				alt_form &= ~2;
			} else if (s[i] == 'L') {
				mode = 2;
				alt_form &= ~2;
			} else if (s[i] == 'd' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u' || s[i] == 'x' || s[i] == 'X' || s[i] == 'c' || s[i] == 's' || s[i] == 'p' || s[i] == 'n' || s[i] == 'm') {
				signed long int tmp_int;
				if (int_count < 4) {
					//write(1, "TraceA\n", strlen("TraceA\n"));
					tmp_int = gen_regs[int_count];
				} else {
					//write(1, "TraceB\n", strlen("TraceB\n"));
					tmp_int = *((signed long int*)stack_params);
					stack_params += sizeof(signed long int);
				}
				itoa(tmp_int, buff, 100, 10, 0);
				write(fd, buff, strlen(buff));
				s += i + 1;
				i = -1;
				int_count++;
				mode = 0;
			} else if (s[i] == 'e' || s[i] == 'E' || s[i] == 'f' || s[i] == 'F' || s[i] == 'g' || s[i] == 'G' || s[i] == 'a' || s[i] == 'A') {
				long double tmp_ldouble;
				if (float_count < 8) {
					//write(1, "TraceC\n", strlen("TraceC\n"));
					tmp_ldouble = vec_regs[float_count];
				} else {
					//write(1, "TraceD\n", strlen("TraceD\n"));
					tmp_ldouble = *((long double*)stack_params);
					stack_params += sizeof(long double);
				}
				char* testing_str = "[DOUBLE_VALUE_HERE]";
				write(fd, testing_str, strlen(testing_str));
				s += i + 1;
				i = -1;
				float_count++;
				/*
				if (mode == 1) {
					float_count++;
				} else {
					long_double_count++;
				}
				*/
				mode = 0;
			}
		} else {
			if (s[i] == '%') {
				write(fd, s, i);
				s += i + 1;
				i = -1;
				mode = 1;
				alt_form = 0;
				zero_padding = 0;
				signage = 0;
				length = 0;
				precision = 0;
			}
		}
		i++;
	}
	if (i > 0) {
		write(fd, s, i);
	}
	return 0;
}

signed int main(unsigned int argc, char* argv[], char* envp[]) {
	char* s = "Testing%%: a-%d, b-%d, c-%d, d-%d, e-%d, f-%d, g-%d, h-%d, i-%d, j-%d\n";
	
	dprintf(1, s, 10, 20, 30, 45, 50, 60, 70, 80, 90, 100);
	/*
	
	char buff[100];
	
	print("int_count: ");
	itoa(int_count, buff, 100, 10, 0);
	print(buff);
	print("\n");
	print("float_count: ");
	itoa(float_count, buff, 100, 10, 0);
	print(buff);
	print("\n");
	print("long_double_count: ");
	itoa(long_double_count, buff, 100, 10, 0);
	print(buff);
	print("\n");
	
	return 0;
	*/
	/*
	unsigned long retval;
	float f;
	double d;
	long double D;
	d = 5.9;
	retval = test_asm_func(d);
	char buff[100];
	itoa(retval, buff, 100, 10, 0);
	print(buff);
	print("\n");
	itoa(sizeof(long double), buff, 100, 10, 0);
	print(buff);
	print("\n");
	return 0;
	*/
	return 0;
}
