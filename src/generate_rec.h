/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro generování kódu v rekurzivním sestupu. 
 */

#ifndef GENERATE_REC_H
#define GENERATE_REC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <string.h>
/*
 * Funkce na vypis instrukci na stdout/do souboru
 */    
void propagate_output();
/*
 * Funkce na generovani instrukci
 */
void generate_instruction(char* instruction);
/*
 * Funkce na generovani navesti
 */
char* generate_label(char* label);
/*
 * Funkce na konkatenaci stringu
 */
char* concat(char *first, char* second);
/*
 * Vestavena funkce na vypsani hodnot na stdout
 */
void buildin_print(int params,char *label);
/*
 * Definice vestavenych funkci
 */
void buildin_function();
/*
 * Funkce na vygenerovani hlavicky IFJcode18
 */
void generate_header();
/*
 * Vestavena funkce na cteni integeru ze stdin
 */
void buildin_inputi();
/*
 * Vestavena funkce na cteni floatu ze stdin
 */
void buildin_inputf();
/*
 * Vestavena funkce na cteni stringu ze stdin
 */
void buildin_inputs();
/*
 * Vestavena funkce na zjisteni ASCII hodnoty
 */
void buildin_ord();
/*
 * Vestavena funkce, ktera vrati znak dle ASCII hodnoty zadane parametrem
 */
void buildin_chr();
/*
 * Vestavena funkce na zjisteni delky retezce zadane parametrem
 */
void  buildin_length();

#ifdef __cplusplus
}
#endif

#endif /* GENERATE_REC_H */

