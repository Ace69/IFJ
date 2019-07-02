/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro list instrukcí
 */

#ifndef INSTR_LIST_H
#define INSTR_LIST_H


#include <stdio.h>
#include <stdlib.h>

typedef struct tElem {
    struct tElem *ptr;
    char* instr;
} *tElemPtr;
                                                         
                                                        
typedef struct {
    tElemPtr Act;
    tElemPtr First;
    tElemPtr Last;
} tList;

/*
 * Pocatecni inicializace listu
 */
void initList(tList* L);

/*
 * Smazani celeho listu
 */
void dispose(tList* L);

/*
 * pridani prvku na konec seznamu
 */
int insert(tList* L, char* instruction);

/*
 * Posune aktivitu na nasledujici instrukci
 */
void next(tList* L);

/*
 * Vytiskne list
 */
void print_list(tList *L);

#endif /* INSTR_LIST_H */


