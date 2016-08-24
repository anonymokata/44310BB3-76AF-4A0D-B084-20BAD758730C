#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

int c_count;
int l_count;
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
			if (previous == 'L') {
				l_count--;
				x_count += 4;
			} else {
				x_count++;
			}
		} else if (numerals[index] == 'L') {
			l_count++;
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

char* add(char* addend1, char* addend2) {
	printf("addend1 : %s, addend2 : %s\n", addend1, addend2);
	c_count = 0;
	l_count = 0;
	x_count = 0;
	v_count = 0;
	i_count = 0;

	tally_numerals(addend1);
	tally_numerals(addend2);

	while (i_count >= 5) {
		v_count++;
		i_count -= 5;
	}

	if (v_count >= 2) {
		x_count++;
		v_count -= 2;
	}

	if (x_count >= 5) {
		l_count++;
		x_count -= 5;
	}

	if (l_count >= 2) {
		c_count++;
		l_count -= 2;
	}

	char* sum = malloc(v_count + i_count + 1);
	strcpy(sum, "");

	append_n_times(sum, "C", c_count);
	if (x_count == 4) {
		if (l_count > 0) {
			strcat(sum, "XC");
		} else {
			strcat(sum, "XL");
		}
	} else {
		append_n_times(sum, "L", l_count);
		append_n_times(sum, "X", x_count);
	}
	if (i_count == 4) {
		if (v_count > 0) {
			strcat(sum, "IX");
		} else {
			strcat(sum, "IV");
		}
	} else {
		append_n_times(sum, "V", v_count);
		append_n_times(sum, "I", i_count);
	}

	return sum;
}
