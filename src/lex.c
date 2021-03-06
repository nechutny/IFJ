/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "lex.h"
#include "string.h"
#include "garbage.h"
#include "main.h"


TToken * token_init() {
    TToken * token;
    
    token = ( TToken * ) _malloc( sizeof( TToken )+ sizeof(char)*16 );
    if( token == NULL ) {
        fprintf( stderr, "%s", strerror( errno ) );
        return NULL;
    }
    
    token->data[0] = '\0';
    token->length = 0;
    token->allocated = 16;
    
    token->type = token_invalid;
    return token;
}

TToken * token_data_add( TToken * token, char * text )
{

    unsigned int len = strlen(text);
    
    unsigned int total_len = len+(token->length)+1;
    
    if( total_len > token->allocated )
    {
        token->allocated = total_len;
        token = _realloc(token, sizeof(TToken) + (sizeof(char)*(token->allocated)));
        if(token == NULL)
        {
            fprintf(stderr, "%s", strerror(errno));
            return 0;
        }
    }
    
        
    strncat(token->data, text, len);

    token->length += len;
    
    return token;
}

TToken * token_buffer;

void token_return_token(TToken * token) {
    token_buffer = token;
}

void token_free( TToken * token ) {
    //string_free( token->data );
    _free( token );
}

//set correct type of token (is it keyword or indentifier?)
void set_identifier(TToken *token){
    char * keywords[]={"var","in","do","repeat","until","case",
                        "of","to","id","mod","not","and","or",
                        "begin","end","while","for","if","then","else",
                        "function","true","false","null",
                        "integer","real","boolean","string","array",
                        "xor","forward","find","readln","write","sort",
                        "length","copy","downto"};
    TToken_type tokens[]={token_var,token_in,token_do,token_repeat,token_until,
        token_case,token_of,token_to,token_id,token_mod,token_not,
                        token_and,token_or,token_begin,token_end,token_while,
                        token_for,token_if,token_then,token_else,
                        token_function,token_true,
                        token_false,token_null,token_integer,token_real,
                        token_boolean,token_string_var,token_array,
                        token_xor,token_forward,token_f_find,token_readln,
                        token_write,token_f_sort,token_f_length,token_f_copy,token_downto};

    token->type = token_identifier;
    for (int i = 0; i <= 37; i++)
    {
        if (strcmp(keywords[i],token->data)==0)
        {
            token->type = tokens[i];
        }
    }
}

//return next token from file
TToken *token_get() {
    TToken *token;
    FILE * file = global.file;
    if(token_buffer != NULL)
    {
        token = token_buffer;
        token_buffer = NULL;
        return token;
    }
    
    if((token = token_init()) == NULL){     //alokace noveho tokenu
        return NULL;
    }
    char buffer[32];
    memset(&buffer,0,32);
    unsigned int buff_i = 0;
    token_last = token;
    TState state = state_init;
//  TState number_state = state_int;            //it goes from int or double to _double_sign_e?
    //string_clear(token->data);
    char c;
    //char sign='+';
    int ascii=0;
    while (1)
    {
        c = fgetc(file);
        if(state != state_apostrophe)
        {
            c = tolower(c);
        }
        switch (state)
        {
            case state_init:
                while(isspace(c))
                {
                    c = tolower(fgetc(file));
                }
                switch (c)
                {
                    case '.':
                        token->type = token_dot;
                        return token;
                    case ',':
                        token->type = token_comma;
                        return token;
                    case '+':
                        token->type = token_add;
                        return token;
                    case '-':
                        token->type = token_sub;
                        return token;
                    case '*':
                        token->type = token_mul;
                        return token;
                    case '=':
                        token->type = token_equal;
                        return token;
                    case ';':
                        token->type = token_semicolon;
                        return token;
                    case ')':
                        token->type = token_parenthesis_right;
                        return token;
                    case '/':
                        token->type = token_slash;
                        return token;
                    case '(':
                        token->type = token_parenthesis_left;
                        return token;
                    case '[':
                        token->type = token_bracket_left;
                        return token;
                    case ']':
                        token->type = token_bracket_right;
                        return token;
                    case '{':
                        state = state_multiline_comment;
                        break;
                    case ':':
                        state = state_colon;
                        break;
                    case '>':
                        state = state_greater;
                        break;
                    case '<':
                        state = state_less;
                        break;
                    case '\'':
                        state = state_apostrophe;
                        break;
                    default:
                        if (isalpha(c) || c == '_')
                        {   //identifikator nebo klicove slovo;
                            state = state_identifier;
                            //string_add_chr(token->data, c);
                            buffer[buff_i] = c; buff_i++;
                        }
                        else if (isdigit(c))
                        {
                            state = state_int;
                        //  printf("int\n");
                            //number_state = state_int;
                            //string_add_chr(token->data, c);
                            buffer[buff_i] = c; buff_i++;
                            break;
                        }
                        else if (c == EOF)
                        {
                            token->type = token_eof;
                            return token;
                        }
                        else
                        {
                            throw_error(error_lex);
                        }
                }
                break;
            case state_identifier:
                if(isalnum(c) || c == '_')
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                }
                else
                {
                    ungetc(c, file);
                    token = token_data_add(token, buffer);
                    set_identifier(token);
                    return token;
                }
                break;
            case state_int:
                if(isdigit(c))
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                }
                else if (c=='.')
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                    state = state__double_dot;
                }
                /*else if (c=='+' || c=='-')
                {
                    //string_add_chr(token->data, c);
stare                   buffer[buff_i] = c; buff_i++;
                    //sign = c;
                    state = state__double_sign_e;
                }*/
                else if (c=='e' || c=='E')
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                    state = state__double_e;
                }
                /*else if (c == EOF || c == '+' || c == ',' || c == '*' ||
                        c == '/' || c == '-' || c == '=' || c == ')' ||
                        c == ';' || c == '>' || c == '<' || c == '[' ||
stare                       c == ']' || c=='{' || isspace(c))   // '(' shouldnt be there)
                {
                    ungetc(c, file);
                    token->type=token_int;
                    token = token_data_add(token, buffer);
                    return token;
                }*/
                else
                {
                    ungetc(c, file);
                    token->type=token_int;
                    token = token_data_add(token, buffer);
                    return token;
                }
                break;
            case state__double_dot:
                if (isdigit(c))
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
        //          printf("int %s\n",buffer);
                    state = state_double;
//                  number_state = state_double;
                }
                else
                {
                    throw_error(error_lex);
                }
                break;
            case state_double:
                if(isdigit(c))
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                }
                /*else if (c=='+' || c=='-')
                {
                    //string_add_chr(token->data, c);
stare                   buffer[buff_i] = c; buff_i++;
                    state = state__double_sign_e;
                }*/
                else if (c=='e' || c=='E')
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                    state = state__double_e;
                }
                /*else if (c == EOF || c == '+' || c == ',' || c == '*' || 
                        c == '/' || c == '-' || c == '=' || c == ')' || 
                        c == ';' || c == '>' || c == '<' || c == '[' ||
stare                       c == ']' || c=='{' || isspace(c))   // '(' shouldnt be there)
                {
                    ungetc(c, file);
                    token->type=token_double;
                    token = token_data_add(token,buffer);
                    return token;
                }*/
                else
                {
                    ungetc(c, file);
                    token->type=token_double;
                    token = token_data_add(token,buffer);
                    return token;
                }
                break;
            case state__double_sign_e:
                /*if (c=='e' || c=='E')
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                    state = state__double_e;
                }
                else
                {
                    fseek(file, -2L, SEEK_CUR);
                    if(number_state==state_int)
                    {
                        token->type=token_int;
stare                       token = token_data_add(token,buffer);
                        return token;
                    }
                    else
                    {
                        token->type=token_double;
                        token = token_data_add(token,buffer);
                        return token;
                    }
                }*/
                if(isdigit(c))
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                    state = state_double_e;
                }
                else
                {
                    throw_error(error_lex);
                }
                break;
            case state__double_e:
                if (isdigit(c))
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                    state = state_double_e;
                }
                else if (c=='+' || c=='-')
                {
                    buffer[buff_i] = c; buff_i++;
                    state = state__double_sign_e;
                }
                else
                {
                    throw_error(error_lex);
                }
                break;
            case state_double_e:
                if (isdigit(c))
                {
                    //string_add_chr(token->data, c);
                    buffer[buff_i] = c; buff_i++;
                }
                /*else if (c == EOF || c == '+' || c == ',' || c == '*' || 
                        c == '/' || c == '-' || c == '=' || c == ')' || 
                        c == ';' || c == '>' || c == '<' || c == '[' ||
stare                       c == ']' || c=='{' || isspace(c))   // '(' shouldnt be there)
                {
                    ungetc(c, file);
                    token->type=token_double;
                    token = token_data_add(token,buffer);
                    return token;
                }*/
                else
                {
                    ungetc(c, file);
                    token->type=token_double;
                    token = token_data_add(token,buffer);
                    return token;
                }
                break;
            case state_multiline_comment:
                if (c=='}')
                {
                    state = state_init;
                }
                else if (c==EOF)
                {
                    throw_error(error_lex);
                }
                break;
            case state_colon:
                if (c=='=')
                {
                    token->type = token_assign;
                    token = token_data_add(token,buffer);
                    return token;
                }
                else
                {
                    ungetc(c, file);
                    token->type = token_colon;
                    token = token_data_add(token,buffer);
                    return token;
                }
            case state_greater:
                if (c=='=')
                {
                    token->type = token_greater_equal;
                    token = token_data_add(token,buffer);
                    return token;
                }
                else
                {
                    ungetc(c, file);
                    token->type = token_greater;
                    token = token_data_add(token,buffer);
                    return token;
                }
            case state_less:
                if (c=='=')
                {
                    token->type = token_less_equal;
                    token = token_data_add(token,buffer);
                    return token;
                }
                else if (c=='>')
                {
                    token->type = token_not_equal;
                    token = token_data_add(token,buffer);
                    return token;
                }
                else
                {
                    ungetc(c, file);
                    token->type = token_less;
                    token = token_data_add(token,buffer);
                    return token;
                }
            case state_apostrophe:
                if (c=='\'')
                {
                    c = fgetc(file);
                    if (c == '\'')
                    {
                        buffer[buff_i] = c; buff_i++;
                        break;
                    }
                    else if (c == '#')
                    {
                        ascii = 0;
                        c = fgetc(file);
                        if (!isdigit(c))    //stara podminka c == '\''
                        {
                            throw_error(error_lex);
                        }
                        while (c == '0')
                        {
                            c = fgetc(file);
                        }
                        if (c == '\'' || !isdigit(c))   //napr #00 
                        {
                            throw_error(error_lex);
                        }
                        else
                        {
                            if (!isdigit(c))
                            {
                                throw_error(error_lex);
                            }
                            else
                            {
                                ascii=(int)c-'0';   //jednotky
                                c = fgetc(file);
                                if (c == '\'')
                                {
                                    buffer[buff_i] = ascii; buff_i++;
                                    break;
                                }
                                else if (!isdigit(c))
                                {
                                    throw_error(error_lex);
                                }
                                else
                                {
                                    ascii = (ascii * 10) + (int)c-'0';  //desitky
                                    c = fgetc(file);
                                    if (c == '\'')
                                    {
                                        buffer[buff_i] = ascii; buff_i++;
                                        break;
                                    }
                                    else if (!isdigit(c))
                                    {
                                        throw_error(error_lex);
                                    }
                                    else
                                    {
                                        ascii = (ascii * 10) + (int)c-'0';      //stovky
                                        c = fgetc(file);
                                        if (c != '\'' || ascii > 255)
                                        {
                                            throw_error(error_lex);
                                        }
                                        else
                                        {
                                            buffer[buff_i] = ascii; buff_i++;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        ungetc(c, file);
                        token->type = token_string;
                        token = token_data_add(token, buffer);
                        return token;
                    }
                }
                else if(c==EOF || c=='\n' || c<=31)
                    {
                        throw_error(error_lex);
                    }
                //string_add_chr(token->data, c);
                buffer[buff_i] = c; buff_i++;
                break;
            default:
    //          printf("Chuck Norris\n");
                break; //you shouldnt get there
        }
        
        if(buff_i == 31)
        {
            token = token_data_add(token, buffer);
            buff_i = 0;
            memset(&buffer, 0, 32);
        }
        
    }
}
