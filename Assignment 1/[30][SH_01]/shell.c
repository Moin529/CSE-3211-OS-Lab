#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

#define SHELL_MAX_INPUT 1024
#define MAX_ARGS 100

void parse_input(char *input, char **args)
{
    int i = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

void cmd_pwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd error");
}

// Handles 'ls -a' argument
void cmd_ls(char **args)
{
    int show_hidden = 0;
    
    if (args[1] != NULL) {
        if (strcmp(args[1], "-a") == 0) show_hidden = 1;
        else {
            printf("ls: currently only supports '-a' manually. Use external ls for more flags.\n");
        }
    }

    DIR *dir = opendir(".");
    struct dirent *entry;

    if (dir == NULL)
    {
        perror("ls error");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (!show_hidden && entry->d_name[0] == '.') 
            continue;
        
        printf("%s  ", entry->d_name);
    }
    printf("\n");
    closedir(dir);
}

void cmd_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }

    if (chdir(args[1]) != 0)
        perror("cd error");
}

// multiple arguments (mkdir dir1 dir2)
void cmd_mkdir(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "mkdir: missing directory name\n");
        return;
    }

    for (int i = 1; args[i] != NULL; i++) {
        if (mkdir(args[i], 0777) != 0)
            perror("mkdir error");
    }
}

// touch file1 file2
void cmd_touch(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "touch: missing file name\n");
        return;
    }

    for (int i = 1; args[i] != NULL; i++) {
        int fd = open(args[i], O_CREAT | O_WRONLY, 0666);
        if (fd < 0)
            perror("touch error");
        else
            close(fd);
    }
}

// rm file1 file2
void cmd_rm(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "rm: missing file name\n");
        return;
    }

    for (int i = 1; args[i] != NULL; i++) {
        if (remove(args[i]) != 0)
            perror("rm error");
    }
}

void cmd_cp(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "cp: source or destination missing\n");
        return;
    }

    FILE *src = fopen(args[1], "rb");
    if (src == NULL)
    {
        perror("cp: source error");
        return;
    }

    FILE *dst = fopen(args[2], "wb");
    if (dst == NULL)
    {
        fclose(src);
        perror("cp: destination error");
        return;
    }

    int ch;
    while ((ch = fgetc(src)) != EOF)
        fputc(ch, dst);

    fclose(src);
    fclose(dst);
}

void cmd_mv(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "mv: source or destination missing\n");
        return;
    }

    if (rename(args[1], args[2]) != 0)
        perror("mv error");
}

// concatenating multiple files (cat file1 file2)
void cmd_cat(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "cat: missing file name\n");
        return;
    }

    for (int i = 1; args[i] != NULL; i++) {
        FILE *fp = fopen(args[i], "r");
        if (fp == NULL)
        {
            perror("cat error");
            continue;
        }

        int ch;
        while ((ch = fgetc(fp)) != EOF)
            putchar(ch);

        fclose(fp);
    }
}

void cmd_echo(char **args)
{
    int i = 1;
    while (args[i] != NULL)
    {
        printf("%s", args[i]);
        if (args[i + 1] != NULL)
            printf(" ");
        i++;
    }
    printf("\n");
}

void run_command(char **args)
{
    if (strcmp(args[0], "pwd") == 0) cmd_pwd();
    else if (strcmp(args[0], "ls") == 0) cmd_ls(args);
    else if (strcmp(args[0], "mkdir") == 0) cmd_mkdir(args);
    else if (strcmp(args[0], "touch") == 0) cmd_touch(args);
    else if (strcmp(args[0], "rm") == 0) cmd_rm(args);
    else if (strcmp(args[0], "cp") == 0) cmd_cp(args);
    else if (strcmp(args[0], "mv") == 0) cmd_mv(args);
    else if (strcmp(args[0], "cat") == 0) cmd_cat(args);
    else if (strcmp(args[0], "echo") == 0) cmd_echo(args);
    else
    {
        // Error Handling
        pid_t pid = fork();
        if (pid == 0)
        {
            execvp(args[0], args);
            fprintf(stderr, "%s: command not found or invalid\n", args[0]);
            exit(1);
        }
        else if (pid > 0)
        {
            waitpid(pid, NULL, 0);
        }
        else
        {
            perror("fork error");
        }
    }
}

int main()
{
    char input[SHELL_MAX_INPUT];
    char *args[MAX_ARGS];

    while (1)
    {
        printf("CSEDU_Shell> ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        if (strlen(input) == 1)
            continue;

        parse_input(input, args);

        if (args[0] == NULL)
            continue;

        if (strcmp(args[0], "exit") == 0)
            break;

        // Background execution (&)
        int bg = 0;
        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "&") == 0) {
                bg = 1;
                args[i] = NULL; 
                break;
            }
        }

        if (args[0] == NULL) continue; 

        // I/O Redirection (<, >)
        int redirect_in = 0, redirect_out = 0;
        char *file_in = NULL, *file_out = NULL;
        
        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "<") == 0) {
                redirect_in = 1;
                file_in = args[i+1];
                args[i] = NULL;
            } else if (strcmp(args[i], ">") == 0) {
                redirect_out = 1;
                file_out = args[i+1];
                args[i] = NULL;
            }
        }
        int saved_stdout = dup(STDOUT_FILENO);
        int saved_stdin = dup(STDIN_FILENO);

        // Apply Redirections
        if (redirect_in && file_in) {
            int fd0 = open(file_in, O_RDONLY);
            if (fd0 < 0) {
                perror("Input redirection error");
                goto restore_io;
            }
            dup2(fd0, STDIN_FILENO);
            close(fd0);
        }
        
        if (redirect_out && file_out) {
            int fd1 = open(file_out, O_CREAT | O_WRONLY | O_TRUNC, 0666);
            if (fd1 < 0) {
                perror("Output redirection error");
                goto restore_io;
            }
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
        }

        if (strcmp(args[0], "cd") == 0) {
            cmd_cd(args);
        } else {
            if (bg) {
                // Background Process
                pid_t pid = fork();
                if (pid == 0) {
                    run_command(args);
                    exit(0);
                } else if (pid > 0) {
                    printf("[Running in background] PID: %d\n", pid);
                } else {
                    perror("Background fork error");
                }
            } else {
                // Foreground Process
                run_command(args);
            }
        }

restore_io:
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
    }
    
    return 0;
}