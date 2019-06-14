#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
 
//the thread function
void *connection_handler(void *);

int scoreTable[10] = {};

int currentRound[2] = {};


int stoper1 = 1;
int stoper2 = 1;

int roundPosition = 0;

int checkerPerRound(int ci,int cj){
	if(ci == cj){
		return 0;
	}else	if(ci == 2 && cj ==1){
		return 1;
	}else if(ci == 3 && cj == 2){
		return 1;
	}else if(ci == 1 && cj == 3){
		return 1;
	}else{
		return 2;
	}
}
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
	
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
     
    listen(socket_desc , 3);
     
    printf("Pres pour la connection ;)<3<3\n");
    c = sizeof(struct sockaddr_in);
     
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        printf("Connection recu\n");
		fflush(stdout);
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
		
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
void *connection_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    int read_size;
    int client_message[2];
	char message[200];
    int count1=0,count2=0;
    int younes = 0;
	while(!younes){
	 
		if( (read_size = recv(sock , client_message , sizeof(client_message) , 0)) > 0 )
		{
			printf("%d : %d\n",client_message[0],client_message[1]);
            printf("joueur1 %d - %d Joueur2\n",count1, count2);
        }
		
		if(client_message[0] == 1){
			currentRound[0] = client_message[1];
			stoper2 = 0;
			while(stoper1);
			if(checkerPerRound(currentRound[0],currentRound[1]) == 1){
				strcpy(message, "vous avez gagne le round\n");
                count1++;
                if (count1 == 5)
                    younes == 1;			
			}else if(checkerPerRound(currentRound[0],currentRound[1]) == 2){
				strcpy(message, "vous avez perdu le round\n");
				count2++;
			}else{
				strcpy(message, "egaliter\n");
			}
			if(roundPosition >=10){
				strcpy(message, "terminer\n");
			}
		}else if(client_message[0] == 2){
			currentRound[1] = client_message[1];
			stoper1 = 0;
			while(stoper2);
			if(checkerPerRound(currentRound[1],currentRound[0]) == 1){
				strcpy(message, "vous avez gagne le round\n");
                count2++;
                if (count2 == 5)
                    younes = 1;			
			}else if(checkerPerRound(currentRound[1],currentRound[0]) == 2){
				strcpy(message, "vous avez perdu le round\n");
				count1++;
			}else{
				strcpy(message,"egaliter\n");
			}
			if(roundPosition >=10){
				strcpy(message, "terminer\n");
			}
		}
	
		
		
		if( send(sock , message , sizeof(message) , 0) < 0)
		{
			printf("Send failed");
		}
		 
		if(read_size == 0)
		{
			printf("Client deconnecter\n");
			fflush(stdout);
			break;
		}
		else if(read_size == -1)
		{
			perror("recv failed");
		}
		
		stoper1 = 1;
		stoper2 = 1;
	}
	char messagefin[100],messagefin2[100];
	
     if(count1==5){
		 printf("joueur 1 gagner\n");
		 strcpy(messagefin,"vous avez gagnez\n");
		 return 0;
		 
	 }

	 if(count2==5){
		 printf("joueur 2 gagner\n");
		 strcpy(messagefin,"vous avez gagnez\n");
		 return 0;
	 } 
    free(socket_desc);
     
    return 0;
}