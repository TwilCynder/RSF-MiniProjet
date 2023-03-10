#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#include <unistd.h>


typedef struct
{
    char Buffer[500];
    int finFich;
}sFich;

int executeFile(char filename[20])
{
        char fichier2[20];
        char fichierNom[20];
        strcpy(fichier2,filename);

        char copy[20];
        int i,j = 0, lg;
        lg=strlen(fichier2);
        for(i=0;i<lg;i++)
        
            if(fichier2[i] != ' ' && fichier2[i] != '\t' && fichier2[i]!= '\n'){copy[j]=fichier2[i];
                j++;       
                copy[j]='\0';
        }
        char str1[20]={'b','a','s','h',' '};
        char command[50];

        strcat(str1,copy);
        //printf("\n TEST:%s",str1);
        system(str1);
        printf("\nle fichier a été exécuté");

        return 1;
        
} 


void envoyerFichier(int sock,char NomFichier[MAX])
{
    char buff3[500];
    sFich buffer;
    buffer.finFich = 0;
    int i = 0 , j = 1;
    //NomFichier[i]=Buff[i];
    //for(i,j; Buff[j] != '\0';i++,j++)
    //    NomFichier[i] = Buff[j];
    //strcpy(NomFichier,Buff);

    FILE* fichier = fopen(NomFichier , "rb");
    printf("nom fichier envoye:%s \n",NomFichier);
    if ( fichier != NULL) // ce test       choue si le fichier n'est pas ouvert
    {
        while (buffer.finFich == 0)
        {
            if(fread( buffer.Buffer , sizeof(char) , 500 , fichier ) <= 0)
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



// Function designed for chat between client and server.
void func(int connfd)
{
        char buff[MAX];char buff2[20];
        int n;
        // infinite loop for chat
        for (;;) {
                bzero(buff, MAX);

                // read the message from client and copy it in buffer
                if(read(connfd, buff, sizeof(buff))>=0){

                // print buffer which contains the client contents
                printf("From client: %s\t To client : ", buff);
                break;
            };
                bzero(buff, MAX);
                n = 0;
                // copy server message in the buffer
                while ((buff[n++] = getchar()) != '\n')
                ;

                // and send that buffer to client
                write(connfd, buff, sizeof(buff));

                // if msg contains "Exit" then server exit and chat ended.
                if (strncmp("exit", buff, 4) == 0) {
                        printf("Server Exit...\n");
                        break;
                }
        }
        strcpy(buff2,buff);
        printf("\nTEST:%s",buff2);
        executeFile(buff2);
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


// Driver function
int main()
{
        char buff2[MAX];
        int sockfd, connfd, len;
        struct sockaddr_in servaddr, cli;

        // socket create and verification
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
                printf("socket creation failed...\n");
                exit(0);
        }
        else
                printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = inet_addr("192.168.10.1");//htonl(INADDR_ANY);


        // Binding newly created socket to given IP and verification
        if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
                printf("socket bind failed...\n");
                exit(0);
        }
        else
                printf("Socket successfully binded..\n");

        // Now server is ready to listen and verification
        if ((listen(sockfd, 5)) != 0) {
                printf("Listen failed...\n");
                exit(0);
        }
        else
                printf("Server listening..\n");
        len = sizeof(cli);

        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
                printf("server accept failed...\n");
                exit(0);
        }
        else
                printf("server accept the client...\n");

                // read the message from client and copy it in buffer
        func(connfd);
        // After chatting close the socket
        close(sockfd);
}

