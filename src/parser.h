/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro parser
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "error.h"
#include "flex_string.h"
#include "main.h"
#include "prec_analysis.h"
#include "scanner.h"
#include "stack.h"
#include "symtable.h"
#include "token_buffer.h"
#include "generate_rec.h"

/*
 * Funkce rekurzivniho sestupu
 */
bool prog();
bool bcomment();
bool end_row();
bool sec();
bool params();
bool param_list();
bool call_params();
bool call_param_list();
bool fce_expr();
bool br_or_not();
bool fce();
bool item();
bool expr();

/*
 * Promenne funkci rekurzivniho sestupu
 */
bool prog_v;
bool bcomment_v;
bool end_row_v;
bool sec_v;
bool params_v;
bool param_list_v;
bool call_params_v;
bool call_param_list_v;
bool fce_expr_v;
bool br_or_not_v;
bool fce_v;
bool item_v;
bool expr_v;

/**
 * Funkce, ktera pousti cely parser.
 * @return
 */
bool parser();

#endif /* PARSER_H */
