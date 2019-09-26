struct buf {
  int flags;
  uint dev;
  uint blockno;
  struct sleeplock lock;
  uint refcnt;      //引用次数
  struct buf *prev; // LRU cache list 最近最少使用的缓存列表
  struct buf *next;
  struct buf *qnext; // disk queue
  uchar data[BSIZE]; //存放的数据
};
#define B_VALID 0x2  // buffer has been read from disk
#define B_DIRTY 0x4  // buffer needs to be written to disk

