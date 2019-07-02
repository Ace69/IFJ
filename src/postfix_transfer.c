/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace převodu infixového zápisu čísla na postfixový
 */

#include "postfix_transfer.h"

int token_to_postfix(postStack *mainStack){
    Token prevToken = tokenBuffer->actual->prevPtr->token;
    switch(prevToken.type){
    case INTEGER:
        if(post_PushInt(mainStack, prevToken.type, prevToken.value.int_value) == INTERN_ERR){
            return INTERN_ERR;
        }
        return 0;
    case FLOAT:
        if(post_PushFloat(mainStack, prevToken.type, prevToken.value.double_value) == INTERN_ERR){
            return INTERN_ERR;
        }
        return 0;
    case STRING:
        if(post_PushString(mainStack, prevToken.type, prevToken.value.string_value.string) == INTERN_ERR){
            return INTERN_ERR;
        }
        return 0;
    case IDENTIFIER:
        if(post_PushIdent(mainStack, prevToken.type, prevToken.value.string_value.string) == INTERN_ERR){
            return INTERN_ERR;
        }
        return 0;
    case NIL:
        if(post_PushEveryType(mainStack, prevToken.type) == INTERN_ERR){
            return INTERN_ERR;
        }
        return 0;
    }
}

int infix_to_postfix(postStack *mainStack, Prec_rules rule){
    if(rule == IDENT){                                        //Jedna li se o identifikator, zpracuje se
        if(token_to_postfix(mainStack) != 0){
            return INTERN_ERR;
        }
    }else if((rule != LBR_E_RBR) && (rule != IDENT)){         //Jedna li se o pravidlo zpracovani operatoru
        switch(rule){
            case(E_MUL_E):                          //Konkretni operator se vlozi na vrchol zasobniku
                if(post_PushEveryType(mainStack, MUL) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;

            case(E_DIV_E):
                if(post_PushEveryType(mainStack, DIV) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;

            case(E_ADD_E):
                if(post_PushEveryType(mainStack, ADD) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;

            case(E_SUB_E):
                if(post_PushEveryType(mainStack, SUB) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;

            case(E_LESS_E):
                if(post_PushEveryType(mainStack, LESSER) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;

            case(E_LESSEQ_E):
                if(post_PushEveryType(mainStack, LEQ) == INTERN_ERR){
                    return INTERN_ERR;
                }
                break;

            case(E_GREATER_E):
                if(post_PushEveryType(mainStack, GREATER) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;

            case(E_GREATEQ_E):
                if(post_PushEveryType(mainStack, GEQ) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;

            case(E_EQ_E):
                if(post_PushEveryType(mainStack, EQQ) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;

            case(E_NEQ_E):
                if(post_PushEveryType(mainStack, NEQ) == INTERN_ERR){
                    return INTERN_ERR;
                }
                return 0;
                break;
        }
    }else if(rule == LBR_E_RBR){
        return 0;
    }
}