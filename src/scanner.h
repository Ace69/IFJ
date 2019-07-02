/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro scanner
 */

#ifndef SCANNER_H
#define	SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "token.h"
#include "flex_string.h"
#include "error.h"

#define KW_COUNT 9
#define TRUE 1
#define FALSE 0

FILE *file;

/*
Datovy typ pro stavy automatu
predpona "F" -> koncovy stav
predpona "S" -> normalni stav, tj nekoncovy
 **[STAV, priklad]**
*/
typedef enum {
  START, //pocatecni stav
  F_INTEGER,        // 1
  S_DECIMAL,        // 1.
  F_FLOAT,          // 1,1
  S_EXPONENT,       // 1.1e
  S_SIGN,           // +/-
  F_ZERO,           // 0
  F_FLOATEXP,       // 2.3e+0
  S_COMMENT,        // #
  F_IDENTIFIER,     // identifikator
  F_FCE,            // identifikator funkce
  S_EXCLAMATION,    // !
  F_NEQ,            // !=
  F_EQUAL,          // =
  F_EQ,             // ==
  F_MINUS,          // -
  F_PLUS,           // +
  F_MUL,            // *
  F_DIV,            // /
  F_COMMA,          // ,
  F_LEFTBRACKET,    // (
  F_RIGHTBRACKET,   // )
  F_SEMICOLON,      // ;
  F_LESS,           // <
  F_LESSEQ,         // <=
  F_GREATER,        // >
  F_GREATEREQ,      // >=
  S_QUOTE,          // "
  F_QUOTE,          // ""
  S_STR,            // prijmeme ASCII > 31 krome " a '\'
  S_DOUBLEHEX,      // prijmeme dvojitou hexa escape sekvenci, napr \xFF
  S_HEX,            // prijmeme hexa escape sekvenci, napr \xF
  S_BACKSLASH,      // "\"
  S_ESC,            // \n,\s,\t,\,"
  S_BCOMMENT,       // zacatek blokoveho komentare
  S_B,              
  S_BE,             
  S_BEG,            
  S_BEGI,            
  S_BEGIN,
  S_COMM_SPACE,     // mezera za slovem begin v blokovem komentari
  S_EQ_COMMENT,     // nacetli jsme "=" a eol=TRUE
  S_E,
  S_EN,
  S_END,
  S_PREEND,         // nacetli jsme =end a cekame na EOL
  F_BCOMMENT,       // konec blokoveho komentare
}State;

/**
 * Nacte a zpracuje token
 */
Token get_token();

/**
 * Pokud se jedna o identifikator zjisti jestli to neni klicove slovo
 */
bool is_keyword(char* string, Token *ptoken);

/**
 * Zpracovani ifentifikatoru
 */
Token identifier_process(flex_string *str, Token *ptoken);

/**
 * Konverze flexible_stringu na integer 
 */
Token integer_process(flex_string *str, Token *ptoken);

/**
 * Konverze flexible_stringu na float 
 */
Token float_process(flex_string *str, Token *ptoken);

/**
 * Zpracovani flexible stringu na char*
 */
Token string_process(flex_string *str, Token *ptoken);

/**
 * Nastavi tokenu *ptoken error_value na err
 */
Token prepare_token(int err, Token *ptoken);

void delegateSourceFile(FILE *f) ;

#endif /* SCANNER_H */

