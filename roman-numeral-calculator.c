#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

#define abacus_row_count 7

struct Row {
	char symbol;
	int count;
};

struct Abacus {
	struct Row rows[abacus_row_count];
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
		}
	};

	return abacus;
}

int is_even(int number) {
	return number % 2 == 0 ? 1 : 0;
}

int get_abacus_index(char symbol, struct Abacus* abacus) {
	int index;

	for (index = 0; index < abacus_row_count; index++) {
		if (symbol == abacus->rows[index].symbol) {
			return index;
		}
	}

	return -1;
}

void tally_numerals(char* input, struct Abacus* abacus) {
	int input_index;
	int end = strlen(input) - 1;
	int abacus_index;
	char previous_symbol = NULL;

	for (input_index = end; input_index >= 0; input_index--) {
		abacus_index = get_abacus_index(input[input_index], abacus);

		if (abacus_index == (abacus_row_count - 1) || !is_even(abacus_index)) {

			abacus->rows[abacus_index].count++;

		} else {

			if (previous_symbol == abacus->rows[abacus_index + 1].symbol) {

				abacus->rows[abacus_index + 1].count--;
				abacus->rows[abacus_index].count += 4;

			} else if (previous_symbol == abacus->rows[abacus_index + 2].symbol) {

				abacus->rows[abacus_index + 2].count--;
				abacus->rows[abacus_index].count += 9;

			} else {
				abacus->rows[abacus_index].count++;
			}
		}

		previous_symbol = input[input_index];
	}
}

void adjust_counts(struct Abacus* abacus) {
	int index;
	int ratio_to_next_row;

	for (index = 0; index < abacus_row_count - 1; index++) {
		ratio_to_next_row = is_even(index) ? 5 : 2;

		while (abacus->rows[index].count >= ratio_to_next_row) {
			abacus->rows[index + 1].count++;
			abacus->rows[index].count -= ratio_to_next_row;
		}
	}
}

void append(char* string, char symbol, int* index) {
	// printf("string : %s, symbol : %c, index : %d\n", string, symbol, *index);
	string[*index] = symbol;
	(*index)++;
}

void append_n_times(char* string, char symbol, int* index, int n) {
	int count;
	for (count = 0; count < n; count++) {
		append(string, symbol, index);
	}	
}

char* to_roman_numerals(struct Abacus* abacus) {
	char* result = malloc(0);
	int result_index = 0;
	int abacus_index;

	for (abacus_index = abacus_row_count - 1; abacus_index >= 0; abacus_index--) {

		if (!is_even(abacus_index) && abacus->rows[abacus_index - 1].count == 4) {

			append(result, abacus->rows[abacus_index - 1].symbol, &result_index);

			if (abacus->rows[abacus_index].count == 0) {
				append(result, abacus->rows[abacus_index].symbol, &result_index);
			} else {
				append(result, abacus->rows[abacus_index + 1].symbol, &result_index);
			}

			abacus_index--;

		} else {
			append_n_times(result, abacus->rows[abacus_index].symbol, &result_index, abacus->rows[abacus_index].count);
		}
		// printf("result : %s\n", result);
	}

	result[result_index] = '\0';

	return result;
}

char* add(char* addend1, char* addend2) {
	struct Abacus abacus = initialize_abacus();

	// printf("addend1 : %s, addend2 : %s\n", addend1, addend2);

	tally_numerals(addend1, &abacus);
	tally_numerals(addend2, &abacus);

	adjust_counts(&abacus);

	// printf("abacus :  i : %d, v : %d, x : %d\n", abacus.rows[0].count, abacus.rows[1].count, abacus.rows[2].count);

	return to_roman_numerals(&abacus);
}
