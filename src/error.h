/**
 *  @project    IFJ 2014/2015
 *  @file       error.h
 *
 *  @author     Stanislav Nechutny - xnechu01
 */
#ifndef _ERROR_H_
#define _ERROR_H_


typedef enum{
    error_program,
    error_semicolon,
    error_identifier,
    error_colon,
    error_end,
    error_begin,
    error_do,
    error_to,
    error_then,
    error_expresion,
    error_dot,
    error_type,
    error_parenthesis_left,
    error_parenthesis_right,
    error_until,
    error_assign,
    error_of,
    error_eof,
    error_bracket_right,
    error_function_not_exists,
    error_function_is_var,
    error_function_already_defined,
    error_var_not_exists,
    error_read_invalid,
    error_read_to_bool,
    error_syntax_in_precedence,
    error_sign_fault,
    error_sign_less_precedence,
    error_function_parametr_precedence,
    error_missing_func_precedence,
    error_left_parenthesis_precedence,
    error_operator_precedence,
    error_non_term_precedence,
    error_not_a_function_precedence,
    error_not_string,
    error_string,
    error_incopatible_types,
    error_dividing_by_zero,
    error_unkown,
    error_uninicialized,
} errors;


void throw_error(errors code);
int file_line();


#endif
