#include "include/prompt_formatting.h"
#include <stdio.h>


//fonctions de formatage
command* command_formatting(char* user_prompt)
{
	// si CTRL-D
	if(user_prompt == NULL)
	{
		return NULL;
	}

	// On enlève les espaces au début et à la fin
	user_prompt = erase_whitespaces(user_prompt);

	//Création de la commande
	command* formatted_command = malloc(sizeof(command));
	int number_of_arguments = get_number_of_arguments(user_prompt);

	// (à ajouter) gérer les prompts trop longs/arguments trop longs
	char** args =  malloc(number_of_arguments * sizeof(char*));

	// Tokenisation du prompt et initialisation des données de la commande
	int i;
	char* command_token = strtok(user_prompt, " ");
	for(i = 0; command_token != NULL; i++)
	{
		args[i] = malloc(strlen(command_token) + 1);
		strcpy(args[i], command_token);
		command_token = strtok(NULL, " ");
	}

	// assignation des valeurs de la commande
	formatted_command -> arguments = args;
	formatted_command -> number_of_args = number_of_arguments;

return formatted_command;
}

char* erase_whitespaces(char* user_prompt)
{
	char* formatted_prompt = user_prompt;
	// délétion des leading whitespaces
	while(isspace(*formatted_prompt)) formatted_prompt++;
	// si prompt uniquement constituée de whitespaces, on retourne la chaine vide
	if(*formatted_prompt == '\0') return formatted_prompt;
	// délétion des trailing whitespaces
	char* trailing = formatted_prompt + strlen(formatted_prompt) - 1;
	while(isspace(*trailing) && trailing > formatted_prompt) trailing--;
	*(trailing + 1) = '\0';

	return formatted_prompt;
}

// Permet de savoir quelle taille donner au tableau des arguments lors du formattage d'une commande à partir 
// du prompt d'un utilisateur, en comptant les espaces.
// Permet d'éviter d'utiliser realloc pour créer un tableau de taille dynamique correspondant au nbre d'args.
int get_number_of_arguments(char* user_prompt)
{
	// si prompt, au moins un argument
	int number_of_arguments = 1;
	while(*user_prompt != '\0')
	{
		if(isspace(*user_prompt) && !isspace(*(user_prompt - 1)))
		{
			number_of_arguments++;
		}
		user_prompt++;
	}

	return number_of_arguments;
}

//affichage du prompt (a compléter)
char* make_prompt(int result, char* pwd)
{
	char *prompt = malloc(MAX_CHAR_PROMPT);
	//char *cop_pwd = pwd; // précaution au vu  

	//Gestion des 30 char
	if(strlen(pwd) > MAX_CHAR_PROMPT - 5) {
		char* new_pwd = malloc(26);
  		strcpy(new_pwd, "...");
  		strncat(new_pwd, pwd + (strlen(pwd) - 23) + 1, 26);
  		pwd = new_pwd;
	}
	//tentative avec couleur
	switch (result) {
	case 0:
		sprintf(prompt, "\001\033[32m\002[%d]\001\033[36m\002%s\001\033[00m\002$ ", result, pwd);
		break;
	case 1 :
		sprintf(prompt, "\001\033[91m\002[%d]\001\033[36m\002%s\001\033[00m\002$ ", result, pwd);
		break;
	//Cas des SIGNAUX (A traiter)
	//case : break;


	default:
		sprintf(prompt, "\001\033[32m\002[%d]\001\033[36m\002%s\001\033[00m\002$ ", result, pwd);
		break;
	}	
	return prompt;
}

