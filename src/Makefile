CC=gcc
CFLAGS=-O2 -std=c99 -pedantic -lm

all: main

main: main.o error.o flex_string.o parser.o prec_analysis.o scanner.o stack.o symtable.o token_buffer.o postfix_stack.o postfix_transfer.o generate_prec.o instr_list.o
	$(CC) main.o error.o flex_string.o parser.o prec_analysis.o scanner.o stack.o symtable.o token_buffer.o postfix_stack.o postfix_transfer.o generate_prec.o instr_list.o -o main $(CFLAGS)

main.o: main.c
	$(CC) -c main.c

error.o: error.c
	$(CC) -c error.c

flex_string.o: flex_string.c
	$(CC) -c flex_string.c

parser.o: parser.c
	$(CC) -c parser.c

prec_analysis.o: prec_analysis.c
	$(CC) -c prec_analysis.c

scanner.o: scanner.c
	$(CC) -c scanner.c

stack.o: stack.c
	$(CC) -c stack.c

symtable.o: symtable.c
	$(CC) -c symtable.c

postfix_stack.o: postfix_stack.c
	$(CC) -c postfix_stack.c

postfix_transfer.o: postfix_transfer.c
	$(CC) -c postfix_transfer.c

token_buffer.o: token_buffer.c
	$(CC) -c token_buffer.c

generate_prec.o: generate_prec.c
	$(CC) -c generate_prec.c

instr_list.o: instr_list.c
	$(CC) -c instr_list.c
run:
	./main
test:
	./testerino.sh

clean:
	rm -rf *o main main.exe
