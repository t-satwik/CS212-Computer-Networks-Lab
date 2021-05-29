#include <stdio.h> 
#include <string.h>  
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
//This code is for II part of the question	
#define TRUE 1 
#define FALSE 0 
#define PORT 8080
	
int main(int argc , char *argv[]) 
{ 
	int opt = TRUE; 
	int main_socket , addrlen , new_socket , client_socket[30] , max_clients = 3 , activity, i , valread , sd; 
	int max_sd; 
	struct sockaddr_in address; 
		
	char buffer[1024] = {0}; //data buffer of 1K 
		

	fd_set readfds; 
		
	//a message 
	char *message = "SERVER: Welcome Message\n"; 
	
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
		
	//create a socket 
	if( (main_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){   //This socket creation is similar to the normal socket creation
        perror("Main Socket  creation failed");   
        exit(EXIT_FAILURE);   
    }

	//set  socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	// assign address here 
	address.sin_family = AF_INET; 			// This is same as what we have done for Q1 and Q2.
	address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 
		
	//bind the socket to localhost port  
	if (bind(main_socket, (struct sockaddr *)&address, sizeof(address))<0){ //This is also same binding what was done for Q1		
		perror("bind failed"); 											   //except that the socket name is different
		exit(EXIT_FAILURE); 
	}

	//try to specify maximum number of pending connections for the socket and listen
	if (listen(main_socket, 3) < 0) //I have used 3 as the backlog value and changed the name to main_socket
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection (Waiting for connections)
	addrlen = sizeof(address); 
	printf("Waiting to accept any incoming connections.\n");
	int exit_bool;
	while(TRUE) 
	{ 
		//clear the socket set 
		FD_ZERO(&readfds);			//FD_ZERO is used to clear all the fds in socket set
	
		//add socket to set 
		FD_SET(main_socket, &readfds);		//FD_SET is used to add a socket to the set.
		max_sd = main_socket;				

		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++){ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd+1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		//If check incoming connection using FD_ISSET
		if (FD_ISSET(main_socket, &readfds))	//checking for activity in the socket, if so adding it using ISSET
		{ 
			//accept the connection
			
			if ((new_socket = accept(main_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  //same as accept for previous questions
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			printf("Connection accepted from client %d.\n", new_socket);
			
			//send new connection greeting message 
			//since the send function returns the number of bytes of the message sent 
			//and the welcome message is intialized earlier, we can use this condition to send messages through new_socket
			if(send(new_socket, message, strlen(message), 0) != strlen(message)){ 
				perror("send"); 
			} 
				
			puts("Welcome message sent successfully"); 
				
			//add new socket to array of sockets 
			for (i = 0; i < max_clients; i++) 
			{ 
				if(client_socket[i]==0){
					client_socket[i] = new_socket;
					printf("The current socket has been added to the array of sockets.\n");
					printf("The updated clients array is: ");
					for(int j=0; j<max_clients;j++){
						printf("%d ", client_socket[j]);
					}
					printf("\n");
					break;
				}
			} 
		} 
			
		//else its some IO operation on some other socket 
		for(i = 0; i < max_clients; i++){		
			sd = client_socket[i]; 
				
			//Check for FD_ISSET 
			if (FD_ISSET(sd , &readfds)){
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((valread = read( sd , buffer, 1024)) == 0)
				{ 
					//Somebody disconnected , get his details and print 
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);   
                    printf("Host disconnected , socket %d, ip %s , port %d \n\n", sd, inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse 
					close( sd ); 
					client_socket[i] = 0; 
					printf("Enter 1 to close the server i.e complete the program, else enter 0: ");
					scanf("%d", &exit_bool);
					if(exit_bool==1){
						return 0;
					}
				} 
					
				//Echo back the message that came in 
				else
				{ 
					printf("The paragraph received from the client %d is: %s\n", sd, buffer); 
					int numChars=strlen(buffer);
					int numWords=0, numSent=0;
					for(int j=0;buffer[j]!='\0';j++){
						if(buffer[j]==' ')
							numWords++;
						if(buffer[j]=='.')
							numSent++;
					}
					numWords++;
					char * values;
					sprintf(values, "%d, %d, %d", numChars, numWords, numSent);
					send(sd , values , strlen(values) , 0); 

    				printf("SERVER: The calculated values(%s) are sent to the client %d\n", values, sd);
				} 
			} 
		} 
	} 
		
	return 0; 
} 

