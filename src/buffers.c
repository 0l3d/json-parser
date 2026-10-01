#include "header.h"

char *str_content_alloc(char *line, size_t *starting_index)
{
	char *str = NULL;
	size_t j = 0;
	size_t str_size = 1;
	size_t end_index = *(starting_index);
	size_t ret = 0;

	printf("\n\n");
	for (j = *(starting_index) + 1; !(line[j] == '\"' && line[j - 1] != '\\') ; j++)
	{
		str_size++;
	}

	str = smalloc(str_size + 1);
	memcpy(str, line + *(starting_index), str_size + 1);
	str[str_size] = '\0';

	printf("Size: %zu\n", str_size);
	printf("ENDING STR: %s\n", str);
	return str;
}
