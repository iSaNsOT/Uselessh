#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define USLSH_TOK_BUFSIZE 64
#define USLSH_TOK_DELIM " \t\r\n\a"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

// ----------------------------------- B COMMANDS -----------------------------------

/*
  Function Declarations for builtin shell commands:
 */
int uslsh_cd(char **args);
int uslsh_help(char **args);
int uslsh_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &uslsh_cd,
    &uslsh_help,
    &uslsh_exit
};

int uslsh_num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int uslsh_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "uslsh: expected argument to \"cd\"\n");
    } 
    else{
        if(chdir(args[1]) != 0){
            perror("uslsh");
        }
    }
    return 1;
}

int uslsh_help(char **args){
    int i;
    printf("Uselessh\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for(i = 0; i < uslsh_num_builtins(); i++){
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int uslsh_exit(char **args){
    return 0;
}

// ----------------------------------- END OF BCOMMANDS -----------------------------------



char **uslsh_split_line(char *line){
    int bufsize = USLSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if(!tokens){
        fprintf(stderr, "uslsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, USLSH_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufsize){
            bufsize += USLSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(!tokens){
                fprintf(stderr, "uslsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, USLSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}


char *uslsh_read_line(void){
    char *line = NULL;
    size_t bufsize = 0; // have getline allocate a buffer for us

    if(getline(&line, &bufsize, stdin) == -1){
        if(feof(stdin)){
            exit(EXIT_SUCCESS);  // We recieved an EOF
        }
        else{
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}



int uslsh_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        // Child process
        if(execvp(args[0], args) == -1){
            perror("uslsh");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0){
        // Error forking
        perror("uslsh");
    } 
    else{
        // Parent process
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


int uslsh_execute(char **args){

    if(args[0] == NULL){
        // An empty command was entered.
        return 1;
    }

    for(int i=0; i < uslsh_num_builtins(); ++i){
        if(strcmp(args[0], builtin_str[i]) == 0) return (*builtin_func[i])(args);
    }

    return uslsh_launch(args);
}



char *uslsh_getuser(void){
    char *user = NULL;
    
    if((user = getlogin()) == NULL){
        fprintf(stderr, "uslsh: username too large or unable to find\n");
        exit(EXIT_FAILURE);
    }

    return user;
}

char *uslsh_getdir(void){
    char *dir = NULL;
    size_t bufsize = 2056;
    
    if(getcwd(dir, bufsize) == NULL){
        fprintf(stderr, "uslsh: path too large or unable to find\n");
        exit(EXIT_FAILURE);
    }

    return dir;
}


void uslsh_loop(void){
    char *line;
    char **args;
    int status;
    char *user;
    char *dir;

    do{
        user = uslsh_getuser();
        dir = uslsh_getdir();
        printf(YEL "%s:" RESET, user);
        printf(BLU "%s" RESET, dir);
        printf("> ");
        line = uslsh_read_line();
        args = uslsh_split_line(line);
        status = uslsh_execute(args);

        free(line);
        free(args);
    }while (status);

    free(user);
    free(dir);
}

int main(int argc, char **argv){
    // Load config files, if any.

    // Run command loop.
    uslsh_loop();

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}