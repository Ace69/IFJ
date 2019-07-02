/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace pro list instrukcí
 */

#include "instr_list.h"
#include "instruction.h"
#include <malloc.h>
#include <string.h>
#include "error.h"

void initList(tList* L){
    L->Act=NULL;
    L->First=NULL;
    L->Last=NULL;
}

void dispose(tList* L){
    tElemPtr tmp;
    while(L->First)
        if(L->First!=NULL){
            tmp=L->First;
            L->First=L->First->ptr;
            free(tmp);
        }
}

int insert(tList* L, char* instruction){
    tElemPtr new = malloc(sizeof(struct tElem));
    if(new==NULL)
        return INTERN_ERR;
    new->instr = malloc(sizeof(strlen(instruction)));
    new->instr= instruction;
    new->ptr=NULL;
    if(L->First==NULL){
        L->First=new;
    }else{
        L->Last->ptr=new;
    }
    L->Last=new;
}

void next(tList* L){
    if (L->Act != NULL)
        L->Act = L->Act->ptr;
}
void listGoto(tList *L, tElemPtr gotoInstr){
    L->Act = gotoInstr;
}

tElemPtr getPointerLast(tList *L){
  return L->Last;
}

void print_list(tList *L){
    FILE *out = fopen("tests/out.in","w");
    if(L != NULL){
        L->Act = L->First;
        while(L->Act != NULL){
            fprintf(out,"%s\n",L->Act->instr);
            L->Act = L->Act->ptr;
        }
    }
}


