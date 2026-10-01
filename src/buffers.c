#include "header.h"

/* returns a pointer to a heap allocated string containing the content of the request */
char *str_content_alloc(char *line, size_t *starting_index)
{
	char *str = NULL;
	size_t j = 0;
	size_t str_size = 1;

	printf("\n\n");
	for (j = *(starting_index) + 1; !(line[j] == '\"' && line[j - 1] != '\\') ; j++)
	{
		str_size++;
	}

	str = smalloc(str_size + 1);
	memcpy(str, line + *(starting_index), str_size + 1);
	str[str_size] = '\0';

	return str;
}
