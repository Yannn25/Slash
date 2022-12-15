slash : main.o prompt_formatting.o internal_functions.o path_formatting.o
	gcc main.o prompt_formatting.o internal_functions.o path_formatting.o -o slash -lreadline

main.o : main.c
	gcc -c main.c -o main.o

path_formatting.o : path_formatting.c include/path_formatting.h
	gcc -c path_formatting.c -o path_formatting.o

prompt_formatting.o : prompt_formatting.c include/prompt_formatting.h
	gcc -c prompt_formatting.c -o prompt_formatting.o

internal_functions.o : internal_functions.c include/internal_functions.h
	gcc -c internal_functions.c -o internal_functions.o

clean:
	rm *.o slash