#include "header.h"
#include <ctype.h>

#define LINE_LEN 4096
#define STARTING_ALLOCATION 512

Bool verbose = True;

int json_parse(const char *file_path, int num_entries, json_data json_entry[])
{
	FILE *fp = fopen(file_path, "r");
	char *line = NULL;	/* used for storing the line buffer in the file */

	size_t total_allocations = 0;
	size_t buffer_increment = STARTING_ALLOCATION;

	Bool open_quote = False;
	int start_quote_index = 0;
	int ending_quote_index = 0;

	int j = 0; /* shared iterator (must be set to 0 after use) */
	int i = 0;	/* used for the current char */

	size_t str_size = 0;
	char *key_value = NULL;

	char buf[LINE_LEN];
	size_t index;
	char *p;
	void *tmp;		/* for reallocs */
	size_t len;		/* for len operations */
	int num_lookups = 0;	/* counts the number of entries looked up exits when
					   everything is done */

	/* relative to the current entry (the key value being looked up
	 * example: in the following line: { "name": "John" } the key 
	 * value being looked up is name and the content associated with
	 * the key value is "John" */
	uint32_t current_entry = 0;	/* store the current entry being looked up */
	Bool valid_key_found = False;	/* only true when a key is matched example: 
					   if we're looking for "name" and we found it,
					   valid_key_found is set to true*/
	Bool valid_key_expr = False;	/* This bool is only true when valid_key_found is true (see above)
					   and if we found a colon (':') following the valid key */


	file_check(fp, file_path);	/* checks for fp being NULL */

	/* This section turns a multi-line file into one line.
	 */
	total_allocations += STARTING_ALLOCATION;
	line = smalloc(total_allocations);
	line[0] = '\0';

	while (fgets(buf, sizeof(buf), fp))
	{
		p = strchr(buf, '\n');

		if (p)
			*p = '\0';

		index = strlen(buf);

		len = strlen(line);
		/* Reallocation */
		if (len + index + 2 > total_allocations)
		{
			buffer_increment = STARTING_ALLOCATION;
			do 
			{
				/* increment the buffer size exponentially */
				total_allocations += buffer_increment;
				buffer_increment <<= 1;
			} while (len + index + 2 > total_allocations);

			tmp = srealloc(line, total_allocations);
			line = tmp;
		}
		/* Reallocation */

		memcpy(line + len, buf, index);
		line[len + index] = ' ';
		line[len + index + 1] = '\0';
	}
	fclose(fp);
	/* This section turns a multi-line file into one line. */

	printf("CODE: %s\n", line);

	do
	{
		open_quote = False;
		i = 0;
		key_value = NULL;
		/* only valid since line is an array of chars */

		while (line[i] != '\0')
		{
			if (isspace((unsigned char)line[i]))
			{	/* isspace checks every whitespace */
				i++;
				continue;
			}

			switch (line[i])
			{
			case '[':
				/* start of array definition */
				break;
			case ']':
				/* end of array definition */
				break;
			case '}':
				/* end of object definition */
				break;
			case '{':
				/* start of object definition */
				break;
			case ',':
				/* comma for seperating */
				break;
			case ';':
				if (!(i > start_quote_index + (signed)str_size))
				{
					if (open_quote || key_value == NULL)
					{
						fprintf(stderr, "Quotes cannot span across multiple lines\n");
						fprintf(stderr, "The following quote is never ended: %s\n", line + start_quote_index - 1);
						exit(1);
					}
				}
				i++;
				continue;

			case '=':	/* both characters are accepted */
			case ':':
				if (valid_key_found)
					valid_key_expr = True;
				break;
			case '"':
				if (!valid_key_expr)
				{
					if (!open_quote)
					{
						open_quote = True;
						start_quote_index = i + 1;
					}

					if (start_quote_index < 0)
					{
						fprintf(stderr, "Invalid value in start quote index");
						/* TODO handle memory leaks on error */
						exit(EXIT_FAILURE);
					}

					j = 0;
					do {
						str_size = (unsigned)to_int32((int64_t)strcspn(line + start_quote_index + j, "\""));
						if (line[(unsigned)start_quote_index + str_size + (unsigned)j] == '\\')
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

					if (key_value != NULL)
					{
						free(key_value);
					}

					key_value = smalloc(str_size + 1);

					/* copy bytes from line into the key_value buffer
					 * memcpy() will only copy 'str_size' bytes into the key_value buffer */
					memcpy(key_value, line + start_quote_index, str_size);
					key_value[str_size] = '\0';
					printf("key value -> %s\n", key_value);	/* prints the key_value as a test */

					valid_key_found = False;
					/* TODO validate num_entries being unsigned or edit fn declaration */
					current_entry = key_match(&valid_key_found, key_value, (unsigned)num_entries, json_entry);

					if (valid_key_found)
					{
						json_entry[current_entry].key_value = key_value;
						printf("entry : %d has been found under the name \"%s\"\n", current_entry, key_value);
					}

					if (str_size > INT32MAX)
					{
						fprintf(stderr, "Avoided integer overflow in json_parse()\n");
						fprintf(stderr, "%lu is greater than the upper bound: %d\n", str_size, INT32MAX);
						/* TODO add failure boolean for freeing all buffers and exiting safely */
						break;
					}

					i += (to_int32((int64_t)str_size) + 1);
					start_quote_index = 0;
					open_quote = False;
					num_lookups++;
					break;
				}
				__attribute__ ((fallthrough));
			default:
				/* full expression is only true if the start_quote_index is 0 */
				if ((!start_quote_index) && valid_key_expr)
				{
					if (json_entry[current_entry].data_type == STRING)
					{
						if (line[i] == '"')
						{
							start_quote_index = i + 1;
						}
						str_size = strcspn(line + start_quote_index, "\"");
						key_value = smalloc(str_size + 1);
						memcpy(key_value, line + start_quote_index, str_size);
						key_value[str_size] = '\0';
						printf("Value of [%s]: %s\n", json_entry[current_entry].key_value, key_value);

						i += (to_int32((int64_t)str_size) + 1);
						valid_key_expr = False;
					}
					else if (json_entry[current_entry].data_type == INTEGER)
					{
						if (line[i] == '"')
						{
							fprintf(stderr, "unexpected symbol '\"' in integer type\n");
							exit(1);
						}

						/* TODO store value in buffer allocated (sizeof(uint64_t) ) */

						/* TODO store the information (buffer was allocated) somewhere in
						 * order to prevent memory leaks */
						json_entry[current_entry].content = malloc(sizeof(int64_t));
						i += parse_integer(line + i, json_entry[current_entry].content);

						if (verbose)
							printf("integer value -> %ld\n", *(int64_t*)(json_entry[current_entry].content));
					}
					else if (json_entry[current_entry].data_type == FLOAT)
					{
						/* currently unsupported */
						if (line[i] == '"')
						{
							fprintf(stderr, "unexpected symbol '%c' in floating type\n", line[i]);
							exit(1);
						}
					}
					else if (json_entry[current_entry].data_type == BOOL)
					{
						/* currently unsupported */
					}
				}
			}

			i++;
		}

	} while (num_entries > num_lookups);

	if (key_value != NULL)
	{
		free(key_value);
	}
	free(line);

	return 0;
}
