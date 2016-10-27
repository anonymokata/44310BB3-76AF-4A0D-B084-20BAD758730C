#include <stdlib.h>
#include <string.h>

#include "roman-numeral-calculator.h"

#define abacus_row_count 7
#define ratio_to_next_row_odd 5
#define ratio_to_next_row_even 2
#define ratio_to_next_next_row 10

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

int ratio_to_next_row(int row_index) {
	return is_even(row_index) ? ratio_to_next_row_odd : ratio_to_next_row_even;
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

int is_preceding_subtractor(int abacus_index, int previous_abacus_index) {
	return is_even(abacus_index) && 
			abacus_index != (abacus_row_count - 1) && 
			(abacus_index + 1 == previous_abacus_index || 
			abacus_index + 2 == previous_abacus_index);
}

ReturnCode tally(char* input, struct Abacus* abacus) {
	int input_index;
	int end = strlen(input) - 1;
	int abacus_index;
	int previous_abacus_index = NULL;

	for (input_index = end; input_index >= 0; input_index--) {
		abacus_index = get_abacus_index(input[input_index], abacus);
		if (abacus_index < 0) {
			return INVALID_CHARACTER;
		}

		if (is_preceding_subtractor(abacus_index, previous_abacus_index)) {

			abacus->rows[previous_abacus_index].count--;
			int carry = (is_even(previous_abacus_index) ? ratio_to_next_next_row : ratio_to_next_row_odd) - 1;
			abacus->rows[abacus_index].count += carry;

		} else {
			abacus->rows[abacus_index].count++;
		}

		previous_abacus_index = abacus_index;
	}

	return SUCCESS;
}

void borrow_if_necessary(int row_index, struct Abacus* abacus) {
	if (abacus->rows[row_index].count == 0 && (row_index + 1) < abacus_row_count) {
		borrow_if_necessary(row_index + 1, abacus);
		abacus->rows[row_index + 1].count--;
		abacus->rows[row_index].count += ratio_to_next_row(row_index);
	}
}

ReturnCode subtractive_tally(char* input, struct Abacus* abacus) {
	int input_index;	
	int end = strlen(input) - 1;
	int abacus_index;
	int previous_abacus_index = NULL;

	for (input_index = end; input_index >= 0; input_index--) {
		abacus_index = get_abacus_index(input[input_index], abacus);
		if (abacus_index < 0) {
			return INVALID_CHARACTER;
		}

		if (is_preceding_subtractor(abacus_index, previous_abacus_index)) {

			abacus->rows[abacus_index].count++;

		} else {
			borrow_if_necessary(abacus_index, abacus);
			abacus->rows[abacus_index].count--;			
		} 

		previous_abacus_index = abacus_index;
	}

	return SUCCESS;
}

void adjust_counts(struct Abacus* abacus) {
	int index;
	int ratio;

	for (index = 0; index < abacus_row_count - 1; index++) {
		ratio = ratio_to_next_row(index);

		while (abacus->rows[index].count >= ratio) {
			abacus->rows[index + 1].count++;
			abacus->rows[index].count -= ratio;
		}
	}
}

void append(char* string, char symbol, int* index) {
	string[*index] = symbol;
	(*index)++;
}

void append_n_times(char* string, char symbol, int* index, int n) {
	int count;
	for (count = 0; count < n; count++) {
		append(string, symbol, index);
	}	
}

void to_roman_numerals(struct Abacus* abacus, char* result) {
	int result_index = 0;
	int abacus_index;

	for (abacus_index = abacus_row_count - 1; abacus_index >= 0; abacus_index--) {

		if (!is_even(abacus_index) && abacus->rows[abacus_index - 1].count == (ratio_to_next_row_odd - 1)) {

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
	}

	result[result_index] = '\0';
}

ReturnCode add(char* addend1, char* addend2, char* sum) {
	struct Abacus abacus = initialize_abacus();
	ReturnCode return_code = SUCCESS;
	
	return_code = tally(addend1, &abacus);
	if (INVALID_CHARACTER == return_code) { return return_code; }
	return_code = tally(addend2, &abacus);
	if (INVALID_CHARACTER == return_code) { return return_code; }

	adjust_counts(&abacus);
	to_roman_numerals(&abacus, sum);

	return return_code;
}

ReturnCode subtract(char* minuend, char* subtrahend, char* difference) {
	struct Abacus abacus = initialize_abacus();
	ReturnCode return_code = SUCCESS;

	return_code = tally(minuend, &abacus);
	if (INVALID_CHARACTER == return_code) { return return_code; }
	return_code = subtractive_tally(subtrahend, &abacus);
	if (INVALID_CHARACTER == return_code) { return return_code; }
	
	adjust_counts(&abacus);
	to_roman_numerals(&abacus, difference);

	return return_code;
}
