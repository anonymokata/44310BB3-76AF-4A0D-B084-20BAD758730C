#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

int x_count;
int v_count;
int i_count;

void tally_numerals(char* numerals) {
	int end;

	end = strlen(numerals) - 1;
	int index;
	char previous;
	for (index = end; index >= 0; index--) {
		if (numerals[index] == 'I') {
			if (previous == 'V') {
				v_count--;
				i_count += 4;
			} else {
				i_count++;
			}
		} else if (numerals[index] == 'V') {
			v_count++;
		}

		previous = numerals[index];
	}

	if (i_count >= 4) {
		v_count += 1;
		i_count -= 5;
	}

	if (v_count >= 2) {
		x_count++;
		v_count -= 2;
	}
}

void append_n_times(char* string, char *suffix, int n) {
	int count;
	for (count = 1; count <= n; count++) {
		strcat(string, character);
	}	
}

char* add(char* x, char* y) {
	x_count = 0;
	v_count = 0;
	i_count = 0;

	tally_numerals(x);
	tally_numerals(y);

	char* result = malloc(v_count + i_count + 1);
	strcpy(result, "");
	if (i_count < 0) {
		strcat(result, "I");
	}

	int count;
	append_n_times(result, "X", x_count);
	// for (count = 1; count <= x_count; count++) {
	// 	strcat(result, "X");
	// }

	for (count = 1; count <= v_count; count++) {
		strcat(result, "V");
	}

	for (count = 1; count <= i_count; count++) {
		strcat(result, "I");
	}

	return result;
}
