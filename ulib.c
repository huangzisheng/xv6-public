#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

//字符串复制函数
//没有考虑重叠的情况，即 t < s < t+strlen(t) 的情况
//添加重叠时的处理
char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  int sl = strlen(t);    //需要复制的字符串的长度
  //内存重叠，从后往前复制
  if(s >= t && s <= t+sl) {
    s = s + sl;
    t = t + sl;
    sl = sl + 1;  //复制的时候需要把字符串的最后一个字符'\0'也一起复制
    while(sl--)
      *s-- = *t--;
  } else {
      //内存没有重叠，从前往后复制
      while((*s++ = *t++) != 0)
        ;
  }
  
  return os;
}

//字符串比较函数
int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

//求字符串长度
uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

//字符查找函数
char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

//从标准输入读取输入到buf
char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);    //一次读取一个字符，方便判断是否读到换行符？
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

//获取指定文件的状态
int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

//字符换转整数
int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')   //当遍历到非数字时，循环结束
    n = n*10 + *s++ - '0';
  return n;
}

//内存移动函数
//也是没有考虑到重叠的情况
//添加重叠时的处理
void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  //当源内存首地址大于目标内存首地址时，正向拷贝
  if (src > dst) {
    while(n-- > 0)
      *dst++ = *src++;
  } else if (src < dst) {
    //当源内存首地址小于目标内存首地址，反向拷贝
      src = src + n - 1;
      dst = dst + n - 1;
      while(n-- > 0)
        *dst-- = *src--;
  }
  
  return vdst;
}
