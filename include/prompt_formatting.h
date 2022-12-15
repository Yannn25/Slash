#ifndef PROMPT_FORMATTING_H
#define PROMPT_FORMATTING_H

// longueur du prompt slash
#define PROMPT_LENGTH 30
#define NUMBER_OF_COLORS 3

// bibliothèques requises
#include <string.h>
#include <stdio.h>
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
void free_command(command* user_command);
char* make_prompt(int result, char* pwd);
int get_number_of_arguments(char* user_prompt);
char* erase_whitespaces(char* user_prompt);
char* shorten_string(char* str, int n);
char* format_pwd(char* pwd, int pwd_max_length);

#endif