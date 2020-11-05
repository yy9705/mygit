#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <sys/stat.h>

#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>

#include <stdbool.h>

#include <sys/wait.h>

#define MAX 100

#define LEN 100

char cmdbuf[LEN]; //接收命令数组

int num1 = 0; //按管道符分割得到的命令个数

int num2 = 0; //按空格分割的到的命令个数

char cmdtemp[MAX]; //临时存放将要用到的cmdtemp1中的命令

char *cmdtemp2[MAX]; //存放按空格分割命令得到的结果

char *cmdtemp1[MAX]; //存放按管道符分割命令得到的结果

char filename[30]; //文件名

//int flag=0;			//后台处理命令标记

void init(); //初始化路径为当前工作路经

int read_command(); //读取命令

int is_internal_cmd(char *cmdbuf); //判断并执行内置命令

void is_pipe(int index); //执行管道

void cut(char *cmdbuf); //按空格分割命令

void cut_pipe(char line[]); //按管道符分割命令

void execute(char *argv[]); //执行外置命令

void redirect_in(char *last); //执行输入重定向

void redirect_out(char *last); //执行输出重定向

char *changeBlank(char cmd[40]); //找到>>,>,<,给其前加上空格,删除后面的空格

/************************主函数***********************/

int main(int argc, char *argv[])

{

  init();

  while (read_command())

  {

    //判断是否为内部命令

    if (is_internal_cmd(cmdbuf) == 1)

      continue;

    else

    {

      char buff[LEN];

      strcpy(buff, changeBlank(cmdbuf));

      int pid;

      pid = fork();

      if (pid == 0)

      {

        cut_pipe(buff);

        strcpy(cmdtemp, cmdtemp1[0]);

        cut(cmdtemp);

        //判断并执行输入重定向

        if (strstr(cmdbuf, "<") != NULL)

        {

          char last[LEN];

          strcpy(last, cmdtemp2[num2 - 1]);

          redirect_in(last);
        }

        //判断并执行输出重定向

        else if (strstr(cmdbuf, ">") != NULL)

        {

          char last[LEN];

          strcpy(last, cmdtemp2[num2 - 1]);

          redirect_out(last);
        }
        * /

            //执行管道和执行普通命令

            is_pipe(0);

        exit(0);
      }

      //if(flag==0)

      wait(NULL);
    }
  }

  return 0;
}

/******************************************************/

//初始路径为当前工作路经

char initpath[50];

void init()

{

  char pathname[50];

  getcwd(pathname, 50);

  strcpy(initpath, pathname);
}

//读取命令

int read_command()

{

  char *user;

  if (strcmp(getenv("USER"), "root"))

    user = "$";

  else

    user = "#";

  char pathname[50];

  char servername[50];

  memset(pathname, 50, 0);

  getcwd(pathname, 50); /*获取当前工作路径*/

  memset(servername, 50, 0);

  gethostname(servername, 50);

  printf("[%s@%s:%s]%s ", getlogin(), servername, pathname, user);

  fflush(stdout); /*刷新缓冲区*/

  memset(cmdbuf, LEN, 0);

  fgets(cmdbuf, LEN, stdin); /*获取命令行所有参数*/

  cmdbuf[strlen(cmdbuf) - 1] = '\0';

  if (cmdbuf[0] == '\0')
    read_command();
}

//执行外部命令

void execute(char *argv[])

{

  if (strstr(cmdbuf, "<") != NULL || strstr(cmdbuf, ">") != NULL)

    argv[num2 - 1] = NULL;

  /*if (strstr(cmdbuf,"&")!=NULL)

	{

		flag=1;

		if (strstr(argv[num2-1],"&")!=NULL)

		{

			printf("%d\n", getpid());

			argv[num2-1]=NULL;

		}

	}*/

  int error;

  error = execvp(argv[0], argv);

  if (error == -1)
    printf("failed!!!\n");

  exit(1);
}

//执行内部命令

int is_internal_cmd(char *cmdbuf)

{

  char _cmdbuf[MAX];

  strcpy(_cmdbuf, cmdbuf);

  cut(_cmdbuf);

  if (strncmp(cmdtemp2[0], "exit", 4) == 0)

  {

    printf("Bey Bey!\n");

    exit(0);
  }

  else if (strncmp(cmdtemp2[0], "cd", 2) == 0)

  {

    char buf[LEN];

    if (chdir(cmdtemp2[1]) >= 0)

    {

      getcwd(buf, sizeof(buf));

      printf("Current dir is:%s\n", buf);
    }

    else if (cmdtemp2[1] == NULL)

      chdir(initpath);

    else

    {

      printf("Error path!\n");
    }

    return 1;
  }

  else if (strcmp(cmdtemp2[0], "pwd") == 0)

  {

    char buf[LEN];

    getcwd(buf, sizeof(buf));

    printf("Current dir is:%s\n", buf);

    return 1;
  }

  return 0;
}

//根据管道符切分命令,并存放到*cmdtemp1[MAX]

void cut_pipe(char line[])

{

  char *save;

  char *cmd = strtok_r(line, "|", &save);

  while (cmd)
  {

    cmdtemp1[num1] = cmd;

    num1++;

    cmd = strtok_r(NULL, "|", &save);
  }

  cmdtemp1[num1] = NULL;
}

//根据空格切分命令,并存放到*cmdtemp2[MAX]

void cut(char cmdbuf[])

{

  char *save;

  char *arg = strtok_r(cmdbuf, " ", &save);

  int i = 0;

  while (arg)
  {

    cmdtemp2[i] = arg;

    i++; //记录命令个数

    arg = strtok_r(NULL, " ", &save);
  }

  cmdtemp2[i] = NULL;

  num2 = i;
}

//输入重定向

void redirect_in(char *last)

{

  int fd;

  if (last[0] == '<')

  {

    strcpy(filename, last + 1);

    fd = open(filename, O_RDONLY);

    if (fd == -1)

    {

      printf("file open failed\n");

      return;
    }

    dup2(fd, 0);

    close(fd);
  }
}

//输出重定向

void redirect_out(char *last)

{

  int fd;

  if (last[0] == '>')

  {

    if (last[1] != '>')

    {

      strcpy(filename, last + 1);

      fd = open(filename, O_CREAT | O_RDWR, 0666); //0666为权限

      if (fd == -1)

      {

        printf("file open failed\n");

        return;
      }

      else

        printf("Create %s success!\n", filename);

      dup2(fd, 1);

      close(fd);
    }

    else

    {

      strcpy(filename, last + 2);

      fd = open(filename, O_CREAT | O_RDWR | O_APPEND, 0666); //0666为权限

      if (fd == -1)

      {

        printf("file open failed\n");

        return;
      }

      else

        printf("Write into %s success!\n", filename);

      dup2(fd, 1);

      close(fd);
    }
  }
}

//管道执行

void is_pipe(int index)

{

  if (index == num1 - 1)

  {

    strcpy(cmdtemp, cmdtemp1[index]);

    cut(cmdtemp);

    execute(cmdtemp2);
  }

  int fd[2];

  pipe(fd); //创建管道，0读，1写

  if (fork() == 0)

  {

    dup2(fd[1], 1);

    close(fd[0]);

    close(fd[1]);

    strcpy(cmdtemp, cmdtemp1[index]);

    cut(cmdtemp);

    execute(cmdtemp2);
  }

  dup2(fd[0], 0);

  close(fd[0]);

  close(fd[1]);

  is_pipe(index + 1);
}

//找到>>,>,<,给其前加上空格,删除后面的空格

char *changeBlank(char cmd[40])

{

  char buf[40];

  char temp[40];

  char temp1[40];

  char *p;

  int t = 0;

  int j = 0, i = 0;

  strcpy(buf, cmd);

  if ((p = strstr(buf, ">>")) != NULL) //先判断有没有“>>”,因为">"包含于“>>”，给其前加上空格,删除后面的空格

  {

    t = (p - buf) / sizeof(char); //找到">>"在命令数组中的下标号

    for (i = 0; i < t; i++)

    {

      strcpy((temp + i), (buf + i));
    }

    strcpy((temp + t), " ");

    strcpy((temp + t + 1), (buf + t));

    strcpy((temp + t + 2), (buf + t + 1));

    for (i = t + 3, j = t + 2; j < sizeof(buf); i++, j++)

    {

      if (strstr(buf, ">> ") != NULL)

        strcpy((temp + i), (buf + j + 1));

      else if (strstr(buf, ">>") != NULL)

        strcpy((temp + i), (buf + j));
    }

    if ((p = strstr(temp, "<")) != NULL)

    {

      t = (p - temp) / sizeof(char);

      for (i = 0; i < t; i++)

      {

        strcpy((temp1 + i), (temp + i));
      }

      strcpy((temp1 + t), " ");

      strcpy((temp1 + t + 1), (temp + t));

      for (i = t + 2, j = t + 1; j < sizeof(temp); i++, j++)

      {

        if (strstr(buf, "< ") != NULL)

          strcpy((temp1 + i), (temp + j + 1));

        else if (strstr(buf, "<") != NULL)

          strcpy((temp1 + i), (temp + j));
      }

      strcpy(cmd, temp1);
    }

    else

    {

      strcpy(cmd, temp);
    }
  }

  else if ((p = strstr(buf, ">")) != NULL) //判断命令行中是不是同时有“>”,有则给其前加上空格,删除后面的空格

  {

    t = (p - buf) / sizeof(char);

    for (i = 0; i < t; i++)

    {

      strcpy((temp + i), (buf + i));
    }

    strcpy((temp + t), " ");

    strcpy((temp + t + 1), (buf + t));

    for (i = t + 2, j = t + 1; j < sizeof(buf); i++, j++)

    {

      if (strstr(buf, "> ") != NULL)

        strcpy((temp + i), (buf + j + 1));

      else if (strstr(buf, ">") != NULL)

        strcpy((temp + i), (buf + j));
    }

    if ((p = strstr(temp, "<")) != NULL)

    {

      t = (p - temp) / sizeof(char);

      for (i = 0; i < t; i++)

      {

        strcpy((temp1 + i), (temp + i));
      }

      strcpy((temp1 + t), " ");

      strcpy((temp1 + t + 1), (temp + t));

      for (i = t + 2, j = t + 1; j < sizeof(temp); i++, j++)

      {

        if (strstr(buf, "< ") != NULL)

          strcpy((temp1 + i), (temp + j + 1));

        else if (strstr(buf, "<") != NULL)

          strcpy((temp1 + i), (temp + j));
      }

      strcpy(cmd, temp1);
    }

    else

    {

      strcpy(cmd, temp);
    }
  }

  else

  {

    if ((p = strstr(buf, "<")) != NULL) //判断命令行中是不是同时有“<”,有则给其前加上空格,删除后面的空格

    {

      t = (p - buf) / sizeof(char);

      for (i = 0; i < t; i++)

      {

        strcpy((temp + i), (buf + i));
      }

      strcpy((temp + t), " ");

      strcpy((temp + t + 1), (buf + t));

      //strcpy((temp+t+2)," ");

      for (i = t + 2, j = t + 1; j < sizeof(buf); i++, j++)

      {

        if (strstr(buf, "< ") != NULL)

          strcpy((temp + i), (buf + j + 1));

        else if (strstr(buf, "<") != NULL)

          strcpy((temp + i), (buf + j));
      }

      strcpy(cmd, temp);
    }
  }

  return cmd;
}