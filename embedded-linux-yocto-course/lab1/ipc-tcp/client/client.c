/*
 * Client TCP simple en langage C.
 *
 * Fonctionnement :
 * 1. Création d'une socket TCP.
 * 2. Préparation de l'adresse IPv4 du serveur.
 * 3. Conversion de l'adresse IP avec inet_pton().
 * 4. Connexion au serveur avec connect().
 * 5. Lecture d'un message depuis le clavier.
 * 6. Envoi du message au serveur.
 * 7. Réception et affichage de la réponse.
 * 8. Répétition de la communication.
 * 9. Fermeture de la socket.
 */

#include <stdio.h>          /* printf(), perror(), fgets(), fwrite(), fflush() */
#include <stdlib.h>         /* EXIT_SUCCESS et EXIT_FAILURE */
#include <string.h>         /* memset(), strlen() et strcmp() */
#include <unistd.h>         /* close() */
#include <sys/socket.h>     /* socket(), connect(), send() et recv() */
#include <netinet/in.h>     /* struct sockaddr_in */
#include <arpa/inet.h>      /* htons() et inet_pton() */

#define SERVER_ADDRESS "192.168.1.51"  /* Adresse IPv4 du serveur local. */
#define SERVER_PORT 2000             /* Port d'écoute du serveur. */
#define BUFFER_SIZE 1024             /* Taille des buffers d'envoi et de réception. */

int main(void)
{
    int socket_fd;                              /* Descripteur de la socket cliente. */
    int conversion_result;                     /* Résultat retourné par inet_pton(). */
    struct sockaddr_in server_addr;            /* Adresse IPv4 et port du serveur. */
    char send_buffer[BUFFER_SIZE];             /* Message à envoyer au serveur. */
    char receive_buffer[BUFFER_SIZE];          /* Réponse reçue du serveur. */
    ssize_t sent_bytes;                        /* Nombre d'octets envoyés. */
    ssize_t received_bytes;                    /* Nombre d'octets reçus. */

    /* Étape 1 : création de la socket TCP/IPv4. */
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); /* AF_INET = IPv4, SOCK_STREAM = TCP. */
    if (socket_fd < 0)                           /* Une valeur négative indique une erreur. */
    {
        perror("Cannot create TCP socket");     /* Affiche la cause de l'erreur système. */
        return EXIT_FAILURE;                    /* Termine le programme avec une erreur. */
    }

    /* Étape 2 : préparation de l'adresse du serveur. */
    memset(&server_addr, 0, sizeof(server_addr)); /* Initialise tous les champs à zéro. */
    server_addr.sin_family = AF_INET;             /* Utilise une adresse IPv4. */
    server_addr.sin_port = htons(SERVER_PORT);    /* Convertit le port au format réseau. */

    /* Étape 3 : conversion de l'adresse IP textuelle en format binaire. */
    conversion_result = inet_pton(AF_INET, SERVER_ADDRESS, &server_addr.sin_addr);
    if (conversion_result == 0)                 /* L'adresse textuelle n'est pas valide. */
    {
        fprintf(stderr, "Invalid IPv4 address: %s\n", SERVER_ADDRESS);
        close(socket_fd);                       /* Libère la socket avant de quitter. */
        return EXIT_FAILURE;
    }
    if (conversion_result < 0)                  /* Une erreur système s'est produite. */
    {
        perror("Failed to convert the server IPv4 address");
        close(socket_fd);                       /* Libère la socket avant de quitter. */
        return EXIT_FAILURE;
    }

    /* Étape 4 : connexion au serveur. */
    if (connect(socket_fd,
                (struct sockaddr *)&server_addr, /* connect() attend un struct sockaddr. */
                sizeof(server_addr)) < 0)         /* Fournit la taille de l'adresse. */
    {
        perror("Cannot connect to the server"); /* Le serveur est peut-être indisponible. */
        close(socket_fd);                       /* Ferme la socket non connectée. */
        return EXIT_FAILURE;
    }

    printf("Connected to server %s:%d\n",      /* Confirme que la connexion est établie. */
           SERVER_ADDRESS,
           SERVER_PORT);

    /* Étapes 5 à 8 : boucle d'envoi et de réception. */
    while (1)
    {
        printf("Client: ");                    /* Affiche l'invite de saisie. */
        fflush(stdout);                        /* Force l'affichage sans attendre '\n'. */

        if (fgets(send_buffer, sizeof(send_buffer), stdin) == NULL) /* Lit le clavier. */
        {
            printf("\nInput closed, disconnecting from server\n");
            break;                             /* Quitte la boucle en fin d'entrée. */
        }

        if (strcmp(send_buffer, "quit\n") == 0) /* La commande quit arrête le client. */
        {
            printf("Closing client connection\n");
            break;
        }

        sent_bytes = send(socket_fd,            /* Utilise la socket connectée. */
                          send_buffer,           /* Pointe vers le message à envoyer. */
                          strlen(send_buffer),   /* Envoie seulement les caractères utiles. */
                          0);                    /* Aucun drapeau particulier. */
        if (sent_bytes < 0)                     /* send() retourne -1 en cas d'erreur. */
        {
            perror("Failed to send data to the server");
            break;
        }

        received_bytes = recv(socket_fd,                  /* Attend la réponse du serveur. */
                              receive_buffer,             /* Stocke ici les octets reçus. */
                              sizeof(receive_buffer) - 1, /* Réserve une place pour '\0'. */
                              0);                          /* Aucun drapeau particulier. */
        if (received_bytes < 0)                           /* Erreur de réception. */
        {
            perror("Failed to receive data from the server");
            break;
        }
        if (received_bytes == 0)                          /* Le serveur a fermé la connexion. */
        {
            printf("Server disconnected\n");
            break;
        }

        receive_buffer[received_bytes] = '\0';            /* Termine la chaîne C. */
        printf("Server: ");                              /* Préfixe la réponse affichée. */
        fwrite(receive_buffer,                            /* Affiche exactement les octets reçus. */
               1,
               (size_t)received_bytes,
               stdout);

        if (receive_buffer[received_bytes - 1] != '\n')  /* Ajoute un saut de ligne si nécessaire. */
        {
            printf("\n");
        }
    }

    /* Étape 9 : fermeture de la socket cliente. */
    if (close(socket_fd) < 0)                 /* close() retourne -1 en cas d'erreur. */
    {
        perror("Failed to close the client socket");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;                      /* Le programme se termine normalement. */
}
