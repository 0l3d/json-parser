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
	Bool key_specified = False;
	int start_quote_index = 0;

	int i = 0;

	size_t str_size = 0;
	char *key_value = NULL;

	char *endptr = NULL;
	long value = 0;

	char buf[LINE_LEN];
	size_t index;
	int digit_size;		/* Calculating integer size */
	long temp_digit;	/* temp integer value for calculations */
	char *p;
	void *tmp;		/* for reallocs */
	size_t len;		/* for len operations */
	int num_lookups = 0;	/* counts the number of entries looked up exits when
					   everything is done */
	uint32_t current_entry = 0;	/* store the current entry being looked up
					   if an entry is matched (found) the counter goes up */

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

			tmp = realloc(line, total_allocations);
			if (tmp == NULL)
			{
				fprintf(stderr, "realloc failed\n");
				exit(EXIT_FAILURE);
			}
			line = tmp;
		}
		/* Reallocation */

		memcpy(line + len, buf, index);
		line[len + index] = ' ';
		line[len + index + 1] = '\0';
	}
	fclose(fp);
	/* This section turns a multi-line file into one line.
	 */

	printf("CODE: %s\n", line);

	do
	{
		Bool key_success = False;
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
				if (key_success)
					key_specified = True;
				break;
			case '"':
				if (!key_specified)
				{
					if (!open_quote)
					{
						open_quote = True;
						start_quote_index = i + 1;
					}

					str_size = strcspn(line + start_quote_index, "\"");

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

					key_success = False;
					/* TODO validate num_entries being unsigned or edit fn declaration */
					current_entry = key_match(&key_success, key_value, (unsigned)num_entries, json_entry);

					if (key_success)
					{
						json_entry[current_entry].key_value = key_value;
						printf("entry : %d\n", current_entry);
					}

					if (str_size > INT32MAX)
					{
						fprintf(stderr, "Avoided integer overflow in json_parse()\n");
						fprintf(stderr, "%lu is greater than the upper bound: %d\n", str_size, INT32MAX);
						/* TODO add failure boolean for freeing all buffers and exiting safely */
						break;
					}

					i += (int)str_size + 1;
					start_quote_index = 0;
					open_quote = False;
					num_lookups++;
					break;
				}
				__attribute__ ((fallthrough));
			default:
				/* full expression is only true if the start_quote_index */
				if ((!start_quote_index) && key_specified)
				{
					if (json_entry[current_entry].data_type == STRING)
					{
						if (line[i] == '"')
						{
							start_quote_index = i + 1;
						}
						str_size = strcspn(line + start_quote_index, "\"");
						key_value = malloc(str_size + 1);
						memcpy(key_value, line + start_quote_index, str_size);
						key_value[str_size] = '\0';
						printf("Value of [%s]: %s\n", json_entry[current_entry].key_value, key_value);

						if (str_size > INT32MAX)
						{
							fprintf(stderr, "Avoided integer overflow in json_parse()\n");
							fprintf(stderr, "%lu is greater than the upper bound: %d\n", str_size, INT32MAX);
							/* TODO add failure boolean for freeing all buffers and exiting safely */
							break;
						}

						i += (int)str_size + 1;
						key_specified = False;
					}
					else if (json_entry[current_entry].data_type == INTEGER)
					{
						if (line[i] == '"')
						{
							fprintf(stderr, "unexpected symbol '\"' in integer type\n");
							exit(1);
						}

						value = strtol(line + i, &endptr, 10);

						/* no characters are valid */
						if (endptr == line + i)
						{
							fprintf(stderr, "invalid integer: %s\n", line + i);
							exit(1);
						}

						/* check the character following the integer */
						if ((*endptr != ';') && (*endptr != '\0') && !isspace((unsigned char)*endptr) && (*endptr != '}'))
						{
							fprintf(stderr, "invalid character '%c' after integer\n", *endptr);
							exit(1);
						}

						if (value < INT32MIN || value > INT32MAX)
						{
							fprintf(stderr, "integer out of bounds : %ld\n", value);
							exit(1);
						}
						/* calculating digit size */
						digit_size = 0;
						temp_digit = value;
						if (temp_digit == 0)
							digit_size = 1;
						else
							while (temp_digit != 0)
							{
								digit_size++;
								temp_digit /= 10;
							}
						/* TODO store value in buffer allocated (sizeof(uint64_t) ) */

						if (verbose)
							printf("integer value -> %ld\n", value);

						i += digit_size;
					}
					else if (json_entry[current_entry].data_type == FLOAT)
					{
						if (line[i] == '"')
						{
							fprintf(stderr, "unexpected symbol '%c' in floating type\n", line[i]);
							exit(1);
						}
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

