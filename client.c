#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define TAILLE_MAX 1000
#include <unistd.h>

typedef struct
{
    char Buffer[500];
    int finFich;
}sFich;

void func(int sockfd)
{
        char buff[MAX];
        int n;
        for (;;) {
                bzero(buff, sizeof(buff));
                printf("Entrer le nom de fichier à exécuter: ");
                n = 0;
                while ((buff[n++] = getchar()) != '\n')
                        ;
                write(sockfd, buff, sizeof(buff));
                bzero(buff, sizeof(buff));

                read(sockfd, buff, sizeof(buff));
                printf("From Server : %s", buff);
                if ((strncmp(buff, "exit", 4)) == 0) {
                        printf("Client Exit...\n");
                        break;
                }
        }

}

int readFile(char filename[10])
{
        FILE* fichier=(sizeof(char)*20);
        fichier=*filename;
        fichier=fopen(filename,"r+");
        char chaine[TAILLE_MAX]="";
        int car=0;
        if (fichier != NULL)
        {
                do
        {

            fgets(chaine,TAILLE_MAX,fichier); // On lit le caractère

                        printf("%s", chaine); // On l'affiche
                        car=fgetc(fichier);
        } while (car != EOF);

                fclose(fichier);
                return 1;
        }
        else{printf("impossible d'ouvrir le fichier");
        return 0;}
}

int executeFile(char* filename){
        FILE* fichier=(sizeof(char)*20);
        fichier=*filename;
        fichier=fopen(filename,"r+");
        char chaine[TAILLE_MAX]="";
        int car=0;
        if (fichier != NULL)
        {

                system(strcat(filename,"./"));
                printf("\nle fichier a été exécuté");
                fclose(fichier);
                return 1;
        }
        else{printf("impossible d'ouvrir le fichier");}
        return 0;
}


void RecevoirFich(int sock , char * Trame)
{
    char tmpNom[499];
    char tst;
    int sortie = 0;
    int lngRcv;
    int i = 0 , j = 1;
    for(i,j; Trame[j] != '\0';i++,j++)
        tmpNom[i] = Trame[j];
    tmpNom[i] = '\0';
    strcpy(tmpNom,Trame);
    printf("3 - reception fichier : %s\n",tmpNom);
    FILE * Fich = fopen(tmpNom,"tmpNom");

    sFich buff;
    buff.finFich = 0;

    if(Fich != NULL )
    {
        while( buff.finFich == 0)
        {
            recv(sock, &buff, sizeof( buff ) , 0);
            fwrite(buff.Buffer , 1 , strlen(buff.Buffer)*sizeof(char) , Fich);
        }
        printf("reception %s fini \n", tmpNom);
        fclose(Fich);
    }
        else{printf("\n erreur ouverture fichier\n");};
}

void envoyerFichier(int sock,char * Buff)
{
    char NomFichier[MAX];
    char buff[500] ;
    sFich buffer;
    buffer.finFich = 0;
    int i = 0 , j = 1;
    NomFichier[i]=Buff[i];
    for(i,j; Buff[j] != '\0';i++,j++)
        NomFichier[i] = Buff[j];
    strcpy(NomFichier,Buff);
    bzero(NomFichier,MAX);

    FILE* fichier =fopen(NomFichier , "rb");


    printf("nom fichier envoyé:%s\n",NomFichier);
    if ( fichier != NULL) // ce test échoue si le fichier n'est pas ouvert
    {
        while (buffer.finFich == 0)
        {
            if( fread( buffer.Buffer , sizeof(char) , 500 , fichier ) <= 0)
                buffer.finFich = 1;
            send(sock, &buffer, sizeof(buffer), 0);
            printf("%s\n" , buffer.Buffer);
            memset(buffer.Buffer,'\0',(500-1));
        }

        printf("envoi %s fini \n", NomFichier);
        fclose(fichier);
    }
    else
        printf("erreur ouverture fichier\n");
}


int main()
{
        // function for chat
//executeFile("TCPclient.c");

        int sockfd, connfd,rc;
        struct sockaddr_in servaddr, cli;

        // socket create and varification
        sockfd = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
        if (sockfd == INVALID_SOCKET) {
                printf("socket creation failed...\n");
                exit(0);
        }
        else
                printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("192.168.10.1");
        servaddr.sin_port = htons(PORT);


        cli.sin_family=AF_INET;
        cli.sin_addr.s_addr=htonl(INADDR_ANY);
        cli.sin_port=htons(0);
        // connect the client socket to server socket
        // connect the client socket to server socket
        if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
                printf("connection with the server failed...\n");
                exit(0);
        }
        else
                printf("connected to the server..\n");

        // function for chat
        //func(sockfd);
        func(sockfd);
        envoyerFichier(sockfd,"client20");
        // close the socket

//rc=bind(sockfd, (struct sockaddr *)&cli,sizeof(cli));
        //rc=sendto(sockfd,"hello",100,0,(struct sockaddr *)&servaddr,sizeof(servaddr));

        // close the socket
        close(sockfd);
        return 1;
}
