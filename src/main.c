#include "header.h"


int main(int argc, char *argv[])
{
	/* no type safety/memory safety for the input since this will 
	 * later be implemented as a function without user input
	 * for now this simply assumes the user has all power */

	const char *file_path = argv[1];	/* will segfault if missing args */

	json_data country =
	{
		NULL,
		STRING,
		"country",
		NULL
	};
	json_data currency =
	{
		NULL,
		STRING,
		"currency",
		NULL
	};
	json_data calling_code =
	{
		NULL,
		STRING,
		"calling code",
		NULL
	};
	json_data population =
	{
		NULL,
		INTEGER,
		"population",
		NULL
	};

	json_data entries[2] = { 0 };

	entries[0] = calling_code;
	entries[1] = population;
	entries[2] = currency;
	entries[3] = country;

	json_parse(file_path, 4, entries);

	if (argc < 2)
	{
		fprintf(stderr, "Missing arguments in command\n");
	}

	return 0;
}

