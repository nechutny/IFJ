/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "garbage.h"
#include "lex.h"

void throw_error(errors code)
{
    int return_code = 1;

	int column = 0;
	int line = file_line(&column);

    fprintf(stderr,"%s:%d:%d: Error: ", global.file_name, line, column);

    switch(code)
    {
        case error_program:
            fprintf(stderr, "Expected 'program'");
            return_code = 2;
            break;

        case error_identifier:
            fprintf(stderr, "Expected identifier");
            return_code = 2;
            break;

        case error_semicolon:
            fprintf(stderr, "Expected ';'");
            return_code = 2;
            break;

        case error_semicolon_unexpected:
            fprintf(stderr, "Unexpected ';'");
            return_code = 2;
            break;

        case error_begin:
            fprintf(stderr,"Expected 'begin'");
            return_code = 2;
            break;

        case error_end:
            fprintf(stderr,"Expected 'end'");
            return_code = 2;
            break;

        case error_unkown_command:
            fprintf(stderr,"Unkown command ");
            return_code = 2;
            break;

        case error_dot:
            fprintf(stderr,"Expected '.'");
            return_code = 2;
            break;

        case error_colon:
            fprintf(stderr,"Expected ':'");
            return_code = 2;
            break;

        case error_expresion:
            fprintf(stderr,"In expresion");
            break;

        case error_type:
            fprintf(stderr,"Wrong datatype");
            return_code = 4;
            break;

        case error_parenthesis_left:
            fprintf(stderr,"Expected '('");
            return_code = 2;
            break;

        case error_parenthesis_right:
            fprintf(stderr,"Expected ')'");
            return_code = 2;
            break;

        case error_then:
            fprintf(stderr,"Expected 'then'");
            return_code = 2;
            break;

        case error_do:
            fprintf(stderr,"Expected 'do'");
            return_code = 2;
            break;

        case error_until:
            fprintf(stderr,"Expected 'until'");
            return_code = 2;
            break;

        case error_assign:
            fprintf(stderr,"Expected ':='");
            return_code = 2;
            break;

        case error_to:
            fprintf(stderr,"Expected 'to'");
            return_code = 2;
            break;

        case error_of:
            fprintf(stderr,"Expected 'of'");
            return_code = 2;
            break;

        case error_eof:
            fprintf(stderr,"Content after 'end.'");
            return_code = 2;
            break;

        case error_bracket_right:
            fprintf(stderr,"Expected ']'");
            return_code = 2;
            break;

        case error_function_not_exists:
            fprintf(stderr,"Called function not exists");
            return_code = 3;
            break;

        case error_function_already_defined:
            fprintf(stderr,"Function already defined");
            return_code = 3;
            break;

        case error_function_is_var:
            fprintf(stderr,"Called function name is variable");
            return_code = 3;
            break;

        case error_function_second_forward:
            fprintf(stderr,"Second forward for function");
            return_code = 3;
            break;

        case error_prototype_mismatch:
            fprintf(stderr,"Function forward declaration doesn't match definition");
            return_code = 3;
            break;

        case error_var_already_defined:
            fprintf(stderr,"Variable already defined");
            return_code = 3;
            break;

        case error_var_not_exists:
            fprintf(stderr,"Used variable not exists");
            return_code = 3;
            break;

        case error_read_invalid:
            fprintf(stderr,"Readed wrong value from stdin");
            return_code = 6;
            break;

        case error_read_to_bool:
            fprintf(stderr,"Read to boolean variable");
            return_code = 4;
            break;

        case error_syntax_in_precedence:
            fprintf(stderr,"Wrong syntax in expression");
            return_code = 2;
            break;

        case error_sign_fault:
            fprintf(stderr,"Got error sign from precedence table");
            return_code = 2;
            break;


        case error_wrong_number_of_argument:
            fprintf(stderr,"Wrong argument in integrated functions");
            return_code = 4;
            break;

        case error_readln_arguments:
            fprintf(stderr, "Wrong number of arguments in readln");
            return_code = 2;
            break;

        case error_sign_less_precedence:
            fprintf(stderr,"Excpects: < in expression");
            return_code = 2;
            break;

        case error_function_parametr_precedence:
            fprintf(stderr,"Wrong function argument in expression");
            return_code = 2;
            break;

        case error_missing_func_precedence:
            fprintf(stderr,"Missing function name in expression");
            return_code = 2;
            break;

        case error_not_a_function_precedence:
            fprintf(stderr,"Not a function in expression");
            return_code = 2;
            break;

        case error_left_parenthesis_precedence:
            fprintf(stderr,"Expects: ( in expression");
            return_code = 2;
            break;

        case error_semicolon_before_end:
            fprintf(stderr,"Semicolon before end");
            return_code = 2;
            break;
        case error_operator_precedence:
            fprintf(stderr,"Expects: operator in expression");
            return_code = 2;
            break;
        case error_not_string:
            fprintf(stderr, "First variable is not string" );
            return_code = 4;
            break;
        case error_string:
            fprintf(stderr, "First variable is string" );
            return_code = 4;
            break;
        case error_incopatible_types:
            fprintf(stderr, "incopatibile type in expresion" );
            return_code = 4;
            break;

        case error_dividing_by_zero:
            fprintf(stderr, "Semantic error dividing by 0");
            return_code = 8;
            break;

        case error_uninicialized:
            fprintf(stderr, "Unicialized variable");
            return_code = 7;
            break;

        case error_to_many_args:
            fprintf(stderr, "Too many arguments");
            return_code = 4;
            break;

        case error_need_more_args:
            fprintf(stderr, "Need more arguments");
            return_code = 4;
            break;

        case error_uninicialized_return_func:
            fprintf(stderr, "Function return unicialized");
            return_code = 7;
            break;

        case error_lex:
            fprintf(stderr, "Invalid token");
            return_code = 1;
            break;

        default:
            fprintf(stderr, "Unkown error");
            return_code = 9;
            break;
    }

    fprintf(stderr," Readed '%s' (type id: %d).\n", token_last->data, token_last->type);

    exit(return_code);
}

int file_line(int *column)
{
    long pos = ftell(global.file);

    long line = 1;
    long readed = 0;
    int character;
    (*column) = 0;

    rewind(global.file);
    while(readed < pos)
    {
	(*column)++;
        character = fgetc(global.file);
        if(character == '\n')
        {
            line++;
            (*column) = 0;
        }
        readed++;
    }

    return line;
}
