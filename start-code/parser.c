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
enum tvalues { program = 257, id, input, output };
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
static int tokens[] = {program, id, '(', input, ',', output, ')', ';',
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

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void program_header_grmr()
{
    in("program_header");
    match(program); match(id); match('('); match(input);
    match(','); match(output); match(')'); match(';');
    out("program_header");
}
static void program_grmr()
{
    in("program");
    program_header_grmr(); var_part_grmr(); stat_part_grmr();
    out("program");
}
static void var_part_grmr()
{
    in("var_part");
    //todo
    out("var_part");
}
static void var_dec_part_grmr()
{
    in("var_dec_part");
    //todo
    out("var_dec_part");
}
static void var_dec_grmr()
{
    in("var_dec");
    //todo
    out("var_dec");
}
static void id_list_grmr()
{
    in("id_list");
    //todo
    out("id_list");
}
static void type_grmr()
{
    in("type");
    //todo
    out("type");
}
static void stat_part_grmr()
{
    in("stat_part");
    //todo
    out("stat_part");
}
static void stat_list_grmr()
{
    in("stat_list");
    //todo
    out("stat_list");
}
static void stat_grmr()
{
    in("stat");
    //todo
    out("stat");
}
static void assign_stat_grmr()
{
    in("assign_stat");
    //todo
    out("assign_stat");
}
static void expr_grmr()
{
    in("expr");
    //todo
    out("expr");
}
static void term_grmr()
{
    in("term");
    //todo
    out("term");
}
static void factor_grmr()
{
    in("factor");
    //todo
    out("factor");
}
static void opperand_grmr()
{
    in("opperand");
    //todo
    out("opperand");
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    in("parser");
    lookahead = pget_token();       // get the first token
    program_header_grmr();          // call the first grammar rule
    out("parser");
    return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
