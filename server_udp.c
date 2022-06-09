#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>

#define SERV_PORT 8888

int main(int argc, char** argv)

{
    int opt;
    int port = SERV_PORT;
    while((opt = getopt(argc, argv, "p:")) != -1){
        switch(opt){
            case 'p': port = atoi(optarg);
		      break;
	    default: printf("unknown option!\n");
		     break;
	}
    }
    printf("port=%d\n", port);
    struct sockaddr_in serv_addr, clnt_addr;

    int lfd;

    int i, n;

    char recvbuf[BUFSIZ];

    socklen_t addrlen;

    lfd = socket(AF_INET, SOCK_DGRAM, 0);

    serv_addr.sin_family = AF_INET;

    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    serv_addr.sin_port = htons(port);

    bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while(1){

        addrlen = sizeof(clnt_addr);

        n = recvfrom(lfd, recvbuf, sizeof(recvbuf), 0,

                (struct sockaddr*)&clnt_addr, &addrlen);

        if(n > 0){
	    printf("receive content: %s\n",recvbuf);

            for(i = 0; i < n; i++){

                recvbuf[i] = toupper(recvbuf[i]);

            }

            n = sendto(lfd, recvbuf, n, 0, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));

            if(n < 0){

                printf("sendto error\n");

            }

        }else if(n == 0){

            printf("client disconnect\n");

            break;

        }else{

            printf("recvfrom error\n");

            break;

        }

    }

    close(lfd);

    return 0;

}


