#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* gets the length of a string */
size_t strlen(const char *str)
{
	size_t len = 0;
	while (*str) len++;
	return len;
}

/* compare 2 strings, returns 0 if successful 1 if not */
int strcmp(const char *str_1, const char *str_2)
{
	while (*str_1 && (*str_1 == *str_2))
	{
		str_1++;
		str_2++;
	}
	return *(const uint8_t*)str_1 - *(const uint8_t*)str_2;
}

/* sets the len of dest with the value */
void *memset(void *dest, int value, size_t len)
{
	uint8_t *d = dest;
	while (len--)
		*d++ = value;
	return dest;
}

/* moves src to len of dest */
void *memmove(void *dest, void *src, size_t len)
{
	uint8_t *d = dest;
	uint8_t *s = src;
	if (d < s) 
		while (len--)
			*d++ = *s++;
	else {
		uint8_t *ls = s + (len - 1);
		uint8_t	*ld = d + (len - 1);
		while (len--)
			*ld-- = *ls--;
	}
	return dest;
}

/* copies len of src to dest */
void *memcpy(void *dest, void *src, size_t len)
{
	uint8_t *d = dest;
	uint8_t *s = src;

	while (len--) {
		*d++ = *s++;
	}
	return dest;
}
