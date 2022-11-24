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
    char option[16];
    char reference[256];

	// cas par défaut (pas de paramètres)
    strcpy(option, "-L");
    strcpy(reference, getenv("HOME"));

	// si un seul paramètre
	if(nb_args == 2)
	{
        //option - seul
        if(strcmp(args[1], "-") == 0) {
            strcpy(option, "-");
           // strcpy(reference, getenv("OLDPWD"));
           strcpy(reference,old_pwd);
        }
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
		else printf("Too many arguments\n");
	}
	
	printf("commande : cd %s %s\n", option, reference);
    printf("getenv oldpwd : %s\n", getenv("OLDPWD"));

    char symlink_path[256] = {0};
    int ret = 0;

    //cd - et maj de oldpwd
    if (strcmp(option, "-") == 0) {
        strcpy(reference, getenv("OLDPWD"));
        strcpy(logical_pwd,reference);
        printf("%s", reference);
    }

    

    // -P
    // cd physique, on met à jour pwd avec l'évaluation du path du symlink
    // cd physique, pas ..
    if(strcmp(option, "-P") == 0 && strcmp(reference, "..") != 0)
    {
        ret = readlink(reference, symlink_path, sizeof(symlink_path));
        
        strcat(logical_pwd, "/");
        // si la reference est un symlink
        if(ret != -1)
        {
            strcat(logical_pwd, symlink_path);
        }
        // si la reference n'est pas un symlink
        else
        {
            strcat(logical_pwd, reference);
        }
    }
    

    // cd logique, pas ..
    else if(strcmp(option, "-L") == 0 && strcmp(reference, "..") != 0)
    {
        strcat(logical_pwd, "/");
        strcat(logical_pwd, reference);
    }
    // cd logique, on met à jour pwd avec le nom du symlink
    // si la ref est .., on chdir sur logical_pwd - readlink(symlink)
    // cd logique, ..
    if(strcmp(option, "-L") == 0 && strcmp(reference, "..") == 0)
    {
        // on supprime le dernier dossier du chemin logique
        strcpy(logical_pwd, delete_last_folder(logical_pwd));
    }
    //cd physique, ..
    if(strcmp(option, "-P") == 0 && strcmp(reference, "..") == 0)
    {
        ret = readlink(get_last_folder(logical_pwd), symlink_path, sizeof(symlink_path));
        // S'il s'agit bien d'un symlink
        if(ret != -1)
        {
            strcat(logical_pwd, "/");
            strcat(logical_pwd, symlink_path);
        }
        // on supprime le dernier dossier du chemin logique (si symlink, dernier dossier évalué, sinon, dernier dossier)
        strcpy(logical_pwd, delete_last_folder(logical_pwd));
    }

	printf("retour cd : %d\n", chdir(logical_pwd));

    
    //strcpy(logical_pwd, update_pwd(logical_pwd, option, reference));
    //printf("getenv: %s\n", getenv("PWD"));
    //printf("logical_pwd : %s\n", logical_pwd);

	return 0;
}

/* Convertit le dernier dossier du chemin en son chemin physique si c'est un symlink
char* last_folder_conversion(char* logical_pwd)
{
    à faire (?)
}
*/

// Supprime le dernier dossier du chemin logique
char* delete_last_folder(char* logical_pwd)
{
        char* pwd_end = logical_pwd + strlen(logical_pwd) -1;
        while(*pwd_end != '/') pwd_end--;
        *(pwd_end) = '\0';
        return logical_pwd;
}


char* get_last_folder(char* logical_pwd)
{
    char* last_folder = logical_pwd + strlen(logical_pwd) - 1;
    while(*last_folder != '/') last_folder--;
    return (last_folder + 1);
}

// met à jour le chemin logique en fonction des cd_slash() effectués
char* update_pwd(char* pwd, char* option, char* reference)
{
    // si on recule d'un dossier, on efface le dernier dossier du chemin logique
    if(strcmp(reference, "..") == 0)
    {

        char* pwd_end = pwd + strlen(pwd) -1;
        while(*pwd_end != '/') pwd_end--;
        *(pwd_end) = '\0';
    }

    return pwd;
}

char *update_old_pwd() {

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
            printf("Current directory : %s\n", getcwd(cwd, sizeof(cwd)));
            return 0;
        }
	}

    //pwd par défaut (logique)
    printf("Current directory : %s\n", logical_pwd);
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

