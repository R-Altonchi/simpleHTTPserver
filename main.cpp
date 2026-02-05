#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr = {
    AF_INET,
    htons(8080),
    0
  };
  
  bind(sockfd, (sockaddr*)&addr, sizeof(addr);
  listen(sockfd, 10);
  printf("Server listening on port 8080...\n");

  while (true) {
  int clientfd = accept(sockfd, NULL, NULL);

  char buffer[1024] = {0};
  recv(clientfd, buffer, sizeof(buffer) - 1, 0);
  printf("Request: %s\n", buffer);

  char* file_request = buffer + 5;
  char response[2048] = {0}; 
  const char* metadata = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";
  strcpy(response, metadata);
  
  //This is to test sending index,html
  if (strncmp(file_request, " ", 1) == 0) {
    FILE* f = fopen("index.html", "r");
    if (f) {
      fread(response + strlen(metadata), 1, 1024, f);
      fclose(f);
    }
  }else if (strncmp(file_request, "api/home", 8) == 0){
      const char* json_response = "{\"message\": \"Welcome to the home API\", \"status\": \"success\"}";
    strcat(response, json_response);
  }else {
    const char* error = "page not found!";
    strcat(response, error);
  }
  
  send(clientfd, response, strlen(response), 0);
  
  close(clientfd);

  }

  close(sockfd);
  
  return 0;
}
