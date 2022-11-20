#include "include/internal_functions.h"

//Commandes disponibles dans slash
const internal_command internal_commands[] = 
{
	{"exit", &exit_slash},
	{"cd", &cd_slash},
	{"pwd", &pwd_slash}
};

//nombres de commandes disponibles
const int number_of_internal_commands = sizeof(internal_commands) / sizeof(internal_command);

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
	// si trop d'arguments
	int nb_args = user_command -> number_of_args;
	if(nb_args > 3)
	{
		printf("usage: cd [-L | -P] [ref | -]\n");
		return 1;
	}
	char** args = user_command -> arguments;
	// cas par défaut (pas de paramètres)
	if(nb_args == 1)
	{
		char* param1 = "-L";
		char* param2 = getenv("HOME");
	}
	// si un seul paramètre
	if(nb_args == 2)
	{
		// si paramètre est une option (-L ou -P), on place $HOME par défaut en 2ème paramètre
		if(strcmp(args[1], "-L") == 0 || strcmp(args[1], "-P") == 0)
		{
			char* param1 = args[1];
			char* param2 = getenv("HOME");
		}
		// sinon, on place l'option -L par défaut
		else
		{
			char* param1 = "-L";
			char* param2 = args[1];
			printf("ok");
		}
	}
	else
	{
		// si option en premier arg
		if(strcmp(args[1], "-L") == 0 || strcmp(args[1], "-P") == 0)
		{
			char* param1 = args[1];
			char* param2 = args[2];
		}
		// si option en dernier (ex : cd dir -P)
		else printf("Too many arguments\n");
	}
	
    /*
	printf("commande : cd %s %s\n", param1, param2);
	printf("retour cd : %d\n", chdir(param2));
    */
	/*char* pwd_args[] = {"pwd", "-L"};
	command pwd = {pwd_args, 2};
	pwd_slash(&pwd);*/

	return 0;
}

//à compléter
int pwd_slash(command* user_command)
{
	//trop d'arguments
	int nb_args = user_command -> number_of_args;
	if(user_command -> number_of_args > 2)
	{
		printf("usage: pwd [-L | -P]\n");
		return 1;
	}

	char** args = user_command -> arguments;
	//Gérer argument
	//Cas par défaut (-L)
	char* param1 = "-L";

	//pwd physique
	if(strcmp(args[1], "-P") == 0)
	{
		char cwd[256];
		printf("Current directory : %s\n", getcwd(cwd, sizeof(cwd)));
		return 0;
	} 

	//pwd logique (à compléter)
	if(strcmp(args[1], "-L") == 0)
	{
		printf("à compléter\n");
		printf("Current directory : %s\n", getenv("PWD"));
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

