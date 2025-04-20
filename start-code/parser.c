/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
/* #include "keytoktab.h"   */       /* when the keytoktab is added   */
/* #include "lexer.h"       */       /* when the lexer     is added   */
/* #include "symtab.h"      */       /* when the symtab    is added   */
/* #include "optab.h"       */       /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 1
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
enum tvalues {  program_tok = 257, id_tok, input_tok,
                output_tok, var_tok, int_tok, real_tok, 
                bool_tok, begin_tok, end_tok, num_tok };

/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
static int tokens[] = {program_tok, id_tok, '(', input_tok, ',', output_tok, ')', ';',
               '$' };

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
static int pget_token()
{
    static int i=0;
    if (tokens[i] != '$') return tokens[i++]; else return '$';
}

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void in(char* s)
{
    if(DEBUG) printf("\n *** In  %s", s);
}
static void out(char* s)
{
    if(DEBUG) printf("\n *** Out %s", s);
}
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    if(DEBUG) printf("\n --------In match expected: %4d, found: %4d",
                    t, lookahead);
    if (lookahead == t) lookahead = pget_token();
    else {
    is_parse_ok=0;
    printf("\n *** Unexpected Token: expected: %4d found: %4d (in match)",
              t, lookahead);
    }
}
static void check_stream(){
    if(lookahead != '$'){
        is_parse_ok = 0;
    }
}

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void prog_header_grmr()
{
    in("program_header");
    match(program_tok); match(id_tok); match('('); match(input_tok);
    match(','); match(output_tok); match(')'); match(';');
    out("program_header");
}
static void prog_grmr()
{
    in("program");
    prog_header_grmr(); var_part_grmr(); stat_part_grmr();
    out("program");
}
static void var_part_grmr()
{
    in("var_part");
    match(var_tok); var_dec_list_grmr();
    out("var_part");
}
static void var_dec_list_grmr()
{
    in("var_dec_part");
    do{
        var_dec_grmr();
    }while(lookahead == id_tok)
    out("var_dec_part");
}
static void var_dec_grmr()
{
    in("var_dec");
    id_list_grmr(); match(':'); type_grmr(); match(';');
    out("var_dec");
}
static void id_list_grmr()
{
    in("id_list");
    match(id_tok);
    while(lookahead == ','){
        match(',');
        match(id_tok);
    }
    out("id_list");
}
static void type_grmr()
{
    in("type");
    if(lookahead == int_tok)
        match(int_tok);
    else if(lookahead == real_tok)
        match(real_tok);
    else if(lookahead == bool_tok)
        match(bool_tok);
    out("type");
}
static void stat_part_grmr()
{
    in("stat_part");
    match(begin_tok); stat_list_grmr(); match(end_tok);
    out("stat_part");
}
static void stat_list_grmr()
{
    in("stat_list");
    stat_grmr(); 
    while(lookahead == ';'){
        match(';');
        stat_grmr(); 
    }
    out("stat_list");
}
static void stat_grmr()
{
    in("stat");
    assign_stat_grmr();
    out("stat");
}
static void assign_stat_grmr()
{
    in("assign_stat");
    match(id_tok); match(':'); match('='); expr_grmr();
    out("assign_stat");
}
static void expr_grmr()
{
    in("expr");
    term_grmr();
    while(lookahead == '+'){
        match('+');
        term_grmr();
    }
    out("expr");
}
static void term_grmr()
{
    in("term");
    factor_grmr();
    while(lookahead == '*'){
        match('*');
        factor_grmr();
    }
    out("term");
}
static void factor_grmr()
{
    in("factor");
    if(lookahead == '('){
        match('(');
        expr_grmr();
        match(')');
    }
    else if(lookahead == id_tok || lookahead == num_tok)
        operand_grmr();
    out("factor");
}
static void operand_grmr()
{
    in("opperand");
    if(lookahead == id_tok)
        match(id_tok);
    if(lookahead == num_tok)
        match(num_tok);
    out("opperand");
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    in("parser");
    lookahead = pget_token();       // get the first token
    prog_grmr();                    // call the first grammar rule
    check_stream();                 // check if stream is empty
    out("parser");
    return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
