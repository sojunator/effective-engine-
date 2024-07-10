#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>



#include <stddef.h>
#include <stdint.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF) // if we found EOF, signal upwards that we are done
			return false; // Put char will call terminal_write, who when just print the string
	return true;
}

uint8_t writeNumberIntoAsciiBuffer(uint32_t number, char * buffer, uint8_t base)
{
	uint32_t i = 0;

	if (number == 0) {
		buffer[i++] = 48;

	}

	while(number != 0) {
		// 0 is 48 in ascii
		uint8_t val = (number % base);

		// 55 is 10 away from A, so if val is 10, we get A
		// If value is greater than 10, after modulu base, we have a number that is represented 
		// With chars aswell, so A becomes a base
		buffer[i++] = (val < 10 ? '0' : 'A') + val;
		number /= base;
	}
 
	for (uint32_t j = 0; j < i / 2; j++) {
		// -1 is due to us not wanting the null byte
		// Reverse order of number
		char tempswap = buffer[j];
		buffer[j] = buffer[(i - 1) - j];
		buffer[(i - 1) - j] = tempswap;
	}
	 
	buffer[i] = '\0';

	return i;
}



int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0; 

	// While no null char, indicating end of string
	while (*format != '\0') {
		// keep within some range
		size_t maxrem = INT_MAX - written;

		// if first char in string, or second, is not a paramertaized char
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++; // Go upwards until we either find a formatter, or null byte
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount; // at this point, amout is equal to how many bytes we could traverse without finding % or null byte
			written += amount;
			continue;
		}

		// we encountered a formatted string, as format is not null byte, and if was false
		const char* format_begun_at = format++;

		// As we do not have any way of doing allocations on the fly, we need to use this stack 
		// memory to move formatted numbers into the VGA buffer, as numbers can take up more space when stored
		// as chars, compared to integers.
		char buffer[32];

		if (*format == 'c') {
			format++;
			// extract value as char
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			int number = va_arg(parameters, int);
			int length = writeNumberIntoAsciiBuffer(number, buffer, 10);


			if (!maxrem) {
				return -1;
			}
			if (!print(buffer, sizeof(char) * length)) {
				return -1;
			}
			written++;

		} else if (*format == 'x') {
			format++;
			int number = va_arg(parameters, int);
			int length = writeNumberIntoAsciiBuffer(number, buffer, 16);
			
			if (!maxrem) {
				return -1;
			}
			if (!print(buffer, sizeof(char) * length)) {
				return -1;
			}
			written++;
		} else {
			// This is a wild card that starts at format location and prints all
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
