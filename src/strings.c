#include "header.h"

/* returns a pointer to a heap allocated string containing the content of the request */
char *str_content_alloc(char *line, size_t *starting_index, size_t *size)
{
	char *str = NULL;
	size_t j = *(starting_index);
	size_t str_size = 0;

	while (line[j] != '\0')
	{
		if (line[j] == '"' && line[j - 1] != '\\')
			break;
		j++;
	}

	if (line[j] != '"')
	{
		fprintf(stderr, "Unterminated string\n");
		exit(EXIT_FAILURE);
	}

	str_size = j - (*starting_index);
	str = smalloc(str_size + 1);

	memcpy(str, line + (*starting_index), str_size);
	str[str_size] = '\0';

	if (size != NULL)
		*(size) = str_size;

	*(starting_index) = j;	/* points to the closing quote */

	return str;
}
