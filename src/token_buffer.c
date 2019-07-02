/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace bufferu pro tokeny - pomoci DLL
 */

#include "token_buffer.h"

void bufferInit(Buffer *b){
    b->first = NULL;
    b->last = NULL;
    b->actual = NULL;
}

int bufferInsert(Buffer *b, Token token){
    buffItem new_item = malloc(sizeof(struct buffer_item));
    if(new_item == NULL){
        return INTERN_ERR;
    }
    
    //Inicializace dat tokenu
    new_item->token.type = token.type;
    new_item->token.value = token.value;
    new_item->token.error_value = token.error_value;
    new_item->nextPtr = NULL;
    
    if(b->first == NULL){       //buffer je prazdny
        b->first = new_item;
        b->last = new_item;
        b->actual = new_item;
    }else{
        new_item->prevPtr = b->last;
        b->last->nextPtr = new_item;
        b->last = new_item;
    }
}

void moveActual(Buffer *b){
    if(b->actual->nextPtr != NULL){
        b->actual = b->actual->nextPtr;
    }
}

void disposeBuffer(Buffer *b) {
    while(b->first != NULL){
        buffItem tmp = b->first;
        b->first = b->first->nextPtr;       
        
        free(&(tmp->token.value.string_value));        //Smazani flex_stringu
        free(tmp);                                  //Smazani celeho prvku vcetne tokenu
    }

  b->first = NULL;
  b->actual = NULL;
  b->last = NULL;
}