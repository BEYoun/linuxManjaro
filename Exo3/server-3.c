    /* Serveur sockets TCP
     * affichage de ce qui arrive sur la socket
     *    socket_server port (port > 1024 sauf root)
     */
     
    #include <stdlib.h>
    #include <strings.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
	#include <time.h>   // pour rand
    #include <string.h>
    int g[10][10];
    void grille(void){
    int i,j;
   
    srand(time(NULL));

    for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			g[i][j]=rand()%9+1;
		}
    }
} 
    
	int nombre(){
	srand(time(NULL));
	int i1,i2,j1,j2;
	do{
	i1=rand()%9+0;
	j1=rand()%9+0;
	}while(g[i1][j1]==0);

	do{
	i2=rand()%9+0;
	j2=rand()%9+0;
	}while(g[i2][j2]==0);

	while(j1==j2 && i1==i2){
		j2=rand()%9+0;
	}
	printf("i1=%d ",i1);
	printf("j1=%d ",j1);
	printf("i2=%d ",i2);
	printf("j2=%d ",j2);
	return g[i1][j1]*g[i2][j2];
    }

	int verification(int i1,int j1,int i2, int j2,int nombre){
    int nbr=g[i1][j1]*g[i2][j2];

    if(nbr==nombre){
		
    	g[i1][j1]=0;
        g[i2][j2]=0;
	return 0;
    }
    return 1;
}

    int main(int argc, char** argv )
    { 
	char datas[] = "hello\n";
      int    sockfd,newsockfd,clilen;
      char c;
      struct sockaddr_in cli_addr,serv_addr;
      int indices[2][2];
      int nombreAfournir;
	  time_t debut,fin,debutP,finP;
	  double diffTemps,diffTemps1;
	  int score=0;
	  char message[255];
      if (argc!=2) {printf ("usage: socket_server port\n");exit(0);}
     
      printf ("server starting...\n");  
     
      /* ouverture du socket */
      sockfd = socket (AF_INET,SOCK_STREAM,0);
      if (sockfd<0) {printf ("impossible d'ouvrir le socket\n");exit(0);}
     
      /* initialisation des parametres */
      bzero((char*) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family       = AF_INET;
      serv_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
      serv_addr.sin_port         = htons(atoi(argv[1]));
     
      /* effectue le bind */
      if (bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
         {printf ("impossible de faire le bind\n");exit(0);}
     
      /* écoute */
      listen(sockfd,1);
     
      /* attente de connection d'un client */
      clilen = sizeof (cli_addr);
      newsockfd = accept (sockfd,(struct sockaddr*) &cli_addr, &clilen);
      if (newsockfd<0) {printf ("accept error\n"); exit(0);}
      printf ("connection accepted\n");
     
     
		printf("je suis serveur\n");
		grille();
		
		do{	
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
				printf("%d\t",g[i][j]);
			}
			printf("\n");
		}
		//envoyer la grille
		write (newsockfd,&g,sizeof(g));
		
		//fournir un nombre
		nombreAfournir=nombre();
		printf("nombre= %d \n",nombreAfournir);
		write (newsockfd,&nombreAfournir,sizeof(nombreAfournir));
		
		//debut time jeu
		time(&debutP);
		
		//debut time 
		time(&debut);
		
		//lire indices
		int v;
		int faux=0;
		do{
		read(newsockfd,&indices,sizeof(indices));

		//fin time		
		time(&fin);
		
		v=verification(indices[0][0],indices[0][1],indices[1][0],indices[1][1],nombreAfournir);
		if(v==0){
		faux=-1;
		}
		write (newsockfd,&faux,sizeof(faux));
		}while(v==1);
		printf("i1=%d, j1=%d\n",indices[0][0],indices[0][1]);
        printf("i2=%d, j2=%d\n",indices[1][0],indices[1][1]);
		
		//test de temps de la reponce
		diffTemps = difftime(fin,debut);
		if(diffTemps>9){
			score-=10;
			if(score<0)
				score =0;
		}else{
			score+=25;
		}
		printf("Le score: %d\n",score);
	
		//envoyer le score
		write (newsockfd,&score,sizeof(score));

		//fin time jeu
		time(&finP);
		diffTemps1 = difftime(finP,debutP);
		
		}while(diffTemps1<20);
		strcpy(message,"Partie fini ^_^!\n");
		write (newsockfd,&message,sizeof(message));
       /*  attention il s'agit d'une boucle infinie 
        *  la socket n'est jamais ferme !
        */
     
       return 1;
     }


