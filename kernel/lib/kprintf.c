/* kprintf, kvsprintf and other printf stuff here */
#include <kstdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer To String, transforms an Integer (different bases) into a string which
 * will be loaded into a buffer
 */
static char *intostr(char *buffer, 
	int value, int base,
	int precision, bool is_uint,
	bool hex_uppercase, bool add_prefix)
{
	char tmp_buffer[36];
	int i, j;
	/* these will be used to transfer to buffers */
	i = 0;
	j = 0;

	/* uint value */
	uint32_t uvalue = (uint32_t)value;
	/* decimal */
	if (base == 10) {
		if (is_uint) {
			do {
				tmp_buffer[i++] = '0' + (uvalue % 10);
			} while (uvalue /= 10);

			while (i--)
				buffer[j++] = tmp_buffer[i];
			buffer[j] = '\0';
			return buffer;
		}
		bool is_negative = false;
		/* check if negative, if it is, flip the value into positive
		 * and make is_negative true
		 */
		if (value < 0) {
			is_negative = true;
			value = -value;
		}
		do {
			tmp_buffer[i++] = '0' + (value % 10);
		} while (value /= 10);

		/* add the sign '- */
		if (is_negative) {
			tmp_buffer[i++] = '-';
		}

		/* copy it in reverse so 4321 == 1234 */
		while (i--)
			buffer[j++] = tmp_buffer[i];
		buffer[j] = '\0';
		return buffer;
	}
	/* hexadecimal */
	else if (base == 16) {
		/* make the hex values */
		char *hex_values = "0123456789abcdef";
		if (hex_uppercase)
			hex_values = "0123456789ABCDEF";
		/* do math */
		do {
			tmp_buffer[i++] = hex_values[uvalue & 0xF];
		} while (uvalue >>= 4);

		/* add precision which is the minimal numbers (eg A > 0A)*/
		while (i < precision)
			tmp_buffer[i++] = '0';

		/* add the '0x' prefix */
		if (add_prefix) {
			tmp_buffer[i++] = 'x';
			tmp_buffer[i++] = '0';
		}

		/* copy copy copy */
		while (i--)
			buffer[j++] = tmp_buffer[i];
		buffer[j] = '\0';
		return buffer;
	}
	/* binary 
	 * same process as hexadecimal but for 1s and 0s
	 */
	else if (base == 2) {
		do {
			tmp_buffer[i++] = '0' + (uvalue & 1);
		} while (uvalue >>= 1);

		while (i < precision)
			tmp_buffer[i++] = '0';
		if (add_prefix) {
			tmp_buffer[i++] = 'b';
			tmp_buffer[i++] = '0';
		}
		while (i--)
			buffer[j++] = tmp_buffer[i];
		buffer[j] = '\0';
		return buffer;	
	}
	buffer[j] = '\0';
	return buffer;
}

/* check if the character can be an integer */
static bool check_if_int(char c)
{
	return (c >= '0' && c <= '9');
}

/* makes a string into an integer
 * returns count 
 */
static int ascii_to_num(int dest, char *str, int *cnt)
{
	while (check_if_int(*str)) {
		dest = dest * 10 + (*str - '0');
		str++;
		(*cnt)++;
	}
	return dest;
}

/* PRINT Formatted into a buffer */
int kvsprintf(char *buffer, const char *fmt, va_list args)
{
	int count = 0;
	int precision = 0;
	bool add_prefix = false;
	bool hex_uppercase = false;
	bool is_uint = false;
	while (*fmt)
	{
		/* check for %d %x %u %p %s %c */
		if (*fmt == '%') {
			fmt++;
			/* check for hex_uppercase */
			if (*fmt == '#') {
				add_prefix = true;
				fmt++;
			}
			/* check for precision */
			if (*fmt == '.') {
				fmt++;
				/* this will be added to fmt to skip the int */
				int tmp = 0;
				/* do the precision */
				precision = ascii_to_num(precision, (char*)fmt, &tmp);
				fmt += tmp;
			}

			/* now check what we can print */
			switch (*fmt) {
				/* char */
				case 'c': {
					*buffer++ = (char)va_arg(args, int);
					break;
				}
				/* string */
				case 's': {
					const char *str = va_arg(args, const char*);
					while (*str)
						*buffer++ = *str++;
					break;
				}
				/* integers */
				case 'u': {
					is_uint = true;
					__attribute__((fallthrough));
				}
				case 'd': {
					char tmp[21];
					intostr(tmp, 
						*fmt == 'd' ? va_arg(args, int) :
						va_arg(args, uint32_t), 
						10, false, is_uint, false, false);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buffer++ = tmp[i];
					break;
				}

				/* hexadecimals and uintptrs */
				case 'X': 
					hex_uppercase = true;
					__attribute__((fallthrough));
				case 'P': 
					hex_uppercase = true;
					__attribute__((fallthrough));
				case 'p':
					__attribute__((fallthrough));
				case 'x': {
					char tmp[11];
					uint32_t value = 0;
					if (*fmt == 'x' || *fmt == 'X')
						value = va_arg(args, uint32_t);
					if (*fmt == 'p' || *fmt == 'P')
						value = va_arg(args, uintptr_t);
					
					intostr(tmp, value, 16, precision, true, hex_uppercase, add_prefix);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buffer++ = tmp[i];
					break;
				}

				/* binary */
				case 'b':
					char tmp[35];
					intostr(tmp, va_arg(args, uint32_t), 2, precision, true, false, add_prefix);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buffer++ = tmp[i];
					break;
				default:
					*buffer++ = *fmt;
					break;
			}
		} else {
			*buffer++ = *fmt;
		}
		fmt++;
		/* always reset */
		precision = 0;
		add_prefix = false;
		is_uint = false;
		hex_uppercase = false;
	}
	*buffer = '\0';
	return count;
}

/* PRINT Formatted, this outputs to kcon */
int kprintf(const char *fmt, ...) 
{
	char buf[2048]; /* 2048 characters */

	va_list args;
	va_start(args, fmt);

	int count = kvsprintf(buf, fmt, args);

	va_end(args);
	/* print it */
	kputs(buf);
	return count;
}