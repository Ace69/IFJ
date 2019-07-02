/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Hlavičkový soubor pro string
 */

#ifndef FLEX_STRING_H
#define	FLEX_STRING_H

typedef struct
{
  char* string;		// Hodnota flexibilniho stringu zakoncena znakem '\0'
  int length;		// Delka flexibilniho stringu
  int size;             // Velikost pameti ktera se alokuje pro flexibilni string
} flex_string;

/**
 * Alokace pameti pro flexibilni string *s
 */
int flex_string_init(flex_string *s);

/*
 * Uvolneni flexibilniho stringu *s z pameti
 */
void flex_string_free(flex_string *s);

/**
 * Provede realokaci pameti a prida na konec stringu *s znak c
 */
int flex_string_add(flex_string *s, char c);

/**
 * Vypise hodnotu flexibilniho stringu *s
 */
char *flex_string_get(flex_string *s);

/**
 * Porovna dva flexibilni stringy *s1 a *s2
 */
int flex_string_cmp(flex_string *s1, flex_string *s2);

/**
 * Porovna flexibilni string *s1 a retezeck znaku *s2
 */
int flex_string_cmp_const(flex_string *s1, char *s2);


#endif /* FLEX_STRING_H */