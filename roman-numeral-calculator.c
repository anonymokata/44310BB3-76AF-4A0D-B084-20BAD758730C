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

static int is_even(const int number) {
    return number % 2 == 0 ? 1 : 0;
}

static int ratio_to_next_row(const int row_index) {
    return is_even(row_index) ? ratio_to_next_row_odd : ratio_to_next_row_even;
}

static int get_abacus_index(const char symbol, const struct Abacus* abacus) {
    int index;

    for (index = 0; index < abacus_row_count; index++) {
        if (symbol == abacus->rows[index].symbol) {
            return index;
        }
    }

    return -1;
}

static int is_preceding_subtractor(const int abacus_index, const int previous_abacus_index) {
    return is_even(abacus_index) && 
            abacus_index != (abacus_row_count - 1) && 
            (abacus_index + 1 == previous_abacus_index || 
            abacus_index + 2 == previous_abacus_index);
}

static int is_too_large(const struct Abacus* abacus) {
    const int last_row = abacus_row_count - 1;
    return abacus->rows[last_row].count >= (ratio_to_next_row(last_row) - 1);
}

static int is_too_small(const struct Abacus* abacus) {
    int index;
    for (index = 0; index < abacus_row_count; index++) {
        if (abacus->rows[index].count > 0) {
            return 0;
        }
    }
    return 1;
}

static ReturnCode adjust_counts(struct Abacus* abacus) {
    int index;
    int ratio;

    for (index = 0; index < abacus_row_count - 1; index++) {
        ratio = ratio_to_next_row(index);

        while (abacus->rows[index].count >= ratio) {
            abacus->rows[index + 1].count++;
            abacus->rows[index].count -= ratio;
        }
    }

    return is_too_large(abacus) ? RESULT_TOO_LARGE : SUCCESS;
}

static ReturnCode tally(const char* input, struct Abacus* abacus) {
    const int end = strlen(input) - 1;
    int input_index;
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

static ReturnCode borrow_if_necessary(const int row_index, struct Abacus* abacus) {
    ReturnCode code = SUCCESS;

    if (abacus->rows[row_index].count == 0) {

        if ((row_index + 1) < abacus_row_count) {
            code = borrow_if_necessary(row_index + 1, abacus);

            if (SUCCESS == code) {
                abacus->rows[row_index + 1].count--;
                abacus->rows[row_index].count += ratio_to_next_row(row_index);
            }
        } else {
            code = RESULT_TOO_SMALL;
        }
    }

    return code;
}

static ReturnCode subtractive_tally(const char* input, struct Abacus* abacus) {
    const int end = strlen(input) - 1;
    int input_index;    
    int abacus_index;
    int next_abacus_index;

    for (input_index = end; input_index >= 0; input_index--) {
        abacus_index = get_abacus_index(input[input_index], abacus);
        next_abacus_index = get_abacus_index(input[input_index - 1], abacus);
        if (abacus_index < 0) {
            return INVALID_CHARACTER;
        }

        if (next_abacus_index >= 0 && is_preceding_subtractor(next_abacus_index, abacus_index)) {
            abacus->rows[next_abacus_index].count++;
            adjust_counts(abacus);
            input_index--;
        }

        ReturnCode code = borrow_if_necessary(abacus_index, abacus);
        if (SUCCESS != code) { return code; }

        abacus->rows[abacus_index].count--;         
    }

    return is_too_small(abacus) ? RESULT_TOO_SMALL : SUCCESS;
}

static void append(char* string, const char symbol, int* index) {
    string[*index] = symbol;
    (*index)++;
}

static void append_n_times(char* string, const char symbol, int* index, const int n) {
    int count;
    for (count = 0; count < n; count++) {
        append(string, symbol, index);
    }   
}

static void to_roman_numerals(const struct Abacus* abacus, char* result) {
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

ReturnCode add(const char* addend1, const char* addend2, char* sum) {
    struct Abacus abacus = initialize_abacus();

    ReturnCode code = tally(addend1, &abacus);
    if (SUCCESS != code) { return code; }
    code = tally(addend2, &abacus);
    if (SUCCESS != code) { return code; }

    code = adjust_counts(&abacus);
    if (SUCCESS != code) { return code; }
    to_roman_numerals(&abacus, sum);

    return code;
}

ReturnCode subtract(const char* minuend, const char* subtrahend, char* difference) {
    struct Abacus abacus = initialize_abacus();

    ReturnCode code = tally(minuend, &abacus);
    if (SUCCESS != code) { return code; }
    code = subtractive_tally(subtrahend, &abacus);
    if (SUCCESS != code) { return code; }

    code = adjust_counts(&abacus);
    if (SUCCESS != code) { return code; }
    to_roman_numerals(&abacus, difference);

    return code;
}
