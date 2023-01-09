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
char old_logical_pwd[MAX_PWD_LENGTH] = {0};
char logical_pwd[MAX_PWD_LENGTH] = {0};

int main(void) 
{
	// initialisation
	int proc;
	rl_outstream = stderr;
	strcpy(old_logical_pwd, getenv("OLDPWD"));
	strcpy(logical_pwd, getenv("PWD"));

	// boucle principale
	while(1)
	{
		// Prompt Slash
		char* slash_prompt = make_prompt(last_return_value, logical_pwd);
		char* user_prompt;
		// tant qu'on nous fournit des lignes blanches, on continue à afficher le prompt
		do
		{
			// Si la commande est vide (Ctrl - D)
			if((user_prompt = readline(slash_prompt)) == NULL)
			{
				free(slash_prompt);
				exit(0);
			}
		}while((strlen(erase_whitespaces(user_prompt)) == 0));
		add_history(user_prompt);

		//On vérifie si l'utilisateur a saisi une commande interne
		command* user_command = command_formatting(user_prompt);
		free(slash_prompt);
		free(user_prompt);
		// Lancement de la commande utilisateur
		last_return_value = execute(user_command);

		//Sinon, on vérifie si l'utilisateur a saisi une commande externe
		//(à mettre plutot dans la fct execute)
		/*if((proc = fork()) == 0) {
			last_return_value = execlp(user_command -> arguments[0], user_command -> arguments[0],NULL);
            free(user_command);
		}*/
		

		// si ni l'un ni l'autre, message d'erreur et on boucle
	}

return 0;
}
