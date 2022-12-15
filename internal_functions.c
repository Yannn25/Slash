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
	int i;
    int result = 0;
	for(i = 0; i < number_of_internal_commands; i++)
	{
		if(strcmp(user_command -> arguments[0], internal_commands[i].command_name) == 0)
		{
            // execution de la commande
			result = (*(internal_commands[i].internal_function))(user_command);
            // libération de la mémoire
            free_command(user_command);
            return result;
		}
	}

    for(int j = 0; j < user_command -> number_of_args; j++)
    {
        free(user_command -> arguments[j]);
    }
    free(user_command);
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
		else 
        {
		    printf("usage: cd [-L | -P] [ref | -]\n");
            return 1;
        }
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
        // si reference absolue, on effectue la procédure standard à partir de la racine
        if(*reference == '/')
        {
            new_logical_pwd = update_logical_pwd(entries, nbre_repertoires, "/");
        }
        // si reference relative
        else new_logical_pwd = update_logical_pwd(entries, nbre_repertoires, logical_pwd);
        chdir_return = chdir(new_logical_pwd);
        // si chdir a fonctionné, on met à jour le nouveau chemin et on sauvegarde l'ancien
        if(chdir_return == 0) 
        {
            strcpy(old_logical_pwd, logical_pwd);
            strcpy(logical_pwd, new_logical_pwd);
            free(new_logical_pwd);
            // supprimer ça si new logical pwd == '/'
            for(int k = 0; k < nbre_repertoires; k++)
            {
                free(entries[k]);
            }
            free(entries);
            return 0;
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
                for(int k = 0; k < nbre_repertoires; k++)
                {
                    free(entries[k]);
                }
                free(entries);
                //free(new_logical_pwd);
                return 0;
            }
            else
            {
                for(int k = 0; k < nbre_repertoires; k++)
                {
                    free(entries[k]);
                }
                free(entries);
                free(new_logical_pwd);
                return 1;
            }
        }
        // si reference relative
        else getcwd(physical_cwd, sizeof(physical_cwd));
        new_logical_pwd = update_logical_pwd(entries, nbre_repertoires, physical_cwd);
        // si chdir chemin physique échoue, alors repertoire inexistant
        if(chdir(new_logical_pwd) == -1)
        { 
            printf("cd: No such file or directory : %s\n", reference);
            for(int k = 0; k < nbre_repertoires; k++)
            {
                free(entries[k]);
            }
            free(entries);
            free(new_logical_pwd);
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
            for(int k = 0; k < nbre_repertoires; k++)
            {
                free(entries[k]);
            }
            free(entries);
            free(new_logical_pwd);
        }
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
        free_command(user_command);
        exit(last_return_value);
    }

	// (à ajouter) tester la valeur de retour de strtol, gérer les cas où l'argument est invalide
    int exit_value = strtol(user_command -> arguments[1], NULL, 10);
    free_command(user_command);
	exit(exit_value);
}
