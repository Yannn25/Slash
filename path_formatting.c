#include "include/path_formatting.h"

// met à jour le chemin logique
// pas forcément besoin de la longueur du tableau, on peut le terminer par un NULL (modifier path parser)
char* update_logical_pwd(char** parsed_reference, int number_of_entries, char* logical_pwd, char* new_logical_pwd)
{
    new_logical_pwd = malloc(MAX_PWD_LENGTH);
    // on copie le chemin logique sauvegardé
    strcpy(new_logical_pwd, logical_pwd);
    for(int i = 0; i < number_of_entries; i++)
    {
        char* entry = parsed_reference[i];
        // si l'entrée considérée est .., on recule d'un dossier (on efface le dernier dossier du chemin logique, sauf si on est à la racine)
        if(strcmp(entry, "..") == 0) 
        {
            // si on n'est pas à la racine
            if(strcmp(new_logical_pwd, "/") != 0) new_logical_pwd = delete_last_folder(new_logical_pwd);
        }
        // si l'entrée considérée est tout autre répertoire que ., on l'ajoute à la fin du chemin logique
        else if(strcmp(entry, ".") != 0) new_logical_pwd = add_folder(new_logical_pwd, entry);
    }

    return new_logical_pwd;
}

// Ajoute un dossier à la fin du chemin logique
char* add_folder(char* logical_pwd, char* reference)
{
    // si on n'est pas à la racine, on ajoute un séparateur
    if(strcmp(logical_pwd, "/") != 0) strcat(logical_pwd, "/");
    strcat(logical_pwd, reference);
    return logical_pwd;
}

// Supprime le dernier dossier du chemin logique
char* delete_last_folder(char* logical_pwd)
{
        char* pwd_end = logical_pwd + strlen(logical_pwd) - 1;
        while(*pwd_end != '/') pwd_end--;
        // on ne supprime pas la racine
        if(get_number_of_directories(logical_pwd) > 1) *(pwd_end) = '\0';
        else strcpy(logical_pwd, "/");
        return logical_pwd;
}

// permet de parser chemin sans appeler realloc
int get_number_of_directories(char* path)
{
    // référence absolue, on part de 0
	int number_of_directories = 0;
    // référence relative, on part de 1
    if(*path != '/') number_of_directories++;

    int i = 0;
	for(; *path != '\0'; i++)
	{
        // ne pas incrémenter pour les / successifs (à ajouter)
		if(*path == '/')
		{
			number_of_directories++;
		}
		path++;
	}
    // si la référence se termine par un /, on a compté un répertoire de trop
    if(*(path - 1) == '/' && i > 0) number_of_directories--;

	return number_of_directories;
}

//Permet de parser un chemin en un tableau de noms de dossiers
char** path_parser(char* path)
{
    int number_of_directories = get_number_of_directories(path);
    if(number_of_directories == 0)
    {
        return NULL;
    }
    
    //si le path ne commence pas par / on l'ajoute au début (pour que strtok fonctionne correctement)
    //on ajoute 2 pour le caractère / qu'on rajoute + le null terminator (pas pris en compte par strlen)
    char* path_cursor = malloc(2 + strlen(path));
    if(*path != '/') 
    {
        strcpy(path_cursor, "/");
        strcat(path_cursor, path);
    }
    else strcpy(path_cursor, path);
	char** directories =  malloc(number_of_directories * sizeof(char*));

	// Tokenisation du chemin
	int i;
	char* directory = strtok(path_cursor, "/");
	for(i = 0; directory != NULL; i++)
	{
		directories[i] = malloc(strlen(directory) + 1);
		strcpy(directories[i], directory);
		directory = strtok(NULL, "/");
	}
    
    return directories;
}

