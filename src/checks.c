#include "header.h"

void file_check(FILE *file_path, const char *filename)
{
	if (file_path == NULL)
	{
		fprintf(stderr,	"file %s not found\n", filename);
		/* TODO free all allocated memory & exit */
		exit(EXIT_FAILURE);	/*file not found */
	}
}

uint32_t to_uint32(uint64_t input)
{
	if (input > UINT32MAX)
	{
		fprintf(stderr, "Avoided integer overflow\n");
		fprintf(stderr, "%" PRIu64 " is greater than the upper bound: %" PRIu32 "\n", input, (uint32_t)UINT32MAX);
	}
	else
	{
		return (uint32_t)input;
	}

	/* TODO free all allocated memory & exit */
	exit(EXIT_FAILURE);
}

int32_t to_int32(int64_t input)
{
	if (input > INT32MAX)
	{
		fprintf(stderr, "Avoided integer overflow\n");
		fprintf(stderr, "%" PRId64 " is greater than the upper bound: %" PRId32 "\n", input, INT32MAX);
	}
	else if (input < INT32MIN)
	{
		fprintf(stderr, "Avoided integer underflow\n");
		fprintf(stderr, "Your input \'%" PRId64 "\' is smaller than the lower bound: %" PRId32 "\n", input, INT32MIN);
	}
	else
	{
		return (int32_t)input;
	}

	/* TODO free all allocated memory & exit */
	exit(EXIT_FAILURE);
}
