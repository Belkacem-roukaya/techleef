# Client TCP en C — description

## 1. Rôle du client TCP

Le client TCP est le programme qui démarre la connexion vers le serveur.

Le serveur suit ce cycle :

```text
socket()
   ↓
bind()
   ↓
listen()
   ↓
accept()
   ↓
recv() / send()
   ↓
close()
```

Le client suit un cycle plus simple :

```text
socket()
   ↓
préparer l'adresse du serveur
   ↓
connect()
   ↓
send() / recv()
   ↓
close()
```

Le client n'utilise normalement pas :

```text
bind()
listen()
accept()
```

Ces fonctions appartiennent principalement au rôle du serveur.

Le client connaît déjà :

* l'adresse IP du serveur ;
* le port du serveur.

Il utilise donc directement `connect()` pour demander une connexion.

---

# 2. Code source

Le code source complet et commenté se trouve dans `client.c`.

---

# 3. Explication générale du fonctionnement

## 3.1 Création de la socket

Le client commence par créer une socket :


Cette fonction demande au noyau Linux de créer une ressource de communication.

La socket créée n'est pas encore connectée.

À ce moment-là, nous avons seulement :

```text
Processus client
       |
       └── socket non connectée
```

Le noyau retourne un descripteur de fichier.

Par exemple :

```text
socket_fd = 3
```

Les premiers descripteurs d'un processus sont généralement :

```text
0 = stdin
1 = stdout
2 = stderr
```

La prochaine ressource ouverte peut donc recevoir le numéro `3`.

---

## 3.2 Préparation de l'adresse du serveur

Le client doit savoir où se trouve le serveur.

Une destination réseau TCP est identifiée par :

```text
adresse IP + port
```

Dans notre cas :

```text
Adresse IP : 127.0.0.1
Port       : 2000
```

Le couple complet est donc :

```text
127.0.0.1:2000
```

La structure utilisée est :


Elle contient notamment :

```text
sin_family
    famille d'adresse

sin_port
    port du serveur

sin_addr
    adresse IPv4 du serveur
```

---

## 3.3 Adresse `127.0.0.1`

L'adresse :

```text
127.0.0.1
```

est appelée adresse de boucle locale ou adresse *loopback*.

Elle représente :

```text
cette machine elle-même
```

Si le client et le serveur sont exécutés sur le même ordinateur, on peut utiliser :


Le trafic ne quitte pas réellement la machine.

Il passe par l'interface réseau virtuelle de boucle locale :

```text
lo
```

On peut la voir sous Linux avec :


---

## 3.4 Serveur situé sur une autre machine

Si le serveur se trouve sur un autre ordinateur, il faut remplacer :


par l'adresse IPv4 réelle du serveur.

Par exemple :


Le chemin devient alors :

```text
Client
   |
   | réseau local
   v
192.168.1.50:2000
   |
Serveur
```

Le serveur doit également écouter sur une adresse accessible.

Avec :


le serveur écoute sur toutes ses interfaces IPv4.

---

# 4. Pourquoi le client n'utilise-t-il pas `bind()` ?

Le serveur doit être trouvé à une adresse connue.

Il utilise donc explicitement :

```text
IP du serveur + port du serveur
```

Par exemple :

```text
0.0.0.0:2000
```

Le serveur appelle alors :


Le client, en revanche, n'a généralement pas besoin de choisir lui-même son port local.

Lors de `connect()`, le noyau lui attribue automatiquement :

* une adresse locale appropriée ;
* un port temporaire disponible.

Ce port est appelé :

```text
port éphémère
```

Par exemple, la connexion peut devenir :

```text
Client : 127.0.0.1:49372
Serveur: 127.0.0.1:2000
```

Le port `2000` est fixe côté serveur.

Le port `49372` a été choisi automatiquement pour le client.

Le client pourrait appeler lui-même `bind()`, mais ce n'est pas nécessaire dans ce programme.

---

# 5. Fonction `inet_pton()`

La ligne importante côté client est :


Le serveur utilisait :


car il voulait écouter sur toutes ses interfaces.

Le client doit au contraire cibler une adresse précise.

L'adresse est écrite dans le code sous forme de texte :

```text
"127.0.0.1"
```

Mais le noyau attend une représentation binaire dans :


`inet_pton()` effectue la conversion.

```text
"127.0.0.1"
       |
       | inet_pton()
       v
représentation IPv4 binaire
```

Le nom peut être lu ainsi :

```text
inet
    Internet

p
    presentation, représentation textuelle

to
    vers

n
    network, représentation réseau
```

---

## Valeurs de retour de `inet_pton()`

### Retour égal à 1

```text
conversion réussie
```

### Retour égal à 0

```text
adresse textuelle invalide
```

Par exemple :

```text
192.168.1
```

ou :

```text
300.1.1.1
```

### Retour égal à -1

```text
erreur système ou famille d'adresse incorrecte
```

---

# 6. La fonction `connect()`

Le client appelle :


Cette fonction démarre la connexion TCP.

Conceptuellement :

```text
Client                                      Serveur

connect() -------------------------------> accept()
```

Pendant `connect()`, le système réalise notamment le *three-way handshake* TCP :

```text
CLIENT                                      SERVEUR

SYN -------------------------------------->

    <---------------------------------- SYN-ACK

ACK -------------------------------------->
```

Après cet échange, la connexion est établie.

Le client peut alors utiliser :


Du côté serveur, `accept()` retourne une nouvelle socket connectée.

---

## Erreur `Connection refused`

Si le client affiche :

```text
Cannot connect to the server: Connection refused
```

cela signifie généralement :

* le serveur n'est pas lancé ;
* le serveur n'écoute pas sur le port demandé ;
* l'adresse IP ou le port sont incorrects.

Il faut lancer le serveur avant le client.

---

# 7. Envoi des données avec `send()`

Le client lit une ligne avec :


Supposons que l'utilisateur écrive :

```text
Bonjour serveur
```

Le buffer contient généralement :

```text
B o n j o u r   s e r v e u r \n \0
```

Le caractère `\n` vient de la touche Entrée.

Le caractère `\0` est ajouté par `fgets()` pour terminer la chaîne C.

Le client envoie ensuite :


`strlen()` ne compte pas le caractère final `\0`.

Le caractère `\0` n'est donc pas transmis.

Ce n'est pas un problème, car le serveur ajoute son propre `\0` après la réception.

---

# 8. Réception avec `recv()`

Après avoir envoyé son message, le client appelle :


Le client attend maintenant la réponse du serveur.

Comme `recv()` est bloquante, le client reste arrêté sur cette ligne jusqu'à ce que :

* le serveur envoie des données ;
* le serveur ferme la connexion ;
* une erreur se produise.

---

## Valeur positive

Si :

```text
received_bytes = 15
```

alors 15 octets ont été reçus.

---

## Valeur égale à zéro

Si :

```text
received_bytes = 0
```

le serveur a fermé proprement sa connexion.

Le client affiche :

```text
Server disconnected
```

---

## Valeur négative

Si :

```text
received_bytes = -1
```

une erreur s'est produite.

Le programme utilise alors :


---

# 9. Ajout du caractère `\0`

`recv()` ne crée pas une chaîne C.

Elle écrit uniquement des octets dans le buffer.

Après la réception :


Supposons que le serveur envoie :

```text
Salut
```

Le client reçoit :

```text
S a l u t
```

Puis ajoute :

```text
\0
```

Le buffer devient :

```text
S a l u t \0
```

Il peut alors être utilisé comme chaîne C.

---

# 10. Communication complète

Le déroulement est le suivant.

```text
CLIENT                                      SERVEUR

socket()                                    socket()
                                               |
                                           bind()
                                               |
                                           listen()
                                               |
connect() -------------------------------> accept()
   |                                           |
   | connexion TCP établie                     |
   |                                           |
fgets()                                       |
   |                                           |
send() ------------------------------------> recv()
                                               |
                                         affiche message
                                               |
                                         fgets()
                                               |
recv() <------------------------------------ send()
   |
affiche réponse
```

Puis le cycle recommence :

```text
Client envoie
      ↓
Serveur reçoit
      ↓
Serveur répond
      ↓
Client reçoit
```

---

# 11. Ordre strict de communication

Le serveur développé précédemment commence par :


Puis il fait :


Le client doit donc faire l'inverse :


Les deux programmes se complètent :

```text
CLIENT                 SERVEUR

send()  ------------>  recv()

recv()  <------------  send()
```

Si le client et le serveur appelaient tous les deux `recv()` en premier :

```text
CLIENT                 SERVEUR

recv()                 recv()
 attend                 attend
```

Les deux resteraient bloqués.

Cette situation est appelée un blocage logique ou *deadlock* de communication.

Chaque côté attend une donnée que l'autre côté n'envoie jamais.

---

# 12. Compilation

Enregistre le code dans un fichier :

```text
client.c
```

Compile-le avec :


Signification des options :

```text
-Wall
    active de nombreux avertissements utiles

-Wextra
    active des avertissements supplémentaires

-Wpedantic
    demande une vérification plus stricte du langage C
```

L'exécutable généré est :

```text
client
```

---

# 13. Compilation du serveur et du client

Dans un terminal :


Puis :


Le dossier contient alors :

```text
server.c
client.c
server
client
```

---

# 14. Exécution sur la même machine

## Premier terminal : serveur

Lance d'abord le serveur :


Résultat :

```text
Server listening on 0.0.0.0:2000...
```

Le serveur est maintenant bloqué dans :


Il attend la connexion d'un client.

---

## Deuxième terminal : client

Lance ensuite le client :


Résultat possible :

```text
Connected to server 127.0.0.1:2000
Client:
```

Le serveur affiche :

```text
Client connected from 127.0.0.1:49372
```

Le numéro `49372` est un exemple de port éphémère attribué au client.

---

# 15. Exemple de communication

Dans le terminal client :

```text
Client: Bonjour serveur
```

Le serveur reçoit :

```text
Client: Bonjour serveur
Server:
```

Dans le terminal serveur, l'utilisateur écrit :

```text
Bonjour client
```

Le terminal client affiche :

```text
Server: Bonjour client
Client:
```

La communication peut continuer.

---

# 16. Quitter le client

Dans cette version, l'utilisateur peut écrire :

```text
quit
```

Le client affiche :

```text
Closing client connection
```

Puis il ferme sa socket.

Le serveur reçoit alors un retour égal à zéro depuis `recv()` et affiche :

```text
Client disconnected
```

---

# 17. Exécution sur deux machines différentes

Supposons :

```text
Machine serveur :
192.168.1.50

Machine cliente :
192.168.1.60
```

Dans le code client, remplace :


par :


Le serveur doit être lancé sur la machine `192.168.1.50`.

Le client doit être lancé sur la machine `192.168.1.60`.

Le serveur utilise déjà :


Il peut donc accepter les connexions reçues sur son interface réseau.

Il faut aussi vérifier :

* que les deux machines communiquent sur le réseau ;
* que le port `2000` n'est pas bloqué par un pare-feu ;
* que l'adresse IP est correcte.

Test possible depuis la machine cliente :


---

# 18. Erreurs présentes dans le code simplifié du cours

Le cours montre approximativement :


Mais le buffer déclaré est souvent :


Il faut utiliser le même nom :


Le cours montre aussi :


Cela peut fonctionner dans un petit exemple, mais le type de retour officiel de `recv()` est :


Il vaut donc mieux écrire :


Même remarque pour `send()`.

---

# 19. Limite importante de `send()`

Dans un programme simple, on écrit souvent :


Cependant, `send()` peut envoyer moins d'octets que demandé.

Par exemple :

```text
100 octets demandés
80 octets envoyés
```

La valeur de retour serait alors :

```text
80
```

Pour un programme robuste, il faut créer une boucle qui continue jusqu'à ce que toutes les données aient été envoyées.

Une fonction possible serait :


Pour ce premier exercice pédagogique, un seul appel à `send()` suffit généralement pour comprendre le mécanisme.

---

# 20. Limite importante de TCP

TCP transmet un flux d'octets.

Il ne conserve pas automatiquement les limites entre les messages.

Supposons que le client appelle :


Le serveur peut recevoir :

```text
BonjourLinux
```

dans un seul appel à `recv()`.

Il peut aussi recevoir :

```text
Bon
```

puis :

```text
jourLinux
```

TCP garantit :

* l'ordre des octets ;
* la fiabilité ;
* la retransmission des données perdues ;
* la détection de la fermeture de la connexion.

TCP ne garantit pas :

```text
un send() = un recv()
```

Pour construire un véritable protocole, il faut définir comment reconnaître la fin d'un message.

Par exemple :

* terminer chaque message par `\n` ;
* envoyer d'abord la taille du message ;
* utiliser une structure avec un en-tête ;
* utiliser un format de protocole précis.

---

# 21. Différence entre le serveur et le client

## Serveur

```text
socket()
bind()
listen()
accept()
recv()
send()
close(client_fd)
close(server_fd)
```

## Client

```text
socket()
inet_pton()
connect()
send()
recv()
close(socket_fd)
```

Le serveur possède deux sockets après `accept()` :

```text
server_fd
    socket d'écoute

client_fd
    socket connectée au client
```

Le client n'en possède qu'une :

```text
socket_fd
    socket connectée au serveur
```

---

# 22. Schéma final

```text
CLIENT                                         SERVEUR

socket()                                       socket()
                                                  |
                                              bind()
                                                  |
                                              listen()
                                                  |
prépare server_addr                               |
                                                  |
inet_pton()                                       |
                                                  |
connect() ------------------------------------> accept()
   |                                              |
   |          connexion TCP établie               |
   |                                              |
fgets(stdin)                                      |
   |                                              |
send(socket_fd) ------------------------------> recv(client_fd)
                                                  |
                                             affiche message
                                                  |
                                             fgets(stdin)
                                                  |
recv(socket_fd) <----------------------------- send(client_fd)
   |
affiche réponse
   |
boucle
   |
close(socket_fd)                              recv() retourne 0
                                                  |
                                             close(client_fd)
                                                  |
                                             close(server_fd)
```

---

# 23. Résumé

Le client TCP réalise cinq opérations principales.

## Création


Il crée une socket TCP IPv4.

## Préparation de la destination


Il prépare l'adresse IP et le port du serveur.

## Connexion


Il demande au noyau d'établir une connexion TCP avec le serveur.

## Communication


Il envoie les messages et reçoit les réponses.

## Fermeture


Il ferme la socket et termine la connexion.

Le cycle complet est donc :

```text
créer
   ↓
indiquer la destination
   ↓
se connecter
   ↓
envoyer et recevoir
   ↓
fermer
```

