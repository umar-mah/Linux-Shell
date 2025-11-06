# Simple Linux Shell ('temp_shell.c')

## Project 01  
- Author: Muhammad Umar 
- Email: mumar2@umbc.edu 
- Student ID: NZ53290 

## Description  
This project implements a simple Linux shell capable of executing basic Linux commands. It supports command execution, a history feature, and reading from the `/proc` filesystem.

## Features  
- Execute basic Linux commands (e.g., `ls`, `cat`, `echo`) 
- Maintain a command history (last 10 commands) 
- Read and display `/proc` filesystem content 
- Exit the shell with `exit` 

## Hurdles  

- Understanding all the parts in general. 
- Coding parse command as I kept getting memory leaks or segmentation errors.
- Figuring out what libraries I can use
- Coding the history feature as I was able to print all the history commands not just the last 10. Eventually, I was able to figure it out.
- Generally, keeping copies of my code.
- And the list goes on. But who likes to complain?

## Some Function Explanations
`get_user_command()`
- This function reads a line of user input from th input dynamically, ensuring that it can handle long input efficiently. Initially, it allocates memory using malloc() with a predefined `INITIAL_BUFFER_SIZE`. It then reads characters one by one using getchar(), storing them in the allocated buffer. If the buffer size is exceeded, it dynamically expands the memory using realloc(). The function ensures that the input string is properly null-terminated. If an EOF is encountered before any input is received, it frees the allocated memory and returns NULL to indicate an empty input.

`parse_command()`
- This function processes the input string and seperates the input into partitions. It begins by allocating memory for the array of strings and a copy of the input to avoid modifying the original. Using strtok(), it splits the input based on whitespace characters such as spaces and tabs. The function dynamically resizes the array using realloc() if necessary, ensuring it can handle a variable number of arguments. Each partition is copied into a separately allocated string, and the last element of the array is set to NULL for compatibility with execvp().

`execute_command()`
- This function handles the execution of parsed commands by creating a child process using fork(). If fork() fails, an error message is printed. In the child process, execvp() is called to replace the process with the specified command, and if it fails, an error message is displayed before the child process exits. Meanwhile, the parent process waits for the child process to complete using waitpid().

`display_history()`
- This function displays the last 10 commands from the history file. It opens the file in read mode ("r") and stores commands in a circular buffer to keep track of the most recent entries. Using fgets(), it reads each line into a buffer and determines the correct starting index to display the last 10 commands in order. If the file cannot be opened, an error message is shown.

## Things to know

- In the function display_history(), the BUFFER_SIZE dictates the maximum number of characters that can be stored for each individual line of history read from the file. The BUFFER_SIZE is set as 1024 bytes in my program. Any command greater than 1024 bytes might not be read correctly.


## How to Compile
### Enter the Project Directory

`make`

## How to Run
### Enter the Project Directory

`make run`

## Checking Memory Leaks
### Enter the Project Directory

`valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./simple_shell`
