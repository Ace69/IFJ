/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 *
 * Implementace pro generování kódu v rekurzivním sestupu 
 */

#include "generate_rec.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LABEL_DIGISTS 10


int labelCounter = 0;
FILE *out;
void propagate_output(){
    out = stdout;
}

void generate_instruction(char* instruction){
    //insert(list,instruction);
    fprintf(out,"%s\n",instruction);
}

char* generate_label(char* label){
    char append[MAX_LABEL_DIGISTS];
    sprintf(append,"%d",labelCounter);
    char* returnVal = malloc(strlen(label)+strlen(append)+1);
    strcpy(returnVal,label);
    strcat(returnVal,append);
    labelCounter++;
    return returnVal;
}

char* concat(char *first, char* second){
    char* returnVal = malloc(strlen(first)+strlen(second)+1);
    strcpy(returnVal,first);
    strcat(returnVal,second);
    //free(first);
    //free(second);
    return returnVal;
}


void buildin_print(int params,char *label){
    generate_instruction(concat("JUMP $%END%",label));
    generate_instruction(concat("LABEL ",label));
    generate_instruction("PUSHFRAME");
    generate_instruction("DEFVAR LF@%return%");
    generate_instruction("MOVE LF@%return% nil@nil");
    for (int i = 1; i <= params; i++){
        char stringParam[4];
        sprintf(stringParam,"%d",i);
        generate_instruction(concat("DEFVAR LF@param",stringParam));
        generate_instruction(concat(concat("MOVE LF@param",stringParam),concat(" LF@%",stringParam)));
    }

    for (int j = 1; j <= params; j++){
        char stringParam[4];
        sprintf(stringParam,"%d",j);
        generate_instruction(concat("WRITE LF@param",stringParam));
    }
    
    generate_instruction("MOVE LF@%return% nil@nil");
    generate_instruction("POPFRAME");
    generate_instruction("RETURN");
    generate_instruction(concat("LABEL $%END%",label));
}

void buildin_inputi(){
    generate_instruction("JUMP $%END%inputi");
    generate_instruction("LABEL $inputi");
    generate_instruction("PUSHFRAME");
    generate_instruction("DEFVAR LF@%return%");
   // generate_instruction("MOVE LF@%invari% LF@1");
    generate_instruction("READ LF@%return% int");
    generate_instruction("POPFRAME");
    generate_instruction("RETURN");
    generate_instruction("LABEL $%END%inputi");
}

void buildin_inputf(){
    generate_instruction("JUMP $%END%inputf");
    generate_instruction("LABEL $inputf");
    generate_instruction("PUSHFRAME");
    generate_instruction("DEFVAR LF@%return%");
   // generate_instruction("MOVE LF@%invarf% LF@1");
    generate_instruction("READ LF@%return% float");
    generate_instruction("POPFRAME");
    generate_instruction("RETURN");
    generate_instruction("LABEL $%END%inputf");
}

void buildin_inputs(){
    generate_instruction("JUMP $%END%inputs");
    generate_instruction("LABEL $inputs");
    generate_instruction("PUSHFRAME");
    generate_instruction("DEFVAR LF@%return%");
   // generate_instruction("MOVE LF@%invars% LF@1");
    generate_instruction("READ LF@%return% string");
    generate_instruction("POPFRAME");
    generate_instruction("RETURN");
    generate_instruction("LABEL $%END%inputs");
}
/*

 */
void buildin_ord(){
    generate_instruction("JUMP $%END%ord");
    generate_instruction("LABEL $ord");
    generate_instruction("PUSHFRAME");
    generate_instruction("DEFVAR LF@%return%");
    generate_instruction("MOVE LF@%return% nil@nil");
    generate_instruction("DEFVAR LF@s");
    generate_instruction("MOVE LF@s LF@%1");
    generate_instruction("DEFVAR LF@i");
    generate_instruction("MOVE LF@i LF@%2");
    
    generate_instruction("DEFVAR LF@cond");
    generate_instruction("DEFVAR LF@delka");
    generate_instruction("STRLEN LF@delka LF@s");
    generate_instruction("LT LF@cond LF@i LF@delka");
    generate_instruction("NOT LF@cond LF@cond");
    generate_instruction("JUMPIFEQ $ord1 LF@cond bool@true");
    generate_instruction("DEFVAR LF@hodnota");
    generate_instruction("STRI2INT LF@hodnota LF@s LF@i");
    generate_instruction("MOVE LF@%return% LF@hodnota");
    generate_instruction("JUMP $ord2");
    generate_instruction("LABEL $ord1");
    generate_instruction("LABEL $ord2");
    generate_instruction("POPFRAME");
    generate_instruction("RETURN");
    generate_instruction("LABEL $%END%ord");
}

void buildin_length(){
    generate_instruction("JUMP $%END%length");
    generate_instruction("LABEL $length");
    generate_instruction("PUSHFRAME");
    generate_instruction("DEFVAR LF@%return%");
    generate_instruction("DEFVAR LF@%s%"); 
    generate_instruction("DEFVAR LF@%var%"); 
    generate_instruction("MOVE LF@%s% LF@%1");
    generate_instruction("STRLEN LF@%var% LF@%s%");
    generate_instruction("MOVE LF@%return% LF@%var%");
    generate_instruction("POPFRAME");
    generate_instruction("RETURN");
    generate_instruction("LABEL $%END%length");
    
}

void buildin_chr(){
    generate_instruction("JUMP $%END%chr");
    generate_instruction("LABEL $chr");
    generate_instruction("PUSHFRAME");
    generate_instruction("DEFVAR LF@%return%");
    generate_instruction("MOVE LF@%return% nil@nil");
    generate_instruction("DEFVAR LF@i");
    generate_instruction("MOVE LF@i LF@%1");
    generate_instruction("DEFVAR LF@vetsi");
    generate_instruction("GT LF@vetsi LF@i int@-1");
    generate_instruction("DEFVAR LF@mensi");
    generate_instruction("LT LF@mensi LF@i int@256");
    generate_instruction("DEFVAR LF@cond");
    generate_instruction("AND LF@cond LF@vetsi LF@mensi");
    generate_instruction("NOT LF@cond LF@cond");
    generate_instruction("JUMPIFEQ $chr1 LF@cond bool@true");
    generate_instruction("INT2CHAR LF@%return% LF@i");
    generate_instruction("JUMP $chr2");
    generate_instruction("LABEL $chr1");
    generate_instruction("EXIT int@4");
    generate_instruction("LABEL $chr2");
    generate_instruction("POPFRAME");
    generate_instruction("RETURN");
    generate_instruction("LABEL $%END%chr");  
}

void buildin_function(){
    buildin_inputi();
    buildin_inputf();
    buildin_inputs();
    buildin_ord();
    buildin_chr();
    buildin_length();
}

void generate_header(){
    generate_instruction("DEFVAR LF@%op1");
    generate_instruction("DEFVAR LF@%op2");
    generate_instruction("DEFVAR LF@%op1%type");
    generate_instruction("DEFVAR LF@%op2%type");
    generate_instruction("DEFVAR LF@%prec%");
}

