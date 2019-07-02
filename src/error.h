/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor s definicemi jednotlivých chybových stavů
 */

#ifndef ERROR_H
#define ERROR_H

#include "token.h"

/**
LEX_ERR - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
SYNTAX_ERR - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)
UNDEF_ID - nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné atp.
EXPR_ERR - sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
ARG_COUNT_ERR - sémantická chyba v programu – špatný počet parametrů u volání funkce
SEMANTIC_ERR - ostatní sémantické chyby
DIV_ZERO_ERR - běhová chyba dělení nulou
INTERN_ERR interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti, atd.).  
 */

#define TOKEN_OK  0
#define LEX_ERR 1
#define SYNTAX_ERR  2
#define UNDEF_ID 3
#define EXPR_ERR  4
#define ARG_COUNT_ERR  5
#define SEMANTIC_ERR 6
#define DIV_ZERO_ERR  9
#define INTERN_ERR 99

/**
 * Nastavi tokenu error value
 */
void set_error(int error, Token *ptoken);

/**
 * Vrati hodnotu error value z tokenu
 */
int get_error(Token *ptoken);

#endif /* ERROR_H */