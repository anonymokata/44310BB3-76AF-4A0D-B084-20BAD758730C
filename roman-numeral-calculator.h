#ifndef ROMAN_NUMERAL_CALCULATOR
#define ROMAN_NUMERAL_CALCULATOR

typedef enum { SUCCESS, INVALID_CHARACTER, RESULT_TOO_LARGE, RESULT_TOO_SMALL } ReturnCode;

ReturnCode add(const char* addend1, const char* addend2, char* sum);

ReturnCode subtract(const char* minuend, const char* subtrahend, char* difference);

#endif