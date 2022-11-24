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

	return 1;
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
    char option[16];
    char reference[256];

	// cas par défaut (pas de paramètres)
    strcpy(option, "-L");
    strcpy(reference, getenv("HOME"));

	// si un seul paramètre
	if(nb_args == 2)
	{
		// option -P en argument
		if(strcmp(args[1], "-P") == 0)
		{
		    strcpy(option, args[1]);
		}
        // si pas option -L (donc un chemin)
        else if(strcmp(args[1], "-L") != 0)
        {
            strcpy(reference, args[1]);
        }
	}
    // si 2 paramètres
	else if(nb_args == 3)
	{
		// si option en premier arg
		if(strcmp(args[1], "-L") == 0 || strcmp(args[1], "-P") == 0)
		{
		    strcpy(option, args[1]);
		    strcpy(reference, args[2]);
		}
		// si option en dernier (ex : cd dir -P)
		else 
        {
		    printf("usage: cd [-L | -P] [ref | -]\n");
            return 1;
        }
	}

    // Si - passé en argument, on remplace la reference par l'ancien pwd
    if(strcmp(reference, "-") == 0)
    {
        strcpy(reference, old_logical_pwd);
    }

    // On parse la liste des dossiers passés en référence
    int nbre_repertoires = get_number_of_directories(reference);
    char** entries = path_parser(reference);

    int chdir_return = 0;
    //si option L, cd sur chemin logique
    if(strcmp(option, "-L") == 0)
    {
        char* new_logical_pwd;
        // si reference absolue, on effectue la procédure standard à partir de la racine
        if(*reference == '/')
        {
            new_logical_pwd = update_logical_pwd(entries, nbre_repertoires, "/", new_logical_pwd);
        }
        // si reference relative
        else new_logical_pwd = update_logical_pwd(entries, nbre_repertoires, logical_pwd, new_logical_pwd);
        chdir_return = chdir(new_logical_pwd);
        // si chdir a fonctionné, on met à jour le nouveau chemin et on sauvegarde l'ancien
        if(chdir_return == 0) 
        {
            strcpy(old_logical_pwd, logical_pwd);
            strcpy(logical_pwd, new_logical_pwd);
        }
    }
    // si option P ou chdir précédent a échoué, chdir sur chemin physique
    if(strcmp(option, "-P") == 0 || chdir_return == -1)
    {
        char physical_cwd[256];
        char* new_logical_pwd;
        // si reference absolue
        if(*reference == '/')
        {
            chdir_return = chdir(reference);
            if(chdir_return == 0)
            {
                getcwd(physical_cwd, sizeof(physical_cwd));
                strcpy(old_logical_pwd, logical_pwd);
                strcpy(logical_pwd, physical_cwd);
                return 0;
            }
        }
        // si reference relative
        else getcwd(physical_cwd, sizeof(physical_cwd));
        new_logical_pwd = update_logical_pwd(entries, nbre_repertoires, physical_cwd, new_logical_pwd);
        // si chdir chemin physique échoue, alors repertoire inexistant
        if(chdir(new_logical_pwd) == -1)
        { 
            printf("cd: No such file or directory\n");
            return 1;
        }
        // si chdir physique fonctionne, on copie le nouveau chemin
        else
        {
            // test cd physique
            //printf("CHDIR PHYSIQUE\n");
            getcwd(physical_cwd, sizeof(physical_cwd));
            strcpy(old_logical_pwd, logical_pwd);
            strcpy(logical_pwd, physical_cwd);
        }
    }

	return 0;
}

//à compléter
int pwd_slash(command* user_command)
{
	//trop d'arguments
	int nb_args = user_command -> number_of_args;
	if(nb_args > 2)
	{
		printf("usage: pwd [-L | -P]\n");
		return 1;
	}

	char** args = user_command -> arguments;
    // si argument fourni
	if(nb_args == 2)
	{
        //pwd physique
        if(strcmp(args[1], "-P") == 0)
        {
            char cwd[256];
            printf("%s\n", getcwd(cwd, sizeof(cwd)));
            return 0;
        }
	}

    //pwd par défaut (logique)
    printf("%s\n", logical_pwd);
    return 0;
}

int exit_slash(command* user_command)
{
    // si signal CTRL-D
	if(user_command == NULL) exit(last_return_value);

    int nb_args = user_command -> number_of_args;

    // trop d'arguments
	if(nb_args > 2)
	{
		printf("usage: exit [val]\n");
		return 1;
	}

    // pas d'argument, valeur par défaut renvoyée
    if(nb_args == 1)
    {
        exit(last_return_value);
    }

	// (à ajouter) tester la valeur de retour de strtol, gérer les cas où l'argument est invalide
	exit(strtol(user_command -> arguments[1], NULL, 10));
}

