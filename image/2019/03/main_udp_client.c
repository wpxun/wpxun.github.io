#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define SERV_PORT 9080
#define BACKLOG 20
#define BUF_SIZE 11

void randstr(char *pointer, int n)
{
    int i,randnum;
    char str_array[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (i = 0; i < n; i++)
    {
        randnum = rand()%62;                    //随机数生成函数
        *pointer = str_array[randnum];          //从字符数组中取值
        pointer++;
    }
    *pointer = '\0';                            //字符串结束符
}

int main(int argc, const char * argv[]) {
    long ret;
    int sockfd;
    struct sockaddr_in addr;
    
    if (argc != 2) {
        printf("input correct ip addr.\n");
        return 1;
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("create socket fail.\n");
        return 1;
    }
    
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    //addr.sin_addr.s_addr = inet_addr(argv[1]);
    inet_aton(argv[1], &addr.sin_addr);  //设定IP地址
    
    char buf[BUF_SIZE];
    
    int i = 1;
    while (1) {
        bzero(buf, sizeof(buf));
        randstr(buf, BUF_SIZE-1);
        ret = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr));
        //printf("ret = %ld\n", ret); 返回发送的字节数
        printf("send: %s\n", buf);
        printf("size send: %d\n", (BUF_SIZE-1)*(i++));
        sleep(10);
    }
    
    //close(sockfd);
    
    return 0;
    
}
