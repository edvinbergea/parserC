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
#include "keytoktab.h"
#include "lexer.h"
/* #include "symtab.h"      */       /* when the symtab    is added   */
/* #include "optab.h"       */       /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 1
static int  lookahead=0;
static int  is_parse_ok=1;

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
    if(DEBUG) printf("\n --------In match\t expected: %-7s  found: %s",
                    tok2lex(t), tok2lex(lookahead));
    if (lookahead == t) lookahead = get_token();
    else {
    is_parse_ok=0;
    printf("\n *** Unexpected Token:\t expected: %-7s  found: %s (in match)",
              tok2lex(t), tok2lex(lookahead));
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
static void prog_grmr(void);
static void prog_header_grmr(void);
static void var_part_grmr(void);
static void var_dec_list_grmr(void);
static void var_dec_grmr(void);
static void id_list_grmr(void);
static void type_grmr(void);
static void stat_part_grmr(void);
static void stat_list_grmr(void);
static void stat_grmr(void);
static void assign_stat_grmr(void);
static void expr_grmr(void);
static void term_grmr(void);
static void factor_grmr(void);
static void operand_grmr(void);

static void prog_grmr()
{
    in("program");
    prog_header_grmr(); var_part_grmr(); stat_part_grmr();
    out("program");
}
static void prog_header_grmr()
{
    in("program_header");
    match(program); match(id); match('('); match(input);
    match(','); match(output); match(')'); match(';');
    out("program_header");
}
static void var_part_grmr()
{
    in("var_part");
    match(var); var_dec_list_grmr();
    out("var_part");
}
static void var_dec_list_grmr()
{
    in("var_dec_part");
    do{
        var_dec_grmr();
    }while(lookahead == id);
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
    match(id);
    while(lookahead == ','){
        match(',');
        match(id);
    }
    out("id_list");
}
static void type_grmr()
{
    in("type");
    if(lookahead == integer)
        match(integer);
    else if(lookahead == real)
        match(real);
    else if(lookahead == boolean)
        match(boolean);
    out("type");
}
static void stat_part_grmr()
{
    in("stat_part");
    match(begin); stat_list_grmr(); match(end); match('.');
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
    match(id); match(assign); expr_grmr();
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
    else if(lookahead == id || lookahead == number)
        operand_grmr();
    out("factor");
}
static void operand_grmr()
{
    in("opperand");
    if(lookahead == id)
        match(id);
    if(lookahead == number)
        match(number);
    out("opperand");
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    in("parser");
    lookahead = get_token();        // get the first token
    prog_grmr();                    // call the first grammar rule               
    check_stream();                 // check if stream is empty
    out("parser");
    return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
