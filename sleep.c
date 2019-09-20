#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf(2, "Usage: sleep seconds...\n");
        exit();
    }
    int seconds = atoi(argv[1]);
    sleep(seconds);
    exit();
}
