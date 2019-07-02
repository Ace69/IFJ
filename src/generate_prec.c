/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace generování instrukcí IFJcode18
 */

#include "generate_prec.h"

void enumerate_posftix(postSElem* elem){
    
    char nil_value[20];
    char numbers[100];
    
    switch(elem->type){
        case(IDENTIFIER):
            if(elem->name != NULL){
                char name[strlen(elem->name)+20];       //20 je priblizna delka retezce "PUSHS LF@"
                sprintf(name,"PUSHS LF@%s", elem->name);
                generate_instruction(name);
            }
            break;

        case(INTEGER):
            if(&elem->integer_value != NULL){
                sprintf(numbers,"PUSHS int@%d", elem->integer_value);
                generate_instruction(numbers);
            }
            break;
        
        case(FLOAT):
            if(&elem->float_value != NULL){
                sprintf(numbers,"PUSHS float@%a", elem->float_value);
                generate_instruction(numbers);
            }
            break;
        
        case(STRING):
            if(elem->string_value != NULL){
                int i = 1;          //Pocitadlo pruchodu puvodnim retezcem
                int j = 0;
                int k = 0;          //Pocitadlo pro naplneni pole nulami
                int length = (int)strlen(elem->string_value);
                char tmp[4];
                char cutted[length*3];
                char output[length*3];
                char string_value[length*3];
                
                while(k < length*3){
                    output[k] = '\0';
                    k++;
                }
                while(elem->string_value[i+1] != '\0'){
                    //Vypis \035 pri zadani '#'
                    if(elem->string_value[i] == '#'){                                   
                        sprintf(tmp,"\\%03d", elem->string_value[i]);
                        strcpy(output,concat(output,tmp));
                        j=j+3;
                    //Vypis \000-\032 pri zadani netisknutelnych znaku    
                    }else if((elem->string_value[i] >= (int)0) && (elem->string_value[i] <= (int)32)){
                        sprintf(tmp,"\\%03d", elem->string_value[i]);
                        strcpy(output,concat(output,tmp));
                        j=j+3;
                    //Prevod \n na \010
                    }else if(elem->string_value[i] == (int)92 && elem->string_value[i+1] == (int)110){
                        sprintf(tmp,"\\%03d", '\n');
                        strcpy(output,concat(output,tmp));
                        j=j+3;
                        i++;
                    //Prevod \s na \032
                    }else if(elem->string_value[i] == (int)92 && elem->string_value[i+1] == (int)115){
                        sprintf(tmp,"\\%03d", ' ');
                        strcpy(output,concat(output,tmp));
                        j=j+3;
                        i++;
                    //Prevod \" na \034
                    }else if(elem->string_value[i] == (int)92 && elem->string_value[i+1] == '"'){
                        sprintf(tmp,"\\%03d", (int)34);
                        strcpy(output,concat(output,tmp));
                        j=j+3;
                        i++;
                    //Prevod \\ na \092
                    }else if(elem->string_value[i] == (int)92 && elem->string_value[i+1] == (int)92){
                        sprintf(tmp,"\\%03d", (int)92);
                        strcpy(output,concat(output,tmp));
                        j=j+3;
                        i++;
                    //Prevod \" na \009
                    }else if(elem->string_value[i] == (int)92 && elem->string_value[i+1] == 't'){
                        sprintf(tmp,"\\%03d", (int)9);
                        strcpy(output,concat(output,tmp));
                        j=j+3;
                        i++;    
                    }else if(elem->string_value[i] == '\\'){                //Prevod \ na \092
                        sprintf(tmp,"\\%03d", elem->string_value[i]);
                        strcpy(output,concat(output,tmp));
                        j=j+3;
                    }else{
                        output[j] = elem->string_value[i];
                    }
                    i++;
                    j++;
                }
                sprintf(string_value,"PUSHS string@%s", output);
                generate_instruction(string_value);                
            }
            break;
            
        case(NIL):
            sprintf(nil_value,"PUSHS nil@nil");
            generate_instruction(nil_value);
            break;
        
        case(ADD):
            generate_add();
            break;
        
        case(SUB):
            generate_sub();
            break;
        
        case(MUL):
            generate_mul();
            break;
        
        case(DIV):
            generate_div();
            break;
            
        case(LESSER):
            generate_lesser();
            break;
            
        case(LEQ):
            generate_leq();
            break;
            
        case(GREATER):
            generate_greater();
            break;
            
        case(GEQ):
            generate_geq();
            break;
            
        case(EQQ):
            generate_eq();
            break;
            
        case(NEQ):
            generate_neq();
            break;
    }
}

void pops_result(){
    generate_instruction("POPS LF@%prec%");
}

void generate_add(){
    char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op1");               //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op2");
    generate_instruction("TYPE LF@%op1%type LF@%op1");  //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");      
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%string", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil")); 
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                  //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("ADDS");
    generate_instruction(concat("JUMP %konec%scitani", labelNum));          //Secteni hodnot
    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                  //Konverze druheho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("ADDS");
    generate_instruction(concat("JUMP %konec%scitani", labelNum));          //Secteni hodnot
    
    
    /*****  Prvni operand je typu string   ******/
    generate_instruction(concat("LABEL %op1%string", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%string%op2%string", labelNum), " LF@%op2%type string@string"));           
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl string
    generate_instruction(concat("LABEL %op1%string%op2%string", labelNum));
    generate_instruction("CONCAT LF@%prec% LF@%op1 LF@%op2");               //Konkatenace stringu
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%scitani", labelNum));
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));
    generate_instruction("EXIT int@4"); 
    
    generate_instruction(concat("LABEL %konec%scitani", labelNum));         //Navesti pro konec scitani
}

void generate_sub(){
    char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op2");               //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op1");
    generate_instruction("TYPE LF@%op1%type LF@%op1");  //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");      
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil")); 
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                  //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("SUBS");
    generate_instruction(concat("JUMP %konec%odcitani", labelNum));          //Odecteni hodnot

    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                  //Konverze druheho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("SUBS");
    generate_instruction(concat("JUMP %konec%odcitani", labelNum));          //Odecteni hodnot
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));
    generate_instruction("EXIT int@4"); 
    
    generate_instruction(concat("LABEL %konec%odcitani", labelNum));         //Navesti pro konec odcitani
}

void generate_mul(){
    char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op1");               //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op2");
    generate_instruction("TYPE LF@%op1%type LF@%op1");  //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");      
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil")); 

    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                  //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("MULS");
    generate_instruction(concat("JUMP %konec%nasobeni", labelNum));          //Nasobeni hodnot
    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                  //Konverze druheho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("MULS");
    generate_instruction(concat("JUMP %konec%nasobeni", labelNum));          //Nasobeni hodnot
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));
    generate_instruction("EXIT int@4"); 
    
    generate_instruction(concat("LABEL %konec%nasobeni", labelNum));         //Navesti pro konec nasobeni
}

void generate_div(){
    char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op2");               //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op1");
    generate_instruction("TYPE LF@%op1%type LF@%op1");  //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");      
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil"));
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));   //Alespon jeden operand byl float         
    generate_instruction("EXIT int@4");                                                                              //preskoc na dalsi blok pro deleni desetinych cisel 
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));           //Celociselne deleni
    generate_instruction(concat(concat("JUMPIFEQ %deleni%0", labelNum), " LF@%op2 int@0"));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("IDIVS");
    generate_instruction(concat("JUMP %konec%deleni", labelNum));          //Deleni hodnot
    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));   //Alespon jeden operand byl float         
    generate_instruction("EXIT int@4");                            //preskoc na dalsi blok pro deleni desetinych cisel 
    
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                      //Konverze kdy prvni operand byl int a druhy float
    generate_instruction(concat("JUMP %op1%float%op2%float", labelNum));    //Preskoceni na deleni floatu
    
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));     //Konverze kdy prvni operand byl float a druhy int
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");
  
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction(concat(concat("JUMPIFEQ %deleni%0", labelNum), " LF@%op2 float@0x0.0p+0"));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("DIVS");
    generate_instruction(concat("JUMP %konec%deleni", labelNum));          //Deleni hodnot
    
    generate_instruction(concat("LABEL %op1%nil", labelNum));           //Nelze delit nilem
    generate_instruction("EXIT int@4"); 
    
    generate_instruction(concat("LABEL %deleni%0", labelNum));            //Osetreni deleni 0
    generate_instruction("EXIT int@9"); 
    
    generate_instruction(concat("LABEL %konec%deleni", labelNum));
}

void generate_lesser(){
    char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op2");               //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op1");
    generate_instruction("TYPE LF@%op1%type LF@%op1");  //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");      
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%string", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil"));
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                  //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("LTS");
    generate_instruction(concat("JUMP %konec%mensi", labelNum));          //Porovnani hodnot
    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                  //Konverze druheho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("LTS");
    generate_instruction(concat("JUMP %konec%mensi", labelNum));          //Porovnani hodnot
    
    /*****  Prvni operand je typu string   ******/
    generate_instruction(concat("LABEL %op1%string", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%string%op2%string", labelNum), " LF@%op2%type string@string"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%string%op2%string", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("LTS");
    generate_instruction(concat("JUMP %konec%mensi", labelNum));          //Porovnani hodnot
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));
    generate_instruction("EXIT int@4"); 
    
    generate_instruction(concat("LABEL %konec%mensi", labelNum));         //Navesti pro konec mensi
}

void generate_leq(){
    char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op2");               //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op1");
    generate_instruction("TYPE LF@%op1%type LF@%op1");  //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");      
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%string", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil"));
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                  //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");                        //Zkontrolovani jestli se hodnoty rovnaji
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat(concat("JUMPIFEQ %konec%mensi%rovno", labelNum), " LF@%prec% bool@true"));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("LTS");                        //Porovnani hodnot
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat("JUMP %konec%mensi%rovno", labelNum));          
    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                  //Konverze druheho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");                        //Zkontrolovani jestli se hodnoty rovnaji
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat(concat("JUMPIFEQ %konec%mensi%rovno", labelNum), " LF@%prec% bool@true"));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("LTS");
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat("JUMP %konec%mensi%rovno", labelNum));   
    
    /*****  Prvni operand je typu string   ******/
    generate_instruction(concat("LABEL %op1%string", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%string%op2%string", labelNum), " LF@%op2%type string@string"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%string%op2%string", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");                        //Zkontrolovani jestli se hodnoty rovnaji
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat(concat("JUMPIFEQ %konec%mensi%rovno", labelNum), " LF@%prec% bool@true"));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("LTS");                                            //Porovnani hodnot
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat("JUMP %konec%mensi%rovno", labelNum));   
    
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));
    generate_instruction("EXIT int@4"); 
    
    generate_instruction(concat("LABEL %konec%mensi%rovno", labelNum));         //Navesti pro konec mensi rovno
    generate_instruction("PUSHS LF@%prec%");
}

void generate_greater(){
    char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op2");               //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op1");
    generate_instruction("TYPE LF@%op1%type LF@%op1");  //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");      
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%string", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil"));
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                  //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("GTS");
    generate_instruction(concat("JUMP %konec%vetsi", labelNum));          //Porovnani hodnot
    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                  //Konverze druheho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("GTS");
    generate_instruction(concat("JUMP %konec%vetsi", labelNum));          //Porovnani hodnot
    
    /*****  Prvni operand je typu string   ******/
    generate_instruction(concat("LABEL %op1%string", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%string%op2%string", labelNum), " LF@%op2%type string@string"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%string%op2%string", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("GTS");
    generate_instruction(concat("JUMP %konec%vetsi", labelNum));          //Porovnani hodnot
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));
    generate_instruction("EXIT int@4"); 
    
    generate_instruction(concat("LABEL %konec%vetsi", labelNum));         //Navesti pro konec vetsi
}

void generate_geq(){
    char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op2");               //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op1");
    generate_instruction("TYPE LF@%op1%type LF@%op1");  //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");      
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%string", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil"));
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                  //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");                        //Zkontrolovani jestli se hodnoty rovnaji
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat(concat("JUMPIFEQ %konec%vetsi%rovno", labelNum), " LF@%prec% bool@true"));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("GTS");                        //Porovnani hodnot
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat("JUMP %konec%vetsi%rovno", labelNum));          
    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                  //Konverze druheho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");                        //Zkontrolovani jestli se hodnoty rovnaji
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat(concat("JUMPIFEQ %konec%vetsi%rovno", labelNum), " LF@%prec% bool@true"));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("GTS");
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat("JUMP %konec%vetsi%rovno", labelNum));   
    
    /*****  Prvni operand je typu string   ******/
    generate_instruction(concat("LABEL %op1%string", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%string%op2%string", labelNum), " LF@%op2%type string@string"));            
    generate_instruction("EXIT int@4");                                     //Druhy operand nebyl integer ani float
    generate_instruction(concat("LABEL %op1%string%op2%string", labelNum));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");                        //Zkontrolovani jestli se hodnoty rovnaji
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat(concat("JUMPIFEQ %konec%vetsi%rovno", labelNum), " LF@%prec% bool@true"));
    generate_instruction("PUSHS LF@%op1");               //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("GTS");                                            //Porovnani hodnot
    generate_instruction("POPS LF@%prec%");
    generate_instruction(concat("JUMP %konec%vetsi%rovno", labelNum));   
    
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));
    generate_instruction("EXIT int@4"); 
    
    generate_instruction(concat("LABEL %konec%vetsi%rovno", labelNum));         //Navesti pro konec mensi rovno
    generate_instruction("PUSHS LF@%prec%");
}

void generate_eq(){
     char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op2");                                       //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op1");

    generate_instruction("TYPE LF@%op1%type LF@%op1 ");                         //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");  
    
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%string", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil")); 
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("MOVE LF@%prec% bool@false");                           //Druhy operand byl string nebo nil -> false
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%rovno", labelNum));
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                          //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");                                      //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");
    generate_instruction(concat("JUMP %konec%rovno", labelNum));                //Porovnani hodnot

    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("MOVE LF@%prec% bool@false");                           //Druhy operand byl string nebo nil -> false
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%rovno", labelNum));
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                          //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");                                      //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");
    generate_instruction(concat("JUMP %konec%rovno", labelNum));                //Porovnani hodnot
    
    
    /*****  Prvni operand je typu string   ******/
    generate_instruction(concat("LABEL %op1%string", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%string%op2%string", labelNum), " LF@%op2%type string@string"));           
    generate_instruction("MOVE LF@%prec% bool@false");                           //Druhy operand nebyl string
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%rovno", labelNum));
    generate_instruction(concat("LABEL %op1%string%op2%string", labelNum));
    generate_instruction("PUSHS LF@%op1");                                      //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");
    generate_instruction(concat("JUMP %konec%rovno", labelNum));                //Porovnani hodnot
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil%op2%nil", labelNum), " LF@%op2%type string@nil"));           
    generate_instruction("MOVE LF@%prec% bool@false");                           //Druhy operand nebyl string
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%rovno", labelNum));
    generate_instruction(concat("LABEL %op1%nil%op2%nil", labelNum));
    generate_instruction("PUSHS LF@%op1");                                      //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");               //Porovnani hodnot
    
    generate_instruction(concat("LABEL %konec%rovno", labelNum));               //Navesti pro konec rovno  
}

void generate_neq(){
     char* labelNum = generate_label("");
    generate_instruction("POPS LF@%op2");                                       //Popnuti hodnot ze zasobniku
    generate_instruction("POPS LF@%op1");

    generate_instruction("TYPE LF@%op1%type LF@%op1 ");                         //Zjisteni jejich typu
    generate_instruction("TYPE LF@%op2%type LF@%op2");  
    
    generate_instruction(concat(concat("JUMPIFEQ %op1%int", labelNum), " LF@%op1%type string@int"));            //Zjisteni typu prvniho operandu
    generate_instruction(concat(concat("JUMPIFEQ %op1%float", labelNum), " LF@%op1%type string@float"));        //A podle toho preskoceni do vetve
    generate_instruction(concat(concat("JUMPIFEQ %op1%string", labelNum), " LF@%op1%type string@string"));      //Kde se budou vykonavat typove kontroly
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil", labelNum), " LF@%op1%type string@nil")); 
    
    /*****  Prvni operand je typu integer   ******/
    generate_instruction(concat("LABEL %op1%int", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%int%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("MOVE LF@%prec% bool@true");                           //Druhy operand byl string nebo nil -> false
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%nerovno", labelNum));
    generate_instruction(concat("LABEL %op1%int%op2%float", labelNum));
    generate_instruction("INT2FLOAT LF@%op1 LF@%op1");                          //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%int%op2%int", labelNum));
    generate_instruction("PUSHS LF@%op1");                                      //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");
    generate_instruction("POPS LF@%prec%");
    generate_instruction("NOT LF@%prec% LF@%prec%");
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%nerovno", labelNum));              //Secteni hodnot

    
    /*****  Prvni operand je typu float   ******/
    generate_instruction(concat("LABEL %op1%float", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%int", labelNum), " LF@%op2%type string@int"));
    generate_instruction(concat(concat("JUMPIFEQ %op1%float%op2%float", labelNum), " LF@%op2%type string@float"));            
    generate_instruction("MOVE LF@%prec% bool@true");                           //Druhy operand byl string nebo nil -> false
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%nerovno", labelNum));
    generate_instruction(concat("LABEL %op1%float%op2%int", labelNum));
    generate_instruction("INT2FLOAT LF@%op2 LF@%op2");                          //Konverze prvniho operandu na float
    generate_instruction(concat("LABEL %op1%float%op2%float", labelNum));
    generate_instruction("PUSHS LF@%op1");                                      //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");
    generate_instruction("POPS LF@%prec%");
    generate_instruction("NOT LF@%prec% LF@%prec%");
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%nerovno", labelNum));              //Nerovno hodnot
    
    
    /*****  Prvni operand je typu string   ******/
    generate_instruction(concat("LABEL %op1%string", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%string%op2%string", labelNum), " LF@%op2%type string@string"));           
    generate_instruction("MOVE LF@%prec% bool@true");                           //Druhy operand nebyl string
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%nerovno", labelNum));
    generate_instruction(concat("LABEL %op1%string%op2%string", labelNum));
    generate_instruction("PUSHS LF@%op1");                                      //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");
    generate_instruction("POPS LF@%prec%");
    generate_instruction("NOT LF@%prec% LF@%prec%"); 
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%nerovno", labelNum));              //Secteni hodnot
    
    /*****  Prvni operand je typu nil   ******/
    generate_instruction(concat("LABEL %op1%nil", labelNum));           
    generate_instruction(concat(concat("JUMPIFEQ %op1%nil%op2%nil", labelNum), " LF@%op2%type string@nil"));           
    generate_instruction("MOVE LF@%prec% bool@true");                           //Druhy operand nebyl string
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%nerovno", labelNum));
    generate_instruction(concat("LABEL %op1%nil%op2%nil", labelNum));
    generate_instruction("PUSHS LF@%op1");                                      //Pushnuti hodnot na zasobnik
    generate_instruction("PUSHS LF@%op2");
    generate_instruction("EQS");
    generate_instruction("POPS LF@%prec%");
    generate_instruction("NOT LF@%prec% LF@%prec%"); 
    generate_instruction("PUSHS LF@%prec%");
    generate_instruction(concat("JUMP %konec%nerovno", labelNum));              //Porovnani hodnot
    
    generate_instruction(concat("LABEL %konec%nerovno", labelNum));             //Navesti pro konec nerovno
}