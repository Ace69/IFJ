/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro převod infixového zápisu čísla na postfixový
 */

#ifndef POSTFIX_TRANSFER_H
#define POSTFIX_TRANSFER_H

#include "prec_analysis.h"
#include "postfix_stack.h"
#include "token.h"
#include "token_buffer.h"


/**
 * Ziska hodnoty z tokenu a ulozi je do zasobniku pro postfix zapis 
 */
int token_to_postfix(postStack *mainStack);

/**
 * Funkce ulozi operator do hlavniho kde bude vysledna postfixova notace 
 */
int infix_to_postfix(postStack *mainStack, Prec_rules rule);

#endif /* POSTFIX_TRANSFER_H */

