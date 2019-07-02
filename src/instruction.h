/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor ... 
 */

#ifndef GENERATOR_H
#define GENERATOR_H

typedef enum{
/*---------- Prace s ramci, volani funkci ----------*/
    INST_MOVE, // <var> <symb>
    INST_CREATEFRAME,
    INST_PUSHFRAME,
    INST_POPFRAME,
    INST_DEFVAR, // <var>
    INST_CALL, //  <label>
    INST_RETURN,
/*---------- Prace s datovym zasobnikem ----------*/
    INST_PUSHS,// <symb>
    INST_POPS, // <var>
    INST_CLEARS,
/*---------- Aritmeticke, relacni, boolovske a konverzni instrukce ----------*/
    INST_INST_ADD, // <var> <symb1> <symb2>
    INST_SUB, // <var> <symb1> <symb2>
    INST_MUL, // <var> <symb1> <symb2>
    INST_DIV, // <var> <symb1> <symb2>
    INST_IDIV, // <var> <symb1> <symb2>
    INST_ADDS,      // zasobnikove verze
    INST_SUBS,
    INST_MULS,
    INST_DIVS,
    INST_IDIVS,                  
    INST_LT,//    <var> <symb1> <symb2>
    INST_GT,//    <var> <symb1> <symb2>
    INST_EQ,//    <var> <symb1> <symb2>
    INST_LTS,   // zasobnikove verze
    INST_GTS,
    INST_EQS,
            
    INST_AND,//    <var> <symb1> <symb2>
    INST_OR,//    <var> <symb1> <symb2>
    INST_NOT,//    <var> <symb1> <symb2>        
    INST_ANDS,  // zasobnikove verze
    INST_ORS,
    INST_NOTS,
            
    INST_INT2FLOAT,//    <var> <symb>
    INST_FLOAT2INT,//    <var> <symb>
    INST_INT2CHAR,//    <var> <symb>
    INST_STRI2INT,//    <var> <symb1> <symb2>
    INST_INT2FLOATS,    // zasobnikove verze
    INST_FLOAT2INTS,
    INST_INT2CHARS,
    INST_STRING2INTS, 
/*---------- Vstupne-vystupni isntrukce ----------*/
    INST_READ, // <var> <type>
    INST_WRITE, // <symb>
/*---------- Prace s retezci ----------*/
    INST_CONCAT,// <var> <symb1> <symb2>
    INST_STRLEN, // <var> <symb>
    INST_GETCHAR, // <var> <symb1> <symb2>
    INST_SETCHAR, // <var> <symb1> <symb2>
/*---------- Prace s typy ----------*/
    INST_TYPE, // <var> <symb>
/*---------- Instrukce pro rizeni toku programu ----------*/
    INST_LABEL, // <label>
    INST_JUMP, // <label>
    INST_JUMPIFEQ, // <label> <symb1> <symb2> 
    INST_JUMPIFNEQ, // <label> <symb1> <symb2>
    INST_JUMPIFEQS,
    INST_JUMPIFNOTEQS, // <label>
    INST_EXIT,// <symb>
/*--------- Ladici instrukce ----------*/
    INST_BREAK,
    INST_DPRINT //<symb>
    }Instructions;
    
/*
 * Funkce, ktera se zavola pri startu interpretace, vytvori hlavicku a vytvori a pushne LC 
 */    
int generate_start();

#endif /* GENERATOR_H */

