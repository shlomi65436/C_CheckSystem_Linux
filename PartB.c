#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    truncate("grades.csv", 0);
    if(argc != 2){
    printf("Argument error");
    return -1;
    }
    char allstudents[256];
    int confd = open(argv[1], O_RDONLY);
    int i = 0;
    char ch;
    while(read(confd, &ch ,1) > 0){
        if(ch == '\n'){
                allstudents[i] = '\0';
        }else{
                allstudents[i] = ch;
                i++;
        }
    }
    printf("Read Config:\n");
    printf("%s\n", allstudents);
    close(confd);
    
    // Read the name of the studs
    char* arr[] = {"ls", "allstudents", NULL};
    int fdchild = fork();
    if(fdchild == -1){
        printf("fork 1 failed");
        exit(-1);
    }else if(fdchild == 0){
    int studlist = open("studlist.txt", O_WRONLY | O_CREAT, 0666);
    close(1);
    if(studlist == -1){
        printf("open studlist failed");
        exit(-1);
    }
    dup(studlist);
    close(studlist);
    execvp("ls" ,arr);
    }

    int status;
    wait(&status);

    // Runs the Students Main
    int studlist2 = open("studlist.txt", O_RDONLY);
    char cha;
    char Studentname[256];
    i = 0;
    while(read(studlist2, &cha, 1) > 0){ //read name
        if(cha != '\n'){
            Studentname[i] = cha;
            i++;
        }else if(cha == '\n'){
                Studentname[i] = '\0';
                i=0;
                int fdchild = fork();
                if(fdchild == -1){
                    printf("fork failed");
                }else if(fdchild == 0){
                    char buffer[300];
                    snprintf(buffer,sizeof(buffer), "./allstudents/%s/main.exe", Studentname);
                    int fdin = open("input.txt", O_RDONLY, 0666);
                    if(fdin < 0){
                        printf("cant open input.txt");
                        return -1;
                    }
                    char *space = " ";
                    char input1[25];
                    char input2[25];
                    int counterin = 0;

                    do{ //read argument 1
                    counterin++;
                    read(fdin,&input1[counterin - 1],1);
                    }while(input1[counterin - 1] != *space);
                    input1[counterin - 1] = '\0';

                    counterin = 0;

                    do{ // read argument 2
                    counterin++;
                    read(fdin,&input2[counterin - 1],1);
                    }while(input2[counterin - 1] != '\0');

                    char* arrs[] = {buffer, input1, input2, NULL};
                    close(1);
                    int fdout = open("studentOutput.txt", O_WRONLY | O_CREAT, 0666);
                        if(fdout == -1){
                            printf("output error");
                        }
                        
                    int x = execvp(arrs[0], arrs);// kill child 1
                        if(x == -1){
                            printf("execvp error");
                        }
                }
                int status2;
                wait(&status2);

                    // comparing
                int fdchild2 = fork();
                if(fdchild2 == -1){
                    printf("fork failed");
                    return -1;
                }
                else if(fdchild2 == 0){
                    char* arra[] = {"./comp.out", "expectedOutput.txt", "studentOutput.txt", NULL};
                    execvp(arra[0], arra); // kill child 2
                    }
                    
                    int status3;
                    wait(&status3);

                    int result = WEXITSTATUS(status3);
                    printf("%s status: %d\n",Studentname ,result);
                    if(result == 2){
                        char t = open("grades.csv", O_WRONLY | O_APPEND);
                        char buff[300];
                        snprintf(buff,sizeof(buff),"%s 100\n",Studentname);
                        int count = 0;
                        while(buff[count] != '\0'){
                            count++;
                        }
                        write(t,buff, count);
                        truncate("studentOutput.txt", 0);
                    }else{
                        int t = open("grades.csv", O_WRONLY | O_APPEND);
                        char buff[300];
                        snprintf(buff,sizeof(buff),"%s 0\n",Studentname);
                        int count = 0;
                        while(buff[count] != '\0'){
                            count++;
                        }
                        write(t,buff, count);
                        truncate("studentOutput.txt", 0);
                    }
        }// per student
    }// finish all students
    truncate("studlist.txt", 0);
    close(studlist2);
    exit(0);
}
