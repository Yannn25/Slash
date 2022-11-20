#ifndef PROMPT_FORMATTING_H
#define PROMPT_FORMATTING_H

// bibliothèques requises
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//Commande utilisateur
typedef struct command
{
	char** arguments;
	int number_of_args;
} command;

//traitement ligne de commande
command* command_formatting(char* prompt);
char* make_prompt(int result, char* pwd);
int get_number_of_arguments(char* user_prompt);
char* erase_whitespaces(char* user_prompt);

#endif