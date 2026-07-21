/*
 * Serveur TCP simple en langage C.
 *
 * Fonctionnement :
 * 1. Création d'une socket TCP.
 * 2. Association de la socket au port 2000.
 * 3. Mise en écoute du serveur.
 * 4. Acceptation d'une connexion cliente.
 * 5. Réception d'un message du client.
 * 6. Lecture et envoi d'une réponse.
 * 7. Répétition de la communication.
 * 8. Fermeture des sockets.
 */

#include <stdio.h>          /* printf(), perror(), fgets(), fwrite(), fflush() */
#include <stdlib.h>         /* EXIT_SUCCESS et EXIT_FAILURE */
#include <string.h>         /* memset(), strlen() et strcpy() */
#include <unistd.h>         /* close() */
#include <sys/socket.h>     /* socket(), bind(), listen(), accept(), recv(), send() */
#include <arpa/inet.h>      /* htons(), htonl(), ntohs() et inet_ntop() */
#include <netinet/in.h>     /* struct sockaddr_in et INADDR_ANY */

#define SERVER_PORT 2000   /* Port d'écoute du serveur. */
#define BUFFER_SIZE 1024   /* Taille des buffers d'envoi et de réception. */
#define LISTEN_BACKLOG 1   /* Nombre de connexions autorisées dans la file d'attente. */

int main(void)
{
    int server_fd;                              /* Socket d'écoute du serveur. */
    int client_fd;                              /* Socket connectée au client. */
    struct sockaddr_in server_addr;             /* Adresse IPv4 du serveur. */
    struct sockaddr_in client_addr;             /* Adresse IPv4 du client. */
    socklen_t client_addr_len;                  /* Taille de l'adresse du client. */
    char client_ip[INET_ADDRSTRLEN];            /* Adresse IP lisible du client. */
    char receive_buffer[BUFFER_SIZE];           /* Message reçu du client. */
    char send_buffer[BUFFER_SIZE];              /* Réponse envoyée au client. */
    ssize_t received_bytes;                     /* Nombre d'octets reçus. */
    ssize_t sent_bytes;                         /* Nombre d'octets envoyés. */

    /* Étape 1 : création de la socket TCP/IPv4. */
    server_fd = socket(AF_INET, SOCK_STREAM, 0); /* AF_INET = IPv4, SOCK_STREAM = TCP. */
    if (server_fd < 0)                           /* Une valeur négative indique une erreur. */
    {
        perror("Cannot create TCP socket");
        return EXIT_FAILURE;
    }

    /* Étape 2 : préparation et association de l'adresse locale. */
    memset(&server_addr, 0, sizeof(server_addr)); /* Initialise la structure à zéro. */
    server_addr.sin_family = AF_INET;             /* Utilise une adresse IPv4. */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Écoute sur toutes les interfaces. */
    server_addr.sin_port = htons(SERVER_PORT);    /* Convertit le port au format réseau. */

    if (bind(server_fd,
             (struct sockaddr *)&server_addr,    /* Adresse locale du serveur. */
             sizeof(server_addr)) < 0)           /* Taille de la structure d'adresse. */
    {
        perror("Failed to bind the socket");
        close(server_fd);
        return EXIT_FAILURE;
    }

    /* Étape 3 : mise en écoute de la socket. */
    if (listen(server_fd, LISTEN_BACKLOG) < 0)   /* Active la file des connexions entrantes. */
    {
        perror("Cannot listen for incoming connections");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Server listening on 0.0.0.0:%d...\n", SERVER_PORT);

    /* Étape 4 : attente et acceptation d'un client. */
    client_addr_len = sizeof(client_addr);       /* accept() peut modifier cette taille. */
    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &client_addr_len);
    if (client_fd < 0)
    {
        perror("Cannot accept client connection");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (inet_ntop(AF_INET,                       /* Convertit l'adresse du client en texte. */
                  &client_addr.sin_addr,
                  client_ip,
                  sizeof(client_ip)) == NULL)
    {
        perror("Cannot convert client IP address");
        strcpy(client_ip, "unknown");           /* Valeur affichée si la conversion échoue. */
    }

    printf("Client connected from %s:%u\n",
           client_ip,
           (unsigned int)ntohs(client_addr.sin_port)); /* Convertit le port au format local. */

    /* Étapes 5 à 7 : boucle de réception et d'envoi. */
    while (1)
    {
        received_bytes = recv(client_fd,                  /* Attend un message du client. */
                              receive_buffer,             /* Stocke ici les octets reçus. */
                              sizeof(receive_buffer) - 1, /* Réserve une place pour '\0'. */
                              0);                          /* Aucun drapeau particulier. */
        if (received_bytes < 0)
        {
            perror("Failed to receive data from client");
            break;
        }
        if (received_bytes == 0)                          /* Le client a fermé la connexion. */
        {
            printf("Client disconnected\n");
            break;
        }

        receive_buffer[received_bytes] = '\0';            /* Termine la chaîne C. */
        printf("Client: ");
        fwrite(receive_buffer, 1, (size_t)received_bytes, stdout);
        if (receive_buffer[received_bytes - 1] != '\n')  /* Améliore l'affichage si nécessaire. */
        {
            printf("\n");
        }

        printf("Server: ");
        fflush(stdout);                                  /* Affiche l'invite immédiatement. */
        if (fgets(send_buffer, sizeof(send_buffer), stdin) == NULL)
        {
            printf("\nInput closed, stopping server communication\n");
            break;
        }

        sent_bytes = send(client_fd,                     /* Socket connectée au client. */
                          send_buffer,                   /* Réponse à envoyer. */
                          strlen(send_buffer),           /* Nombre d'octets utiles. */
                          0);                            /* Aucun drapeau particulier. */
        if (sent_bytes < 0)
        {
            perror("Failed to send data to client");
            break;
        }
    }

    /* Étape 8 : fermeture des sockets. */
    if (close(client_fd) < 0)                 /* Ferme d'abord la connexion active. */
    {
        perror("Failed to close client socket");
    }
    if (close(server_fd) < 0)                 /* Ferme ensuite la socket d'écoute. */
    {
        perror("Failed to close server socket");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
