    /* Client pour les sockets
     *    socket_client ip_server port
     */
     
    #include <stdlib.h>
    #include <strings.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
	#include <time.h>   // pour rand
	int indices[2][2];
	
	void afficheGrille(int g[10][10]){
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
				printf("%d\t",g[i][j]);
			}
			printf("\n");
		}
		
	}
	void entrerIndices(int g[10][10]) {
		int i1,j1,i2,j2;
		do{
			do{
				printf("\nEntrer l'indice i du premier nombre: ");
				scanf("%d",&i1);
			}while(i1<0 || i1>9);
			
			do{
				printf("\nEntrer l'indice j du premier nombre: ");
				scanf("%d",&j1);
			}while(j1<0 || j1>9);
			
		}while(g[i1][j1]==0);

		do{
			do{
				printf("\nEntrer l'indice i du deuxieme nombre: ");
				scanf("%d",&i2);
			}while(i2<0 || i2>9);
			
			do{
				printf("\nEntrer l'indice j du deuxieme nombre: ");
				scanf("%d",&j2);
			}while(j2<0 || j2>9);
			
		}while(g[i2][j2]==0);
		
		while(j1==j2 && i1==i2){
			printf("\nEntrer l'indice j du deuxieme nombre: ");
			scanf("%d",&j2);
		}
		indices[0][0]=i1;
		indices[0][1]=j1;
		indices[1][0]=i2;
		indices[1][1]=j2;
		printf("\nVotre selection est : premier nombre(%d), deuxieme nombre(%d)\n",g[i1][j1],g[i2][j2]); 
	}
     int main(int argc, char** argv )
    { 
      int    sockfd,newsockfd,clilen;
      char c;
      struct sockaddr_in cli_addr,serv_addr;
      int matrice[10][10];
	  int nombreFournit;
	  int score;
	  char message[255];
	  
      if (argc!=3) {printf ("usage  socket_client server port\n");exit(0);}
          
      /*
       *  partie client 
       */
      printf ("client starting\n");  
     
      /* initialise la structure de donnee */
      bzero((char*) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family       = AF_INET;
      serv_addr.sin_addr.s_addr  = inet_addr(argv[1]);
      serv_addr.sin_port         = htons(atoi(argv[2]));
     
      /* ouvre le socket */
      if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
        {printf("socket error\n");exit(0);}
     
      /* effectue la connection */
      if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
        {printf("socket error\n");exit(0);}
     
     
      /* repète dans le socket tout ce qu'il entend */
	  printf("je suis client\n");
		while(1){
			
         read(sockfd,&matrice,sizeof(matrice));printf("encore");
        //afficher grille
		afficheGrille(matrice);
		//recuperer nombre fournit par serveur
		read(sockfd,&nombreFournit,sizeof(nombreFournit));
		printf("\nNombre fournit par serveur est: %d ",nombreFournit);
		//donner indices
		int faux;
		do{
	    entrerIndices(matrice);
		write (sockfd,&indices,sizeof(indices));
		read(sockfd,&faux,sizeof(faux));
		if(faux==0){
			printf("\nMultiplication incorrecte!!! Réessayer");
		}
		}while(faux==0);
          
		//lire le score
		read(sockfd,&score,sizeof(score));
		printf("Le score: %d\n",score);
		if(read(sockfd,&message,sizeof(message))!=1){
		printf("%s",message);
		return 0;
		}
		//continue;
	    }
		
      /*  attention il s'agit d'une boucle infinie 
       *  le socket n'est jamais ferme !
       */
     
       return 1;
     
    }


