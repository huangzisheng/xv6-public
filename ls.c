#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

//path是指定文件的路径，fmtname将返回指向文件名字符串的指针
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  //从后往前遍历，找到字符“/”
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  //根据文件描述符获取文件状态，文件状态存放在结构体st里
  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  //接下来根据文件的类型：文件或目录文件 进行不同的操作
  switch(st.type){
  case T_FILE:
    //输出该文件对应的文件名、文件类型、文件的i节点、文件的大小
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    //此时fd对应的文件是目录文件，循环从目录文件中读取每一个结构体dirent
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)    //索引节点号如果是0的话跳过？
        continue;
      memmove(p, de.name, DIRSIZ);//p指向的是buf后的第一个位置，此操作将在当前目录文件里遍历到的文件的文件名追加到buf后面
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){     //获取对应文件的状态
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      //输出该文件对应的文件名、文件类型、文件的i节点、文件的大小
      printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  //如果ls的参数不够，则表示对当前目录执行ls操作
  if(argc < 2){
    ls(".");
    exit();
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit();
}
