slash : main.o prompt_formatting.o internal_functions.o 
	gcc main.o prompt_formatting.o internal_functions.o -o slash -lreadline

main.o : main.c
	gcc -c main.c -o main.o

prompt_formatting.o : prompt_formatting.c include/prompt_formatting.h
	gcc -c prompt_formatting.c -o prompt_formatting.o

internal_functions.o : internal_functions.c include/internal_functions.h
	gcc -c internal_functions.c -o internal_functions.o

clean:
	rm *.o slash