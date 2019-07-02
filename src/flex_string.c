/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace stingu
 */

#include <string.h>
#include <malloc.h>
#include "flex_string.h"

//Pocet bytu pro pocatecni inicializaci flexibilniho stringu
#define FLEX_STRING_INIT_LEN 8      

#define FLEX_STRING_ERROR 99
#define FLEX_STRING_OK 0


int flex_string_init(flex_string *s){
    s->string = (char*) malloc(FLEX_STRING_INIT_LEN);
    if(s->string == NULL){
        return FLEX_STRING_ERROR;
    } 
   
    s->string[0] = '\0';
    s->length = 0;
    s->size = FLEX_STRING_INIT_LEN;
   
    return FLEX_STRING_OK;
}


void flex_string_free(flex_string *s){
    free(s->string);
}


int flex_string_add(flex_string *s, char c){
    //Pokud neni dostatek mista je potreba naalokovovat vetsi
    if (s->length + 1 >= s->size){
        int new_size = s->length + FLEX_STRING_INIT_LEN;
        s->string = (char*) realloc(s->string, new_size);
        if(s->string == NULL){
            return FLEX_STRING_ERROR;
        } 
        s->size = new_size;
    }
    
    s->string[s->length] = c;
    s->length++;
    s->string[s->length] = '\0';
    
    return FLEX_STRING_OK;
}


char *flex_string_get(flex_string *s){
    return s->string;
}


int flex_string_cmp(flex_string *s1, flex_string *s2){
     return strcmp(s1->string, s2->string);
}


int flex_string_cmp_const(flex_string *s1, char *s2){
    return strcmp(s1->string, s2);
}