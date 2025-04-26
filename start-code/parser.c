/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"
#include "lexer.h"
#include "symtab.h"
#include "optab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
static int  lookahead=0;
static int  is_parse_ok=1;
static int  match_ok=1;

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
static void syntax_error(char* fmt, ...){
    va_list args;
    printf("SYNTAX: ");
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    puts("");
    is_parse_ok = 0;
}
static void semantic_error(char* fmt, ...){
    va_list args;
    printf("SEMANTIC: ");
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    puts("");
    is_parse_ok = 0;
}
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    match_ok = 1;
    if(DEBUG) printf("\n --------In match\t expected: %-7s  found: %s",
                    tok2lex(t), tok2lex(lookahead));
    if (lookahead == t) lookahead = get_token();
    else {
        match_ok = 0;
        if(t == typ) syntax_error("expected: a type  found: '%s'", tok2lex(lookahead));
        else if(t == number) syntax_error("expected: opperand  found: '%s'", tok2lex(lookahead));
        else syntax_error("expected: '%s'  found: '%s'", tok2lex(t), tok2lex(lookahead));
    }
}
static void check_stream(){
    if(lookahead != '$'){
        syntax_error("extra symbols after end of parse!");
    }
}
static void check_add_name(char* str){
    if(match_ok){
        if(!find_name(str))
            addv_name(str);
        else semantic_error("variable '%s' already declared!", str);
    }
}
static int left_right_cmp(toktyp a, toktyp b){
    if(a != b || (a != integer && a != boolean && a != real)){
        return 0;
    } 
    else { return 1;}
}

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void   prog_grmr(void);
static void   prog_header_grmr(void);
static void   var_part_grmr(void);
static void   var_dec_list_grmr(void);
static void   var_dec_grmr(void);
static void   id_list_grmr(void);
static void   type_grmr(void);
static void   stat_part_grmr(void);
static void   stat_list_grmr(void);
static void   stat_grmr(void);
static void   assign_stat_grmr(void);
static toktyp expr_grmr(void);
static toktyp term_grmr(void);
static toktyp factor_grmr(void);
static toktyp operand_grmr(void);

static void prog_grmr()
{
    in("program");
    prog_header_grmr(); var_part_grmr(); stat_part_grmr();
    out("program");
}
static void prog_header_grmr()
{
    in("program_header");
    match(program);
    char *pname = strdup(get_lexeme());
    match(id); 
    if(match_ok){addp_name(pname);}
    match('('); match(input);
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
    char *vname = strdup(get_lexeme());
    match(id);
    check_add_name(vname);
    while(lookahead == ','){
        match(',');
        vname = strdup(get_lexeme());
        match(id);
        check_add_name(vname);
    }
    out("id_list");
}
static void type_grmr()
{
    in("type");
    int tok = lookahead;
    if(lookahead == integer)
        match(integer);
    else if(lookahead == real)
        match(real);
    else if(lookahead == boolean)
        match(boolean);
    else match(typ);
    if(match_ok){setv_type(tok);}
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
    char *vname = strdup(get_lexeme());
    toktyp left_type; 
    match(id); 
    if(match_ok){
        if(!find_name(vname))
            semantic_error("variable '%s' not declared!", vname);
        else left_type = get_ntype(vname);
    }
    match(assign);
    toktyp right_type = expr_grmr();
    if(!left_right_cmp(left_type, right_type)){semantic_error("'%s' can not be assigned to '%s'!",
                                tok2lex(left_type), tok2lex(right_type));}
    out("assign_stat");
}
static toktyp expr_grmr()
{
    in("expr");
    toktyp res_type = term_grmr();
    while(lookahead == '+'){
        match('+');
        res_type = get_otype('+', res_type, term_grmr());
    }
    out("expr");
    return res_type;
}
static toktyp term_grmr()
{
    in("term");
    toktyp res_type = factor_grmr();
    while(lookahead == '*'){
        match('*');
        res_type = get_otype('*', res_type, factor_grmr());
    }
    out("term");
    return res_type;
}
static toktyp factor_grmr()
{
    in("factor");
    toktyp res_type;
    if(lookahead == '('){
        match('(');
        res_type = expr_grmr();
        match(')');
    }
    else res_type = operand_grmr();
    out("factor");
    return res_type;
}
static toktyp operand_grmr()
{
    in("opperand");
    char *vname = strdup(get_lexeme());
    toktyp res_type;
    if(lookahead == id){
        match(id);
        if(!find_name(vname)){
            semantic_error("variable '%s' not declared!", vname);
            res_type = undef;
        }
        else res_type = get_ntype(vname);
    }
    else if(lookahead == number){
        match(number);
        res_type = integer;
    }
    else match(number);
    out("opperand");
    return res_type;
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
    p_symtab();
    out("parser");
    return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
