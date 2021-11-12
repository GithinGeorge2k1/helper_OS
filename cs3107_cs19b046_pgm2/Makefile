all: result input logic

result: cs3107_cs19b046_pgm2_main.c
	gcc cs3107_cs19b046_pgm2_main.c -ll -o result.out
input: cs3107_cs19b046_pgm2_input.c
	gcc cs3107_cs19b046_pgm2_input.c -ll -o input.out
logic: cs3107_cs19b046_pgm2_dictLogic.c
	gcc cs3107_cs19b046_pgm2_dictLogic.c -ll -o logic.out
clean: 
	rm -f result.out input.out logic.out foo.txt outputFile.txt
