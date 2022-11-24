#ifndef PATH_FORMATTING_H
#define PATH_FORMATTING_H

//bibliothèques requises
#include <string.h>
#include <stdlib.h>
#include "global.h"

// manipulation chemins
char* update_logical_pwd(char** parsed_reference, int number_of_entries, char* logical_pwd, char* new_logical_pwd);
char* add_folder(char* logical_pwd, char* reference);
char* delete_last_folder(char* logical_pwd);
int get_number_of_directories(char* path);
char** path_parser(char* path);

#endif