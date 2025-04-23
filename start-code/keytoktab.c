/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
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

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
}tab;

/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
    {"id", 	            id},
    {"number",      number},
    {":=", 	        assign},
    {"predef",      predef},
    {"tempty",      tempty},
    {"undef", 	     undef},
    {"error",        error},
    {"type",           typ},
    {"$",              '$'},
    {"(",              '('},
    {")",              ')'},
    {"*",              '*'},
    {"+",              '+'},
    {",",              ','},
    {"-",              '-'},
    {".",              '.'},
    {"/",              '/'},
    {":",              ':'},
    {";",              ';'},
    {"=",              '='},
    {"TERROR", 	    nfound}
};

static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	      input},
	{"output", 	     output},
	{"var", 	        var},
	{"begin", 	      begin},
	{"end", 	        end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	       real},
	{"KERROR", 	     nfound}
};

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/

void p_toktab()
{ 
    puts("");
    puts("________________________________________________________ ");
    puts(" THE PROGRAM KEYWORDS ");
    puts("________________________________________________________ ");
    for(int i = 0; keywordtab[i].token != nfound; i++){
        printf("%11s%5d \n", keywordtab[i].text, keywordtab[i].token);
    }
    puts("________________________________________________________ ");
    puts(" THE PROGRAM TOKENS ");
    puts("________________________________________________________ ");
    for(int i = 0; tokentab[i].token != nfound; i++){
        printf("%11s%5d \n", tokentab[i].text, tokentab[i].token);
    }
    puts("________________________________________________________ ");
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
static toktyp search_str(tab* arr, char* str){
    for(int i = 0; arr[i].token != nfound; i++){
        if(strcmp(arr[i].text, str) == 0)
            return arr[i].token;
    }
    return nfound;
}
static char* search_tok(tab* arr, toktyp tok){
    for(int i = 0; arr[i].token != nfound; i++){
        if(arr[i].token == tok)
            return arr[i].text;
    }
    return NULL;
}

toktyp lex2tok(char * fplex)
{
    toktyp t;

    t = search_str(keywordtab, fplex);
    if (t != nfound) return t;

    t = search_str(tokentab, fplex);
    if (t != nfound) return t;

    if(isdigit(fplex[0]))
        return number;

    return id;
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
    toktyp res = search_str(keywordtab, fplex);
    return res != nfound ? res : id;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
    char* res;
    if((res = search_tok(keywordtab, ftok)) != NULL || (res = search_tok(tokentab, ftok)) != NULL)
        return res;
    else{
        return NULL;
    }
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
