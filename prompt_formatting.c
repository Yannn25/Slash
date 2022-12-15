#include "include/prompt_formatting.h"

//fonctions de formatage
command* command_formatting(char* user_prompt)
{
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

void free_command(command* user_command)
{
    for(int i = 0; i < user_command -> number_of_args; i++)
    {
        free(user_command -> arguments[i]);
    }
	free(user_command -> arguments);
    free(user_command);
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

// renvoie les n derniers caractères d'une chaine de caracteres
char* shorten_string(char* str, int n)
{
	// si chaine pas assez longue on renvoie la chaine telle quelle
	if(n > strlen(str)) return str;

	char* short_str = malloc(n + 1);
	char* cursor = str + strlen(str);
	int i;
	for(i = 0; i < n; i++)
	{
		cursor--;
	}
	strcpy(short_str, cursor);
	return short_str;
}

char* format_pwd(char* pwd, int pwd_max_length)
{
	char* formatted_pwd = shorten_string(pwd, pwd_max_length);
	for(int i = 0; i < 3; i++)
	{
		formatted_pwd[i] = '.';
	}

	return formatted_pwd;
}

//affichage du prompt (a compléter)
char* make_prompt(int result, char* pwd)
{
	const char* red = "\033[91m";
	const char* green = "\033[32m";
	const char* blue = "\033[34m";
	const char* default_color = "\033[00m";

	const char* start_tag = "\001";
	const char* end_tag = "\002";

	// calcul de la taille du prompt
	const char* template = "[n]$ ";
	const int template_size = strlen(template);
	const int pwd_max_length = PROMPT_LENGTH - template_size;
	int prompt_size = 0;
	//taille des tags
	prompt_size += NUMBER_OF_COLORS * 2;
	//taille des codes couleur
	prompt_size += NUMBER_OF_COLORS * strlen(default_color);
	prompt_size += template_size;
	prompt_size += pwd_max_length;

	char* formatted_pwd;
	if(strlen(pwd) > pwd_max_length) 
	{
		formatted_pwd = format_pwd(pwd, pwd_max_length);
	}
	else
	{
		formatted_pwd = malloc(pwd_max_length + 1);
		strcpy(formatted_pwd, pwd);
	}

	// creation du prompt
	char* prompt = calloc(prompt_size + 1, 1);
	strcpy(prompt, start_tag);
	// si commande précédente évaluée avec succès, couleur verte utilisée
	if(!result) strcat(prompt, green);
	// sinon, couleur rouge
	else strcat(prompt, red);
	strcat(prompt, end_tag);

	// Conversion du retour de la dernière commande en char*
	// (enlever le nbre magique)
	char result_str[3];
	sprintf(result_str, "%d", result);

	// code de retour entre crochets 
	strcat(prompt, "[");
	strcat(prompt, result_str);
	strcat(prompt, "]");

	// répertoire courant 
	strcat(prompt, start_tag);
	strcat(prompt, blue);
	strcat(prompt, end_tag);
	strcat(prompt, formatted_pwd);

	strcat(prompt, start_tag);
	strcat(prompt, default_color);
	strcat(prompt, end_tag);
	strcat(prompt, "$ "); 

	free(formatted_pwd);

return prompt;
}
