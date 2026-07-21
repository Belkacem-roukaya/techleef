# Fonctionnement d'un serveur TCP en C

Ce document décrit les principales étapes du fonctionnement d'un serveur TCP sous Linux, depuis la création de la socket jusqu'à la fermeture de la connexion.

## Vue d'ensemble

Un serveur TCP suit généralement les étapes suivantes :

1. créer une socket avec `socket()` ;
2. lui associer une adresse et un port avec `bind()` ;
3. la placer en attente de connexions avec `listen()` ;
4. accepter un client avec `accept()` ;
5. recevoir et envoyer des données avec `recv()` et `send()` ;
6. fermer les sockets avec `close()`.

---

## 1. Création de la socket

```c
server_fd = socket(AF_INET, SOCK_STREAM, 0);
```

Cette instruction demande au noyau Linux de créer une socket TCP IPv4. La fonction retourne un **descripteur de fichier**.

Par exemple :

```text
server_fd = 3
```

Les descripteurs habituels sont :

| Descripteur | Ressource |
|---:|---|
| `0` | entrée standard (`stdin`) |
| `1` | sortie standard (`stdout`) |
| `2` | sortie d'erreur (`stderr`) |
| `3` | première ressource ouverte par le programme |

Une socket Linux est donc manipulée presque comme un fichier.

## 2. Préparation de l'adresse du serveur

```c
struct sockaddr_in server_addr;
```

Cette structure contient l'adresse IPv4 du serveur. Sa forme simplifiée est la suivante :

```c
struct sockaddr_in {
    sa_family_t    sin_family;
    in_port_t      sin_port;
    struct in_addr sin_addr;
};
```

Elle regroupe trois informations principales :

| Champ | Rôle |
|---|---|
| `sin_family` | famille d'adresses, par exemple IPv4 |
| `sin_port` | numéro du port |
| `sin_addr` | adresse IPv4 |

## 3. Association au port

```c
bind(
    server_fd,
    (struct sockaddr *)&server_addr,
    sizeof(server_addr)
);
```

Avant `bind()`, la socket existe, mais elle ne possède pas encore d'adresse réseau locale. Après l'appel, elle peut par exemple représenter :

```text
0.0.0.0:2000
```

Le couple **adresse IP + port** permet d'identifier le service réseau.

## 4. Mise en écoute

```c
listen(server_fd, LISTEN_BACKLOG);
```

Cette fonction transforme la socket en **socket d'écoute**. Elle ne communique pas directement avec le client : son rôle est de recevoir les nouvelles demandes de connexion.

## 5. Acceptation d'un client

```c
client_fd = accept(
    server_fd,
    (struct sockaddr *)&client_addr,
    &client_addr_len
);
```

L'appel à `accept()` crée une seconde socket. Le processus possède alors :

- `server_fd`, la socket d'écoute ;
- `client_fd`, la socket utilisée pour communiquer avec le client accepté.

```text
             server_fd
                 │
          attend les clients
                 │
              accept()
                 │
             client_fd
                 │
     communication avec un client
```

> **À retenir :** `server_fd` continue d'attendre de nouvelles connexions, tandis que `client_fd` sert aux échanges avec un client précis.

## 6. Réception d'un message

```c
received_bytes = recv(
    client_fd,
    receive_buffer,
    sizeof(receive_buffer) - 1,
    0
);
```

Trois résultats sont possibles :

| Valeur retournée | Signification |
|---:|---|
| `> 0` | nombre d'octets reçus |
| `0` | le client a fermé proprement la connexion |
| `< 0` | une erreur s'est produite |

Exemple : si `received_bytes` vaut `12`, le serveur a reçu 12 octets.

## 7. Terminaison de la chaîne

```c
receive_buffer[received_bytes] = '\0';
```

Le réseau transmet des octets et ne connaît pas la notion de chaîne de caractères en C. Si le client envoie `Hello`, le tampon contient les caractères suivants :

```text
H e l l o
```

Une chaîne C valide doit cependant se terminer par le caractère nul :

```text
H e l l o \0
```

Le caractère `\0` marque la fin de la chaîne.

> **Attention :** cette instruction ne doit être exécutée que si `received_bytes` est strictement positif. Si `recv()` retourne `-1`, l'index serait invalide.

## 8. Lecture de la réponse

```c
fgets(send_buffer, sizeof(send_buffer), stdin);
```

Cette fonction lit une ligne saisie au clavier et la stocke dans `send_buffer`.

```text
clavier → stdin → fgets() → send_buffer
```

## 9. Envoi de la réponse

```c
send(
    client_fd,
    send_buffer,
    strlen(send_buffer),
    0
);
```

Les données suivent alors ce chemin :

```text
send_buffer → send() → client_fd → réseau → client
```

---

## Schéma complet

```text
CLIENT                                      SERVEUR

socket()                                    socket()
   │                                           │
connect() ────────────────────────────────> bind()
                                               │
                                           listen()
                                               │
                                           accept()
                                               │
send() ────────────────────────────────────> recv()
                                               │
recv() <──────────────────────────────────── send()
                                               │
close()                                    close(client_fd)
                                           close(server_fd)
```

## Point important sur TCP

TCP ne transmet pas réellement des messages séparés : il transmet un **flux continu d'octets**.

Par exemple, le client peut effectuer deux envois successifs :

```c
send(fd, "Bonjour", 7, 0);
send(fd, "Linux", 5, 0);
```

Le serveur peut recevoir toutes les données en une seule fois :

```text
BonjourLinux
```

Il peut aussi les recevoir en plusieurs parties :

```text
Bon
jourLinux
```

TCP garantit principalement :

- l'ordre des octets ;
- la fiabilité de la transmission ;
- la retransmission en cas de perte.

En revanche, TCP ne garantit pas qu'un appel à `send()` corresponde exactement à un appel à `recv()`.

Pour créer un véritable protocole réseau, il faut donc définir une méthode permettant de séparer les messages, par exemple :

- un retour à la ligne ;
- une taille placée avant le message ;
- un caractère spécial ;
- une structure de protocole précise.

> **Conclusion :** une application TCP doit elle-même définir les limites de ses messages et gérer les envois ou réceptions partiels.
