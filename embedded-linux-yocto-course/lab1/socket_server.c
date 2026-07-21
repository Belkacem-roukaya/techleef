/* 
* Serveur TCP simple en langage C. 
* * Fonctionnement : 
* 1. Création d'une socket TCP. 
* 2. Association de la socket au port 2000. 
* 3. Mise en écoute du serveur. 
* 4. Acceptation d'une connexion cliente. 
* 5. Réception d'un message du client. 
* 6. Lecture d'une réponse depuis le clavier. 
* 7. Envoi de la réponse au client. 
* 8. Fermeture des sockets. 
*/

#include <stdio.h>                   //Fournit notamment : * - printf() * - perror() * - fgets() * - stdin 
#include <stdlib.h>                 //Fournit notamment : * - exit() * - EXIT_SUCCESS * - EXIT_FAILURE 
#include <string.h>                //Fournit notamment : * - strlen() * - memset() 
#include <unistd.h>               //Fournit notamment : * - close() 
#include <sys/socket.h>          //Fournit les fonctions et types liés aux sockets : * - socket() * - bind() * - listen() * - accept() * - recv() * - send() * - struct sockaddr 
#include <arpa/inet.h>          //Fournit notamment : * - htons() * - inet_ntop() 
#include <netinet/in.h>        //Fournit les structures et constantes IPv4 : * - struct sockaddr_in * - INADDR_ANY */


#define SERVER_PORT 2000     //Numéro de port utilisé par le serveur.
#define BUFFER_SIZE 1024    //Taille maximale des buffers utilisés pour recevoir et envoyer des données. 
#define LISTEN_BACKLOG 1   /* Nombre maximal de connexions pouvant rester en attente dans la file
						    * avant d'être acceptées par accept(). */

int main(void){

	int server_fd;   	                                  /* * Descripteur de fichier de la socket serveur. 
						                                  * * Cette socket sera utilisée pour : 
						                                  * - écouter les demandes de connexion ; 
						                                  * - accepter de nouveaux clients. 
						                                  */

	int client_fd;                                         /* * Descripteur de fichier de la socket cliente. 
						                                   * * Cette socket sera créée par accept(). 
						                                   * Elle sera utilisée pour communiquer avec le client connecté. 
						                                   */

	struct sockaddr_in server_addr;			               /* * Structure contenant l'adresse IPv4 du serveur. 
					                                       * * Elle contiendra : 
						                                   * - la famille d'adresse ; 
						                                   * - l'adresse IP ; 
						                                   * - le port. 
						                                   */

	struct sockaddr_in client_addr;			                /* * Structure qui recevra l'adresse du client connecté. 
											                * * Elle permettra notamment de connaître : 
											                * - l'adresse IP du client ; 
											                * - le port source du client. 
											                */

	socklen_t client_addr_len = sizeof(client_addr);		/* * Taille de la structure client_addr. 
											                * * accept() utilise un pointeur vers cette variable. 
											                * Cette variable doit donc être initialisée avant l'appel à accept(). 
											                */

	char receive_buffer[BUFFER_SIZE];					    /* Buffer dans lequel les données reçues du client seront enregistrées.*/

	ssize_t received_bytes;									/* * Variable contenant le nombre d'octets retourné par recv(). 
															* * ssize_t est un entier signé adapté aux fonctions d'entrée/sortie. 
															* Il peut contenir : 
															* - une valeur positive : nombre d'octets reçus ; 
															* - 0 : client déconnecté ; 
															* - -1 : erreur. 
															*/

	ssize_t sent_bytes;										/* Variable contenant le nombre d'octets envoyés par send().*/

	char client_ip[INET_ADDRSTRLEN];						/* * Buffer utilisé pour convertir l'adresse IP binaire du client 
															* en texte lisible, par exemple "192.168.1.20". 
															*/

	server_fd = socket(AF_INET, SOCK_STREAM, 0);			/* * Étape 1 : création de la socket serveur. 
															* * AF_INET : 
															* utilisation du protocole IPv4. 
															* * SOCK_STREAM : * utilisation d'une communication orientée connexion. 
															* Pour AF_INET, cela correspond normalement à TCP. 
															* * 0 : 
															* demande au système de choisir automatiquement le protocole 
															* adapté à AF_INET + SOCK_STREAM, donc TCP. 
															*/

	if (server_fd < 0){										/* * socket() retourne : 
															* - un entier positif ou nul en cas de succès ; 
															* - -1 en cas d'erreur. 
															*/

	perror("Cannot create TCP socket");						/* * perror() affiche notre message suivi du message correspondant 
															* à la dernière erreur système. 
															* * Exemple : 
															* Cannot create TCP socket: Permission denied 
															*/

	exit(EXIT_FAILURE);										/* * EXIT_FAILURE indique que le programme se termine avec une erreur. */
	}


	memset(&server_addr, 0, sizeof(server_addr));           /* * Avant de remplir server_addr, on met toute la structure à zéro. 
	                                                        * * Cela évite de conserver des valeurs indéfinies dans certains champs 
															* inutilisés ou réservés. 
	                                                        */

	server_addr.sin_family = AF_INET;					    /* * sin_family précise la famille d'adresse utilisée. 
															* * Ici, AF_INET signifie IPv4. 
															*/

	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);		/* * INADDR_ANY signifie que le serveur acceptera les connexions 
															* reçues sur toutes les interfaces réseau de la machine. 
															* * Cela correspond à l'adresse : 
															* * 0.0.0.0 
															* * Par exemple, le serveur pourra recevoir des connexions via : 
															* - l'interface Wi-Fi ; 
															* - l'interface Ethernet ; 
															* - l'interface loopback ; 
															* - toute autre interface IPv4 active. 
															* * htonl() convertit la valeur vers l'ordre des octets réseau. 
															*/

		server_addr.sin_port = htons(SERVER_PORT);			/* * Le port réseau doit être stocké dans l'ordre des octets réseau. 
															* * htons signifie : 
															* * Host TO Network Short 
															* * Host : * format utilisé par le processeur local. 
															* * Network : * format standard utilisé par les protocoles réseau. 
															* * Short : 
															* entier non signé sur 16 bits, comme un numéro de port. 
															*/

		if (bind(server_fd,	(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
		/* * Étape 2 : association de la socket à une adresse IP et à un port. 		,
		* * bind() reçoit : 
		* * 1. server_fd 
		*      le descripteur de la socket créée précédemment. 
		* * 2. (struct sockaddr *)&server_addr 
		*      un pointeur vers l'adresse du serveur. 
		* * 3. sizeof(server_addr) 
		*      la taille de la structure d'adresse. 
		*/

        {  
			perror("Failed to bind the socket");            /* * Si bind() échoue, cela peut signifier par exemple : 
															* - que le port est déjà utilisé ; 
															* - que l'adresse n'est pas valide ; 
															* - que le processus n'a pas les permissions nécessaires. 
                                                            */

			close(server_fd);								/* * Comme la socket a déjà été créée, il faut la fermer 
			                                                * avant de terminer le programme. */
														     
			exit(EXIT_FAILURE);
		}

		if (listen(server_fd, LISTEN_BACKLOG) < 0)
		/* * Étape 3 : passage de la socket en mode écoute. 
		* * listen() transforme la socket en socket passive. 
		* Elle attend désormais des demandes de connexion. 
		* * LISTEN_BACKLOG vaut ici 1. 
		* Cela signifie qu'une connexion peut rester en attente dans la file. 
		*/

		{ 
			error("Cannot listen for incoming connections"); 
			close(server_fd); exit(EXIT_FAILURE); 
		}
       
		printf( "Server listening on 0.0.0.0:%d...\n", SERVER_PORT );/* * À ce stade : 
		                                                             * * - la socket existe ; 
																	 * - elle est liée au port 2000 ; 
																	 * * - elle écoute les connexions. 
		                                                             */

		client_fd = accept( 
			server_fd, 
			(struct sockaddr *)&client_addr, 
			&client_addr_len );                                     /* * Étape 4 : attente et acceptation d'un client. 
			                                                         * * accept() est bloquante par défaut. 
																	 * * Cela signifie que le programme s'arrête temporairement ici 
																	 * * tant qu'aucun client ne se connecte. 
																	 * * * Quand un client arrive, accept() : 
																	 * * * 1. retire la première connexion de la file d'attente ; 
																	 * * 2. crée une nouvelle socket connectée ; 
																	 * * 3. retourne son descripteur dans client_fd ; 
																	 * * 4. remplit client_addr avec l'adresse du client ; 
																	 * * 5. met à jour client_addr_len. 
			                                                         */

		if (client_fd < 0)											/* 
																	 * accept() retourne -1 en cas d'erreur. 
																	 */

		{
			perror("Cannot accept client connection"); 
			close(server_fd); 
			
			exit(EXIT_FAILURE);
		}

		if (inet_ntop(                                             /* * inet_ntop() convertit une adresse IPv4 binaire en texte. 
			                                                       * * Par exemple : 
																   * * valeur binaire 
																   * * devient 
																   * * "192.168.1.50" 
			                                                       */
			AF_INET,
			&client_addr.sin_addr,
			client_ip,
			sizeof(client_ip)
			) == NULL)
		{
			perror("Cannot convert client IP address");
			strcpy(client_ip, "unknown");                           /* * L'échec de cette conversion n'empêche pas forcément la communication.
			                                                        * On place donc une valeur par défaut dans le buffer. 
			                                                        */
		}
		
		printf( 
			"Client connected from %s:%u\n", 
			client_ip, 
			ntohs(client_addr.sin_port) );                          /* * ntohs() signifie : 
			                                                        * * Network TO Host Short 
																	* * Le port du client est enregistré dans l'ordre réseau. 
																	* ntohs() le convertit en entier utilisable par le système local. 
			                                                        */
												
		/* * Étape 5 : boucle de communication. 
		* * Le serveur : 
		* - attend un message ; 
		* - l'affiche ; 
		* - attend une réponse au clavier ; 
		* - envoie cette réponse ; 
		* - recommence. 
		*/

		while (1) {
			/* * recv() reçoit des données depuis la socket cliente. 
			* * Paramètre 1 : 
			* client_fd * socket connectée au client. 
			* * Paramètre 2 : 
			* receive_buffer 
			* adresse du buffer de réception. 
			* * Paramètre 3 : 
			* sizeof(receive_buffer) - 1 
			* nombre maximal d'octets à recevoir. 
			* * On réserve une case pour ajouter '\0'. 
			* * Paramètre 4 : 
			* 0 
			* aucun drapeau particulier. 
			*/
		received_bytes = recv( 
			client_fd, 
			receive_buffer, 
			sizeof(receive_buffer) - 1, 
			0 
		);

		/* 
		* Cas 1 : recv() retourne une valeur négative. 
		* * Cela indique une erreur. 
		*/
		if (received_bytes < 0) 
		{ 
			perror("Failed to receive data from client"); 
			break; 
		}

		/* 
		* Cas 2 : recv() retourne 0. 
		* * Cela signifie que le client a fermé proprement sa connexion. 
		*/
	     if (received_bytes == 0) 
		 { 
			printf("Client disconnected\n"); 
			break; 
		}

		receive_buffer[received_bytes] = '\0';              				/* 
																			* recv() reçoit des octets bruts. 
																			* * Elle n'ajoute pas automatiquement le caractère nul '\0' 
																			* nécessaire pour terminer une chaîne C. 
																			* * Si le client envoie 5 octets : 
																			* * H e l l o 
																			* * received_bytes vaut 5. 
																			* * On écrit donc '\0' à l'indice 5 : 
																			* * H e l l o \0 
																			*/
		
		printf("Client: ");                                 
		fwrite( receive_buffer, 1, (size_t)received_bytes, stdout );		/* 
																			* Affichage du message reçu. 
																			* * On utilise ici fwrite() plutôt que seulement printf("%s") 
																			* afin d'afficher exactement le nombre d'octets reçus. 
																			*/
																		    
		if (receive_buffer[received_bytes - 1] != '\n')                     /* 
																			* Si le message ne contient pas de retour à la ligne, 
																			* on en ajoute un pour améliorer l'affichage. 
																			*/
		{ 
			printf("\n"); 
		}	 
		
		printf("Server: ");                                                  /* Demande au serveur de saisir une réponse. */
		
		fflush(stdout);                                                      /* 
																			* fflush(stdout) force immédiatement l'affichage du texte. 
																			* * Sans cela, le texte "Server: " pourrait rester dans un buffer 
																			* avant d'apparaître dans le terminal. 
																			*/

		if (fgets(                                                           /* 
																			* fgets() lit une ligne depuis l'entrée standard. 
																			* * stdin correspond ici au clavier. 
																			* * fgets() : 
																			* - lit au maximum BUFFER_SIZE - 1 caractères ; 
																			* - ajoute automatiquement '\0' ; 
																			* - conserve généralement le caractère '\n'. 
																			*/
		        send_buffer,
				sizeof(send_buffer), 
				stdin 
			) == NULL)

       { 
		/* * Cela peut arriver si : 
		* - l'utilisateur envoie EOF avec Ctrl+D ; 
		* - une erreur de lecture se produit. 
		*/ 
			printf("\nInput closed, stopping server communication\n"); 
			break; 
		}

		sent_bytes = send(                                                  /* 
																			* send() envoie les données au client. 
																			* * Paramètre 1 : 
																			* client_fd 
																			* socket du client. 
																			* * Paramètre 2 : 
																			* send_buffer 
																			* adresse des données à envoyer. 
																			* * Paramètre 3 : 
																			* strlen(send_buffer) 
																			* nombre d'octets à envoyer. 
																			* * Paramètre 4 : 
																			* 0 
																			* aucun drapeau particulier. 
																			*/
				client_fd, 
				send_buffer, 
				strlen(send_buffer), 
				0
			);

			if (sent_bytes < 0)
			{
				/* 
				* send() retourne une valeur négative en cas d'erreur. 
				*/
			perror("Failed to send data to client"); 
			break;
			}
}

if (close(client_fd) < 0)
{
	/* 
	* Étape 6 : fermeture de la socket cliente. 
	* * Elle doit être fermée avant la socket serveur, 
	* car elle représente la connexion active avec le client. 
	*/
    perror("Failed to close client socket");
}

if (close(server_fd) < 0)
{
	/* 
	* Fermeture de la socket d'écoute. 
	* * Après cette fermeture, le serveur n'accepte plus de connexions. 
	*/
     perror("Failed to close server socket");
}

return EXIT_SUCCESS;                                    /* 
														* EXIT_SUCCESS indique que le programme s'est terminé normalement. 
														*/












