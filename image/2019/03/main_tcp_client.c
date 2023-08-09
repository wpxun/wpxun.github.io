#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define SERV_PORT 8090
#define BACKLOG 20
#define BUF_SIZE 100001

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
    int sockfd;
    struct sockaddr_in addr;
    
    if (argc != 2) {
        printf("input correct ip addr.\n");
        return 1;
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket fail.\n");
        return 1;
    }
    
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    //addr.sin_addr.s_addr = inet_addr(argv[1]);
    inet_aton(argv[1], &addr.sin_addr);  //设定IP地址
    
    if (connect(sockfd, (struct sockaddr *)(&addr), sizeof(struct sockaddr_in)) < 0) {
        printf("create connect fail.\n");
        return 1;
    } else {
        printf("create connect success.\n");
    }
    
    char buf[BUF_SIZE];
    
    int i = 1;
    while (1) {
        if (i > 4) {
            break;
        }
        bzero(buf, sizeof(buf));
        randstr(buf, BUF_SIZE-1);
        send(sockfd, buf, strlen(buf), 0);
        printf("send: %s\n", buf);
        printf("size send: %d\n", (BUF_SIZE-1)*(i++));
        sleep(1);
    }
    
    close(sockfd);
    //shutdown(sockfd, <#int#>)
    
    return 0;
    
}
