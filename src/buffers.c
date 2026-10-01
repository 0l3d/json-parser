#include "header.h"

char *str_content_alloc(char *line, size_t *starting_index)
{
	char *str = NULL;
	size_t j = 1;
	size_t str_size = 0;
	size_t end_index = 0;
	printf("str:%sline end\n", line);
	printf("str:%sline end\n", line + *(starting_index));
	do {
		end_index += strcspn(line + *(starting_index) + j, "\"");
		if (line[end_index - 1] == '\\')
		{
			/* the quote is escaped with a backslash "\"text\"" */
			printf("%ld str \'%s\n\'", j, line + end_index);
			j++;
			continue;
		}
		else
		{
			break;
		}
	} while (1);

	str_size = end_index + j;

	str = smalloc(str_size + 1);
	memcpy(str, line + *(starting_index), str_size + 1);
	str[str_size] = '\0';

	printf("Size: %zu\n", str_size);
	printf("ENDING STR: %s\n", str);
	return str;
}
