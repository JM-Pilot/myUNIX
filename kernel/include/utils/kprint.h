#ifndef UTILS_KPRINT_H
#define UTILS_KPRINT_H

#include <stdarg.h>

/* kernel vsprintf
 * @note this prints directly into kernel console (kcon)
 * @param buf -- output buffer
 * @param fmt -- format string
 * @param args -- arguments list
 * @return numbers of characters copied
 */
int kvsprintf(char *buf, const char *fmt, va_list args);

/* kernel printf,
 * @note this prints directly into kernel console (kcon)
 * @param fmt -- format string
 * @param ... -- arguments
 * @return numbers of characters printed
 */
int kprintf(const char *fmt, ...);

#endif /* UTILS_KPRINT_H */