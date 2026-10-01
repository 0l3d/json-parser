#include "header.h"

char *str_content_alloc(char *line, size_t *starting_index)
{
	do {
		str_size = strcspn(line + *(starting_index) + j, "\"");
		if (line[*(starting_index) + str_size + j] == '\\')
		{
			/* the quote is escaped with a backslash "\"text\"" */
			j++;
			continue;
		}
		else
		{
			break;
		}
	} while (1);
