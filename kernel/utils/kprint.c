#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <utils/kprint.h>
#include <kernel/kernel.h>
#include <drivers/uart/serial.h>
/* helper function to turn integers to string 
 * @param buf -- output
 * @param precision -- minimum values printed (precision = 4, value = 0x1, output = 0x0001)
 * @param value -- integer we will turn into string
 * @param base -- 10 for decimal, 16 for hexadecimal, 2 for binary
 * @param add_prefix -- adds 0x to hex and 0b to binary
 * @param hex_uppercase -- uppercase hex values (0x1a -> 0x1A)
 * @param is_unsigned -- unsigned value
 */
static void intostr(char *buf, int precision,
	long value, int base,
	bool add_prefix, bool hex_uppercase,
	bool is_unsigned)
{
	unsigned long uint_value = (unsigned long)value;
	char tmp[66];
	int i = 0;
	int j = 0;
	/* decimal */
	if (base == 10) {
		if (is_unsigned) {
			do {
				tmp[i++] = '0' + (uint_value % 10);
			} while (uint_value /= 10);

			/* transfer */
			while (i--)
				buf[j++] = tmp[i];
			buf[j] = '\0';
			return; /* nothing to do */
		}
		bool is_negative = false;
		if (value < 0) {
			is_negative = true;
			value = -value; /* flip */
		}
		do {
			tmp[i++] = '0' + (value % 10);
		} while (value /= 10);

		if (is_negative) {
			tmp[i++] = '-';
		}
		while (i--)
			buf[j++] = tmp[i];
		
		buf[j] = '\0';
		return;
	}
	/* hexadecimal */
	else if (base == 16) {
		const char *hex = "0123456789abcdef";
		if (hex_uppercase)
			hex = "0123456789ABCDEF";
		do {
			tmp[i++] = hex[uint_value & 0xF];
		} while (uint_value >>= 4);

		while (i < precision)
			tmp[i++] = '0';

		if (add_prefix) {
			tmp[i++] = 'x';
			tmp[i++] = '0';
		}
		while (i--)
			buf[j++] = tmp[i];
		buf[j] = '\0';
		return;
	}

	/* binary */
	else if (base == 2) {
		do {
			tmp[i++] = '0' + (uint_value & 1);
		} while (uint_value >>= 1);

		while (i < precision)
			tmp[i++] = '0';

		if (add_prefix) {
			tmp[i++] = 'b';
			tmp[i++] = '0';
		}

		while (i--)
			buf[j++] = tmp[i];
		buf[j] = '\0';
		return;
	}
}

/* check if the character can be an integer */
static bool check_if_int(char c)
{
	return (c >= '0' && c <= '9');
}

/* makes a string into an integer
 * returns count 
 */
static int strtoint(int dest, char *str, int *cnt)
{
	while (check_if_int(*str)) {
		dest = dest * 10 + (*str - '0');
		str++;
		(*cnt)++;
	}
	return dest;
}
int kvsprintf(char *buf, const char *fmt, va_list args)
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
				precision = strtoint(precision, (char*)fmt, &tmp);
				fmt += tmp;
			}

			/* now check what we can print */
			switch (*fmt) {
				/* char */
				case 'c': {
					*buf++ = (char)va_arg(args, int);
					break;
				}
				/* string */
				case 's': {
					const char *str = va_arg(args, const char*);
					while (*str)
						*buf++ = *str++;
					break;
				}
				/* integers */
				case 'u': {
					is_uint = true;
					__attribute__((fallthrough));
				}
				case 'i': __attribute__((fallthrough));
				case 'd': {
					char tmp[21];
					intostr(tmp, precision,
						*fmt == 'd' ? va_arg(args, int) :
						va_arg(args, uint32_t), 10, false, false, is_uint);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buf++ = tmp[i];
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
					
					intostr(tmp, precision, value, 16, add_prefix, hex_uppercase, is_uint);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buf++ = tmp[i];
					break;
				}

				/* binary */
				case 'b':
					char tmp[35];
					intostr(tmp, precision, va_arg(args, uint32_t), 2, add_prefix, hex_uppercase, is_uint);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buf++ = tmp[i];
					break;

				/* long integers -> uint64 */
				case 'L':
					hex_uppercase = true;
					__attribute__((fallthrough));
				case 'l': {
					switch (*(++fmt)) {
						case 'X':
							hex_uppercase = true;
							__attribute__((fallthrough));
						case 'x': {
							char tmp[19];
							uint64_t value = va_arg(args, uint64_t);

							intostr(tmp, precision, value, 16,
								add_prefix, hex_uppercase, true);

							for (int i = 0; tmp[i] != '\0'; i++)
								*buf++ = tmp[i];
							break;
						}

						case 'u': {
							char tmp[21];
							uint64_t value = va_arg(args, uint64_t);

							intostr(tmp, precision, value, 10,
								false, false, true);

							for (int i = 0; tmp[i] != '\0'; i++)
								*buf++ = tmp[i];
							break;
						}

						case 'd': __attribute__((fallthrough));
						case 'i': {
							char tmp[21];
							int64_t value = va_arg(args, int64_t);

							intostr(tmp, precision, value, 10,
								false, false, false);

							for (int i = 0; tmp[i] != '\0'; i++)
							*buf++ = tmp[i];
						break;
						}
					}
					break;
				}
				default:
					*buf++ = *fmt;
					break;
			}
		} else {
			*buf++ = *fmt;
		}
		fmt++;
		/* always reset */
		precision = 0;
		add_prefix = false;
		is_uint = false;
		hex_uppercase = false;
	}
	*buf = '\0';
	return count;
}

int kprintf(const char *fmt, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, fmt);

	int ret = kvsprintf(buf, fmt, args);
	va_end(args);

	console_write(&kcon, buf);
	serial_puts(buf);
	return ret;
}

int kprint(int log, const char *fmt, ...)
{
	uint32_t oldfg = kcon.colfg;
	switch (log) {
		case KLOG_WARN:
			kcon.colfg = 0xFFA500;
			break;
		case KLOG_ERROR:
			kcon.colfg = 0xFF0000;
			break;
		case KLOG_DEBUG:
			kcon.colfg = 0x808080;
			break;
	}
	char buf[2048];
	va_list args;
	va_start(args, fmt);

	int ret = kvsprintf(buf, fmt, args);
	va_end(args);

	console_write(&kcon, buf);
	serial_puts(buf);

	kcon.colfg = oldfg;
	return ret;
}