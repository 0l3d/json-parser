#include "header.h"

void *smalloc(size_t size)
{
	void *ptr = NULL;
	if (size <= 0)
	{
		fprintf(stderr, "Invalid malloc() call with size %lu\n", size);
		exit(1);
	}

	ptr = malloc(size);	/* allocate memory */
	if (ptr == NULL)
	{
		fprintf(stderr, "malloc() fn failed to allocated memory of size %lu on the heap\n", size);
		exit(1);
	}
	return ptr;	/* return pointer to buffer */
}

void *srealloc(void *ptr, size_t size)
{
	if (size <= 0)
	{
		fprintf(stderr, "Invalid realloc() call with size %lu\n", size);
		exit(EXIT_FAILURE);
	}

	if (ptr == NULL)
	{
		/* even though realloc() can handle NULL, we still print a warning for the developper */
		fprintf(stderr, "Warning: pointer passed into srealloc() is NULL\n");
		/* ptr = malloc(size);  malloc is used inside of realloc() instead */
	}

	ptr = realloc(ptr, size);	/* allocate memory */

	if (ptr == NULL)
	{
		fprintf(stderr, "realloc() fn failed to allocated memory of size %lu on the heap\n", size);
		exit(EXIT_FAILURE);
	}
	return ptr;	/* return pointer to buffer */
}
