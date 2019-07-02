/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace zásobníku
 */

#include "stack.h"

void SInit(Stack *s){
    s->topPtr = NULL;
}

int Push(Stack *s, Prec_symbols stack_symbol){
    SElem *newElem = malloc(sizeof(struct stack_elem));
    if(newElem == NULL){
        return INTERN_ERR;
    }
    
    //Inicializace hodnot noveho prvku
    newElem->stack_symbol = stack_symbol;
    newElem->nextPtr = s->topPtr;
    s->topPtr = newElem;
    return 0;
}

void Pop(Stack *s){
    SElem *elemPtr;
    if(s->topPtr != NULL){
        elemPtr = s->topPtr;
        s->topPtr = s->topPtr->nextPtr;
        free(elemPtr);
    }
}

SElem* Top(Stack *s){
    return (s->topPtr);
}

bool SEmpty(Stack *s){
    return (s->topPtr == NULL);
}

void SFreeStack(Stack *s){
    while(s->topPtr != NULL){
        Pop(s);
    }
}

SElem* stack_top_terminal(Stack *s){
    SElem *item = s->topPtr;
    while(item != NULL){                //Pruchod celym zasobnikem dokud nenarazim na terminal
        if(item->stack_symbol != S_NO_TERM){
            return item;                //Nasel jsem terminal, vracim
        }
        item = item->nextPtr;           //Pokracuju dal
    }
}

int stack_insert_operator(Stack *s){
    SElem* previous = NULL;
    SElem* item = s->topPtr;
    
    while(item != NULL){                //Pruchod celym zasobnikem dokud nenarazim na terminal
        if(item->stack_symbol != S_NO_TERM){        //Nasel jsem terminal
            
            SElem *newElem = malloc(sizeof(struct stack_elem));
            if(newElem == NULL){
                return INTERN_ERR;      //Chyba alokace
            }
            
            if(previous == NULL){       //Terminal je na vrcholu zasobniku
                newElem->nextPtr = s->topPtr;
                newElem->stack_symbol = S_LESS;
                s->topPtr = newElem;
            }else{
                newElem->nextPtr = item;            //Terminal je nekde uprostred
                previous->nextPtr = newElem;
                newElem->stack_symbol = S_LESS;
            }
            return 0;                
        }
        previous = item;
        item = item->nextPtr;           //Pokracuju dal v hledani
    }
    
    return 1;                   //Nebyl nalezen terminal? nemelo by nastat..
}
