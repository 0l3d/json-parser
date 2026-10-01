#include "header.h"
#include <ctype.h>

#define LINE_LEN 4096
#define STARTING_ALLOCATION 512

const Bool verbose = True;

int json_parse(const char *file_path, uint32_t num_entries, json_data json_entry[])
{
	/* json_state is an enum defined in src/enums.h
	 * it is used in order to store the current parsing mode (or the type of value expected */
	json_state state = EXPECT_KEY;

	FILE *fp = fopen(file_path, "r");
	char *line = NULL;	/* used for storing the line buffer in the file */

	size_t total_allocations = 0;
	size_t buffer_increment = STARTING_ALLOCATION;

	size_t start_quote_index = 0;

	unsigned int i = 0;	/* used for the current char */

	size_t str_size = 0;
	char *key_value = NULL;	/* TODO fix the thousands of memory leaks/lost pointers */
	char *content = NULL;	/* TODO fix the thousands of memory leaks/lost pointers */

	char buf[LINE_LEN];
	size_t index;
	char *p;
	void *tmp;		/* for reallocs */
	size_t len;		/* for len operations */
	uint32_t num_lookups = 0;	/* (iterator) counts the number of entries looked up exits when
					   everything is done */

	/* relative to the current entry (the key value being looked up
	 * example: in the following line: { "name": "John" } the key 
	 * value being looked up is name and the content associated with
	 * the key value is "John" */
	uint32_t current_entry = 0;	/* store the current entry being looked up */
	Bool valid_key_found = False;	/* only true when a key is matched example: 
					   if we're looking for "name" and we found it,
					   valid_key_found is set to true */

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

				break;

			case '=':	/* both characters are accepted */
			case ':':
				if (state == EXPECT_COLON)
				{
					state = EXPECT_CONTENT;
				}
				else if (state == EXPECT_KEY)
				{
					fprintf(stderr, "Syntax error in JSON, a colon is only expected to be after a key_value\n");
					/* TODO print context for easy debugging */
					exit(1);
				}
				else
				{
					fprintf(stderr, "Syntax error in JSON, expected content (value).\nDouble colons are not allowed */ \n");
					exit(1);
				}

				break;
			case '"':
				if (state == EXPECT_KEY)
				{
					start_quote_index = i + 1;

					key_value = str_content_alloc(line, &start_quote_index);
					if (verbose)
					{
						printf("key value -> %s\n", key_value);	/* prints the key_value as a test */
					}

					valid_key_found = False;
					/* TODO validate num_entries being unsigned or edit fn declaration */
					current_entry = key_match(&valid_key_found, key_value, (unsigned)num_entries, json_entry);

					if (valid_key_found)
					{
						json_entry[current_entry].key_value = key_value;
						printf("entry : %d has been found under the name \"%s\"\n", current_entry, key_value);
						state = EXPECT_COLON;
						num_lookups++;
					}
					else
					{
						fprintf(stderr, "Key not found: \"%s\"\n", key_value);
						/* TODO handle memory leaks */
						exit(1);
					}

					if (str_size > INT32MAX)
					{
						fprintf(stderr, "Avoided integer overflow in json_parse()\n");
						fprintf(stderr, "%lu is greater than the upper bound: %d\n", str_size, INT32MAX);
						/* TODO add failure boolean for freeing all buffers and exiting safely */
						break;
					}

					i += (to_uint32(str_size) + 1);
					start_quote_index = 0;
					break;
				}
				__attribute__ ((fallthrough));
			default:
				/* full expression is only true if the start_quote_index is 0 */
				if ((!start_quote_index) && state == EXPECT_CONTENT)
				{
					if (json_entry[current_entry].data_type == STRING)
					{
						if (line[i] == '"')
						{
							start_quote_index = i + 1;
						}

						/* get the value in the line & store it inside of content string */
						content = str_content_alloc(line, &start_quote_index);

						printf("Value of [%s]: %s\n", json_entry[current_entry].key_value, content);

						i += (to_uint32(str_size) + 1);
						state = EXPECT_KEY;
					}
					else if (json_entry[current_entry].data_type == INTEGER)
					{
						if (line[i] == '"')
						{
							fprintf(stderr, "unexpected symbol '\"' in integer type\n");
							exit(1);
						}

						/* TODO store the information (buffer was allocated) somewhere in
						 * order to prevent memory leaks */
						json_entry[current_entry].content = malloc(sizeof(int64_t));
						i += parse_integer(line + i, json_entry[current_entry].content);

						if (verbose)
							printf("integer value -> %ld\n", *(int64_t*)(json_entry[current_entry].content));

						state = EXPECT_KEY;
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

	} while (num_lookups <= num_entries);

	if (key_value != NULL)
	{
		free(key_value);
	}
	free(line);

	return 0;
}
