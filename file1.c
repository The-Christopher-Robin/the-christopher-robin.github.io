1a
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(){
int fd = open("file.txt", O_RDONLY);
int size = lseek(fd, 0, SEEK_END);
char ch;
while (size--) {
lseek(fd, size, SEEK_SET);
read (fd, &ch, 1);
putchar(ch);
}
close(fd);
return 0;
}

1b
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include <sys/wait.h>
int main(){
int fd = open("test.txt", O_RDONLY);
int pid = fork();
char buffer [10];
if(pid == 0) {
read(fd, buffer, 10);
buffer [10]='\0';
printf("Child Process %s\n", buffer);
}else{
wait (NULL);
read(fd, buffer, 10);
buffer [10] = '\0';
printf("Parent read: %s\n", buffer);
}
close(fd);
return 0;
}


2a
#include<stdio.h>
#include<sys/stat.h>
int main(){
struct stat ft;
stat("file.txt",&ft);
printf("size is %ld bytes \n",ft.st_size);
printf("no of links:%ld\n",ft.st_nlink);
printf("access time: %ld\n",ft.st_atime);
printf("file inode: %ld\n",ft.st_ino);
printf("modified time : %ld\n",ft.st_mtime);
return 0;
}

2b
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<dirent.h>
#include<sys/stat.h>

int main(){
DIR *dir;
struct dirent *entry;
struct stat fs;
dir = opendir(".");
while((entry=readdir(dir))!=NULL){
stat(entry->d_name,&fs);
if(fs.st_size==0){
printf("empty files are :%s",entry->d_name);
remove(entry->d_name);
}
}

return 0;
}




3a
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<dirent.h>
#include<sys/stat.h>
int main(){
DIR *dir = opendir(".");
struct dirent *entry;
struct stat fs;

while (entry=readdir(dir)) {
stat (entry->d_name,&fs);
if (fs.st_size==0){
printf("empty files are: %s", entry->d_name);
remove(entry->d_name);
}
}
return 0;
}

3b
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include<sys/stat.h>
int main(){
	struct dirent *dir;
	struct stat mystat;
	DIR *dp = opendir(".");
	if(dp){
		while(dir = readdir(dp)){
			stat(dir->d_name,&mystat);
			printf("%ld %o %d %d %s %s\n",mystat.st_ino,mystat.st_mode,mystat.st_uid,mystat.st_gid,ctime(&mystat.st_atime),dir->d_name);
		}
	}
	return 0;
}

4a
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[]) {
    const char *original_file = argv[1];
    char hard_link[256];
    char soft_link[256];
    struct stat fileStat;

    snprintf(hard_link, sizeof(hard_link), "%s_hardlink", original_file);
    snprintf(soft_link, sizeof(soft_link), "%s_softlink", original_file);

    int fd = open(original_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, "Hello, World!\n", 14);
    close(fd);

    link(original_file, hard_link);

    symlink(original_file, soft_link);

    printf("Original file info:\n");
    stat(original_file, &fileStat);
    printf("Inode: %ld\n", (long)fileStat.st_ino);
    printf("Hard link count: %ld\n", (long)fileStat.st_nlink);
    printf("Size: %ld bytes\n", (long)fileStat.st_size);
    printf("Permissions: %o\n", fileStat.st_mode & 0777);
    printf("Last access: %s", ctime(&fileStat.st_atime));
    printf("Last modification: %s", ctime(&fileStat.st_mtime));
    printf("Last status change: %s\n", ctime(&fileStat.st_ctime));

    printf("Hard link info:\n");
    stat(hard_link, &fileStat);
    printf("Inode: %ld\n", (long)fileStat.st_ino);
    printf("Hard link count: %ld\n", (long)fileStat.st_nlink);
    printf("Size: %ld bytes\n", (long)fileStat.st_size);
    printf("Permissions: %o\n", fileStat.st_mode & 0777);
    printf("Last access: %s", ctime(&fileStat.st_atime));
    printf("Last modification: %s", ctime(&fileStat.st_mtime));
    printf("Last status change: %s\n", ctime(&fileStat.st_ctime));

    printf("Soft link info:\n");
    lstat(soft_link, &fileStat);
    printf("Inode: %ld\n", (long)fileStat.st_ino);
    printf("Hard link count: %ld\n", (long)fileStat.st_nlink);
    printf("Size: %ld bytes\n", (long)fileStat.st_size);
    printf("Permissions: %o\n", fileStat.st_mode & 0777);
    printf("Last access: %s", ctime(&fileStat.st_atime));
    printf("Last modification: %s", ctime(&fileStat.st_mtime));
    printf("Last status change: %s\n", ctime(&fileStat.st_ctime));

    unlink(original_file);
    unlink(hard_link);
    unlink(soft_link);

    return 0;
}


4b
4bmain.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
extern char **environ;

int main() {
    pid_t pid = fork();

    if (pid == 0) { 
        char *args[] = {"./4binterpreter.sh", "child_arg1", "child_arg2", NULL};
        char *envp[] = {"CHILD_ENV1=value1", "CHILD_ENV2=value2", NULL};
        execve("./4binterpreter.sh", args, envp);
    } else { 
        int status;
        waitpid(pid, &status, 0);

        char *args[] = {"./4binterpreter.sh", "parent_arg1", "parent_arg2", NULL};
        execve("./4binterpreter.sh", args, environ);
    }

    return 0;
}


4bechoall.c
// echoall.c
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("Arguments:\n");
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    printf("\nEnvironment Variables:\n");
    for (int i = 0; envp[i] != NULL; ++i) {
        printf("envp[%d]: %s\n", i, envp[i]);
    }

    return 0;
}

4binterpreter.sh
#!/bin/sh
# interpreter.sh
./4bechoall "$@"

4bechoall


5a
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <utime.h>

int main(int argc, char *argv[]) {
    struct stat source_stat;

    stat(argv[1], &source_stat);

    struct utimbuf times = {source_stat.st_atime, source_stat.st_mtime};
    utime(argv[2], &times);
printf("Access and modification times copied from %s to %s\n", argv[1], argv[2]);
    return 0;
}



5b
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
void handle_sigint(int sig) {
	printf("Caught SIGINT\n");
	signal(SIGINT, SIG_DFL);
}
int main() {
	struct sigaction sa;
	sa.sa_handler = handle_sigint;
	sigaction (SIGINT, &sa, NULL);
while (1) {
	printf ("Running...\n"); sleep(1);
}
	return 0;
}

6a
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd1, fd2;
    char buf[50];

    fd1 = open("test.txt", O_RDWR);               // Open "test.txt" for reading and writing
    fd2 = open("sample.txt", O_CREAT | O_RDWR, 0777); // Open (or create) "sample.txt" for reading and writing
    dup2(fd1, fd2);                               // Duplicate fd1 to fd2

    read(fd1, buf, 20);                           // Read 20 bytes from fd1 into buf
    lseek(fd2, 0, SEEK_END);                      // Move file offset of fd2 to the end of the file
    write(fd2, buf, 20);                          // Write 20 bytes from buf to fd2
    printf("%s\n", buf);                          // Print the buffer content
    close(fd1);                                   // Close fd1
    close(fd2);                                   // Close fd2
    return 0;                                     // Return success
}

6b
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    
    int fd = open(argv[1], O_RDWR);
    struct flock fvar = {
        .l_type = F_WRLCK,
        .l_whence = SEEK_END,
        .l_start = -100,
        .l_len = 100
    };
    fcntl(fd, F_SETLK, &fvar);

    
    char buffer[51];  
    lseek(fd, -50, SEEK_END);
    ssize_t bytes_read = read(fd, buffer, 50);
    
    buffer[bytes_read] = '\0';


    puts(buffer);


    fvar.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &fvar);

    close(fd);
    return 0;
}


7a
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<setjmp.h>
jmp_buf buf;
int main(){
volatile int x = 10;
register int y = 50;
printf("before jmp functions");
printf("x and y values %d %d\n ",x,y);
if(setjmp(buf)!=0){
printf("x and y values after jump functions: %d %d\n ",x,y);
return 0;
}
x=30;
y=40;
longjmp(buf, 1);
}

7b
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
int src = open(argv[1], O_RDONLY);
int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
char buffer[4096];
ssize_t bytesRead;
while ((bytesRead = read(src, buffer, sizeof(buffer))) > 0) {
write(dest, buffer, bytesRead);
}
close(src);
close(dest);
return 0;
}

8a
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
int main(){
DIR *dir;
struct dirent *entry;
struct stat fs;
dir = opendir(".");
while((entry = readdir(dir))!=NULL){
stat(entry->d_name,&fs);
if(fs.st_size == 0){
printf("removing file : %s\n",entry->d_name);
remove(entry->d_name);
}
}
return 0;}

8b
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
int main() {
 pid_t pid;
 int status;
 pid_t parent_pid = getpid();
 pid = fork();
 if (pid == 0) {
 pid_t child_pid = getpid();
 printf("Child process (PID: %d) executing...\n", child_pid);
 execl("./p1", "p1", "test.txt", NULL);
 perror("execl");
 exit(EXIT_FAILURE);
 } else {
 printf("Parent process (PID: %d) executing...\n", parent_pid);
 waitpid(pid, &status, 0);
 printf("Parent process: Child process (PID: %d) has exited.\n", pid);
 }
 return 0;
}

p1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
 char *filename = argv[1];
 if (access(filename, F_OK) == 0) {
 printf("File '%s' exists and can be accessed.\n", filename);
 } else {
 printf("File '%s' does not exist or cannot be accessed.\n", filename);
 }
 return 0;
}



CODE2/dest.txt
kri
CODE2/file.txt
hello
CODE2/file1.txt
CODE2/source.txt
kri
CODE2/test.txt
Lorem epsum yayyy



9a
9achmod.c
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
int main() {
    struct stat statbuf;

    stat("foo", &statbuf);
    chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID);
    chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    return 0;
}

9aumask.c
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<sys/stat.h>
#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main() {
    umask(0);
    creat("foo", RWRWRW);
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    creat("bar", RWRWRW);
    return 0;
}


9b
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main() {
    int file = open("testfile.txt", O_RDONLY);
    char buffer[25];

    read(file, buffer, 20);
    write(STDOUT_FILENO, buffer, 20);
    printf("\n");

    lseek(file, 10, SEEK_SET);
    read(file, buffer, 20);
    write(STDOUT_FILENO, buffer, 20);
    printf("\n");

    lseek(file, 10, SEEK_CUR);
    read(file, buffer, 20);
    write(STDOUT_FILENO, buffer, 20);
    printf("\n");

    int n = lseek(file, 0, SEEK_END);
    printf("size of file is %d bytes\n", n);

    close(file);
    return 0;
}


10a
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>

void create_daemon() {
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    open("/dev/null", O_RDONLY); // stdin
    open("/dev/null", O_WRONLY); // stdout
    open("/dev/null", O_WRONLY); // stderr
}

int main() {
    create_daemon();
    openlog("daemon_example", LOG_PID, LOG_DAEMON);
    while (1) {
        syslog(LOG_NOTICE, "Daemon is running...");
        sleep(30); // Sleep for 30 seconds
    }
    closelog();
    return EXIT_SUCCESS;
}

10b
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int cpid = fork();
    int cpid2 = fork();

    if (cpid != 0 && cpid2 != 0) {
        int status;
        printf("\nParent: I am going to wait for any child process to finish\n");
        int waitPID = wait(&status);
        printf("\nParent: Waited for a child, the return value of wait(): %d\n", waitPID);
        printf("\nParent: The exit code of terminated child: %d\n", WEXITSTATUS(status));

        printf("\nParent: Now I am going to wait specifically for the process with process ID: %d\n", cpid2);
        waitPID = waitpid(cpid2, &status, 0);
        printf("\nParent: Waited for child, the return value of waitpid(): %d\n", waitPID);
        printf("\nParent: The exit code of terminated child: %d\n", WEXITSTATUS(status));
        exit(1);
    } else if (cpid == 0 && cpid2 != 0) {
        printf("\nChild1: My process ID is: %d, and my exit code is 1\n", getpid());
        exit(1);
    } else if (cpid != 0 && cpid2 == 0) {
        printf("\nChild2: My process ID is: %d, and my exit code is 2\n", getpid());
        exit(2);
    } else {
        printf("\nChild3: My process ID is: %d, and my exit code is 3\n", getpid());
        exit(3);
    }

    return 0;
}

11a
	#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    printf("dup:\n");
    int fd1 = 0, fd2 = 0;
    char buf[10] = "abcdef";

    fd1 = open("t2.txt", O_RDWR);

    fd2 = dup(fd1);

    printf("fd1: %d, fd2: %d\n", fd1, fd2);
    write(fd1, buf, 6);
    write(fd2, "123", 3);

    close(fd1);
    close(fd2);

    printf("dup2:\n");
    int fd11 = 0, fd12 = 0;
    char buff[10] = "abcdef";

    fd11 = open("dest.txt", O_RDWR);

    dup2(fd11, fd12);

    printf("fd11: %d, fd12: %d\n", fd11, fd12);
    write(fd11, buff, 6);
    write(fd12, "123", 3);

    close(fd11);
    close(fd12);

    return 0;
}


11b
	
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execl("./p2", "p2", argv[1], argv[2], NULL);
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child terminated abnormally\n");
        }
    }
    return 0;
}


11p1.c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    int sum = num1 + num2;
    printf("Sum: %d\n", sum);
    return 0;
}

destination.txt
This is a test file for copying. Hello

example.txt
Hello

source.txt
This is a test file for copying. Hello

test.txt
Lorem epsum yayyy

test2.txt
Hello i am in CSE

test3.txt
This is a test file. It contains more than 100 bytes of data for testing purposes. Ensure it has enough content to be read.

testfile.txt
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit.


#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/types.h>
#include<time.h>
#include<dirent.h>
#include<utime.h>
#include<signal.h>
#include<errno.h>
#include<setjmp.h>
#include <syslog.h>

10a
cc -o de filename.c
./de
ps -axj

1a.c

```bash
  cc 1a.c
  ./a.out test.txt
```
1b.c

```bash
  cc 1b.c
  ./a.out test.txt
```

2a.c

```bash
  cc 2a.c
  ./a.out test.txt
```

2b.c

```bash
  cc 2b.c
  ./a.out test.txt
```
3a.c

```bash
  cc 3a.c
  ./a.out
```
3b.c

```bash
  cc 3b.c
  ./a.out
```
4a.c

It deletes the text file, hence make test.txt again after execution
```bash
  cc 4a.c
  ./a.out test.txt
```
4b.c

Change the name of interpreter file from 4binterpreter.sh(in 4bmain.c) to as required

Change the name of echoall.c file from 4bechoall.c to as required in interpreter.sh file
```bash
  gcc -o 4bechoall 4bechoall.c
  chmod +x 4binterpreter.sh
  cc 4bmain.c
  ./a.out
```
 5a.c
 
 After calling stat function we can see change parameter displayed is same
 ```bash
  cc 5a.c
  ./a.out test.txt test2.txt
  stat test.txt
  stat test2.txt
```

 5b.c
 
 After calling stat function we can see change parameter displayed is same
 ```bash
  cc 5b.c
  ./a.out
```
 6a.c
 
 Create an empty file in the directory

 Program will delete it(cross check with ls)
 ```bash
  cc 6a.c
  ./a.out
```

6b.c

 ```bash
  cc 6b.c
  ./a.out test3.txt
```
Open a new terminal and then run the program again

```bash
  cc 6b.c
  ./a.out test3.txt
```

7a.c 

```bash
  cc 7a.c
  ./a.out
```

7b.c 

```bash
  cc 7b.c
  ./a.out source.txt destination.txt
```
8a.c is same as 6a.c 

8b.c

Create an example.txt file

```bash
  vi example.txt
  cc 8b.c
  ./a.out
```
9a.c 

Change file names as you want

```bash
  cc 9aumask.c
  ./a.out
  cc 9achmod.c
  ./a.out
```
9b.c 

Create testfile.txt 
```bash
  cc 9b.c
  ./a.out
```

10.a

```bash
  cc 10.c
  ./a.out
```

10.b

```bash
  cc 10.b
  ./a.out
```
