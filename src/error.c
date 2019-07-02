/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace funkci pro error_value tokenu
 */

#include "error.h"

void set_error(int error, struct token *ptoken){
    ptoken->error_value = error;
}

int get_error(struct token *ptoken){
    return ptoken->error_value;
}