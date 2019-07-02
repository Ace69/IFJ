/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace pro zásobník na ukládání postfixového zápisu
 */

#include "postfix_stack.h"

void post_SInit(postStack *s){
    s->topPtr = NULL;
}

int post_PushIdent(postStack *s, Type type, char* name){
    postSElem *newElem = malloc(sizeof(struct post_stack_elem));
    if(newElem == NULL){
        return INTERN_ERR;
    }
    
    //Inicializace hodnot noveho prvku
    newElem->name = name;
    newElem->type = type;
    newElem->nextPtr = s->topPtr;
    s->topPtr = newElem;
    return 0;
}

int post_PushInt(postStack *s, Type type, int integer_value){
    postSElem *newElem = malloc(sizeof(struct post_stack_elem));
    if(newElem == NULL){
        return INTERN_ERR;
    }
    
    //Inicializace hodnot noveho prvku
    newElem->type = type;
    newElem->integer_value = integer_value;
    newElem->nextPtr = s->topPtr;
    s->topPtr = newElem;
    return 0;
}

int post_PushFloat(postStack *s, Type type, float float_value){
    postSElem *newElem = malloc(sizeof(struct post_stack_elem));
    if(newElem == NULL){
        return INTERN_ERR;
    }
    
    //Inicializace hodnot noveho prvku
    newElem->type = type;
    newElem->float_value = float_value;
    newElem->nextPtr = s->topPtr;
    s->topPtr = newElem;
    return 0;
}

int post_PushString(postStack *s, Type type, char* string_value){
    postSElem *newElem = malloc(sizeof(struct post_stack_elem));
    if(newElem == NULL){
        return INTERN_ERR;
    }
    
    //Inicializace hodnot noveho prvku
    newElem->type = type;
    newElem->string_value = string_value;
    newElem->nextPtr = s->topPtr;
    s->topPtr = newElem;
    return 0;
}

int post_PushEveryType(postStack *s, Type type){
    postSElem *newElem = malloc(sizeof(struct post_stack_elem));
    if(newElem == NULL){
        return INTERN_ERR;
    }
    
    //Inicializace hodnot noveho prvku
    newElem->type = type;
    newElem->nextPtr = s->topPtr;
    s->topPtr = newElem;
    return 0;
}

int post_PushAll(postStack *s, postSElem* copy){
    postSElem *newElem = malloc(sizeof(struct post_stack_elem));
    if(newElem == NULL){
        return INTERN_ERR;
    }
    
    //Inicializace hodnot noveho prvku
    newElem->type = copy->type;
    newElem->name = copy->name;
    newElem->integer_value = copy->integer_value;
    newElem->float_value = copy->float_value;
    newElem->string_value = copy->string_value;
    newElem->nextPtr = s->topPtr;
    s->topPtr = newElem;
    return 0;
}

void post_Pop(postStack *s){
    postSElem *elemPtr;
    if(s->topPtr != NULL){
        elemPtr = s->topPtr;
        s->topPtr = s->topPtr->nextPtr;
        free(elemPtr);
    }
}

postSElem* post_Top(postStack *s){
    return (s->topPtr);
}

void reverse_stack(postStack *source, postStack *dest){
    postSElem* tmp = (post_Top(source));
    while(tmp != NULL){
        post_PushAll(dest, tmp);
    tmp = tmp->nextPtr;
    }
}

bool post_SEmpty(postStack *s){
    return (s->topPtr == NULL);
}

void post_SFreeStack(postStack *s){
    while(s->topPtr != NULL){
        post_Pop(s);
    }
}