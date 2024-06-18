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
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

void printFlags(int fd) {
    int flags = fcntl(fd, F_GETFL);
    printf("File flags for descriptor %d:\n", fd);
    printf("O_APPEND: %s\n", (flags & O_APPEND) ? "Yes" : "No");
    printf("O_NONBLOCK: %s\n", (flags & O_NONBLOCK) ? "Yes" : "No");
    printf("O_SYNC: %s\n", (flags & O_SYNC) ? "Yes" : "No");
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        printFlags(atoi(argv[1]));
    } else {
        printf("Usage: %s <file descriptor>\n", argv[0]);
        return 1;
    }
    return 0;
}


3a
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
execl("/bin/sh","sh","-c","ls -l",(char *)0);
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
	DIR *dp;
	dp = opendir(".");
	if(dp){
		while(dir = readdir(dp)){
			stat(dir->d_name,&mystat);
			printf("%ld %o %d %d %s %s\n",mystat.st_ino,mystat.st_mode,mystat.st_uid,mystat.st_gid,ctime(&mystat.st_atime),dir->d_name);
		}
	}
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
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *source_file = argv[1];
    const char *dest_file = argv[2];

    struct stat source_stat;
    struct utimbuf times;

    // Get the access and modification times of the source file
    if (stat(source_file, &source_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    // Copy access and modification times to the destination file
    times.actime = source_stat.st_atime;
    times.modtime = source_stat.st_mtime;
    if (utime(dest_file, &times) == -1) {
        perror("utime");
        exit(EXIT_FAILURE);
    }

    printf("Access and modification times copied from %s to %s\n", source_file, dest_file);

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
dir opendir(".");
while ((entry=readdir(dir))!=NULL) {
stat (entry->d_name,&fs);
if (fs.st_size==0){
printf("empty files are: %s", entry->d_name);
remove(entry->d_name);
}
}
return 0;
}

6b
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd;
    char buffer[51];  // Buffer to hold the last 50 bytes plus a null terminator
    struct flock fvar;

    fd = open(argv[1], O_RDWR);

    fvar.l_type = F_WRLCK;
    fvar.l_whence = SEEK_END;
    fvar.l_start = -100;
    fvar.l_len = 100;

    fcntl(fd, F_SETLK, &fvar);

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
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
int main(){
int pid = fork();
if(pid<0){
printf("error");
}
else if(pid == 0){
printf("child process execution started ids are (PID: %d, PPID: %d)\n",getpid(),
getppid());
const char * fp = "file.txt";
if(access(fp,F_OK)==0){
printf("child process can access file1.txt");
}
if (access(fp, R_OK) == 0) {
printf("Child Process: File %s is readable.\n", fp);}
}
else{
wait(NULL);
printf("Parent Process (PID: %d): Child process (PID: %d) exited.\n", getpid(), pid);
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
int main()
{
struct stat statbuf;
/* turn on set-group-ID and turn off group-execute */
if (stat("foo", &statbuf) < 0)
	perror("stat error for foo");
if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
	perror("chmod error for foo");
/* set absolute mode to "rw-r--r--" */
if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
	perror("chmod error for bar");
exit(0);
}

9aumask.c
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<sys/stat.h>
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main()
{
umask(0);
if (creat("foo", RWRWRW) < 0)
	printf("creat error for foo");
umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
if (creat("bar", RWRWRW) < 0)
	printf("creat error for bar");
exit(0);
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
    setsid();
    umask(0);
    chdir("/");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
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