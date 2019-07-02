/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro generování instrukcí IFJcode18
 */

#ifndef GENERATE_H
#define GENERATE_H

#include <stdlib.h>
#include <stdio.h>
#include "postfix_stack.h"
#include "generate_rec.h"


/**
 * Zjisti jaky typ ma prvek na zasobniku pro ktery bude generovana instrukce
 */
void enumerate_posftix(postSElem* elem);

/**
 * Ulozi vysledek do predem domluvene hodnoty LF@%prec%
 */
void pops_result();

/**
 * Vygeneruje instrukce pro mensi nez "+"
 */
void generate_add();

/**
 * Vygeneruje instrukce pro odcitani "-"
 */
void generate_sub();

/**
 * Vygeneruje instrukce pro nasobeni "*"
 */
void generate_mul();

/**
 * Vygeneruje instrukce pro deleni "/"
 */
void generate_div();

/**
 * Vygeneruje instrukce pro mensi nez "<"
 */
void generate_lesser();

/**
 * Vygeneruje instrukce pro mensi rovno "<="
 */
void generate_leq();

/**
 * Vygeneruje instrukce pro vetsi nez ">"
 */
void generate_greater();

/**
 * Vygeneruje instrukce pro vetsi rovno ">="
 */
void generate_geq();

/**
 * Vygeneruje instrukce rovna se "=="
 */
void generate_eq();

/**
 * Vygeneruje instrukce pro nerovna se "!="
 */
void generate_neq();

#endif /* GENERATE_H */

