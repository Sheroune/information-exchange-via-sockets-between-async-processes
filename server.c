#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_SIZE 4096

int main(){
   char buff[MAX_SIZE];

   int sock_fd;

   struct sockaddr_in server_addr;
   struct sockaddr_in client1_addr;
   struct sockaddr_in client2_addr;

   if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      printf("Error creating socket\n");
      exit(1);
   }

   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(3425);
   server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

   client1_addr.sin_family = AF_INET;
   client1_addr.sin_port = htons(3426);
   client1_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

   client2_addr.sin_family = AF_INET;
   client2_addr.sin_port = htons(3427);
   client2_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

   if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      printf("Error binding socket\n");
      exit(1);
   }

   memset(buff, 0, MAX_SIZE);
   FILE* names = popen("tree -i | head --lines=-2 | sed '1d'", "r");
   fread(buff, 1, sizeof(buff), names);
   pclose(names);

   printf("Sending info to clients\n\n");
   sendto(sock_fd, buff, strlen(buff), 0, (struct sockaddr*)&client1_addr, sizeof(client1_addr));
   sendto(sock_fd, buff, strlen(buff), 0, (struct sockaddr*)&client2_addr, sizeof(client2_addr));

   //while(1) {
   for (int i = 0; i < 2; i++) {
      memset(buff, 0, MAX_SIZE);
      if(recvfrom(sock_fd, buff, sizeof(buff),  0, NULL, NULL) < 0) {
         printf("Error recvfrom\n");
         exit(1);
      }
      printf("Info from client:\n%s\n", buff);
   }

   close(sock_fd);

   return 0;
}
