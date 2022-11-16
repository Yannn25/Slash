#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

typedef struct command
{
	char** arguments;
	int number_of_args;
} command;

//Commandes internes

//Shell
command* command_formatting(char* prompt);
char* make_prompt(int result, char* pwd);
int get_number_of_arguments(char* user_prompt);
char* erase_whitespaces(char* user_prompt);

int main(void)
{
	// initialisation
	int proc;
	int result = 0;
	rl_outstream = stderr;

	// boucle principale
	while(1)
	{
		// Prompt Slash
		char* prompt = readline("$ ");
		printf("prompt = %s\n", prompt);
		add_history(prompt);

		//On vérifie si l'utilisateur a saisi une commande interne

		command* user_command = command_formatting(erase_whitespaces(prompt));
		
		for(int i = 0; i < user_command -> number_of_args; i++)
		{
			printf("%s\n", user_command -> arguments[i]);
		}
		printf("%d\n", user_command -> number_of_args);
		if(strcmp(user_command -> arguments[0], "exit") == 0)
		{
			exit(strtol(user_command -> arguments[1], NULL, 10));
		}

		/* Sinon, on vérifie si l'utilisateur a saisi une commande externe
		if((proc = fork()) == 0)
		{
			execl("/bin/pwd", "pwd", NULL);
		}
		*/

		// si ni l'un ni l'autre, message d'erreur et on boucle
	}

return 0;
}

//fonctions de formattage
command* command_formatting(char* user_prompt)
{
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
	char* prompt;

return prompt;
}


