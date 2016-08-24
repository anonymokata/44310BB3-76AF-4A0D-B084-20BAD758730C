#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

struct Abacus {
	int c, l, x, v, i;
};

void tally_numerals(char* numerals, struct Abacus* abacus) {
	int end;

	end = strlen(numerals) - 1;
	char previous = '\0';
	int index;
	for (index = end; index >= 0; index--) {
		if (numerals[index] == 'I') {
			if (previous == 'X') {
				abacus->x--;
				abacus->i += 9;
			} else if (previous == 'V') {
				abacus->v--;
				abacus->i += 4;
			} else {
				abacus->i++;
			}
		} else if (numerals[index] == 'V') {
			abacus->v++;
		} else if (numerals[index] == 'X') {
			if (previous == 'C') {
				abacus->c--;
				abacus->x += 9;
			} else if (previous == 'L') {
				abacus->l--;
				abacus->x += 4;
			} else {
				abacus->x++;
			}
		} else if (numerals[index] == 'L') {
			abacus->l++;
		} else if (numerals[index] == 'C') {
			abacus->c++;
		}

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
	struct Abacus abacus = { 0, 0, 0, 0, 0 };

	tally_numerals(addend1, &abacus);
	tally_numerals(addend2, &abacus);

	while (abacus.i >= 5) {
		abacus.v++;
		abacus.i -= 5;
	}

	if (abacus.v >= 2) {
		abacus.x++;
		abacus.v -= 2;
	}

	while (abacus.x >= 5) {
		abacus.l++;
		abacus.x -= 5;
	}

	if (abacus.l >= 2) {
		abacus.c++;
		abacus.l -= 2;
	}

	char* sum = malloc(abacus.v + abacus.i + 1);
	strcpy(sum, "");

	append_n_times(sum, "C", abacus.c);
	if (abacus.x == 4) {
		if (abacus.l > 0) {
			strcat(sum, "XC");
		} else {
			strcat(sum, "XL");
		}
	} else {
		append_n_times(sum, "L", abacus.l);
		append_n_times(sum, "X", abacus.x);
	}
	if (abacus.i == 4) {
		if (abacus.v > 0) {
			strcat(sum, "IX");
		} else {
			strcat(sum, "IV");
		}
	} else {
		append_n_times(sum, "V", abacus.v);
		append_n_times(sum, "I", abacus.i);
	}

	return sum;
}
