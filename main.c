

// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<signal.h>
#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported



// Greeting shell during startup
void init_shell()
{
    printf("\033[H\033[J");// refresh screnn

    char* username = getenv("USER");//get user from environment variable
    printf("\n\n\nUSER is: @%s ", username);
    printf("\n");
    sleep(1);//sleep second

    //clear();
}

// Function to take input
int takeInput(char* str)
{
    char* buf;

    buf = readline("\n>> ");// get line from user only text of the line remains
    if (strlen(buf) != 0) {
        add_history(buf); //history of line
        strcpy(str, buf);

        return 0;
    } else {
        return 1;
    }
}



// Function where the system command is executed
void execArgs(char** parsed)
{    // Forking a child
    pid_t pid = fork(); // fork make two process one for parent another for child

     if (pid == 0) { // pid for child
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        wait(NULL); //wait for child
        return;
    }
}


// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 2, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;

    ListOfOwnCmds[0] = "exit";//put build in commands in array
    ListOfOwnCmds[1] = "cd";

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
    case 1:
        printf("\n exitttt\n");
        exit(0);
    case 2:
       chdir(parsed[1]);// change dire because cd doesnot work in execvp
        //printf("%s",parsed[1]);
        return 1;

    case 3:
        username = getenv("USER"); //getuser name from environmen variables
        printf("\nHello %s Use help to know more..\n",
            username);
        return 1;
    default:
        break;
    }

    return 0;
}


// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;


    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " "); //separate words based on space skip words with zero length to avoid extra spaces

        if (parsed[i] == NULL) //argument ==null break
            break;

       if(strcmp(parsed[i],"&")==0)// to find & in firefox &
        i--;

        if (strlen(parsed[i]) == 0)
            i--; //count length of array

}


}

 int processString(char* str, char** parsed)
{   //call parseSApace


    char* strpars[2];
    int i = 0;

    if (i) {
        parseSpace(strpars[0], parsed);


    } else {

        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed))
        return 0; //builtin fun
    else
        return 1 ; //command
}
void handler(int sig){
FILE *f;
f= fopen("logfile.log","a+");//to append and crete in log file
pid_t pid;

//printf("child id %d terminated.\n", pid);
fprintf(f,"child terminated .\n");
fclose(f);

}

int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    int sig;
    init_shell();
    while (1) {
        // print shell line

        // take input
        if (takeInput(inputString))
            continue;
        // process
        signal(SIGCHLD,handler);
        execFlag = processString(inputString,
        parsedArgs);
        // execflag returns zero if there is no command
        // or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.

        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);


    }
    return 0;
}


