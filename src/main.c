#include "header.h"


int main(int argc, char *argv[])
{
	/* no type safety/memory safety for the input since this will 
	 * later be implemented as a function without user input
	 * for now this simply assumes the user has all power */

	const char *file_path = argv[1];	/* will segfault if missing args */
    int i = 0;
    int j = 0;  
    json_content contents[5];


	json_data country =
	{
		NULL,
        {ARRAY, NULL_VALUE},
		"countries",
        NULL,
	};
	json_data cold =
	{
		NULL,
        {VALUE, BOOL},
		"cold",
        NULL
	};
	json_data currency =
	{
		NULL,
        {ARRAY, NULL_VALUE},
		"currencies",
        NULL
	};
	json_data calling_code =
	{
		NULL,
        {VALUE, STRING},
		"calling code",
        NULL
	};
	json_data population =
	{
		NULL,
        {VALUE, INTEGER},
		"population",
        NULL
	};
    

    json_data entries[5];

    calling_code.content = &contents[0];
    population.content = &contents[1];
    currency.content = &contents[2];
    country.content = &contents[3];
    cold.content = &contents[4];
	
	entries[0] = calling_code;
	entries[1] = population;
	entries[2] = currency;
	entries[3] = country;
	entries[4] = cold;

	json_parse(file_path, 4, entries);
    
    printf("main.c (debug): \n");
    for (i = 0; i < 4; i++) {
        printf("KEY: %s\n", entries[i].key_value);
        switch (entries[i].type.content_type) {
            case VALUE:
            switch (entries[i].type.data_type) { 
                case INTEGER:
                    printf("TYPE: INTEGER \n");
                    printf("VALUE: %ld\n", entries[i].content->value.data.integer);
                    break;
                case STRING:
                    printf("TYPE: STRING \n");
                    printf("VALUE: %s\n", entries[i].content->value.data.string);
                    break;
                case BOOL:
                    break;
                case CHAR:
                    break;
                case FLOAT:
                    break;
                case UNKNOWN:
                    printf("Unknown type.");
                    break;
            }
            break;
          case ARRAY:
            printf("TYPE: ARRAY\n");
            printf("Array List: \n");
            for(j = 0; entries[i].content->array[j].type != UNKNOWN; j++) {
                printf("Array [%d]. value: %s\n", j, entries[i].content->array[j].data.string);
            } 
            break;
          case OBJECT:
            break;
        }
    }


	if (argc < 2)
	{
		fprintf(stderr, "Missing arguments in command\n");
	}

	return 0;
}

