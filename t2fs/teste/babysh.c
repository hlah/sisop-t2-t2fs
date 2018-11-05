#include "t2fs.h"
#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 10

int process_command(char * command, char** argv, int max_args) {
	int i = 0;
	int j = 0;
	int r = 0;
	int args = 0;

	do {
		// ignora espaços em branco
		while( command[i] == ' ' || command[i] == '\t' || command[i] == '\n' )
			i++;

		if( command[i] == '\0' ) {
			break;
		}

		// procura fim  
		j = i;
		while( command[j] != ' ' && command[j] != '\t' && command[j] != '\n' && command[j] != '\0' )
			j++;
		command[j] = '\0';

		// realloca argumento
		strcpy( command + r, command + i);
		argv[args] = command + r;
		r += j-i+1;
		args++;

		i = j + 1;
	} while( command[i] != '\0' && args < max_args );


	return args;
}

void ll(char* path) {

	DIR2 dir = opendir2(path);

	if( dir < 0 ) {
		printf("Could not open directory '%s'\n", path);
	}
	else {
		//printf("Directory '%s':\n", path);
		DIRENT2 dir_entry;
		while( readdir2( dir, &dir_entry ) == 0 ) {
			printf("%d\t%s\t%d\n", dir_entry.fileType, dir_entry.name, dir_entry.fileSize);
		}
		closedir2(dir);
	}

}

void pwd() {
	char cwd[300];
	getcwd2(cwd, 300);
	printf("%s\n", cwd);
}

void cd( char * path ) {
	if( chdir2(path) != 0 ) {
		printf("invalida path '%s'\n", path);
	}
}

void help() {
	printf("Commandos:\n");
	printf("pwd \t\t mostra diretorio de trabalho atual.\n");
	printf("cd [caminho] \t muda diretorio de trabalho para 'caminho'.\n");
	printf("ll [caminho] \t lista o coteudo do diretorio 'caminho'.\n");
	printf("help \t\t imprime ajuda.\n");
	printf("exit \t\t sai do babysh.\n");
}

void mkdir(char * pathname) {
	if( mkdir2(pathname) != 0) {
		printf("Could not create directory '%s'\n", pathname);
	}
}

int execute_command(char * command) {

	char* argv[MAX_ARGS];
	int args = process_command(command, argv, MAX_ARGS);


	//printf("%d\n", args);

	if( args > 0 ) {
		if( strcmp( argv[0], "exit" ) == 0 ) {
			return 0;
		}
		else if( strcmp( argv[0], "ll") == 0 ) {
			if( args > 1 ) {
				ll( argv[1] );
			} else {
				ll( "." );
			}
		}
		else if( strcmp( argv[0], "pwd" ) == 0 ) {
			pwd();
		} else if( strcmp( argv[0], "cd" ) == 0 ) {
			if( args > 1 ) {
				cd( argv[1] );
			} else {
				cd( "/" );
			}
		}
		else if( strcmp( argv[0], "pwd" ) == 0 ) {
			help();
		} else if( strcmp( argv[0], "mkdir" ) == 0 ) {
			if( args > 1 ) {
				mkdir( argv[1] );
			} else {
				printf("mkdir needs one argument.\n");
			}
		}
	}

	return 1;
}

int main() {

	char* user_input;
	size_t buffer_size = 256;

	user_input = malloc(256);

	printf("Bem-vindo ao babysh 0.0.1!\n");
	help();

	do {
		printf("> ");
		getline(&user_input, &buffer_size, stdin);
	} while( execute_command(user_input) );

	free(user_input);

	return 0;
}
