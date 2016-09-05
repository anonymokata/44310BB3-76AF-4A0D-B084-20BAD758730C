#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

struct Abacus {
	int d, c, l, x, v, i;
};

void tally_numerals(char* numerals, struct Abacus* abacus) {
	int end;

	end = strlen(numerals) - 1;
	char previous = NULL;
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
			if (previous == 'D') {
				abacus->d--;
				abacus->c += 4;
			} else {
				abacus->c++;
			}
		} else if (numerals[index] == 'D') {
			abacus->d++;
		}

		previous = numerals[index];
	}
}

void set_char_and_increment(char* string, char character, int* index) {
	// printf("string : %s, character : %c, index : %d\n", string, character, *index);
	string[*index] = character;
	(*index)++;
}

void set_char_and_increment_n_times(char* string, char character, int* index, int n) {
	int count;
	for (count = 0; count < n; count++) {
		set_char_and_increment(string, character, index);
	}	
}

char* add(char* addend1, char* addend2) {
	struct Abacus abacus = { 0, 0, 0, 0, 0, 0 };

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

	if (abacus.c >= 5) {
		abacus.d++;
		abacus.c -= 5;
	}

	char* sum = malloc(abacus.c + abacus.l + abacus.x + abacus.v + abacus.i + 1);
	int index = 0;

	if (abacus.c == 4) {
		set_char_and_increment(sum, 'C', &index);
		set_char_and_increment(sum, 'D', &index);
	} else {
		set_char_and_increment_n_times(sum, 'D', &index, abacus.d);
		set_char_and_increment_n_times(sum, 'C', &index, abacus.c);
	}

	if (abacus.x == 4) {
		set_char_and_increment(sum, 'X', &index);
		if (abacus.l > 0) {
			set_char_and_increment(sum, 'C', &index);
		} else {
			set_char_and_increment(sum, 'L', &index);
		}
	} else {
		set_char_and_increment_n_times(sum, 'L', &index, abacus.l);
		set_char_and_increment_n_times(sum, 'X', &index, abacus.x);
	}

	if (abacus.i == 4) {
		set_char_and_increment(sum, 'I', &index);
		if (abacus.v > 0) {
			set_char_and_increment(sum, 'X', &index);
		} else {
			set_char_and_increment(sum, 'V', &index);
		}
	} else {
		set_char_and_increment_n_times(sum, 'V', &index, abacus.v);
		set_char_and_increment_n_times(sum, 'I', &index, abacus.i);
	}

	sum[index] = '\0';

	return sum;
}
