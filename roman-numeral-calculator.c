#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

char* add(char* x, char* y)
{
	int i = 0;
	int v = 0;
	char previous;

	int xend;
	xend = strlen(x) - 1;
	int index;
	for (index = xend; index >= 0; index--)
	{
		if (x[index] == 'I')
		{
			if (previous == 'V')
			{
				v--;
				i += 4;
			} else {
				i++;
			}
		}
		else if (x[index] == 'V')
		{
			v++;
		}
		previous = x[index];
	}

	int yend;
	yend = strlen(y) - 1;
	previous = '\0';
	for (index = yend; index >= 0; index--)
	{
		if (y[index] == 'I')
		{
			if (previous == 'V')
			{
				v--;
				i += 4;
			}
			else
			{
				i++;
			}
		}
		else if (y[index] == 'V')
		{
			v++;
		}
		previous = y[index];
	}

	char* result = malloc(strlen(x) + strlen(y) + 1);
	strcpy(result, "");
	int count;
	if (i >= 4) {
		i -= 5;
		v += 1;
	}
	if (i < 0) {
		strcat(result, "I");
	}
	for (count = 1; count <= v; count++)
	{
		strcat(result, "V");
	}
	for (count = 1; count <= i; count++)
	{
		strcat(result, "I");
	}

	return result;
}
