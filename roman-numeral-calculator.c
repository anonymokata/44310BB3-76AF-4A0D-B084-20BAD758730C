#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

int x_count;
int v_count;
int i_count;

void tally_numerals(char* numerals) {
	int end;

	end = strlen(numerals) - 1;
	char previous = '\0';
	int index;
	for (index = end; index >= 0; index--) {
		if (numerals[index] == 'I') {
			if (previous == 'X') {
				x_count--;
				i_count += 9;
			} else if (previous == 'V') {
				v_count--;
				i_count += 4;
			} else {
				i_count++;
			}
		} else if (numerals[index] == 'V') {
			v_count++;
		} else if (numerals[index] == 'X') {
			x_count++;
		}
		printf("char : %c, x : %d, v : %d, i : %d\n", numerals[index], x_count, v_count, i_count);

		previous = numerals[index];
	}
}

void append_n_times(char* string, char* suffix, int n) {
	int count;
	for (count = 1; count <= n; count++) {
		strcat(string, suffix);
	}	
}

char* add(char* x, char* y) {
	printf("x : %s, y : %s\n", x, y);
	x_count = 0;
	v_count = 0;
	i_count = 0;

	tally_numerals(x);
	tally_numerals(y);

	while (i_count >= 5) {
		v_count++;
		i_count -= 5;
	}
	printf("x : %d, v : %d, i : %d\n", x_count, v_count, i_count);

	if (v_count >= 2) {
		x_count++;
		v_count -= 2;
	}
	printf("x : %d, v : %d, i : %d\n", x_count, v_count, i_count);

	char* result = malloc(v_count + i_count + 1);
	strcpy(result, "");

	append_n_times(result, "X", x_count);
	if (i_count == 4) {
		if (v_count > 0) {
			strcat(result, "IX");
		} else {
			strcat(result, "IV");
		}
	} else {
		append_n_times(result, "V", v_count);
		append_n_times(result, "I", i_count);
	}

	return result;
}
