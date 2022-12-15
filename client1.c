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

   if (bind(sock_fd, (struct sockaddr*)&client1_addr, sizeof(client1_addr)) < 0) {
      printf("Error binding socket\n");
      exit(1);
   }

   memset(buff, 0, MAX_SIZE);

   if(recvfrom(sock_fd, buff, sizeof(buff),  0, NULL, NULL) < 0) {
      printf("Error recvfrom\n");
      exit(1);
   }
   printf("Info from server:\n%s", buff);

   memset(buff, 0, MAX_SIZE);
   FILE* lines = popen("grep -c $ *", "r");
   fread(buff, 1, sizeof(buff), lines);
   pclose(lines);

   printf("\nSending info to server\n");
   sendto(sock_fd, buff, strlen(buff), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));


   //printf("Address: %s\n", client_addr.sun_path);
   //printf("Buffer %s\n", buff);
   //printf("Namelen var %d\n", namelen);

   close(sock_fd);

   return 0;
}