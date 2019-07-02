/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Knihovna definujici errory
 */

#ifndef ERROR_H
#define	ERROR_H

#include "../header/scanner.h"

/**
 * TOKEN_OK - Token byl v poradku prijat
 * LEX_ERR - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
 * SYNTAX_ERR - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)
 * UNDEF_ID - nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné atp.
 * EXPR_ERR - sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
 * ARG_COUNT_ERR - sémantická chyba v programu – špatný počet parametrů u volání funkce
 * SEMANTIC_ERR - ostatní sémantické chyby
 * DIV_ZERO_ERR - běhová chyba dělení nulou
 * INTERN_ERR interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti, atd.).  
 */

#define TOKEN_OK 0
#define LEX_ERR 1
#define SYNTAX_ERR 2
#define UNDEF_ID 3
#define EXPR_ERR 4
#define ARG_COUNT_ERR 5
#define SEMANTIC_ERR 6
#define DIV_ZERO_ERR 9
#define INTERN_ERR 99   

/**
 * Nastavi error_value tokenu na hodnotu error
 */
void set_error(int error, Token *ptoken);

/**
 * Vraci hodnotu error_value tokenu
 */
int get_error(Token *ptoken);

#endif /* ERROR_H */
