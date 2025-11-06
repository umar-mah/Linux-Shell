/*
project: 01
author: Muhammad Umar 
email: mumar2@umbc.edu
student id: NZ53290
due date: 2/28/21 11:59:59 pm
description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"

/*
In this project, you are going to implement a number of functions to 
create a simple linux shell interface to perform basic linux commands
*/
#define HISTORY_FILE ".421sh"
#define MAX_LINES 10
/* 
DEFINE THE FUNCTION PROTOTYPES
user_prompt_loop()
get_user_command()
parse_command()
execute_command()
*/
void user_prompt_loop();
char* get_user_command();
char **parse_command(char *input);
void execute_command(char **command);
void free_parsed_command(char **parsedCommand);
void save_to_history(const char *command);
void display_history();
void read_proc_file(const char *file_path);

int main(int argc, char **argv)
{
    /*
    Write the main function that checks the number of arguments passed to ensure 
    no command-line arguments are passed; if the number of arguments is greater 
    than 1 then exit the shell with a message to stderr and return a value of 1.
    Otherwise, call the user_prompt_loop() function to get user input repeatedly 
    until the user enters the "exit" command.
    */
    if (argc > 1) {
        fprintf(stderr, "Error: This shell will not accept command-line arguments.\n");
        return 1;
    }
    FILE *file = fopen(HISTORY_FILE, "w");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    fclose(file);
    user_prompt_loop();
    exit(0);
    return 0;
}

#define BUFFER_SIZE 1024

    /*
    read_proc_file(const char *file_path):
    Reads the proc file and prints the required data
    */
void read_proc_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
   
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file)) {
        printf("%s", buffer);
    }
   
    fclose(file);
}
/*
user_prompt_loop():
Get the user input using a loop until the user exits, prompting the user for a command.
Gets the command and sends it to a parser, then compares the first element to the two
different commands ("/proc" and "exit"). If it's none of these commands, 
send it to the execute_command() function. If the user decides to exit, then exit(0)
or exit with the user given value.
*/
void user_prompt_loop()
{
    printf("$ ");
    fflush(stdout);
   
    char *userInput = get_user_command();
    // Looping userInput as long as it is not NULL
    while (userInput != NULL) {
        char **parseInput = parse_command(userInput);
        // Empty command
        if (parseInput[0] != NULL) {
            // Exit command
            if (strcmp(parseInput[0], "exit") == 0) {
                // Free allocated memory
                for (int i = 0; parseInput[i] != NULL; i++) {
                    free(parseInput[i]);
                }
                free(parseInput);
                free(userInput);
                exit(0);
            }
            // Proc Command
            else if (strncmp(parseInput[0], "/proc", 5) == 0) {
            	save_to_history(userInput);
                read_proc_file(parseInput[0]);
            }
       	    else if (strcmp(parseInput[0], "history") == 0) {
                display_history();
            }
            else {
            	save_to_history(userInput);
                execute_command(parseInput);
            }
        }
       
        //  Deallocating memory for this iteration
        for (int i = 0; parseInput[i] != NULL; i++) {
            free(parseInput[i]);
        }
        free(parseInput);
        free(userInput);
       
        printf("$ ");
        fflush(stdout);
        // Next Command
        userInput = get_user_command();
    }
}

/*
get_user_command():
Take input of arbitrary size from the user and return it to user_prompt_loop()
*/
#define INITIAL_BUFFER_SIZE 256

char* get_user_command()
{
    /*
    Functions you may need: 
        malloc(), realloc(), getchar(), or any other similar functions.
    */
    char *buffer = malloc(INITIAL_BUFFER_SIZE);
    if (!buffer) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    
    size_t size = INITIAL_BUFFER_SIZE;
    size_t length = 0;
    int c;
    
    while ((c = getchar()) != '\n' && c != EOF) {
        buffer[length++] = c;
        
        if (length >= size) {
            size = size * 2;
            char *new_buffer = realloc(buffer, size);
            if (!new_buffer) {
                free(buffer);
                perror("realloc failed");
                exit(EXIT_FAILURE);
            }
            buffer = new_buffer;
        }
    }
    buffer[length] = '\0';
    
    // EOF encountered without any input, input is set to NULL
    
    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }
    return buffer;
}

/*
parse_command():
Take command grabbed from the user and parse it appropriately.
Example: 
    user input: "ls -la"
    parsed output: ["ls", "-la", NULL]
Example: 
    user input: "echo     hello                     world  "
    parsed output: ["echo", "hello", "world", NULL]
*/


char **parse_command(char *input) {
    size_t buffersize = INITIAL_BUFFER_SIZE;
    size_t index = 0;
    char **splitInput = malloc(buffersize * sizeof(char *));
    if (!splitInput) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    char *inputCopy = malloc(strlen(input) + 1); // Allocating memory to hold the memory coming from user_promp_loop
    if (!inputCopy) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    strcpy(inputCopy, input); // Copying input coming from user_prompt_loop

    char *tempInput = strtok(inputCopy, " \t\r\n");
    while (tempInput != NULL) {
        if (index == buffersize) {
            buffersize *= 2; // Resizing buffer size to hold large commands
            char **newSplitInput = realloc(splitInput, buffersize * sizeof(char *));
            if (!newSplitInput) {
                perror("realloc failed");
                exit(EXIT_FAILURE);
            }
            splitInput = newSplitInput;
        }

        splitInput[index] = malloc(strlen(tempInput) + 1);
        if (!splitInput[index]) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        strcpy(splitInput[index], tempInput);
        index++;
        tempInput = strtok(NULL, " \t\r\n"); // Seperating input 
    }
    splitInput[index] = NULL;

    free(inputCopy);

    return splitInput;
}

void free_parsed_command(char **parsedCommand) {
    for (size_t i = 0; parsedCommand[i] != NULL; i++) {
        free(parsedCommand[i]);
    }
    free(parsedCommand);
}
/*
execute_command():
Execute the parsed command if the command is neither /proc nor exit;
fork a process and execute the parsed command inside the child process.
*/
void execute_command(char **command)
{
    /*
    Functions you may need: 
        fork(), execvp(), waitpid(), and any other useful function.
    */
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return;
    }
    
    if (pid == 0) {
        if (execvp(command[0], command) == -1) {
            perror("Execution Failed");
        }
        exit(EXIT_FAILURE);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
    }
}

    /*
    save_to_history(const char *command):
    Saves the commands into the history fike
    */

void save_to_history(const char *command) {
    FILE *file = fopen(HISTORY_FILE, "a");
    if (!file) {
        perror("Failed to open history file.");
        return;
    }
    fprintf(file, "%s\n", command);
    fclose(file);
}

   /*
   display_history():
    Displays last 10 commands in the history file.
    */
void display_history() { 
	FILE *file = fopen(HISTORY_FILE, "r"); 
	if (!file) { 
		perror("Failed to open history file"); 
		return; 
	} 
	char lines[MAX_LINES][BUFFER_SIZE]; // 2D Array to store last 10 commands
	int count = 0;
	int index = 0; 
	while (fgets(lines[index], BUFFER_SIZE, file)) { 
		index = (index + 1) % MAX_LINES; // Moves index circularly 
		if (count < MAX_LINES) { 
		count++; 
		} 
	} 
	fclose(file);
	int start = count < MAX_LINES ? 0 : index; // Starting index changes
	for (int i = 0; i < count; i++) { 
		printf("%s", lines[(start + i) % MAX_LINES]); 
	} 
}
