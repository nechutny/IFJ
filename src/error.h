/**
 * 	@project	IFJ 2014/2015
 *	@file		error.h
 *
 *	@author		Stanislav Nechutny - xnechu01
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
	error_unkown
} errors;


void throw_error(errors code);
int file_line();


#endif