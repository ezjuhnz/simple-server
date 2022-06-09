#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#define SERV_PORT 7000
//running command: "./server_tcp -p 7001"
int main(int argc, char** argv)

{
	int opt;
	int port = SERV_PORT;
	while((opt = getopt(argc, argv, "p:")) != -1){
          switch (opt){
            case 'p': port = atoi(optarg);
		      break;
            default: break;
	  }
	}

	struct sockaddr_in serv_addr, clnt_addr;

	socklen_t clnt_len;

	int lfd, cfd;

	int n, i;

	char buf[BUFSIZ];

	lfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(lfd < 0){

		printf("%s\n", "socket error");

		exit(1);

	}

	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	serv_addr.sin_port = htons(port);

	bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(lfd, 128);

	clnt_len = sizeof(serv_addr);
	cfd = accept(lfd, (struct sockaddr*)&clnt_addr, &clnt_len);
	if(cfd == -1){
	    printf("accept error\n");
	    if(errno == EAGAIN || errno == EWOULDBLOCK){
		    printf("normal error");
	    }else{
		 printf("innormal error\n");
		 return -1;
	    }
	}
	while(1){

 	  n = read(cfd, buf, BUFSIZ);

	  if(n > 0){
	        printf("receive centent: %s\n", buf);

		for(i = 0; i < n; i++){

			buf[i] = toupper(buf[i]);
		}
	  }

	  write(cfd, buf, n);
        }
	close(cfd);

	close(lfd);

	return 0;

}
