#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>  /* strlen, strcmp */
#include <ctype.h> 
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

GtkWidget *resultat;


int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000],server_reply2[200];
    int tab[2] = {};
int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    
     
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("vien de me connecter\n");
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "test.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    resultat = GTK_WIDGET(gtk_builder_get_object(builder,"resultat"));
    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    close(sock);
    printf("fin parte\n");
    gtk_main_quit();
}

void on_ciseau_clicked(){
    tab[1]=1;
    if( send(sock , tab , sizeof(tab) , 0) < 0)
        {
            puts("Send failed");
        }
        if( recv(sock , server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
    }
    printf("ciseau\n");
    printf("%s\n",server_reply);
    gtk_label_set_text(GTK_LABEL(resultat), (const gchar*) server_reply);
}
void on_feuille_clicked(){
    tab[1]=2;
    if( send(sock , tab , sizeof(tab) , 0) < 0)
    {
        puts("Send failed");
    }
    if( recv(sock , server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
    }
    printf("feuille\n");
    printf("%s\n",server_reply);
    gtk_label_set_text(GTK_LABEL(resultat), (const gchar*) server_reply);
}

void on_pierre_clicked(){
    tab[1]=3;
    if( send(sock , tab , sizeof(tab) , 0) < 0)
    {
        puts("Send failed");
    }
    if( recv(sock , server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
    }
    printf("feuille\n");
    printf("%s\n",server_reply);
    gtk_label_set_text(GTK_LABEL(resultat), (const gchar*) server_reply);
}
void on_Select_changed(GtkComboBox *c){
    printf("Commmencer\n");
}

void on___glade_unnamed_5_changed(GtkEntry *e){
    char tmp[128];
    sprintf(tmp,"%s",gtk_entry_get_text(e));
    if(strcmp(tmp, "joueur2")!=0){
        tab[0]=2;
    }else{
        tab[0]=1;
    }
    printf(tmp);
}