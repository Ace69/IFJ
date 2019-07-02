/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro tabulku symbolů
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>


typedef enum{                // typ identifikatoru
    VAR,                     // promenna
    FUNCTION                 // funkce
    }SymbolType;
    
typedef struct{              // co budeme uchovavat o identifikatorech v TS
    SymbolType s_type;       // typ identifikatoru - VAR/FCE
    bool defined;            // jestli byl symbol definovan
    int params;              // seznam formalnich parametru
    struct tBSTNode* tBSTNodePtr;   //Ukazatel na podtabulku symbolu 
    bool definedGenerate;
    }TData;
    
typedef struct tBSTNode { // struktura BVS
	char* Key;                                    
	TData BSTNodeCont;                                          
	struct tBSTNode * LPtr;                                   
	struct tBSTNode * RPtr;                                   
} *tBSTNodePtr;

/**
 * Inicializace BVS
 */
void BSTInit(tBSTNodePtr* RootPtr);

/**
 * Nalezení identifikatoru/fce K v BVS a predani dat BSTNodeCont
 */
int BSTSearch(tBSTNodePtr RootPtr, char* K, TData* BSTNodeCont);

/**
 * Vložení identifikatoru K s daty Cont do BVS
 */
int BSTInsert(tBSTNodePtr* RootPtr, char *K, TData Cont);

/**
 * Smazání celého stromu s kořenem RootPtr
 */
void BSTDispose(tBSTNodePtr* RootPtr);

/**
 * Smazani jednoho prvku
 * @param RootPtr
 * @param key
 * @return 
 */
tBSTNodePtr BSTDelete(tBSTNodePtr RootPtr, char *key);

/**
 * Rekurzivni pre_order pruchod stromem, v BSTNodeCont
 */
bool pre_order(tBSTNodePtr* RootPtr);

/**
 * Vypise nazev promenne/fce a jestli byla definovana
 */
void print_pre_order(tBSTNodePtr* RootPtr);

extern tBSTNodePtr* symbolTable;
extern tBSTNodePtr* fceSymbolTable;
#endif /* SYMTABLE_H */

