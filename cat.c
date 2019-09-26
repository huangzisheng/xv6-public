#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

//显示文件描述符fd对应的文件的内容
void
cat(int fd)
{
  int n;  //用于下面的read函数，表示每次循环读取到的字节数

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {      //写入读取的内容到标准输出
      printf(1, "cat: write error\n");
      exit();
    }
  }
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){//如果在命令行界面只输入cat，则等待继续输入，然后复述
    cat(0);
    exit();     //实际上发现没有退出啊......，cat程序不会退出，需要按Ctr+D才退出，看看怎么修改
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){    //打开cat后面参数(文件路径+文件名)表示的文件
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
    cat(fd);        //打开成功后调用cat输出文件内容  
    close(fd);
  }
  exit();
}
