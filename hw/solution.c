#include<string.h>
/* 
 * print_file_list()
 */
void print_file_list() {

    pid_t pid;

    pid = fork();
    
    if(pid == -1)
    {
        perror("fork():");
        exit(1);
    }
    if(pid == 0)                                 //make one keiki
    {
        char* const args[] = {"ls", "-l", NULL};    //create a null terminated array of args for execv
        execv("/bin/ls", args);                     //execute the ls command
    }

    
    int exit_code;                                          //exit
    if (waitpid(pid, &exit_code, 0) == -1)                  //wait for the process ID of the child to finish
    {
        perror("waitpid()");                                //if theres an error
        exit(1);                                            //exit
    }
    
    
    fprintf(stdout, "\n\n** Command successful **\n\n"); 


}


/*
 * cat_file(char *file)
 * 	file: the name of the file to view
 */
void cat_file(char *file) {


    if(access(file, F_OK) == 0)                 //if the file exists
    {
        pid_t pid;                              //create a process ID
        
        pid = fork();                           //make a bebeh
        
        if(pid == 0)                            //if its the bebeh
        {
        
            char* const arguments[] = {"cat", file, NULL}; //null terminated list of args
            execv("/bin/cat", arguments);                  //execute
        
        }

    
    int exit_code;                                          //exit
    if (waitpid(pid, &exit_code, 0) == -1)                  //wait for the process ID of the child to finish
    {
        perror("waitpid()");                                //if theres an error
        exit(1);                                            //exit
    }
        fprintf( stdout, "\n\n*** Command successful ***\n\n");
    }
    else
    {
        
        fprintf( stdout, "\n\n*** Command failed ***\n\n");
    }

}




/*
 * count_pattern(char *file, char *pattern)
 * 	file: the file in which to search for a pattern
 * 	pattern: the pattern
 */
void count_pattern(char *file, char *pattern) {

    
    pid_t pid, pid1;                                            //create two process IDs
    int pipefd[2];                                              //create a pipe for two commands
    
    char *gargs[] = {"grep", pattern, file, NULL};              //arguments for the commands
    char *wargs[] = {"wc", "-l", NULL};

    pipe(pipefd);                                               //create a unidirectional pipe that takes the arguments

    pid = fork();                                               //fork
    if(pid == 0)                                                //child
    {
        dup2(pipefd[1], STDOUT_FILENO);                         //dup2 uses the file number specifed in this case that of STDOUT_FILENO
        close(pipefd[0]);                                       //close the left end of the pipe

        execv("/bin/grep", gargs); //execute that piped command
        
    
    }

    pid1 = fork();                                              //
    
    if(pid1 == 0)
    {
    
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);

        execv("/usr/bin/wc", wargs);

    }

    
    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);    
    wait(NULL);

    
    if(access(file, F_OK) == 0)
    {
        puts("\n\n** Command Sucessful **\n");
    }
    else
    {
        puts("\n\n** Command Failed **\n");
    }

}
/*
 * filter_pattern(char *file, char *pattern, char *dst)
 * 	file: the file in which to search for a pattern
 * 	pattern: the pattern
 * 	dst: the output file
 */
void filter_pattern(char *file, char *pattern, char *dst) {



    if(access(file, F_OK) == 0)
    {
    
        FILE *fp = fopen(dst, "w");
        pid_t pid;
        pid = fork();

        if(pid == 0)
        {
            close(1);
            dup(fileno(fp));
            char* const args[] = {"grep", pattern, file, NULL};
            execv("/bin/grep", args);
        }


    
        int exit_code;
        if (waitpid(pid, &exit_code, 0) == -1)
        {
            perror("waitpid()");
            exit(1);
        }
    
        puts("\n\n ** Command Successful** \n\n");
    }
    else
    {
        puts("\n\n ** Command Failed **\n\n");
    }

}



/*
 * replace_pattern(char *file, char *src_pattern, char *dst_pattern)
 * 	file: the file in which to search for a pattern
 * 	src_pattern: the pattern to replace
 * 	dst_pattern: the pattern to replace it with
 */
void replace_pattern(char *file, char *src_pattern, char *dst_pattern) {


    char str[90];
    strcpy(str, "s/");              //Concatenate the string so that it can
    strcat(str, src_pattern);       //be passed as the second argument of 
    strcat(str, "/");               //sed
    strcat(str, dst_pattern);
    strcat(str, "/g\0");  

    pid_t pid, pid1;                //create two process IDs for the forks
    
    
    pid = fork();                   //create a forked process
    
    if(pid == 0)                    //in the child process
    {   
        
        
        close(1);                                           //close stdout
        FILE *fp = fopen("/tmp/sedstuff", "w");             //open a /tmp/ file
        char* const args[] = {"sed", str, file, NULL};      //create a list of arguments to pass to sed
        execv("/bin/sed", args);                            //execute the sed process image
    }
    
    int exit_code;                                          //exit
    if (waitpid(pid, &exit_code, 0) == -1)                  //wait for the process ID of the child to finish
    {
        perror("waitpid()");                                //if theres an error
        exit(1);                                            //exit
    }

    pid1 = fork();                                          //create a new forked process

    if(pid1 == 0)                                           //in the child
    {
        char* const args[] = {"mv", "/tmp/sedstuff", file, NULL}; //use the mv command with the /tmp/file as the src and file as dst
        execv("/bin/mv", args);                                 //execute the mv process
    
    }
    
    int exit_code1;                                         
    if (waitpid(pid1, &exit_code1, 0) == -1)            //wait for the keeds
    {
        perror("waitpid()");
    }

}

