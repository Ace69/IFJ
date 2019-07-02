/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro buffer tokenu - pomoci DLL
 */

#ifndef TOKEN_BUFFER_H
#define TOKEN_BUFFER_H

#include "token.h"
#include "scanner.h"


typedef struct buffer_item{
    struct buffer_item *prevPtr;
    struct buffer_item *nextPtr;
    Token token;
}*buffItem;

typedef struct buffer{
    buffItem first;
    buffItem last;
    buffItem actual;
}Buffer;

/**
 * Inicializace bufferu
 */
void bufferInit(Buffer *b);


/**
 * Vlozeni noveho tokenu na konec bufferu  
 */
int bufferInsert(Buffer *b, Token token);


/**
 * Posunuti ukazatele na aktualni prvek na jeho nasledovnika
 */
void moveActual(Buffer *b);

/**
 * Smazani celeho bufferu
 */
void disposeBuffer(Buffer *b);

extern Buffer* tokenBuffer;

#endif /* TOKEN_BUFFER_H */

