#include "enums.h"

typedef struct
{
	char *parent_object;	/* currently unused */
	json_data_type data_type;
	char *key_value;	/* store the string associated with the value 
				   { "name": "John" } "name" being the key_value */
	void *content;	/* used to point to the data */
} json_data;
