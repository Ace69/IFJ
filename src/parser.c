/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace pro parser
 */
#include "parser.h"

/* ------------------------------- Globalni promenne  ------------------------------ */
//Urcuje 1 nebo 2 pruchod syn. analyzy
bool firstPass = true;
//Prave prochazeny token
Token token;
//Buffer pro tokeny, pouziva se v 2. pruchodu
Buffer* tokenBuffer;
//Print Flag
bool printFlag = false;
//Urcuje jestli se nachazime v hlavnim tele nebo v IF,DEF,WHILE 
bool mainBody = true;
//Tabulka symbolu
tBSTNodePtr* symbolTable;
//Ukazatel na pod tabulku symbolu funkce, pokud neni ve funkci = NULL
tBSTNodePtr* fceSymbolTable = NULL;
//Nazev posledni funkce pri definici
char* lastFceName;
//Nazev posledni promenne nebo funkce pri volani
char* lastVarName;
//Pomocne pole pro test vypis ze souboru
char* tokenArr[TOKENS] = {"INTEGER","FLOAT","STRING","IDENTIFIER","EQ","EQQ","GREATER","LESSER"
,"GEQ","LEQ","NEQ","MUL","DIV","ADD","SUB","LBRACKET","RBRACKET","BCOMMENT"
,"COMMA","SEMICOLON","EOL","EOFF","DEF","DO","ELSE","END","IF","NOT","NIL","THEN","WHILE"};
//pocet parametru volanne funkce(posledni)
int lastParamsCount = -1;
//pocet zanoreni cyklu a podminek
int nestCounter = 0;
//do ktere promenne se ma hodnota priradit, na urovni generovani kodu
char* assign_to = NULL;
//navratova hodnota funkce( -1 -> nil, 0 -> %return%, 1 -> %prec%)
int from_prec = -1;
/* ------------------------------- Globalni makra  ------------------------------- */
#define PREPARE_BUFFER tokenBuffer->actual = tokenBuffer->first;

#define PRINT_TOKEN(ATTR)\
    if(printFlag){ \
        printf("[TOKEN] > %s \n", ATTR); \
    }

#define PRINT(ATTR) \
    if(printFlag){ \
        printf(ATTR); \
    }

#define PRINT_SYMTABLE \
    if(printFlag){ \
        printf("\n -------- Vypis Tabulky Symbolu -------- \n"); \
        print_pre_order(symbolTable); \
        printf("-------- Konec vypisu Tabulky Symbolu -------- \n"); \
    }

#define PRINT_FCE_SYMTABLE  \
    if(printFlag){ \
        printf("\n -------- Vypis PODTabulky Symbolu -------- \n"); \
        print_pre_order(fceSymbolTable); \
        printf("-------- Konec vypisu Tabulky Symbolu -------- \n"); \
    }

#define INI_FCE_SYMTABLE \
    fceSymbolTable =  malloc(sizeof(tBSTNodePtr)); \
    BSTInit(fceSymbolTable);

#define INC_PARAMS \
    if(!firstPass){ \
        lastParamsCount++; \
    }

#define ONLY_SEC_PASS(ATTR) \
    if(!firstPass){ \
        ATTR \
    }
#define CHECK_NAME \
    if(!firstPass){ \
        if(!check_name()){ \
            fprintf(stderr,"V promenne je znak ? nebo !"); \
            exit(SEMANTIC_ERR); \
        }  \
    }
#define FCE 1
#define EXPR 0
#define MAX_DIGITS_PARAMS 4
#define MAX_INT_DIGITS 10
#define MAX_FLOAT_DIGITS 23

/* ------------------------------- Pomocne funkce  ------------------------------- */
Token read_next(){
   Token token = tokenBuffer->actual->token;
   if(tokenBuffer->actual->nextPtr != NULL){
       //Nejsme na konci bufferu
       moveActual(tokenBuffer);
   }
   return token;
}

bool check_name(){
    TData data; 
    char* key; 
    key = flex_string_get(&token.value.string_value);
    if(strchr(key,'?')!= NULL || strchr(key,'!')!= NULL){
        if(BSTSearch((*symbolTable), key,&data) == TRUE){
            if(data.s_type == FUNCTION){
                return true;
            } 
        }else{
            return false;
        }
    }else{
        return true;
    }
}
char* generate_lbl(char* label){
    if(!firstPass){
       return generate_label(label);
    }else{
        return NULL;
    }
}

Token get_token_checked(){
    Token token;
    if(firstPass){
        token = get_token();
        if(token.error_value != 0){
            fprintf(stderr,"Chyba lexikalni analyzy");
            //[EXIT]
            exit(token.error_value);
        }else{
            bufferInsert(tokenBuffer,token);
        }
    }else{
        token = read_next();
    }
    return token;
    
}

void printf_buffer(){
    if(printFlag){
        PRINT("\nVypisuju buffer\n")
        tokenBuffer->actual = tokenBuffer->first;
        for(int i = 0;tokenBuffer->actual != NULL;i++){
            printf("[TOKEN][%d] > %s\n",i,tokenArr[tokenBuffer->actual->token.type]);
            tokenBuffer->actual = tokenBuffer->actual->nextPtr;
        }
        PRINT("\n")
    }
}

bool search_def_genarate(){
    TData data;
    char* key;
    if(token.type == EQ){
        key = lastVarName;
    }else{
        key = flex_string_get(&token.value.string_value);
    }
    if(fceSymbolTable != NULL){
        BSTSearch((*fceSymbolTable), key,&data);
    }else{
        BSTSearch((*symbolTable), key, &data);
    }
    if(data.definedGenerate == true){
        return true;
    }else{
        return false;
    }
}
void insert_def_genarate(){
    TData data;
    char* key;
    if(token.type == EQ){
        key = lastVarName;
    }else{
        key = flex_string_get(&token.value.string_value);
    }
    if(fceSymbolTable != NULL){
        BSTSearch((*fceSymbolTable), key,&data);
        data.definedGenerate = true;
        BSTInsert(symbolTable,key,data);
    }else{
        BSTSearch((*symbolTable), key,&data);
        data.definedGenerate = true;
        BSTInsert(symbolTable,key,data);
    }
}

bool searchFce_symtable(){
    TData data;
    char* key;
    key = flex_string_get(&token.value.string_value);
    
    if(BSTSearch((*symbolTable), key,&data) == TRUE){
        if(data.s_type == FUNCTION){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void insert_buildin_fce(char* name,int params){
    TData data;
    data.defined = true;
    data.definedGenerate = false;
    data.tBSTNodePtr = malloc(sizeof(tBSTNodePtr));
    BSTInit(&data.tBSTNodePtr);
    //TODO Prodiskutovat, vlozeni roota
    TData test;
    test.defined = true;
    BSTInsert(&(data.tBSTNodePtr),"%root%", test);
    data.s_type = FUNCTION;
    data.params = params;
    BSTInsert(symbolTable,name,data);
}

void change_params(char* name,int newParams){
    TData data;
    char* key = name;
    /*if(token.type == EQ){
        key = lastVarName;
    }else{
        key = flex_string_get(&token.value.string_value);
    }*/
    BSTSearch((*symbolTable), key,&data);
    data.params = newParams;
    BSTInsert(symbolTable,name,data);
}

void insert_all_buildin(){
    insert_buildin_fce("print",1);
    insert_buildin_fce("ord",2);
    insert_buildin_fce("chr",1);
    insert_buildin_fce("inputi",0);
    insert_buildin_fce("inputf",0);
    insert_buildin_fce("inputs",0);
    insert_buildin_fce("length",1);
}

int insert_symtable(SymbolType type,bool def){
    bool searched;

    TData data;
    //TODO predefinovani promenne, mozna jen otestovat
    char* key;
    bool inFce;
    if(token.type == EQ){
        key = lastVarName;
    }else{
        key = flex_string_get(&token.value.string_value);
    }
    if(firstPass && fceSymbolTable != NULL){
        if(BSTSearch((*fceSymbolTable), key,&data) == TRUE){
            searched = true;
        }else{
            searched = false;
        }
        inFce = true;
    }else{
        if(BSTSearch((*symbolTable), key,&data) == TRUE){
            searched = true;
        }else{
            searched = false;
        }
        inFce = false;
    }
    data.definedGenerate = false;
    if(searched){
        if(data.defined){
            //uz je definovan
            if(def && type == FUNCTION){
                //je volan s definici
                //[EXIT]
                fprintf(stderr,"2x Definovana funkce");
                exit(UNDEF_ID);
            }else if(!def && data.s_type == FUNCTION){
                return FCE;
            }
            return EXPR;
        }else{ 
            if(def){ 
                data.defined = true;
                if(type == FUNCTION){
                    data.tBSTNodePtr = malloc(sizeof(tBSTNodePtr));
                    data.s_type = type;
                    BSTInit(&data.tBSTNodePtr);
                    //TODO Prodiskutovat, vlozeni roota
                    TData test;
                    test.defined = true;
                    BSTInsert(&(data.tBSTNodePtr),"%root%", test);
                    INI_FCE_SYMTABLE;
                    (*fceSymbolTable) = data.tBSTNodePtr;
                    data.params = 0;
                }
            } 
        }
    }else{
        data.s_type = type;
        if(def){
            data.defined = true;
            if(data.s_type == FUNCTION){
                data.tBSTNodePtr = malloc(sizeof(tBSTNodePtr));
                BSTInit(&data.tBSTNodePtr);
                //TODO Prodiskutovat, vlozeni roota
                TData test;
                test.defined = true;
                BSTInsert(&(data.tBSTNodePtr),"%root%", test);
                INI_FCE_SYMTABLE;
                (*fceSymbolTable) = data.tBSTNodePtr;
                data.params = 0;
            }
        }else{
            data.defined = false;
        }
    }
    //nakonec insertneme
    if(firstPass){
        if(inFce){
            if(data.s_type != FUNCTION){
                BSTInsert(fceSymbolTable,key,data);
            }
        }else{
            BSTInsert(symbolTable,key,data);
        }
    }
    PRINT_SYMTABLE;
    return EXPR;
}

bool check_params(){
    if(!firstPass){
        TData data;
        BSTSearch((*symbolTable), lastVarName,&data);
        if(lastParamsCount == data.params){
            //lastParamsCount = -1;
            return true;
        }else{
            //lastParamsCount = -1;
            return false;
        }
    }else{
        return true;
    }
}

void include_param(){
    TData data;
    BSTSearch((*symbolTable), lastFceName,&data);
    data.params++;
    
    TData tmp;  
    tmp.s_type = VAR;
    tmp.defined = true;
    BSTInsert(&(data.tBSTNodePtr),flex_string_get(&token.value.string_value),tmp);
    
    BSTInsert(symbolTable,lastFceName,data);
}

/* ------------------------------- Funkce pravidel rekurzivniho sestupu ------------------------------- */

/*
 * [1] = Pravidlo tvaru NETERMINAL -> TERMINAL
 * [2] = Pravidlo tvaru NETERMINAL -> TERMINAL NETERMINAL ...
 * [3] = Pravidlo tvaru NETERMINAL -> NETERMINAL NETERMINAL ...
 * [4] = Pravidlo tvaru NETERMINAL -> TERMINAL NETERMINAL TERMINAL...
 * [5] = Pravidlo tvaru NETERMINAL -> EPSILON
 * [6] = OSTATNI
 */
bool prog(){
    //EOL
    prog_v = false;
    if(token.type == EOL){
        //[2]PROG -> eol PROG
        PRINT_TOKEN("EOL");
        token = get_token_checked();
        prog_v = prog();
        return prog_v;
    }
    //EOF
    else if(token.type == EOFF){
        //[1]PROG -> eof
        PRINT_TOKEN("EOF");
        prog_v = true;
        token = get_token_checked();
        return prog_v;	
    }
    //bcomment
    else if(token.type == BCOMMENT){
        //[2]PROG -> bcomment PROG
        PRINT_TOKEN("BCOMMENT");
        token = get_token_checked();
        prog_v = prog();
        return prog_v;	
    }
    //sec
    else if(token.type == IDENTIFIER || token.type == IF || token.type == DEF || token.type == WHILE || 
            token.type == LBRACKET || token.type == INTEGER || token.type == FLOAT || token.type == STRING || token.type == NIL){
        //[3]PROG -> SEC END_ROW PROG
        prog_v = sec() && end_row() && prog();
        return prog_v;		
    }
    //Epsilon
    else if((token.type == ELSE || token.type == END) && mainBody==false){
        //[5]PROG -> ''
        PRINT_TOKEN("EPSILON");
        prog_v = true;
        return prog_v;
    }
    //chyba
    return false;
}

bool end_row(){
    end_row_v = false;
    //EOL
    if(token.type == EOL){
        //[1]END_ROW -> eol
        PRINT_TOKEN("EOL");
        end_row_v = true;
        token = get_token_checked();
        return end_row_v;	
    }
    //EOF
    else if(token.type == EOFF){
        //[1]END_ROW -> eof
        PRINT_TOKEN("EOF");
        end_row_v = true;
        token = get_token_checked();
        return end_row_v;	
    }
    return false; 
}

bool sec(){
    sec_v = false;
    //identifier
    if(token.type == IDENTIFIER){
        //[2]SEC -> identifier eq FCE_EXPR
        PRINT_TOKEN("IDENTIFIER");
        CHECK_NAME;
        if(!firstPass){
            if(!searchFce_symtable()){
                assign_to = flex_string_get(&token.value.string_value);
                if(!search_def_genarate()){
                    ONLY_SEC_PASS(generate_instruction(concat("DEFVAR LF@",assign_to));)
                }
            }
        }
        if(firstPass){
            //TODO pridani radku s lastVarName
            lastVarName = flex_string_get(&token.value.string_value);
            sec_v = fce();
            if(token.type != EQ){
                return sec_v;
            }
        }else{
            lastVarName = flex_string_get(&token.value.string_value);
            if(searchFce_symtable()){
                sec_v = fce();
                return sec_v;
            }
            token = get_token_checked();
        }
        //token = get_token_checked();
        
        if(token.type == EQ){
            PRINT_TOKEN("EQ");
            
            if(firstPass){
                insert_symtable(VAR,true);
            }else{
                if(!search_def_genarate()){
                    ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE LF@",assign_to)," nil@nil"));)
                    insert_def_genarate();
                }
            }
            token = get_token_checked();
            sec_v = fce_expr();
            assign_to = NULL;
            return sec_v;
        }else if((token.type == EOL || token.type == MUL || token.type == ADD || token.type == SUB || token.type == DIV)){
            if(!firstPass){
                tokenBuffer->actual = tokenBuffer->actual->prevPtr;
            }
            
            sec_v = expr();
            return sec_v;
        }
    }
    else if(token.type == LBRACKET){
        //PRINT_TOKEN("VALUE");
        sec_v = expr();
        return sec_v;
    }
    //identifier
    else if(token.type == INTEGER || token.type == FLOAT || token.type == STRING || token.type == NIL){
        //PRINT_TOKEN("VALUE");
        token = get_token_checked();
        if((token.type == EOL || token.type == MUL || token.type == ADD || token.type == SUB || token.type == DIV)){
            if(!firstPass){
                tokenBuffer->actual = tokenBuffer->actual->prevPtr;
            }
            sec_v = expr();
            return sec_v;
        }
    }
    
    //while
    else if(token.type == WHILE){
        //[4]SEC -> while EXPR do eol PROG end
        PRINT_TOKEN("WHILE");
        if(!firstPass){
            char *assign_to_hold = assign_to;
            char* lastVarNameHold = lastVarName;
            buffItem returnPlace = tokenBuffer->actual;
            while(token.type != END){
                if(token.type == IDENTIFIER){
                    lastVarName = flex_string_get(&token.value.string_value);
                    token = get_token_checked();
                    if(token.type == EQ){
                        assign_to = lastVarName;
                        if(!search_def_genarate()){
                            ONLY_SEC_PASS(generate_instruction(concat("DEFVAR LF@",lastVarName));)
                            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE LF@",assign_to)," nil@nil"));)
                            insert_def_genarate();        
                        }
                    }
                }
                token = get_token_checked();
            }
            tokenBuffer->actual = returnPlace->prevPtr;
            token = get_token_checked();
            assign_to = assign_to_hold;
            lastVarName = lastVarNameHold;
        }
        char* label = generate_lbl("$cyklus");
        ONLY_SEC_PASS(generate_instruction(concat("LABEL ",label));)
        nestCounter++;
        mainBody=false;
        token = get_token_checked();
        if(expr()){
            if(token.type == DO){
                PRINT_TOKEN("DO");
                char* cond_while = generate_lbl("$isbool");
                ONLY_SEC_PASS(generate_instruction("TYPE LF@%op1%type LF@%prec%");)
                ONLY_SEC_PASS(generate_instruction(concat(concat("JUMPIFEQ ", cond_while)," LF@%op1%type string@bool"));)
                ONLY_SEC_PASS(generate_instruction("EQ LF@%prec% LF@%op1%type string@nil");)
                ONLY_SEC_PASS(generate_instruction("NOT LF@%prec% LF@%prec%");)
                ONLY_SEC_PASS(generate_instruction(concat("LABEL ",cond_while));)
                ONLY_SEC_PASS(generate_instruction("NOT LF@%prec% LF@%prec%");)
                char* label2 = generate_lbl("$cyklusEnd");
                ONLY_SEC_PASS(generate_instruction(concat(concat("JUMPIFEQ ", label2)," LF@%prec% bool@true"));)
                token = get_token_checked();
                if(token.type == EOL){
                    PRINT_TOKEN("EOL");
                    
                    token = get_token_checked();
                    if(prog()){
                        if(token.type == END){
                            PRINT_TOKEN("END");
                            ONLY_SEC_PASS(generate_instruction(concat("JUMP ",label));)
                            ONLY_SEC_PASS(generate_instruction(concat("LABEL ",label2));)       
                            nestCounter--;
                            if(nestCounter == 0){
                                mainBody = true; 
                            }
                            sec_v = true;
                            token = get_token_checked();
                            return sec_v;
                        }
                    }
                }
            }
        }
    }
    //if
    else if(token.type == IF){
        //[4]SEC -> if EXPR then eol PROG else eol PROG end
        PRINT_TOKEN("IF");
        nestCounter++;
        mainBody = false;
        token = get_token_checked();
        if(expr()){
            if(token.type == THEN){
                PRINT_TOKEN("THEN");
                char* cond_if = generate_lbl("$isbool");
                ONLY_SEC_PASS(generate_instruction("TYPE LF@%op1%type LF@%prec%");)
                ONLY_SEC_PASS(generate_instruction(concat(concat("JUMPIFEQ ", cond_if)," LF@%op1%type string@bool"));)
                ONLY_SEC_PASS(generate_instruction("EQ LF@%prec% LF@%op1%type string@nil");)
                ONLY_SEC_PASS(generate_instruction("NOT LF@%prec% LF@%prec%");)
                ONLY_SEC_PASS(generate_instruction(concat("LABEL ",cond_if));)
                ONLY_SEC_PASS(generate_instruction("NOT LF@%prec% LF@%prec%");)
                char *label = generate_lbl("$ifjump");
                ONLY_SEC_PASS(generate_instruction(concat(concat("JUMPIFEQ ", label)," LF@%prec% bool@true"));)
                token = get_token_checked();
                if(token.type == EOL){
                    PRINT_TOKEN("EOL");
                    token = get_token_checked();
                    if(prog()){
                        char* label2 = generate_lbl("$ifjump");
                        ONLY_SEC_PASS(generate_instruction(concat("JUMP ",label2));)
                        if(token.type == ELSE){
                            PRINT_TOKEN("ELSE");
                            ONLY_SEC_PASS(generate_instruction(concat("LABEL ",label));)
                            token = get_token_checked();
                            if(token.type == EOL){
                                PRINT_TOKEN("EOL");
                                token = get_token_checked();
                                if(prog()){
                                    if(token.type == END){
                                        PRINT_TOKEN("END");
                                        ONLY_SEC_PASS(generate_instruction(concat("LABEL ",label2));)
                                        nestCounter--;
                                        if(nestCounter == 0){
                                            mainBody = true;
                                        }
                                        sec_v = true;
                                        token = get_token_checked();
                                        return sec_v;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //def
    else if(token.type == DEF){
        //[5]SEC -> def identifier lbracket PARAMS rbracket eol PROG end
        PRINT_TOKEN("DEF");
        nestCounter++;
        mainBody = false;
        token = get_token_checked();
        if(token.type == IDENTIFIER){
            PRINT_TOKEN("IDENTIFIER");
            lastFceName = flex_string_get(&token.value.string_value);
            ONLY_SEC_PASS(generate_instruction(concat("JUMP $%END%",lastFceName));)
            ONLY_SEC_PASS(generate_instruction(concat("LABEL $",lastFceName));)
            ONLY_SEC_PASS(generate_instruction("PUSHFRAME");)
            ONLY_SEC_PASS(generate_header();)
            ONLY_SEC_PASS(generate_instruction("DEFVAR LF@%return%");)
            ONLY_SEC_PASS(generate_instruction("MOVE LF@%return% nil@nil");)
            if(firstPass){
                insert_symtable(FUNCTION,true);
            }else{
                TData *tmp;
                tmp = malloc(sizeof(TData));
                BSTSearch((*symbolTable), lastFceName, tmp);
                fceSymbolTable = &tmp->tBSTNodePtr;
            }
            from_prec = -1;
            token = get_token_checked();
            if(token.type == LBRACKET){
                PRINT_TOKEN("LBRACKET");
                token = get_token_checked();
                if(params()){
                    if(token.type == RBRACKET){
                        PRINT_TOKEN("RBRACKET");
                        token = get_token_checked();
                        if(token.type == EOL){
                            PRINT_TOKEN("EOL");
                            token = get_token_checked();
                            if(prog()){
                                if(token.type == END){
                                    PRINT_TOKEN("END");
                                    nestCounter--;
                                    if(from_prec == -1){
                                        ONLY_SEC_PASS(generate_instruction("MOVE LF@%return% nil@nil");)
                                    }else if(from_prec == 0){
                                        ONLY_SEC_PASS(generate_instruction("MOVE LF@%return% TF@%return%");)
                                    }else if(from_prec == 1){
                                        ONLY_SEC_PASS(generate_instruction("MOVE LF@%return% LF@%prec%");)
                                    }
                                    ONLY_SEC_PASS(generate_instruction("POPFRAME");)
                                    ONLY_SEC_PASS(generate_instruction("RETURN");)
                                    ONLY_SEC_PASS(generate_instruction(concat("LABEL $%END%",lastFceName));)
                                    mainBody = true;
                                    if(firstPass){
                                        PRINT_FCE_SYMTABLE;
                                    }
                                    fceSymbolTable = NULL;
                                    sec_v = true;
                                    token = get_token_checked();
                                    return sec_v;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool params(){
    params_v = false;
    //identifier
    if(token.type == IDENTIFIER){
        //[2]PARAMS -> identifier PARAM_LIST
        PRINT_TOKEN("IDENTIFIER");
        
        char* parameterName = flex_string_get(&token.value.string_value);
        ONLY_SEC_PASS(generate_instruction(concat("DEFVAR LF@",parameterName));)
        ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE LF@",parameterName),concat(" LF@%","1")));)
        
        if(firstPass){
            include_param();
        }
        token = get_token_checked();
        int paramsCounter = 1;
        params_v = param_list(paramsCounter);
        return params_v;
    }
    //rbracket
    else if(token.type == RBRACKET){
        //[5]PARAMS ->  ''
        PRINT_TOKEN("EPSILON");
        params_v = true;
        return params_v;
    }
    return false;
}

bool param_list(int paramsCount){
    param_list_v = false;
    //comma
    if(token.type == COMMA){
       //[2] PARAM_LIST -> comma identifier PARAM_LIST
       PRINT_TOKEN("COMMA");
       token = get_token_checked();
       if(token.type == IDENTIFIER){
            PRINT_TOKEN("IDENTIFIER");
            paramsCount++;
            char* parameterName = flex_string_get(&token.value.string_value);
            char paramsCountString[MAX_DIGITS_PARAMS];
            sprintf(paramsCountString,"%d",paramsCount);
            ONLY_SEC_PASS(generate_instruction(concat("DEFVAR LF@",parameterName));)
            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE LF@",parameterName),concat(" LF@%", paramsCountString)));)
            
            if(firstPass){
                include_param();
            }
            token = get_token_checked();
            param_list_v = param_list(paramsCount);
            return param_list_v;
       }
    }
    else if(token.type == RBRACKET){
        //[5]PARAM_LIST ->  ''
        PRINT_TOKEN("EPSILON");
        param_list_v = true;
        return param_list_v;
    }
    return false;
}

bool call_params(){
    call_params_v = false;
    if(!firstPass){
        lastParamsCount = 0;
    }
    //Epsilon EOL, EOF, RBRACKET
    if(firstPass){
        if(fceSymbolTable != NULL){
            BSTDelete((*fceSymbolTable),lastVarName);
        }
    }
    if(token.type == EOL || token.type == EOFF || token.type == RBRACKET){
        // -> ''
        PRINT_TOKEN("EPSILON");
        if(!firstPass && strcmp("print",lastVarName) == 0){
            change_params("print",lastParamsCount);
        }
        call_params_v = true;
        return call_params_v;
    }   
    //
    else if(token.type == IDENTIFIER || token.type == INTEGER || token.type == FLOAT || token.type == STRING || token.type == NIL){
        //[3]CALL_PARAMS -> ITEM CALL_PARAM_LIST
        if(token.type == IDENTIFIER){
            ONLY_SEC_PASS(generate_instruction("DEFVAR TF@%1");)
            ONLY_SEC_PASS(generate_instruction(concat("MOVE TF@%1 LF@",flex_string_get(&token.value.string_value)));)
        }else if(token.type == INTEGER){
            ONLY_SEC_PASS(generate_instruction("DEFVAR TF@%1");)
            char str[MAX_INT_DIGITS];
            sprintf(str, "%d", token.value.int_value);
            ONLY_SEC_PASS(generate_instruction(concat("MOVE TF@%1 int@",str));)
        }else if(token.type == FLOAT){
            ONLY_SEC_PASS(generate_instruction("DEFVAR TF@%1");)
            char str[MAX_FLOAT_DIGITS];
            sprintf(str, "%a", token.value.double_value);
            ONLY_SEC_PASS(generate_instruction(concat("MOVE TF@%1 float@",str));)
        }else if(token.type == STRING){
            ONLY_SEC_PASS(generate_instruction("DEFVAR TF@%1");)
            ONLY_SEC_PASS(generate_instruction(concat("MOVE TF@%1 string@",flex_string_get(&token.value.string_value)));)
        }else if(token.type == NIL){
            ONLY_SEC_PASS(generate_instruction("DEFVAR TF@%1");)
            ONLY_SEC_PASS(generate_instruction("MOVE TF@%1 nil@nil");)
        }
        int paramCounter = 1;
        call_params_v = item() && call_param_list(paramCounter);
        return call_params_v;		
    }
    return false;    
}

bool call_param_list(int paramsCount){
    call_param_list_v = false;
    //Epsilon EOL, EOF, RBRACKET
    if(token.type == EOL || token.type == EOFF || token.type == RBRACKET){
        // -> ''
        PRINT_TOKEN("EPSILON");
        if(!firstPass && strcmp("print",lastVarName) == 0){
            change_params("print",lastParamsCount);
        }
        call_param_list_v = true;
        return call_param_list_v;
    }
    
    //comma
    else if(token.type == COMMA){
        //[2] CALL_PARAM_LIST -> comma ITEM CALL_PARAM_LIST
        PRINT_TOKEN("COMMA");
        token = get_token_checked();
        /*
        if(firstPass){
            include_param();
        }
        */
        paramsCount++;
        char paramsCountString[MAX_DIGITS_PARAMS];
        sprintf(paramsCountString,"%d",paramsCount);
        if(token.type == IDENTIFIER){
            char* parameterName = flex_string_get(&token.value.string_value);
            ONLY_SEC_PASS(generate_instruction(concat("DEFVAR TF@%",paramsCountString));)
            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE TF@%",paramsCountString),concat(" LF@",parameterName)));)
        }else if(token.type == INTEGER){
            ONLY_SEC_PASS(generate_instruction(concat("DEFVAR TF@%",paramsCountString));)
            char str[MAX_INT_DIGITS];
            sprintf(str, "%d", token.value.int_value);
            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE TF@%",paramsCountString),concat(" int@",str)));)
        }else if(token.type == FLOAT){
            ONLY_SEC_PASS(generate_instruction(concat("DEFVAR TF@%",paramsCountString));)
            char str[MAX_FLOAT_DIGITS];
            sprintf(str, "%a", token.value.double_value);
            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE TF@%",paramsCountString),concat(" float@",str)));)
        }else if(token.type == STRING){
            char* parameterName = flex_string_get(&token.value.string_value);
            ONLY_SEC_PASS(generate_instruction(concat("DEFVAR TF@%",paramsCountString));)
            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE TF@%",paramsCountString),concat(" string@",parameterName)));)
        }else if(token.type == NIL){
            ONLY_SEC_PASS(generate_instruction(concat("DEFVAR TF@%",paramsCountString));)
            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE TF@%",paramsCountString)," nil@nil"));)
        }
        call_param_list_v = item() & call_param_list(paramsCount);
        return call_param_list_v;        
    }  
    return false;
}

bool fce_expr(){
    if(token.type == IDENTIFIER){
        //[3] FCE_EXPR -> FCE
        lastVarName = flex_string_get(&token.value.string_value);
        if(!firstPass){
            //TODO
            if(!searchFce_symtable()){
                fce_expr_v = expr();
                return fce_expr_v;
            }
        }
        fce_expr_v = fce();
        return fce_expr_v;
    }else if((token.type == INTEGER) || (token.type == FLOAT) || (token.type == STRING) || (token.type == NIL) || (token.type == LBRACKET)){
        PRINT_TOKEN("EXPR")
        expr();
        return true;
    }
}

bool br_or_not(){
    br_or_not_v = false;
    if(token.type == EOL || token.type == EOFF || token.type == IDENTIFIER || token.type == INTEGER || token.type == FLOAT || token.type == STRING || token.type == NIL){
        //[3]PROG -> SEC END_ROW PROG
        br_or_not_v = call_params();
        if(!check_params(lastParamsCount)){
            //Pocet parametru se lisi
            fprintf(stderr,"Chybny pocet parametru");
            exit(ARG_COUNT_ERR);
        }
        return br_or_not_v;		
    }
    else if(token.type == LBRACKET){
        //[5]BR_OR_NOT -> lbracket CALL_PARAMS rbracket
        PRINT_TOKEN("LBRACKET");
        
        token = get_token_checked(); 
        if(call_params()){
            if(token.type == RBRACKET){
                PRINT_TOKEN("RBRACKET");
                br_or_not_v = true;
                token = get_token_checked();
                if(!check_params(lastParamsCount)){
                    //Pocet parametru se lisi
                    fprintf(stderr,"Chybny pocet parametru");
                    exit(ARG_COUNT_ERR);
                }
                return br_or_not_v;
            }
        }
    }
    else if((token.type == MUL || token.type == ADD || token.type == SUB || token.type == DIV)){
        //Za identifikatorem je operator -> jedna se o vyraaz
        br_or_not_v = expr();
        return br_or_not_v;
    }
    else if(token.type == EQ){
        return true;
    }
    return false;
}

bool fce(){
    fce_v = false;
    if(token.type == IDENTIFIER){
        ONLY_SEC_PASS(generate_instruction("CREATEFRAME");)
        //[2]FCE -> identifier BR_OR_NOT
        PRINT_TOKEN("IDENTIFIER");
        if(firstPass){
            insert_symtable(VAR,false);
        }
        token = get_token_checked();
        fce_v = br_or_not();
        if(!firstPass){
            if(strcmp(lastVarName,"print") == 0){
                char* label = generate_lbl("$print");
                buildin_print(lastParamsCount,label);
                ONLY_SEC_PASS(generate_instruction(concat("CALL ",label));)
            }else{
                ONLY_SEC_PASS(generate_instruction(concat("CALL $",lastVarName));)
            }
        }
            lastParamsCount = -1;
        //TODO proverit
        if(assign_to != NULL){
            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE LF@",assign_to)," TF@%return%"));)
        }
        from_prec = 0;
        return fce_v;
    }
    return false;
}

bool item(){
    INC_PARAMS;
    item_v = false;
    if(token.type == IDENTIFIER){
        //[1] ITEM -> identifier
        if(firstPass){
            insert_symtable(VAR,false);
        }
        PRINT_TOKEN("IDENTIFIER");
        item_v = true;
        token = get_token_checked();
        return item_v;	
    }
    else if(token.type == INTEGER){
        //[1] ITEM -> integer
        PRINT_TOKEN("INTEGER");
        item_v = true;
        token = get_token_checked();
        return item_v;	
    }
    else if(token.type == FLOAT){
        //[1] ITEM -> float
        PRINT_TOKEN("FLOAT");
        item_v = true;
        token = get_token_checked();
        return item_v;	
    }
    else if(token.type == STRING){
        //[1] ITEM -> string
        PRINT_TOKEN("STRING");
        item_v = true;
        token = get_token_checked();
        return item_v;	
    }
    else if(token.type == NIL){
        //[1] ITEM -> nil
        PRINT_TOKEN("NIL");
        item_v = true;
        token = get_token_checked();
        return item_v;	
    }
    return false;
}

bool expr(){
    //Volani precedencni analyzy
    if(firstPass){
        while((token.type != EOL) && (token.type != THEN) && (token.type != DO) && (token.type != EOFF)){
            PRINT_TOKEN("HODNOTA V VYRAZU")
            token = get_token_checked();
        }
    }else{
        PRINT("\nPREDANI PREC. ANALYZE\n")
        
        tokenBuffer->actual = tokenBuffer->actual->prevPtr;
        int prec_error_output = prec_analysis(tokenBuffer);
        if(prec_error_output != 0){
            fprintf(stderr,"Chyba Precedencni analyzy");
            //[EXIT]
            exit(prec_error_output);
        }
        from_prec = 1;
        if(assign_to != NULL){
            ONLY_SEC_PASS(generate_instruction(concat(concat("MOVE LF@",assign_to)," LF@%prec%"));)
        }
        token = get_token_checked();
    }
    return true;
}

/* ----------------------------  Hlavni telo Parseru ---------------------------- */

bool parser(){
    tokenBuffer = malloc(sizeof(Buffer));
    bufferInit(tokenBuffer);
    symbolTable =  malloc(sizeof(tBSTNodePtr));
    BSTInit(symbolTable);
    char s[1000];
    
    char c;
    /*while(true){
        c = getc(file);
        printf("%d  >  %c\n",c,c);
        if(c == EOF){
            break;
        }
    }*/
    token = get_token_checked();
    if(prog()){
        insert_all_buildin();
        PRINT("\nPrvni pruchod se mega povedl\n");
        firstPass = false;
        propagate_output();
        generate_instruction(".IFJcode18");
        generate_instruction("CREATEFRAME");
        generate_instruction("PUSHFRAME");
        generate_header();
        buildin_function();
        //TODO natavit %prec% na NIL, mozna nebude potreba
        PREPARE_BUFFER;
        if(!pre_order(symbolTable)){
            fprintf(stderr,"Nedefinovana funkce nebo promenna");
            //return 1;
            //[EXIT]
            exit(UNDEF_ID);
        }
        token = get_token_checked();
        if(prog()){
            
            PRINT("\nDruhy pruchod se mega povedl\n")
            return true;
        }else{
            PRINT("\nDruhy pruchod se kriticky nepovedl")
            fprintf(stderr,"Chyba Syntakticke analyzy");
            //[EXIT]
            exit(SYNTAX_ERR);
        }
    }else{
        PRINT("\nPrvni pruchod se kriticky nepovedl\n");
        fprintf(stderr,"Chyba Syntakticke analyzy");
        //[EXIT]
        exit(SYNTAX_ERR);
    }
    //fclose(f);
}
