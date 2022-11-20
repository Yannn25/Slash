#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "include/prompt_formatting.h"
#include "include/internal_functions.h"
#include "include/global.h"

// paramètrage des variables globales
int last_return_value = 0;
char logical_pwd[MAX_PWD_LENGTH] = {0};

int main(void) 
{
	// initialisation
	int proc;
	rl_outstream = stderr;
	strcpy(logical_pwd, getenv("PWD"));

	// boucle principale
	while(1)
	{
		// Prompt Slash
		char* prompt = readline("$ ");
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

