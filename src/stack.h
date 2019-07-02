/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro zásobník
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include "prec_symbols.h"
#include "prec_analysis.h"

typedef struct stack_elem{
    Prec_symbols stack_symbol;    // symbol, ktery je v zasobniku
    struct stack_elem *nextPtr;
}SElem;

typedef struct stack{
    SElem *topPtr;
}Stack;

/**
 * Inicializace zasobniku
 */
void SInit(Stack *s);

/**
 * Vlozeni prvku na vrchol zasobniku
 */
int Push(Stack *s, Prec_symbols stack_symbol);

/**
 * Odebrani prvku z vrcholu zasobniku
 */
void Pop(Stack *s);

/**
 * Vraceni prvku na vrcholu zasobniku 
 */
SElem* Top(Stack *s);

/**
 * Otestovani prazdnosti zasobniku 
 */
bool SEmpty(Stack *s);

/**
 * Smaze cely zasobnik
 */
void SFreeStack(Stack *s);

/**
 * Vraci terminal nejblizsi k vrcholu zasobniku
 */
SElem* stack_top_terminal(Stack *s);

/**
 * Vklada symbol (v nasem pripade operator) pred terminal co je nejvysse na zasobniku {provadi se pri operaci < "shift")
 */
int stack_insert_operator(Stack *s);

extern Stack *stackPtr;

#endif /* STACK_H */

