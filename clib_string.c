#include "clib_string.h"
#include "syscalls.h"

void print(char *str) {
	if (str == 0) {
		return;
	}
	write(1, str, strlen(str));
	return;
}

void itoa(register signed int number, register char* buf, register signed int buf_len, register signed int base, register signed int set_width) {
	register signed long int num;
	num = number;
	if(base < 0) {
		base = -base;
		if(number < 0) {
			num = -num;
		}
	}
	if(base > 16 || base < 2){
		return;
	}
	if(buf_len < 2){
		return;
	}
	if(set_width != 0){
		if (set_width < 0 || num >= 0) {
			if (buf_len <= set_width) {
				return;
			}
		} else {
			if (buf_len <= set_width + 1) {
				return;
			}
		}
	}
	//Check if zero since this will fail to loop and can be easily handled now
	register unsigned char i_2;
	if(num == 0){
		if (set_width != 0) {
			if (set_width < 0) {
				set_width = -set_width;
			}
			i_2 = 0;
			while (i_2 < set_width) {
				buf[i_2] = '0';
				i_2++;
			}
			buf[i_2] = 0;
			return;
		} else {
			buf[0] = '0';
			buf[1] = 0;
			return;
		}
	}else{
		register unsigned char i_then_length;
		i_then_length = 0;
		i_2 = 0;
		//i_then_length is now an index
		//Append "-" character for negatives
		if(num < 0){
			if (set_width < 0) {
				set_width++;
			}
			num = -num;
			buf[0] = '-';
			buf_len--;
			buf++;
		}
		if (set_width < 0) {
			set_width = -set_width;
		}
		//Find Characters
		while(num > 0 && i_then_length < buf_len){
			i_2 = num % base;
			if(i_2 < 10){
				buf[(unsigned int)i_then_length] = '0' + i_2;
			}else{
				buf[(unsigned int)i_then_length] = '7' + i_2;
			}
			num /= base;
			i_then_length++;
		}
		while (i_then_length < set_width && i_then_length < buf_len) {
			buf[(unsigned int)i_then_length] = '0';
			i_then_length++;
		}
		//i_then_length is now a length count for char array
		//Loop to fix character order
		i_2 = 0;
		register char tmpchar;
		while(i_2 < (i_then_length / 2) && i_2 < buf_len){
			tmpchar = buf[(int)((i_then_length - i_2) - 1)];
			buf[(int)((i_then_length - i_2) - 1)] = buf[(unsigned int)i_2];
			buf[(unsigned int)i_2] = tmpchar;
			i_2++;
		}
		if(i_then_length < buf_len){
			buf[(unsigned int)i_then_length] = 0;
		}else{
			buf[(unsigned int)(i_then_length - 1)] = 0;
		}
	}
	return;
}

signed long _dprintf(signed int fd, const char *format, unsigned long* gen_regs, long double* vec_regs, void* stack_params, unsigned long vector_regs_used) {
	// Parse String
	//void* stack_ptr = stack_params;
	char buff[50];
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
			} else if (s[i] == 's') {
				char* pstr;
				if (int_count < 4) {
					pstr = (char*)(*(gen_regs + int_count));
				} else {
					pstr = (char*)stack_params;
					stack_params += sizeof(char*);
				}
				print(pstr);
				s += i + 1;
				i = -1;
				int_count++;
				mode = 0;
			} else if (s[i] == 'd' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u' || s[i] == 'x' || s[i] == 'X' || s[i] == 'c' || s[i] == 'p' || s[i] == 'n' || s[i] == 'm') {
				signed long int tmp_int;
				if (int_count < 4) {
					tmp_int = *(gen_regs + int_count);
				} else {
					tmp_int = *((signed long int*)stack_params);
					stack_params += sizeof(signed long int);
				}
				itoa(tmp_int, buff, 50, 10, 0);
				write(fd, buff, strlen(buff));
				s += i + 1;
				i = -1;
				int_count++;
				mode = 0;
			} else if (s[i] == 'e' || s[i] == 'E' || s[i] == 'f' || s[i] == 'F' || s[i] == 'g' || s[i] == 'G' || s[i] == 'a' || s[i] == 'A') {
				long double tmp_ldouble;
				
				if (mode == 2) {
					// Long Double
					if (float_count < vector_regs_used && float_count < 8) {
						tmp_ldouble = *(vec_regs + float_count);
					} else {
						tmp_ldouble = *((long double*)stack_params);
						stack_params += sizeof(long double);
					}
					//itoa(ftoi(tmp_ldouble), buff, 100, 10, 0);
				} else {
					// Double or Float
					double tmp_double;
					if (float_count < vector_regs_used && float_count < 8) {
						//dprintf(1, "TraceA i: (%d)\n", float_count);
						tmp_double = *((double*)(vec_regs + float_count));
					} else {
						tmp_double = *((double*)stack_params);
						stack_params += sizeof(long double);
					}
					tmp_ldouble = tmp_double;
					//itoa(ftoi(tmp_double), buff, 100, 10, 0);
				}
				
				
				if (tmp_ldouble < 0.0) {
					tmp_ldouble = -tmp_ldouble;
					write(1, "-", 1);
				}
				
				unsigned int k;
				k = 1;
				while (tmp_ldouble >= 1000000000.0) {
					tmp_ldouble /= 1000000000.0;
					k++;
				}
				unsigned int v;
				unsigned int j;
				j = 0;
				while (j < k) {
					v = tmp_ldouble;
					tmp_ldouble -= v;
					if (j == 0) {
						itoa(v, buff, 50, 10, 0);
					} else {
						itoa(v, buff, 50, 10, 9);
					}
					write(1, buff, strlen(buff));
					tmp_ldouble *= 1000000000.0;
					j++;
				}
				/*
				v = tmp_ldouble;
				tmp_ldouble -= v;
				if (j) {
					j = 9;
				}
				itoa(v, buff, 50, 10, j);
				write(1, buff, strlen(buff));
				*/
				
				// Precision
				//tmp_ldouble *= 1000000000.0;
				v = tmp_ldouble;
				//tmp_ldouble -= v;
				itoa(v, buff, 50, 10, 9);
				write(1, ".", 1);
				write(1, buff, strlen(buff) - 3);
				
				//char* testing_str = "[DOUBLE_VALUE_HERE]";
				//itoa(ftoi(2000000000.0), buff, 100, 10, 0);
				//write(fd, testing_str, strlen(testing_str));
				//write(fd, buff, strlen(buff));
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

unsigned int strlen(register const char* str) {
	if (str == 0) {
		return 0;
	}
	register unsigned int i = 0;
	while (str[i] != 0) {
		i++;
	}
	return i;
}