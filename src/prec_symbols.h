/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor obsahující symboly pro precedenční analýzu
 */

#ifndef PREC_SYMBOLS_H
#define PREC_SYMBOLS_H

/* Seznam symbolu v precedencni tabulce
 * tzn. nic jineho nam do zasobniku nemuze prijit
 */
 typedef enum{
     S_MUL,//           *
     S_DIV,//           /
     S_PLUS,//          +
     S_MINUS,//         -
     S_LESS,//          <
     S_LESSEQ,//        <=
     S_GREAT,//         >
     S_GREATEQ,//       >=
     S_EQQ,//           ==
     S_NEQ,//           !=
     S_LBRACKET,//      (
     S_RBRACKET,//      )
     S_IDENTIFIER,//    i
     S_DOLLAR,//        $, (DO, WHILE, EOL) tzn. symboly, ktere ukoncuji precedencni analyzu a predavaji rizeni zpet rekurzivni analyze
     S_NO_TERM //       neterminal E
     }Prec_symbols;

#endif /* PREC_SYMBOLS_H */

