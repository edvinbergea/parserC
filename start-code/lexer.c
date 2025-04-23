/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
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
#define BUFSIZE 1024
#define LEXSIZE   30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{
    int c, i = 0;
    while ((c = getchar()) != EOF && i < BUFSIZE-1) {
        buffer[i++] = (char)c;
    }
    buffer[i] = '\0';
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/

static void pbuffer()
{
    puts(buffer);
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{
    char c = buffer[pbuf];
    if (c == '\0') return;
    if (plex < LEXSIZE - 1) {
        lexbuf[plex++] = c;
        lexbuf[plex]   = '\0';
    } else {/* To long */}
    pbuf++;
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{
    plex       = 0;
    lexbuf[0]  = '\0';

    if (buffer[pbuf] == '\0') {
        return '$';
    }
    // skip white space
    while (isspace(buffer[pbuf])) 
        pbuf++;
    // match id and keywords
    if (isalpha(buffer[pbuf])) {
        do {
            get_char();
        } while (isalnum(buffer[pbuf]));
    }
    //match numbers
    else if (isdigit(buffer[pbuf])) {
        do {
            get_char();
        } while (isdigit(buffer[pbuf]));
    }
    //match other
    else {
        get_char();
        if (lexbuf[0] == ':' && buffer[pbuf] == '=') {
            get_char();
        }
    }
    return lex2tok(lexbuf);
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{
    return lexbuf;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
