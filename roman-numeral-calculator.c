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

typedef ReturnCode (*TallyFunction)(const int, const int, int*, struct Abacus*);

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

static int is_preceding_subtractor(const int abacus_index, const int next_abacus_index) {
    return is_even(next_abacus_index) && 
            next_abacus_index != (abacus_row_count - 1) && 
            (next_abacus_index + 1 == abacus_index || 
            next_abacus_index + 2 == abacus_index);
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

static ReturnCode additive_tally(const int abacus_index, const int next_abacus_index, int* numeral_index, struct Abacus* abacus) {
    if (next_abacus_index >= 0 && is_preceding_subtractor(abacus_index, next_abacus_index)) {
        int carry = (is_even(abacus_index) ? ratio_to_next_next_row : ratio_to_next_row_odd) - 1;
        abacus->rows[next_abacus_index].count += carry;
        (*numeral_index)--;
    } else {
        abacus->rows[abacus_index].count++;
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

static ReturnCode subtractive_tally(const int abacus_index, const int next_abacus_index, int* numeral_index, struct Abacus* abacus) {
    if (next_abacus_index >= 0 && is_preceding_subtractor(abacus_index, next_abacus_index)) {
        abacus->rows[next_abacus_index].count++;
        adjust_counts(abacus);
        (*numeral_index)--;
    }

    ReturnCode code = borrow_if_necessary(abacus_index, abacus);

    abacus->rows[abacus_index].count--;

    return code;
}

static ReturnCode iterate_over_numeral(const char* numeral, TallyFunction tally_function, struct Abacus* abacus) {
    const int end = strlen(numeral) - 1;
    ReturnCode code = SUCCESS;
    int numeral_index;    
    int abacus_index;
    int next_abacus_index;

    for (numeral_index = end; numeral_index >= 0; numeral_index--) {
        abacus_index = get_abacus_index(numeral[numeral_index], abacus);
        next_abacus_index = get_abacus_index(numeral[numeral_index - 1], abacus);
        if (abacus_index < 0) {
            code = INVALID_CHARACTER;
            break;
        }

        code = tally_function(abacus_index, next_abacus_index, &numeral_index, abacus);
        if (code != SUCCESS) {
            break;
        }
    }

    return code;
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

static void to_numeral(const struct Abacus* abacus, char* result) {
    int result_index = 0;
    int abacus_index;
    int lower_abacus_index;

    for (abacus_index = abacus_row_count - 1; abacus_index >= 0; abacus_index--) {
        lower_abacus_index = abacus_index - 1;

        if (!is_even(abacus_index) && abacus->rows[lower_abacus_index].count == (ratio_to_next_row_odd - 1)) {

            const int subtracted_abacus_index = abacus->rows[abacus_index].count == 0 
                ? abacus_index 
                : abacus_index + 1;
            append(result, abacus->rows[lower_abacus_index].symbol, &result_index);
            append(result, abacus->rows[subtracted_abacus_index].symbol, &result_index);

            abacus_index--;

        } else {
            append_n_times(result, abacus->rows[abacus_index].symbol, &result_index, abacus->rows[abacus_index].count);
        }
    }

    result[result_index] = '\0';
}

static ReturnCode compute(const char* numeral1, TallyFunction tally_function1, 
                          const char* numeral2, TallyFunction tally_function2, 
                          char* result_numeral) {
    struct Abacus abacus = initialize_abacus();

    ReturnCode code = iterate_over_numeral(numeral1, tally_function1, &abacus);
    if (SUCCESS != code) { return code; }

    code = iterate_over_numeral(numeral2, tally_function2, &abacus);
    if (SUCCESS != code) { return code; }

    code = adjust_counts(&abacus);
    if (SUCCESS != code) { return code; }

    if (is_too_small(&abacus)) { return RESULT_TOO_SMALL; }

    to_numeral(&abacus, result_numeral);

    return code;
}

ReturnCode add(const char* addend1, const char* addend2, char* sum) {
    return compute(addend1, &additive_tally, addend2, &additive_tally, sum);
}

ReturnCode subtract(const char* minuend, const char* subtrahend, char* difference) {
    return compute(minuend, &additive_tally, subtrahend, &subtractive_tally, difference);
}
