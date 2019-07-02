/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 *
 * Implementrace precedenční analýzy
 */

#include "prec_analysis.h"
#include "postfix_transfer.h"

Stack *stackPtr;
postStack *postfixStackPtr;
int error_value;

Index prec_tbl[14][14]=                                         //Implementovana tabulka pro precedencni analyzu
{
//            *  /  +  -  <  <= >  >= == !=  (  )  i  $   Znak na vstupu
/* * */    {  V, V, V, V, V, V, V, V, V, V,  M, V, M, V},
/* / */    {  V, V, V, V, V, V, V, V, V, V,  M, V, M, V},
/* + */    {  M, M, V, V, V, V, V, V, V, V,  M, V, M, V},
/* - */    {  M, M, V, V, V, V, V, V, V, V,  M, V, M, V},
/* < */    {  M, M, M, M, E, E, E, E, E, E,  M, V, M, V},
/* <= */   {  M, M, M, M, E, E, E, E, E, E,  M, V, M, V},
/* > */    {  M, M, M, M, E, E, E, E, E, E,  M, V, M, V},
/* >= */   {  M, M, M, M, E, E, E, E, E, E,  M, V, M, V},
/* == */   {  M, M, M, M, E, E, E, E, E, E,  M, V, M, V},    
/* != */   {  M, M, M, M, E, E, E, E, E, E,  M, V, M, V},
/* ( */    {  M, M, M, M, M, M, M, M, M, M,  M, R, M, E},
/* ) */    {  V, V, V, V, V, V, V, V, V, V,  R, V, E, V},
/* i */    {  V, V, V, V, V, V, V, V, V, V,  R, V, E, V},
/* $ */    {  M, M, M, M, M, M, M, M, M, M,  M, E, M, E}
//Zasobnik
};

Prec_symbols token_to_sym(Buffer* buffer){
    TData content;
    switch(buffer->actual->token.type){
    case INTEGER:
        return S_IDENTIFIER;
        break;
        
    case FLOAT:
        return S_IDENTIFIER;
        break;
        
    case STRING:
        return S_IDENTIFIER;
        break;
        
    case IDENTIFIER:
        content.defined = 0;
        if(fceSymbolTable != NULL){             //Pokud jsme zanoreni ve funkci, vyhledavame v ve fceSymbolTable
            if(BSTSearch((*fceSymbolTable), buffer->actual->token.value.string_value.string, &content) == FALSE){      //Chyba pokud se ve vyrazu vyskytne nedefinovana promenna
                error_value = UNDEF_ID;
                return S_IDENTIFIER;
            }
        }else{              //Nejsme zanoreni
            if(BSTSearch((*symbolTable), buffer->actual->token.value.string_value.string, &content) == FALSE){      //Chyba pokud se ve vyrazu vyskytne nedefinovana promenna
            error_value = UNDEF_ID;
            return S_IDENTIFIER;
            }
        }
        
        return S_IDENTIFIER;
        break;
        
    case NIL:
        return S_IDENTIFIER;
        break;
        
    case EQQ:
        return S_EQQ;
        break;
        
    case GREATER:
        return S_GREAT;
        break;
        
    case GEQ:
        return S_GREATEQ;
        break;
        
    case LESSER:
        return S_LESS;
        break;
        
    case LEQ:
        return S_LESSEQ;
        break;
        
    case NEQ:
        return S_NEQ;
        break;
        
    case MUL:
        return S_MUL;
        break;
        
    case DIV:
        return S_DIV;
        break;
        
    case ADD:
        return S_PLUS;
        break;
        
    case SUB:
        return S_MINUS;
        break;
        
    case RBRACKET:     
        return S_RBRACKET;
        break;
        
    case LBRACKET:
        return S_LBRACKET;
        break;
        
    default:
        return S_DOLLAR;
    }
}

Prec_rules rules(){
    //Pokud se na zasobniku nenachazi dostatecny pocet symbolu, muze se pouzit pouze jedine pravidlo a nemusime prochazet vsechny podminky
    if(stackPtr->topPtr->nextPtr->nextPtr->nextPtr != NULL){           
        SElem* op0 = stackPtr->topPtr->nextPtr->nextPtr->nextPtr;
        SElem* op1 = stackPtr->topPtr->nextPtr->nextPtr;
        SElem* op2 = stackPtr->topPtr->nextPtr;
        SElem* op3 = stackPtr->topPtr;

        if(op0->stack_symbol == S_LESS && op1->stack_symbol==S_LBRACKET && op2->stack_symbol==S_NO_TERM && op3->stack_symbol==S_RBRACKET){ // pokud mame E -> (E)
            return LBR_E_RBR;
        }

        // pouzijeme pravidla, ktere maji 3 symboly na prave strane
        if(op0->stack_symbol == S_LESS && op1->stack_symbol==S_NO_TERM && op3->stack_symbol==S_NO_TERM){ // pokud mame posloupnost neterminal,operator,neterminal
            if(op2->stack_symbol==S_MUL){ // E -> E*E
                return E_MUL_E;
            }
            else if(op2->stack_symbol==S_DIV){ // E= E / E
                return E_DIV_E;
            }
            else if(op2->stack_symbol==S_PLUS){ // E = E + E
                return E_ADD_E;
            }
            else if(op2->stack_symbol==S_MINUS){ // E = E - E
                return E_SUB_E;
            }
            else if (op2->stack_symbol==S_LESS){ // E = E < E
                return E_LESS_E;
            }
            else if (op2->stack_symbol==S_LESSEQ){ // E = E <= E
                return E_LESSEQ_E;
            }
            else if (op2->stack_symbol==S_GREAT){ // E = E > E
                return E_GREATER_E;
            }
            else if (op2->stack_symbol==S_GREATEQ){ // E = E <= E
                return E_GREATEQ_E;
            }
            else if (op2->stack_symbol==S_EQQ){ // E = E == E
                return E_EQ_E;
            }
            else if (op2->stack_symbol==S_NEQ){ // E = E != E
                return E_NEQ_E;
            }
        }
        // mame pouze jeden operand
        if(op2->stack_symbol == S_LESS && op3->stack_symbol==S_IDENTIFIER){ // pouzijeme pravidlo E -> i
            return IDENT;
        }else{
            return INVALID;             //Neexistuje zadne pravidlo
        }
    }else{
        SElem* op1 = stackPtr->topPtr->nextPtr->nextPtr;
        SElem* op2 = stackPtr->topPtr->nextPtr;
        SElem* op3 = stackPtr->topPtr;
        // mame pouze jeden operand
        if(op2->stack_symbol == S_LESS && op3->stack_symbol==S_IDENTIFIER){ // pouzijeme pravidlo E -> i
            return IDENT;
        }else{
            return INVALID;             //Neexistuje zadne pravidlo
        }
    }
}

int reduce(Prec_rules rule){
    switch(rule){
        case(INVALID):
            return SYNTAX_ERR;
        case(IDENT):            //E -> i
            Pop(stackPtr);
            Pop(stackPtr);
            
            if(Push(stackPtr,S_NO_TERM) != 0){
                return INTERN_ERR;
            }
            break;
        case(LBR_E_RBR):        //vsechna ostatni pravidla
        case(E_MUL_E):
        case(E_DIV_E):
        case(E_ADD_E):
        case(E_SUB_E):
        case(E_LESS_E):
        case(E_LESSEQ_E):
        case(E_GREATER_E):
        case(E_GREATEQ_E):
        case(E_EQ_E):
        case(E_NEQ_E):
            Pop(stackPtr);
            Pop(stackPtr);
            Pop(stackPtr);
            Pop(stackPtr);
            if(Push(stackPtr,S_NO_TERM) != 0){
                return INTERN_ERR;
            }
            break;
    }
}

int prec_analysis(Buffer *buff){
    Stack stack;            //Vytvoreni zasobniku
    stackPtr = &(stack);
    SInit(stackPtr);
    error_value = 0;
    
    postStack mainStack;                //Vytvoreni zasobniku pro postfixovy zapis
    postfixStackPtr = &(mainStack);
    post_SInit(postfixStackPtr);

    
    
    if(Push(stackPtr,S_DOLLAR) != 0){
        return INTERN_ERR;
    }
    while(1){            
        Prec_symbols a = (stack_top_terminal(stackPtr)->stack_symbol); //  terminal nejblize vrcholu
        Prec_symbols b = token_to_sym(buff); // aktualni znak na vstupu
        if(error_value == 3){
            return UNDEF_ID;
        }
        Prec_rules rule;
        
        switch(prec_tbl[a][b]){           
        case(R):                // "R" v precedencni tabulce
            if(Push(stackPtr,b) != 0){
                return INTERN_ERR;
            }
            moveActual(buff);
            break;
            
        case(M):                // "M" (mensi) v precedencni tabulce
            if(stack_insert_operator(stackPtr) == INTERN_ERR){ //Na zasobniku zamenime symbol "a" za "a<"
                SFreeStack(stackPtr);
                post_SFreeStack(postfixStackPtr);
                return INTERN_ERR;
            }      
            if(Push(stackPtr,b) != 0){
                return INTERN_ERR;
            }    
            moveActual(buff);            //posun na dalsi token v bufferu
            break;
            
        case(V):                    // "V" bude se pouzivat pravidlo
            rule = rules(stackPtr);
            if((rule != INVALID)){
                reduce(rule);
                if(infix_to_postfix(postfixStackPtr, rule) != 0){
                    return INTERN_ERR;
                }
            }else{
                SFreeStack(stackPtr);
                post_SFreeStack(postfixStackPtr);
                return SYNTAX_ERR;
            }
            break;
        case(E):                 // "E" prazdne policko v tabulce
            if((a == S_DOLLAR) && (b == S_DOLLAR)){
                SFreeStack(stackPtr);           //Inicializace zasobniku pro prevraceni postixoveho zapisu pro jeho vycisleni
                postStack finalStack;
                postStack* finalStackPtr = &finalStack;
                post_SInit(finalStackPtr);
                
                reverse_stack(postfixStackPtr, finalStackPtr);      
                
                postSElem* genElem = finalStackPtr->topPtr;
                while(genElem != NULL){           //pruchod celym zasobnikem
                    enumerate_posftix(genElem);   //generovani IFJcode18
                    genElem = genElem->nextPtr;
                }
                pops_result();                    //uklada vysledek vyrazu na domluvene misto
                post_SFreeStack(postfixStackPtr);       //Smazani zasobniku
                post_SFreeStack(finalStackPtr);
                return 0;
            }else{
                SFreeStack(stackPtr);
                post_SFreeStack(postfixStackPtr);
                return SYNTAX_ERR;
            }
        }
    }   
}

