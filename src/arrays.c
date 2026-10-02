#include "header.h"

/* Handles recursive parsing for nested JSON arrays.
 * Parsing Algorithm:
 * '[' -> find the next comma -> ',' -> extract the value between commas or brackets
 * -> recognize the type -> add element to array variable with the detected type
 * -> loop until ']' is encountered
 * -> return the newly allocated array
 */
/* For now, I only parse 2 array elements. */
size_t json_array_parser(json_data *content, char *line, size_t c_pos)
{
	size_t i = c_pos + 1;
	json_value buffer;
	size_t total_values = 0;
	size_t str_size = 0;
	Bool reached_end = False; 

	/* Only 2 for now. */
	content->content->array = smalloc(3 * sizeof(json_value));
	while (line[i] != '\0' && reached_end == False) 
	{
		switch (line[i]) 
		{
		case '[':
			/* recursion */
			break;
		case ']':
			/* The last element could be any type, but for now I am only adding strings. */
			content->content->array[total_values].type = STRING;
			content->content->array[total_values++].data.string = buffer.data.string;
			i++;
			reached_end = True;
			content->content->array[total_values].type = UNKNOWN;
			/* The last element is set to UNKNOWN to mark the end of the array. */
			break;
		case '{':
			/* OBJECT */ 
			break;
		case '}':
			/* OBJECT */
			break;
		case '"':
			/* STRING */ 
			buffer.type = STRING;
			i++;
			buffer.data.string = str_content_alloc(line, &i, &str_size);
			break;
		case ',':
			/* Flush buffer into content array and move to the next item. */
			content->content->array[total_values].type = STRING;
			content->content->array[total_values++].data.string = buffer.data.string;
			break;
		default:	
			/* INTEGER */
			break;
		}
		i++;
	}
	return i;
}

