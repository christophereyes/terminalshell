#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <curses.h>
#include <sys/stat.h>

int clean_stdin()
{
    while(getchar()!='\n');
    return 1;
}

int main(void)
{
    pid_t child; /*data type process ID is created called child*/
    DIR * d;    /*a pointer pointing to a directory stream*/
    struct dirent * de; /*de is the same as d from the line above*/
    int i,c,errnum,k,direct,w;
    char s[1024],cmd[1024]; /*fixed length buffers*/
    char endN;
    struct stat buf;//this structure is used in obtaining file information with the system call stat
    time_t t; /*t is an integer value representing time since   UTC */

    while (1){
        t = time(NULL); /*seconds assigned to t*/
        printf("Time: %s\n", ctime( &t ));//ctime is used in order
        getcwd(s, FILENAME_MAX);//by using header file limits.h this should handle buffer overflow
        if ( errno == ERANGE)//this error check should handle errors due to length
        {
            printf("Directory Name is too long!");
        }
        else
        {
            printf("Current Directory: %s \n", s);//
        }
        d = opendir(".");
        c = 0;
        i = 0;
        printf("Directories: \n");

        while((de = readdir(d)))
        {//opening directories until reach end of directory
            if ((de->d_type) & DT_DIR)//checks to see that current type is directory
            {
                printf("\t\t\t %d %s \n", c++, de->d_name);
                i++;
                if ((c % 8) == 0)
                {
                    printf("Hit N for Next\nOr P for prev\n");
                    k = getchar();getchar();
                    printf("%c",k);
                    if ((k == 'N') || (k == 'n'))
                    {
                        continue;
                    }
                    else if ((k == 'P') || (k == 'p'))
                    {
                        rewinddir(d);
                        c = 0;
                        continue;
                    }
                    else
                    {
                        printf("OK then\n");
                        break;
                    }
                }
            }
        }
        closedir(d);
        d = opendir(".");
        c = 0;
        printf("\nFiles: \n");
        while((de = readdir(d)) != NULL )
        {
            if(((de->d_type) & DT_REG))//checks to see that current type is a file
            {
                printf("\t\t\t %d %s\n",c++, de->d_name);
                if( (c % 8)==0)
                {
                    printf("Hit N for Next\n");
                    k = getchar();getchar();
                    printf("%c",k);
                    if((k == 'N') || (k == 'n'))
                    {
                        continue;
                    }
                    else if ((k == 'P') || (k == 'p'))
                    {
                        rewinddir(d);
                        c = 0;
                        continue;
                    }
                    else
                    {
                        printf("OK then\n");
                        break;
                    }
                }
            }
        }
            closedir(d);
            printf( "-------------------\n");
            printf("Operations:\n");
            printf("E\tEdit\nR\tRun\nC\tChange Directory\nQ\tQuit\nI\tInfo\n");
            printf(">> ");
            c = getchar(); //grabbing entry for whatever value user enters
            switch(c)
            {
                case 'Q':
                case 'q': exit(0);
                case 'E':
                case 'e': printf("Edit what?:");
                          scanf("%s",s);
                          strcpy(cmd, "vi ");
                          strcat(cmd, s);
                          system(cmd);
                          break;
                case 'R':
                case 'r': printf("Run what?:");
                          scanf("%s",cmd);
                          //printf("%s is%s executable.\n", cmd, stat(cmd, &buf) == 0 && buf.st_mode & S_IXUSR ? "" : "not");
                          system(cmd);
                          perror("Error: ");
                          break;
                case 'C':
                case 'c': do
                          {
                            printf("\nEnter a valid number ");
                          }while(((scanf("%d%c", &direct, &endN)!= 2 || endN != '\n') && clean_stdin()));
                          d = opendir(".");
                          printf("Number entered: %d\n", direct);
                          for(de = readdir(d); direct > 0 ; direct--)
                          {
                              printf("before de_name: %s\n",de->d_name);
                              (de = readdir(d));
                              printf("after de_name:%s\n",de->d_name);
                          }
                          if(((de->d_type) & DT_REG))
                          {
                              printf("Directory :%s\n",de->d_name);
                          }
                          if(chdir(de->d_name) != 0){
                              perror("");
                          }
                          closedir(d);
                          break;
                case 'I':
                case 'i': printf("File name?:");
                          scanf("%s", s);
                          stat(s,&buf);
                          printf("st_mode = %o\n",buf.st_mode);
                          printf("File Size: \t\t%lld bytes\n",buf.st_size);
                          printf("File Permissions: \t");
                          printf( (S_ISDIR(buf.st_mode)) ? "d" : "-");
                          printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
                          printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
                          printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
                          printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
                          printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
                          printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
                          printf( (buf.st_mode & S_IROTH) ? "r" : "-");
                          printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
                          printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
                          break;
            }
        }
    }