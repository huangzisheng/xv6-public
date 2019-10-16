#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

//统计文件中的行数l、单词数w以及总包含的字符个数c(包括空格和换行符)
void
wc(int fd, char *name)
{
  int i, n;
  int l, w, c, inword;

  l = w = c = 0;
  inword = 0;   //0表示当前字符不在单词里，可能是空格或换行符。1表示当前字符在单词里。方便计算单词个数
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      c++;    //字符数加1
      if(buf[i] == '\n')
        l++;    //行数加1
      if(strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if(!inword){
        w++;    //如果当前字符不在单词里，则遍历到的单词个数加1
        //如果是一个单词后有连续两个空格，那么这种计算方法不适用啊
        inword = 1;
      }
    }
  }
  if(n < 0){
    printf(1, "wc: read error\n");
    exit();
  }
  printf(1, "%d %d %d %s\n", l, w, c, name);
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    wc(0, "");
    exit();
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "wc: cannot open %s\n", argv[i]);
      exit();
    }
    wc(fd, argv[i]);
    close(fd);
  }
  exit();
}
