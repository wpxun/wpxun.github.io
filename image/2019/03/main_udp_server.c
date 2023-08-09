#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 256
#define SERV_PORT 9080

int main(int argc, const char * argv[]) {
    
    long ret;
    char buf[BUF_SIZE];
    int sockfd;
    struct sockaddr_in host_addr;
    struct sockaddr_in client_addr;
    
    unsigned int client_addr_len = sizeof(struct sockaddr_in);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("udp socket error.\n");
        return 1;
    }
    
    bzero(&host_addr, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(SERV_PORT);
    host_addr.sin_addr.s_addr = INADDR_ANY;
    
    if ((bind(sockfd, (struct sockaddr *)&host_addr, sizeof(host_addr))) == -1) {
        printf("bind error.\n");
        return 1;
    }
    
    printf("waitting...\n");
    
    while (1) {
        bzero(buf, sizeof(buf));
        ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (ret == -1) {
            printf("recvfrom error.\n");
            return 1;
        }
        printf("client: %s:%d \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("recieve:%s\n", buf);
        sleep(2);
    }
    
    //close(sockfd);
    
    return 0;
}
