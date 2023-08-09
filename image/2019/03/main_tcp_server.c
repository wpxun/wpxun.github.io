#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>

#define PORT 8090
#define BACKLOG 20
#define BUF_SIZE 1256

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    pid = wait(&stat);//获取子进程进程号
    printf("child %d terminated\n", pid);
    return;
}

int main() {
    signal(SIGCHLD,sig_chld);//处理SIGCHLD信号
    
    int sockfd;
    int clienfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("fail to create socket.\n");
        return 1;
    } else {
        printf("socket success, id = %d.\n", sockfd);
    }
    
    struct sockaddr_in addr; // 和 sockaddr 可以相互转换
    struct sockaddr_in client_addr;
    size_t addr_len = sizeof(struct sockaddr_in);
    unsigned int client_addr_len = sizeof(struct sockaddr_in);
    bzero(&addr, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sockfd, (struct sockaddr *)(&addr), sizeof(struct sockaddr)) < 0) {
        printf("bind fail.\n");
        return 1;
    } else {
        printf("bind success.\n");
    }
    
    // 监听网络端口
    int ret;
    ret = listen(sockfd, BACKLOG); // listen 将套接字转换成倾听套接字，也就是把主动套接字转换成被动套接字
    if (ret == -1) {
        printf("listen fail.\n");
        return 1;
    } else {
        printf("listen success.\n");
    }
    
    pid_t pid;
    char buf[BUF_SIZE];
    char buf_rev[BUF_SIZE];
    
    pid = getpid();
    
    while (1) {
        if (pid == 0) {
            exit(0);
        }
        
        clienfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
  
        if (clienfd == -1) {
            printf("accept fail.\n");
            return 1;
        } else {
            printf("accept success, id = %d, pid = %d.\n", clienfd, getpid());
        }
        
        pid = fork();
        long revc_n;
        
        if (pid == 0) {
            sleep(10);
            while (1) {
                bzero(buf_rev, addr_len);
                revc_n = recv(clienfd, buf_rev, sizeof(buf), 0);
                if (revc_n == 0) {
                    break;
                }
                printf("%d %d receive: %s\n", clienfd, getpid(), buf_rev); //自动有换行，如果是自己输出，一定要加上换行才会输出，否则会等到缓存满
                sleep(1);
            }
        }
        
        close(clienfd);
    }
    
    return 0;
}
