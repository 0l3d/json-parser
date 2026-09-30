#include "header.h"

#include <ctype.h>

uint8_t parse_integer(const char *str, int64_t *result)
{
	char *endptr;
	long value;
	uint8_t digit_size = 0;
	int64_t temp_digit = 0;

	value = strtol(str, &endptr, 10);

	/* no characters are valid */
	if (*(endptr) == str[0])
	{
		fprintf(stderr, "invalid integer: %s\n", str);
		/* TODO handle memory leaks on fail */
		exit(EXIT_FAILURE);
	}

	/* check the character following the integer */
	if ((*endptr != ';') && (*endptr != ',') && (*endptr != '\0') && !isspace((unsigned char)*endptr) && (*endptr != '}'))
	{
		fprintf(stderr, "invalid character '%c' after integer\n", *endptr);
		/* TODO handle memory leaks on fail */
		exit(EXIT_FAILURE);
	}

	/* calculating digit size (characters) */
	digit_size = 0;
	temp_digit = value;
	if (temp_digit == 0)
		digit_size = 1;
	else
		while (temp_digit != 0)
		{
			digit_size++;
			temp_digit /= 10;
			/* divide by 10 for base 10 
			 * no support planned for other bases (2, 8, 16 ...) */
		}

	*result = (int64_t)value;
	return digit_size;	/* in characters (base 10 only) */
}
