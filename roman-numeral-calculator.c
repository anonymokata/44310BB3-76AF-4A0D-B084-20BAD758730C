#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

#define abacus_rows 7

struct Row {
	char character;
	int count;
};

struct Abacus {
	struct Row rows[abacus_rows];
	int m, d, c, l, x, v, i;
};

struct Abacus initialize_abacus(void) {
	struct Abacus abacus = {
		{
			{ 'I', 0 },
			{ 'V', 0 },
			{ 'X', 0 },
			{ 'L', 0 },
			{ 'C', 0 },
			{ 'D', 0 },
			{ 'M', 0 }
		},
		0, 0, 0, 0, 0, 0, 0
	};

	return abacus;
}

int get_row_index(char character, struct Abacus* abacus) {
	int index;

	for (index = 0; index < abacus_rows; index++) {
		if (character == abacus->rows[index].character) {
			return index;
		}
	}

	return -1;
}

void tally_numerals(char* numerals, struct Abacus* abacus) {
	int end;

	end = strlen(numerals) - 1;
	char previous = NULL;
	int index;
	int row_index;
	for (index = end; index >= 0; index--) {
		row_index = get_row_index(numerals[index], abacus);

		if (row_index == (abacus_rows - 1) || row_index % 2 > 0) {

			abacus->rows[row_index].count++;

		} else {

			if (previous == abacus->rows[row_index + 1].character) {

				abacus->rows[row_index + 1].count--;
				abacus->rows[row_index].count += 4;

			} else if (previous == abacus->rows[row_index + 2].character) {

				abacus->rows[row_index + 2].count--;
				abacus->rows[row_index].count += 9;

			} else {
				abacus->rows[row_index].count++;
			}
		}
		// if (current == 'I') {
		// 	if (previous == 'X') {
		// 		abacus->x--;
		// 		abacus->i += 9;
		// 	} else if (previous == 'V') {
		// 		abacus->v--;
		// 		abacus->i += 4;
		// 	} else {
		// 		abacus->i++;
		// 	}
		// } else if (current == 'V') {
		// 	abacus->v++;
		// } else if (current == 'X') {
		// 	if (previous == 'C') {
		// 		abacus->c--;
		// 		abacus->x += 9;
		// 	} else if (previous == 'L') {
		// 		abacus->l--;
		// 		abacus->x += 4;
		// 	} else {
		// 		abacus->x++;
		// 	}
		// } else if (current == 'L') {
		// 	abacus->l++;
		// } else if (current == 'C') {
		// 	if (previous == 'M') {
		// 		abacus->m--;
		// 		abacus->c += 9;
		// 	} else if (previous == 'D') {
		// 		abacus->d--;
		// 		abacus->c += 4;
		// 	} else {
		// 		abacus->c++;
		// 	}
		// } else if (current == 'D') {
		// 	abacus->d++;
		// } else if (current == 'M') {
		// 	abacus->m++;
		// }

		previous = numerals[index];
	}
}

void reduce(struct Abacus* abacus) {
	int index;
	for (index = 0; index < abacus_rows - 1; index++) {
		if (index % 2 == 0) {
			while (abacus->rows[index].count >= 5) {
				abacus->rows[index + 1].count++;
				abacus->rows[index].count -= 5;
			}
		} else {
			if (abacus->rows[index].count >= 2) {
				abacus->rows[index + 1].count++;
				abacus->rows[index].count -= 2;
			}
		}
	}
}

void append(char* string, char character, int* index) {
	// printf("string : %s, character : %c, index : %d\n", string, character, *index);
	string[*index] = character;
	(*index)++;
}

void append_n_times(char* string, char character, int* index, int n) {
	int count;
	for (count = 0; count < n; count++) {
		append(string, character, index);
	}	
}

char* add(char* addend1, char* addend2) {
	struct Abacus abacus = initialize_abacus();

	printf("addend1 : %s, addend2 : %s\n", addend1, addend2);

	tally_numerals(addend1, &abacus);
	tally_numerals(addend2, &abacus);

	printf("abacus :  i : %d, v : %d, x : %d\n", abacus.rows[0].count, abacus.rows[1].count, abacus.rows[2].count);

	reduce(&abacus);

	printf("abacus :  i : %d, v : %d, x : %d\n", abacus.rows[0].count, abacus.rows[1].count, abacus.rows[2].count);

	// while (abacus.i >= 5) {
	// 	abacus.v++;
	// 	abacus.i -= 5;
	// }

	// if (abacus.v >= 2) {
	// 	abacus.x++;
	// 	abacus.v -= 2;
	// }

	// while (abacus.x >= 5) {
	// 	abacus.l++;
	// 	abacus.x -= 5;
	// }

	// if (abacus.l >= 2) {
	// 	abacus.c++;
	// 	abacus.l -= 2;
	// }

	// while (abacus.c >= 5) {
	// 	abacus.d++;
	// 	abacus.c -= 5;
	// }

	// if (abacus.d >= 2) {
	// 	abacus.m++;
	// 	abacus.d -= 2;
	// }

	char* sum = malloc(abacus.c + abacus.l + abacus.x + abacus.v + abacus.i + 1);
	int index = 0;
	int row_index;

	for (row_index = abacus_rows - 1; row_index >= 0; row_index--) {
		if (row_index % 2 != 0) {
			if (abacus.rows[row_index - 1].count == 4) {
				append(sum, abacus.rows[row_index - 1].character, &index);

				if (abacus.rows[row_index].count == 0) {
					append(sum, abacus.rows[row_index].character, &index);
				} else {
					append(sum, abacus.rows[row_index + 1].character, &index);
				}

				row_index--;
			} else {
				append_n_times(sum, abacus.rows[row_index].character, &index, abacus.rows[row_index].count);
			}
		} else {
			append_n_times(sum, abacus.rows[row_index].character, &index, abacus.rows[row_index].count);
		}
		printf("sum : %s\n", sum);
	}

	// append_n_times(sum, 'M', &index, abacus.m);

	// if (abacus.c == 4) {
	// 	append(sum, 'C', &index);
	// 	if (abacus.d > 0) {
	// 		append(sum, 'M', &index);
	// 	} else {
	// 		append(sum, 'D', &index);
	// 	}
	// } else {
	// 	append_n_times(sum, 'D', &index, abacus.d);
	// 	append_n_times(sum, 'C', &index, abacus.c);
	// }

	// if (abacus.x == 4) {
	// 	append(sum, 'X', &index);
	// 	if (abacus.l > 0) {
	// 		append(sum, 'C', &index);
	// 	} else {
	// 		append(sum, 'L', &index);
	// 	}
	// } else {
	// 	append_n_times(sum, 'L', &index, abacus.l);
	// 	append_n_times(sum, 'X', &index, abacus.x);
	// }

	// if (abacus.i == 4) {
	// 	append(sum, 'I', &index);
	// 	if (abacus.v > 0) {
	// 		append(sum, 'X', &index);
	// 	} else {
	// 		append(sum, 'V', &index);
	// 	}
	// } else {
	// 	append_n_times(sum, 'V', &index, abacus.v);
	// 	append_n_times(sum, 'I', &index, abacus.i);
	// }

	sum[index] = '\0';

	return sum;
}
