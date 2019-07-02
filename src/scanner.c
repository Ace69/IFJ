/* 
 * Project: Implementace překladače imperativního jazyka IFJ18.
 * Authors: David Bulawa <xbulaw01@stud.fit.vutbr.cz>
 *          Jakub Dolejší <xdolej09@stud.fit.vutbr.cz>
 *          František Policar <xpolic04@stud.fit.vutbr.cz>
 *          Tomáš Svěrák <xsvera04@stud.fit.vutbr.cz>
 * 
 * Implementace scanneru na zaklade konecneho automatu
 */

//Include s header slozky je nutne psat jako "**"
#include "error.h"
#include "scanner.h"

void delegateSourceFile(FILE *f) {
	file = f;
}

bool is_keyword(char* string, Token *ptoken){
    char* keywordsArray[KW_COUNT] = {"def","do","else","end","if","not","nil","then","while"};
    bool retval = true;
    if((strcmp(string, keywordsArray[0])) == 0){
        ptoken->type = DEF;
    }else if((strcmp(string, keywordsArray[1])) == 0){
        ptoken->type = DO;
    }else if((strcmp(string, keywordsArray[2])) == 0){
        ptoken->type = ELSE;
    }else if((strcmp(string, keywordsArray[3])) == 0){
        ptoken->type = END;
    }else if((strcmp(string, keywordsArray[4])) == 0){
        ptoken->type = IF;
    }else if((strcmp(string, keywordsArray[5])) == 0){
        ptoken->type = NOT;
    }else if((strcmp(string, keywordsArray[6])) == 0){
        ptoken->type = NIL;
    }else if((strcmp(string, keywordsArray[7])) == 0){
        ptoken->type = THEN;
    }else if((strcmp(string, keywordsArray[8])) == 0){
        ptoken->type = WHILE;
    }else{
        ptoken->value.string_value.string = string;
        retval = false;
    }
    return retval;
}

Token prepare_token(int err, Token *ptoken){
    set_error(err, ptoken);
    return *ptoken;
}

Token identifier_process(flex_string* str, Token* ptoken){
    char *s1 = flex_string_get(str);
    is_keyword(s1, ptoken);
    
    return(prepare_token(TOKEN_OK, ptoken));
}

Token string_process(flex_string* str, Token* ptoken){
    char *s1 = flex_string_get(str);
    ptoken->value.string_value.string = s1;
    
    ptoken->type = STRING;
    return(prepare_token(TOKEN_OK, ptoken));
}

Token integer_process(flex_string* str, Token* ptoken){
    char *s1 = flex_string_get(str);
    char *ptr;
    
    ptoken->value.int_value = (int) strtol(s1, &ptr, 10);       //Konverze stringu na integer
    if(*ptr != '\0'){                                           //Test zda znak za poslednim byl znak ukoncujici retezce
        return (prepare_token(INTERN_ERR, ptoken));
    }
    ptoken->type = INTEGER;
    return (prepare_token(TOKEN_OK, ptoken));
}

Token float_process(flex_string* str, Token* ptoken){
    char *s1 = flex_string_get(str);
    char *ptr;
    
    ptoken->value.double_value = strtod(s1, &ptr);       //Konverze stringu na double
    if(*ptr != '\0'){                                    //Test zda znak za poslednim byl znak ukoncujici retezce
        return (prepare_token(INTERN_ERR, ptoken));
    }
    ptoken->type = FLOAT;
    return(prepare_token(TOKEN_OK, ptoken));
}

static bool eol=TRUE;       // overeni zacatku radku, TRUE=precteny znak byl EOL,-> novy radek, FALSE=precteny znak nebyl EOL

//Cte ze standartniho vstupu po znacich, dle znaku se vybere stav automatu a nasledne se tokenu priradi Type/navratova hodnota Erroru
Token get_token(){
    char c;
    State state = START;
    Token token;
    Token *ptoken = &token;
    
    flex_string string;    // vytvoreni retezce, do ktereho budeme ukladat/konkatenovat nactene zanky
    flex_string *str = &string;
    flex_string_init(str);   // inicalizace retezce
    if(str == NULL){
        set_error(INTERN_ERR, ptoken);
    }
    
    while(1){
        c = getc(file);
        switch(state){
            case (START):                       //ZACATEK STAVU VYCHAZEJICICH ZE STARTU
                if((c == '_') || ((c >= 97) && (c <= 122))){      //Zadano v ASCII hodnotach --> c == '_' nebo c == a-z
                    eol = FALSE;
                    state = F_IDENTIFIER;
                    flex_string_add(str, c);
                }else if(c == '"'){
                    eol = FALSE;
                    state = S_QUOTE;
                    flex_string_add(str, c); 
                }else if(c=='0'){
                    eol=FALSE;
                    state=F_ZERO;
                    flex_string_add(str, c);
                }
                else if((c > 48) && (c <= 57)){      // 0 < c <= 9        48 je ASCII hodnota 0 a 57 je ASCII hodnota 9
                    eol=FALSE;
                    flex_string_add(str, c);
                    state=F_INTEGER;
                }
                else if(c=='!'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    state=S_EXCLAMATION;
                }
                else if(c=='-'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    token.type=SUB;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else if(c=='+'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    token.type=ADD;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else if(c=='*'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    token.type=MUL;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else if(c=='/'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    token.type=DIV;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else if(c=='('){
                    eol=FALSE;
                    flex_string_add(str, c);
                    token.type=LBRACKET;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else if(c==')'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    token.type=RBRACKET;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else if(c==','){
                    eol=FALSE;
                    flex_string_add(str, c);
                    token.type=COMMA;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else if(c==';'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    token.type=SEMICOLON;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else if(c=='>'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    state=F_GREATER;
                }
                else if(c=='<'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    state=F_LESS;
                }
                else if ((c=='=') && (eol==FALSE)){
                    flex_string_add(str, c);
                    state=F_EQUAL;
                }
                else if ((c=='=') && (eol==TRUE)){
                    flex_string_add(str, c);
                    state=S_BCOMMENT;
                    eol=FALSE;
                }else if (c == '#'){
                    eol = FALSE;
                    state=S_COMMENT;
                }else if (c == '\n'){
                    eol = TRUE;
                    state=START;
                    ptoken->type = EOL;
                    return(prepare_token(TOKEN_OK, ptoken));
                }else if (isspace(c)){
                    eol = FALSE;
                    state=START;
                }else if (c == EOF){
                    ptoken->type = EOFF;
                    return(prepare_token(TOKEN_OK, ptoken));
                }else{
                    return(prepare_token(LEX_ERR, ptoken));
                }
                
                break;                  ////KONEC STAVU VYCHAZEJICICH ZE STARTU      
                
                
                
            case(F_IDENTIFIER):         //ZACATEK STAVU PRO IDENTIFIKATORY
                if((c == '_') || ((c >= 65) && (c <= 90)) || ((c >= 97) && (c <= 122))){      //Zadano v ASCII hodnotach --> c == '_' nebo c == A-Z nebo c == a-z
                    state = F_IDENTIFIER;
                    flex_string_add(str, c);
                }else if((c == '!') || (c == '?')){
                    state = START;
                    flex_string_add(str, c);
                    token.type = IDENTIFIER;
                    return(identifier_process(str, ptoken));
                }else{
                    state = START;
                    ungetc(c, file);
                    token.type = IDENTIFIER;
                    return(identifier_process(str, ptoken));
                }
                break;                  //KONEC STAVU PRO IDENTIFIKATORY
                
            case(F_ZERO):               //ZACATEK STAVU PRO CISLA
                if(c=='e' || c=='E'){
                    eol=FALSE;
                    state=S_EXPONENT;
                    flex_string_add(str, c);
                }
                else if(c=='.'){
                    eol=FALSE;
                    state=S_DECIMAL;
                    flex_string_add(str, c);
                }
                else{
                    eol=FALSE;
                    ungetc(c,file);
                    token.type=INTEGER; // 
                    state=START;
                    return(integer_process(str, ptoken));
                }
                break;
            
            case(S_EXPONENT):
                if(c=='+' || c=='-'){
                    eol=FALSE;
                    state=S_SIGN;
                    flex_string_add(str, c);
                }
                else if(isdigit(c) != 0){ // pokud je cislo
                    eol=FALSE;
                    state=F_FLOATEXP;
                    flex_string_add(str, c);
                }
                else{
                    return (prepare_token(INTERN_ERR, ptoken));
                }
                break;

            case(S_DECIMAL):
                if(isdigit(c) != 0){
                    eol=FALSE;
                    state=F_FLOAT;
                    flex_string_add(str, c);
                }
                else{
                    return (prepare_token(LEX_ERR, ptoken));
                }
                break;

            case(S_SIGN):
                if(isdigit(c) != 0){
                    eol=FALSE;
                    state=F_FLOATEXP;
                    flex_string_add(str, c);
                }
                else{
                    return (prepare_token(LEX_ERR, ptoken));
                }
                break;

            case(F_FLOATEXP):
                if(isdigit(c) != 0){
                    eol=FALSE;
                    state=F_FLOATEXP;
                    flex_string_add(str, c);
                }
                else{
                    eol=FALSE;
                    ungetc(c,file);
                    token.type=FLOAT;
                    state=START;
                    return(float_process(str, ptoken)); // neexistuje dalsi konfigurace, proto vratime znak a posleme token
                }
                break;

            case(F_FLOAT):
                if(isdigit(c) != 0){
                    eol=FALSE;
                    state=F_FLOAT;
                    flex_string_add(str, c);
                }
                else if(c=='e' || c=='E'){
                    eol=FALSE;
                    state=S_EXPONENT;
                    flex_string_add(str, c);
                }
                else{
                    eol=FALSE;
                    ungetc(c,file);
                    state=START;
                    token.type=FLOAT;
                    return(float_process(str, ptoken));
                }
                break;

            case(F_INTEGER):
                if(isdigit(c) != 0){
                    eol=FALSE;
                    flex_string_add(str, c);
                    state=F_INTEGER;
                }
                else if(c=='e' || c=='E'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    state=S_EXPONENT;
                }
                else if(c=='.'){
                    eol=FALSE;
                    flex_string_add(str, c);
                    state=S_DECIMAL;
                }
                else{
                    eol=FALSE;
                    ungetc(c,file);
                    token.type=INTEGER;
                    state=START;
                    return(integer_process(str, ptoken));
                }
                break;                  //KONEC STAVU PRO CISLA
                
                
                
            case(S_COMMENT):            //STAV PRO RADKOVY KOMENTAR
                if(c == '\n' || c == EOF){
                    state = START;
                    ungetc(c,file);
                }
                break;
                
                

            case(S_BCOMMENT):           // ZACATEK JEDNOTLIVYCH STAVU BLOKOVEHO KOMENTARE
                if(c=='b'){
                    flex_string_add(str, c);
                    state=S_B;
                }
                else{
                    return (prepare_token(LEX_ERR, ptoken));
                }
                break;
                
            case(S_B):
                if(c=='e'){
                    flex_string_add(str, c);
                    state=S_BE;
                }
                else{
                    return (prepare_token(LEX_ERR, ptoken));
                }
                break;

            case(S_BE):
                if(c=='g'){
                    flex_string_add(str, c);
                    state=S_BEG;
                }
                else{
                    return (prepare_token(LEX_ERR, ptoken));
                }
                break;

            case(S_BEG):
                if(c=='i'){
                    flex_string_add(str, c);
                    state=S_BEGI;
                }
                else{
                    return (prepare_token(LEX_ERR, ptoken));
                }
                break;

            case(S_BEGI):
                if(c=='n'){
                    flex_string_add(str, c);
                    state=S_BEGIN;
                }
                else{
                    return (prepare_token(LEX_ERR, ptoken));
                }
                break;

            case(S_BEGIN):
                if(c==' '){
                    flex_string_add(str, c);
                    state=S_COMM_SPACE;
                }
                else if (c == '\n'){
                    eol=TRUE;
                    state=S_EQ_COMMENT;
                    
                }else{
                    return (prepare_token(LEX_ERR, ptoken));
                }
                break;

            case(S_COMM_SPACE):
                if(c=='\n'){
                    eol=TRUE;
                    state=S_EQ_COMMENT;
                }else if(c == EOF){
                    return (prepare_token(LEX_ERR, ptoken));
                }else{
                    eol=FALSE;
                    state=S_COMM_SPACE;
                }
                break;

            case(S_EQ_COMMENT):
                if(c=='=' && eol==TRUE){
                    eol=FALSE;
                    flex_string_add(str, c);
                    state=S_E;
                }else if(c == EOF){
                    return (prepare_token(LEX_ERR, ptoken));
                }else if(c=='\n'){
                    eol=TRUE;
                    state=S_EQ_COMMENT;
                }else{
                    eol=FALSE;
                    state=S_COMM_SPACE;
                }
                break;

            case(S_E):
                if(c=='e'){
                    flex_string_add(str, c);
                    state=S_EN;
                }else if(c == EOF){
                    return (prepare_token(LEX_ERR, ptoken));
                }else{
                    state=S_COMM_SPACE;
                }
                break;

            case(S_EN):
                if(c=='n'){
                    flex_string_add(str, c);
                    state=S_END;
                }else if(c == EOF){
                    return (prepare_token(LEX_ERR, ptoken));
                }else{
                    state=S_COMM_SPACE;
                }
                break;

            case(S_END):
                if(c=='d'){
                    flex_string_add(str, c);
                    state=S_PREEND;
                }else if(c == EOF){
                    return (prepare_token(LEX_ERR, ptoken));
                }else{
                    state=S_COMM_SPACE;
                }
                break;
                
            case(S_PREEND):
                if(c==' '){
                    flex_string_add(str, c);
                    state=F_BCOMMENT;
                }else if((c == EOF) || (c == '\n')){
                    eol=TRUE;
                    state=START;
                    ungetc(c, file);
                    token.type=BCOMMENT;
                    return (prepare_token(TOKEN_OK, ptoken));
                }else{
                    state=S_COMM_SPACE;
                }
                break;   

            case(F_BCOMMENT):
                if(c=='\n'){
                    eol=TRUE;
                    state=START;
                    ungetc(c, file);
                    token.type=BCOMMENT;
                    return (prepare_token(TOKEN_OK, ptoken));
                }else if(c == EOF){
                    eol=TRUE;
                    state=START;
                    ungetc(c, file);
                    token.type=BCOMMENT;
                    return (prepare_token(TOKEN_OK, ptoken));
                } else{
                    eol=FALSE;
                    state=F_BCOMMENT;
                }
                break;                  // KONEC STAVU PRO BLOKOVE KOMENTARE



            case(F_EQUAL):              //ZACATEK STAVU PRO LOGICKE OPERATORY
                if(c=='='){
                    flex_string_add(str, c);
                    token.type=EQQ;
                    state=START;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else{
                    state=START;
                    token.type=EQ;
                    ungetc(c,file);
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                break;

            case(F_GREATER):
                if(c=='='){
                    flex_string_add(str, c);
                    token.type=GEQ;
                    state=START;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else{
                    state=START;
                    token.type=GREATER;
                    ungetc(c,file);
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                break;

            case(F_LESS):
                if(c=='='){
                    flex_string_add(str, c);
                    token.type=LEQ;
                    state=START;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else{
                    state=START;
                    token.type=LESSER;
                    ungetc(c,file);
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                break;

            case(S_EXCLAMATION):
                if(c=='='){
                    state=START;
                    flex_string_add(str, c);
                    token.type=NEQ;;
                    return(prepare_token(TOKEN_OK, ptoken));
                }
                else{
                    return(prepare_token(LEX_ERR, ptoken));
                }
                break;                  //KONEC STAVU PRO LOGICKE OPERATORY

                
                
            case(S_QUOTE):              //ZACATEK STAVU PRO RETEZCE
                if(c == '"'){
                    eol = FALSE;
                    state = START;
                    flex_string_add(str, c);
                    token.type = STRING;
                    return(string_process(str, ptoken));
                }else if(c == 92){      //92 = ASCII hodnota '\'
                    eol = FALSE;
                    state = S_BACKSLASH;
                    flex_string_add(str, c);
                }else if((c > 31) && (c != 92) && (c != 34)){       //c == jakykoliv tisknutelny znak krome '\' a '"'
                    eol = FALSE;
                    state = S_STR;
                    flex_string_add(str, c);
                }else{
                    return(prepare_token(LEX_ERR, ptoken));
                }
                break;
                        
            case(S_STR):
                if((c > 31) && (c != 92) && (c != 34)){
                    eol = FALSE;
                    state = S_STR;
                    flex_string_add(str, c);
                }else if(c == 92){      //92 = ASCII hodnota '\'
                    eol = FALSE;
                    state = S_BACKSLASH;
                    flex_string_add(str, c);
                }else if(c == '"'){
                    eol = FALSE;
                    state = START;
                    flex_string_add(str, c);
                    token.type = STRING;
                    return(string_process(str, ptoken));
                }else{
                    return(prepare_token(LEX_ERR, ptoken));
                }
                break;
                
            case(S_BACKSLASH):
                if((c == 92) || (c == '"') || (c == 'n') || (c == 's') || (c == 't')){
                    eol = FALSE;
                    state = S_STR;
                    flex_string_add(str, c);
                }else if(c == 'x'){
                    eol = FALSE;
                    state = S_HEX;
                    flex_string_add(str, c);
                }else{
                    return(prepare_token(LEX_ERR, ptoken));
                }
                break;

            case(S_HEX):
                if(((c >= 48) && (c <= 57)) || ((c >= 65) && (c <= 70) || ((c >= 97) && (c <= 102)))){   //c = 0-9, a-f, A-F       (0 <= c <= 9) && (A <= c <= F) && (a <= c <= f)
                    eol = FALSE;
                    state = S_DOUBLEHEX;
                    flex_string_add(str, c);
                }else{
                    return(prepare_token(LEX_ERR, ptoken));
                }
                break;
                
            case(S_DOUBLEHEX):
                if(((c >= 48) && (c <= 57)) || ((c >= 65) && (c <= 70) || ((c >= 97) && (c <= 102)))){   //c = 0-9, a-f, A-F       (0 <= c <= 9) && (A <= c <= F)
                    eol = FALSE;
                    state = S_STR;
                    flex_string_add(str, c);
                }else if(c == 92){          //92 = ASCII hodnota '\'
                    eol = FALSE;
                    state = S_BACKSLASH;
                    flex_string_add(str, c);
                }else if(c == '"'){          //
                    eol = FALSE;
                    state = START;
                    flex_string_add(str, c);
                    token.type = STRING;
                    return(string_process(str, ptoken));
                }else{
                    return(prepare_token(LEX_ERR, ptoken));
                }         
                break;                  //KONEC STAVU PRO RETEZCE
        }
    }
}
