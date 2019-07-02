/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor s definicemi jednotlivých chybových stavů
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "flex_string.h"

/*
typ Tokenu(cele cislo, vyraz, ...)
*/
#define TOKENS 31
typedef enum{
    INTEGER,              // Cele cislo
    FLOAT,                // desetinne cislo
    STRING,               // retezec
    IDENTIFIER,           // identifikator              // klicove slovo
    EQ,                   // =
    EQQ,                  // ==
    GREATER,              // >
    LESSER,               // <
    GEQ,                  // >=
    LEQ,                  // <=
    NEQ,                  // !=
    MUL,                  // *
    DIV,                  // /
    ADD,                  // +
    SUB,                  // -
    LBRACKET,             // (
    RBRACKET,             // )
    BCOMMENT,             // Blokovy komentar
    COMMA,                // ,
    SEMICOLON,            // ;
    EOL,                  // EOL
    EOFF,                 // EOF
    DEF,                  //Klicove slova
    DO,
    ELSE,
    END,
    IF,
    NOT,
    NIL,
    THEN,
    WHILE
}Type;

//Struktura tokenu, ve formatu [type,atribute], pricemz atribute je nepovinny(napr. u operatoru)
typedef struct token {
     Type type;
     int error_value;
     union value{
         int int_value;
         double double_value;
         flex_string string_value;
     }value;
} Token;

#endif /* TOKEN_H */

