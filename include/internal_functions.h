#ifndef INTERNAL_FUNCTIONS_H
#define INTERNAL_FUNCTIONS_H

// bibliothèques requises
#include <stdio.h>
#include <unistd.h>
#include "prompt_formatting.h"
#include "path_formatting.h"
#include "global.h"

//Commandes internes
typedef struct internal_command
{
	char* command_name;
	int (*internal_function)(command*);
} internal_command;

// execution commandes internes
int execute(command*);
//commandes internes
int cd_slash(command*);
int pwd_slash(command*);
int exit_slash(command*);

#endif