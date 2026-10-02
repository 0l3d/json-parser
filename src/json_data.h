#include "enums.h"

#define NULL_VALUE 0

typedef struct 
{
	union {
		char* string;
		int64_t integer;
		double ffloat;
		Bool boolean;
	} data;
	json_data_type type;
} json_value;

typedef struct 
{
	json_content_type type;
	json_value *array;
	json_value value;
} json_content;

typedef struct 
{
	json_content_type content_type;
	json_data_type data_type;
} json_type;


typedef struct
{
	char *parent_object;	/* currently unused */
	json_type type;
	char *key_value;	/* store the string associated with the value 
				   { "name": "John" } "name" being the key_value */
	json_content *content;
} json_data;

