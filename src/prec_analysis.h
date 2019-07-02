/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 *  
 * Hlavičkový soubor pro precedenční analýzu
 */

#ifndef PREC_ANALYSIS_H
#define PREC_ANALYSIS_H

#include "prec_symbols.h"
#include "stack.h"
#include "token_buffer.h"
#include "postfix_stack.h"
#include "symtable.h"
#include "generate_prec.h"

// Seznam pravidel precedencni tabulky
typedef enum{
    E_MUL_E,//         E = E * E
    E_DIV_E,//         E = E / E
    E_ADD_E,//         E = E + E
    E_SUB_E,//         E = E - E
    E_LESS_E,//        E = E < E
    E_LESSEQ_E,//      E = E <= E
    E_GREATER_E,//     E = E > E
    E_GREATEQ_E,//     E = E >= E
    E_EQ_E, //         E = E == E
    E_NEQ_E, //        E = E != E
    LBR_E_RBR,//       E = (E)
    IDENT, //          E = i
    INVALID //         NEPLATNE PRAVIDLO
    }Prec_rules;
    
typedef enum
{
    M,	//      < mensi
    V,	//      > vetsi
    R,	//      = rovno
    E	//       chyba
} Index;


/*
*Funkce, ktera vezme token, podiva se na jeho typ, zkontroluje, jestli takovy symbol muze byt, a pote
 * ho na dany symbol prevede(vrati), v jinem pripade vrati DOLLAR
 */
Prec_symbols token_to_sym(Buffer* buffer);

/*
 * Funkce na kontrolu "gramatiky" vyrazu
 * Funkce bude vracet pravidla, pokud byly nalezeny, jinak INVALID(funkce je typu Prec_rules, tudiz zde nemuzeme rovnou vyhodit SYNTAX_ERR)
 * Zaroven zjistuje zda se pred pravidlem nachazi znak "<"
 */
Prec_rules rules();

/*
 * Funkce na redukci pravidel
 */
int reduce(Prec_rules rule);

/*
 * Finalni precedencni analyza, ktera se bude volat z parseru
 */
int prec_analysis(Buffer *buff);

#endif /* PREC_ANALYSIS_H */
