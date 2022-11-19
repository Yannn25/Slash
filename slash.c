#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

//Commande utilisateur
typedef struct command
{
	char** arguments;
	int number_of_args;
} command;

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

//traitement ligne de commande
command* command_formatting(char* prompt);
char* make_prompt(int result, char* pwd);
int get_number_of_arguments(char* user_prompt);
char* erase_whitespaces(char* user_prompt);

//Commandes disponibles dans slash
const internal_command internal_commands[] = 
{
	{"exit", &exit_slash},
	{"cd", &cd_slash},
	{"pwd", &pwd_slash}
};

// paramètrage
const int number_of_internal_commands = sizeof(internal_commands) / sizeof(internal_command);
int last_return_value = 0;

int main(void)
{
	// initialisation
	int proc;
	rl_outstream = stderr;

	// boucle principale
	while(1)
	{
		// Prompt Slash
		char* prompt = readline("$ ");
		printf("prompt = %s\n", prompt);
		add_history(prompt);

		//On vérifie si l'utilisateur a saisi une commande interne
		command* user_command = command_formatting(prompt);
		// Lancement de la commande utilisateur
		last_return_value = execute(user_command);

		/* Sinon, on vérifie si l'utilisateur a saisi une commande externe
		(à mettre plutot dans la fct execute)
		if((proc = fork()) == 0)
		{
			execl("/bin/pwd", "pwd", NULL);
		}
		*/

		// si ni l'un ni l'autre, message d'erreur et on boucle
	}

return 0;
}

// execution commandes internes
int execute(command* user_command)
{
	//Gestion du CTRL-D
	if(user_command == NULL)
	{
		exit_slash(user_command);
	}

	int i;
	for(i = 0; i < number_of_internal_commands; i++)
	{
		if(strcmp(user_command -> arguments[0], internal_commands[i].command_name) == 0)
		{
			return (*(internal_commands[i].internal_function))(user_command);
		}
	}

	return -1;
}

//Commandes internes
// à compléter
int cd_slash(command* user_command)
{
	if(user_command -> number_of_args > 3)
	{
		printf("usage: cd [-L | -P] [ref | -]\n");
		return 1;
	}
	return 0;
}

//à compléter
int pwd_slash(command* user_command)
{
	int nb_args = user_command -> number_of_args;
	if(user_command -> number_of_args > 2)
	{
		printf("usage: pwd [-L | -P]\n");
		return 1;
	}

	char** args = user_command -> arguments;
	//Gérer argument
	//Cas par défaut (-L)
	if(args[1] == NULL)
	{
		args[1] = "-L";
	}

	//pwd physique
	if(strcmp(args[1], "-P") == 0)
	{
		printf("Current directory : %s\n", getenv("PWD"));
		return 0;
	} 

	//pwd logique (à compléter)
	if(strcmp(args[1], "-L") == 0)
	{
		printf("à compléter\n");
		return 0;
	}

	printf("usage: pwd [-L | -P]\n");
	return 1;
}

int exit_slash(command* user_command)
{
	if(user_command == NULL) exit(last_return_value);
	if(user_command -> number_of_args > 2)
	{
		printf("usage: exit [val]\n");
		return 1;
	}

	// (à ajouter) tester la valeur de retour de strtol, gérer les cas où l'argument est invalide
	exit(strtol(user_command -> arguments[1], NULL, 10));
}

//fonctions de formattage
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
	char* prompt;

return prompt;
}


