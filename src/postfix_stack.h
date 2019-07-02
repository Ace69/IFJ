/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro zásobník na ukládání postfixového zápisu
 */

#ifndef POSTFIX_STACK_H
#define POSTFIX_STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "flex_string.h"
#include "token.h"

typedef struct post_stack_elem{
    char* name;
    Type type;
    int integer_value;
    double float_value;
    char* string_value;
    struct post_stack_elem *nextPtr;
}postSElem;

typedef struct poststack{
    postSElem *topPtr;
}postStack;

/**
 * Inicializace zasobniku
 */
void post_SInit(postStack *s);

/**
 * Vlozeni identifikatoru na vrchol zasobniku 
 */
int post_PushIdent(postStack *s, Type type, char* name);

/**
 * Vlozeni integeru na vrchol zasobniku 
 */
int post_PushInt(postStack *s, Type type, int integer_value);

/**
 * Vlozeni floatu na vrchol zasobniku 
 */
int post_PushFloat(postStack *s, Type type, float float_value);

/**
 * Vlozeni stringu na vrchol zasobniku 
 */
int post_PushString(postStack *s, Type type, char* string_value);

/**
 * Pushne na zasbonik bez ohledu na typ (nezajima nas hodnota prvku)
 */
int post_PushEveryType(postStack *s, Type type);

/**
 * Odebrani prvku z vrcholu zasobniku
 */
void post_Pop(postStack *s);

/**
 * Otestovani prazdnosti zasobniku, pokud je prazdny navratova hodnota != 0
 */
bool post_SEmpty(postStack *s);

/**
 * Smaze cely zasobnik
 */
void post_SFreeStack(postStack *s);

/**
 * Prevrati zasobnik
 */
void reverse_stack(postStack *source, postStack *dest);
#endif /* POSTFIX_STACK_H */