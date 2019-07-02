/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace tabulky symbolů
 */

#include "symtable.h"
#include "error.h"
#define PRINT_TABLE(ATTR)  \
        printf("\n -------- Vypis PODTabulky Symbolu -------- \n"); \
        print_pre_order(ATTR); \
        printf("-------- Konec vypisu PODTabulky Symbolu -------- \n"); \

void BSTInit(tBSTNodePtr* RootPtr){
    (*RootPtr) = NULL;
}

int BSTInsert(tBSTNodePtr* RootPtr, char *K, TData Cont){
    if((*RootPtr)==NULL){
        tBSTNodePtr prvek=malloc(sizeof(struct tBSTNode));
        if(prvek==NULL){
            return INTERN_ERR;
        }
        else{
            prvek->Key=K;
            prvek->LPtr=NULL;
            prvek->RPtr=NULL; // jeste nedoreseno, co vse budeme ukladat!!
            prvek->BSTNodeCont.defined=Cont.defined;
            prvek->BSTNodeCont.params=Cont.params;
            prvek->BSTNodeCont.s_type=Cont.s_type;
            prvek->BSTNodeCont.tBSTNodePtr = Cont.tBSTNodePtr;
            (*RootPtr)=prvek;
        }
    }else{
        if(strcmp((*RootPtr)->Key,K)<0){ // Vkladana hodnota je mensi
            BSTInsert(&(*RootPtr)->LPtr,K, Cont);
        }
        else if(strcmp((*RootPtr)->Key,K)>0){ //Vkladana hodnota je vetsi
            BSTInsert(&(*RootPtr)->RPtr,K, Cont);
        }
        else if(strcmp((*RootPtr)->Key,K)==0){ /*pokud vkladame stejny symbol, zde by se mozna parser mel zachovat jinak; vyhodit error ci warning 
                                               * ze tu promenou ci funkci jiz mame deklarovanou, protoze se nachazi v TS*/
            (*RootPtr)->BSTNodeCont.s_type=Cont.s_type;
            (*RootPtr)->BSTNodeCont.defined=Cont.defined;
            (*RootPtr)->BSTNodeCont.tBSTNodePtr = Cont.tBSTNodePtr;
            (*RootPtr)->BSTNodeCont.definedGenerate = Cont.definedGenerate;
            (*RootPtr)->BSTNodeCont.params=Cont.params; //Updatuju pocet parametru
            return UNDEF_ID;
        }
    }
}

int BSTSearch(tBSTNodePtr RootPtr, char* K, TData* BSTNodeCont){
    if(RootPtr==NULL){
        return FALSE;
    }
    else{
        if(strcmp(RootPtr->Key,K)==0){
            (*BSTNodeCont) = (RootPtr->BSTNodeCont); //nasli jsme, vracime odkaz na data
            return TRUE;
        }
        else if(strcmp(RootPtr->Key,K)<0){
            return (BSTSearch(RootPtr->LPtr, K, BSTNodeCont));
        }
        else if(strcmp(RootPtr->Key,K)>0){
            return (BSTSearch(RootPtr->RPtr, K, BSTNodeCont));
        }
    }
}
void BSTDispose(tBSTNodePtr* RootPtr){
    if((*RootPtr)==NULL)
        return;
    BSTDispose(&(*RootPtr)->LPtr);
    BSTDispose(&(*RootPtr)->RPtr);
    free((*RootPtr));
    (*RootPtr)=NULL;
}

bool pre_order(tBSTNodePtr* RootPtr){
    bool returnType;
    if((*RootPtr) != NULL){
        //printf("[KONTROLSYM] > Key: %s | Definovano: %d\n",(*RootPtr)->Key,(*RootPtr)->BSTNodeCont.defined);
        if((*RootPtr)->BSTNodeCont.defined == TRUE){
            if((*RootPtr)->BSTNodeCont.s_type == FUNCTION){
                if(!pre_order(&(*RootPtr)->BSTNodeCont.tBSTNodePtr)){
                    return false;
                }
            }
            if(!pre_order(&(*RootPtr)->LPtr)){
                return false;
            }
            if(!pre_order(&(*RootPtr)->RPtr)){
                return false;
            }
        }else{
            return false;
        }
    }
    return true;
}
tBSTNodePtr minValueNode(tBSTNodePtr node) 
{ 
    tBSTNodePtr current = node; 
  
    /* loop down to find the leftmost leaf */
    while (current->LPtr != NULL) 
        current = current->LPtr; 
  
    return current; 
} 
tBSTNodePtr BSTDelete(tBSTNodePtr RootPtr, char *key) { 

    if (RootPtr == NULL) return RootPtr; 

    if (strcmp(RootPtr->Key,key)<0){ 
        RootPtr->LPtr = BSTDelete(RootPtr->LPtr, key); 
    }else if (strcmp(RootPtr->Key,key)>0){ 
        RootPtr->RPtr = BSTDelete(RootPtr->RPtr, key); 
    }else{ 
        if (RootPtr->LPtr == NULL){ 
            tBSTNodePtr temp = RootPtr->RPtr; 
            RootPtr = NULL;
            free(RootPtr); 
            return temp; 
        }else if (RootPtr->RPtr == NULL){
            tBSTNodePtr temp = RootPtr->LPtr;
            RootPtr = NULL;
            free(RootPtr); 
            return temp; 
        }
        tBSTNodePtr temp = minValueNode(RootPtr->RPtr); 
  
        RootPtr->Key = temp->Key; 
  
        RootPtr->RPtr = BSTDelete(RootPtr->RPtr, temp->Key); 
    } 
    return RootPtr; 
}

void print_pre_order(tBSTNodePtr* RootPtr){
    if((*RootPtr) != NULL){
        if((*RootPtr)->BSTNodeCont.s_type == FUNCTION){
            printf("[SYMTABLE] > Key: %s | ParamsCount: %d | Definovano: %d\n",(*RootPtr)->Key,(*RootPtr)->BSTNodeCont.params,(*RootPtr)->BSTNodeCont.defined);
            PRINT_TABLE(&(*RootPtr)->BSTNodeCont.tBSTNodePtr);
        }else{
            printf("[SYMTABLE] > Key: %s | Definovano: %d\n",(*RootPtr)->Key,(*RootPtr)->BSTNodeCont.defined);
        }
        print_pre_order(&(*RootPtr)->LPtr);
        print_pre_order(&(*RootPtr)->RPtr);
    }
}

