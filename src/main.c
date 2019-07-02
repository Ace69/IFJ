/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace hlavní program
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"
#include "parser.h"

/*
 * 
 */
int main(int argc, char** argv) {
    /*FILE *file = fopen("in.txt", "r");
    
    Token *rohlik;
    Token parek;
    parek.error_value = 0;
    rohlik = &parek;
    while(parek.error_value != LEX_ERR){
        parek = get_token(file);
    }
    fclose(file);
    
    if(parek.error_value == LEX_ERR){
        return 42;
    } 
    int a = parek.value.int_value;
    printf("Ta sracka funguje a precetla: %d \n", a);
    */
    FILE *f = stdin;
    if(f == NULL){
        fprintf(stderr,"Nepodarilo se nacist vstup");
        exit(INTERN_ERR);
    }
    //FILE *f = stdin;
    delegateSourceFile(f);
    parser();
    fclose(f);
    return 0;
}
