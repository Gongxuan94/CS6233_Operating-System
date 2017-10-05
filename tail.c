#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

void tail(int fd, int subLine) 
{
  int n, i;
  int totalLine;  // total lines
  int printLine;  // the last 10 lines begin at printLine
  int fileTemp;

  // use file to store strings
  fileTemp = open("tailTemp", O_CREATE | O_WRONLY);
  while ((n = read(fd, buf, sizeof(buf))) > 0) 
  {    
    if (n < 0) 
    {
      printf(2, "tail: read error. Exiting...\n");
      exit();
    }
    write (fileTemp, buf, n);
    for (i = 0; i < n; ++i) 
    {
      if (buf[i] == '\n')
        ++totalLine;
    }
  }
  close(fileTemp);

  if (totalLine < subLine)
    printLine = 0;  //print all lines
  printLine = totalLine - subLine;

  fileTemp = open("tailTemp", 0);
  int countLine = 0;
  while ((n = read(fileTemp, buf, sizeof(buf))) > 0) 
  {
    for (i = 0; i < n; ++i) 
    {
      if (countLine >= printLine)
        write(1, &buf[i], 1);
      if (buf[i] == '\n')
        ++countLine;
    }
  }
  
  close(fileTemp);
  unlink("tailTemp");
}

int main(int argc, char *argv[]) 
{
  int subLine = 10;
  int fd = -1;
  int i; 

  if (argc <= 1) 
  {
    tail(0, subLine);  // open std input 
    exit();
  }

  if (argc > 1 && argv[1][0] == '-') 
  {
    char *getLine = (char *) &argv[1][1];
    subLine = atoi(getLine);
    if (argc == 2) 
    {
      tail(0, subLine);    // open std input
      exit(); 
    }
    else // argc >= 3
    {
      if ((fd = open (argv[2], 0)) > 0) 
      {
        tail(fd, subLine);
        close(fd);
        exit();
      } 
      else
      {
        printf(2, "tail: can not open %s\n", argv[2]);
        exit();
      }
    }
  }

  if (argc >= 2) 
  {
    for (i = 1; i < argc; i++) 
    {
      if ((fd = open (argv[i], 0)) < 0) 
      {
        printf(2, "tail: can not open %s\n", argv[i]);
        exit();
      }
      else 
      {
        tail(fd, subLine);
        close(fd);
      }
    }
  }
  exit();
}
