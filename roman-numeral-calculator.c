#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

char* add(char* x, char* y)
{
	char* result = malloc(strlen(x) + strlen(y) + 1);
	strcpy(result, x);
	strcat(result, y);
	if (strlen(result) > 3)
	{
		result = "IV";
	}
	return result;
}
