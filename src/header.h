#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "json_data.h"

/* parse.c */
	int json_parse(const char *file_path, int num_entries, json_data json_entry[]);

/* compare.c */
	Bool str_compare(const char *arg, const char *str);
	uint32_t key_match(Bool *success, const char *key_value, uint32_t num_entries, json_data json_entry[]);

/* memory.c */
	void *srealloc(void *ptr, size_t size);
	void *smalloc(size_t size);

/* checks.c */
	void file_check(FILE *file_path, const char *filename);

/* integers */
	void parse_integer(const char *str, int32_t *result);


#define INT16MAX (32767)
#define INT16MIN (-32767)
#define UINT16MAX (65535)

#define INT32MAX (2147483647)
#define INT32MIN (-2147483647)
#define UINT32MAX (4294967295)
