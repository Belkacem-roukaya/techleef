# Compilation et déploiement du client et du serveur TCP

Le client TCP est compilé et exécuté sur le PC. Le serveur TCP est transféré, compilé et exécuté sur le Raspberry Pi.

## 1. Trouver l'adresse IP du Raspberry Pi

Depuis la console UART du Raspberry Pi, afficher ses adresses IP :

```bash
hostname -I
```

Ou afficher les interfaces réseau :

```bash
ip -br address
```

Exemple :

```text
wlan0    UP    192.168.1.51/24
```

Dans cet exemple, l'adresse IPv4 du Raspberry Pi est `192.168.1.51`.

Depuis le PC, vérifier que le Raspberry Pi est accessible :

```bash
ping -c 4 192.168.1.51
```

> L'adresse IP peut changer après un redémarrage ou une reconnexion au réseau. Il faut la vérifier avant chaque test.

## 2. Activer la communication SSH

Sur le Raspberry Pi, installer et activer le serveur SSH si nécessaire :

```bash
sudo apt install openssh-server
sudo systemctl enable --now ssh
sudo systemctl status ssh
```

Depuis le PC, tester la connexion SSH :

```bash
ssh roukaya@192.168.1.51
```

Dans cette commande :

- `roukaya` est le nom de l'utilisateur du Raspberry Pi ;
- `192.168.1.51` est l'adresse IP du Raspberry Pi.

Lors de la première connexion, SSH demande de confirmer l'identité de la machine distante. Répondre :

```text
yes
```

Puis saisir le mot de passe de l'utilisateur `roukaya`.

## 3. Configurer l'adresse du serveur dans le client

Le client doit utiliser l'adresse IP du Raspberry Pi :

```c
#define SERVER_ADDRESS "192.168.1.51"
#define SERVER_PORT 2000
```

L'adresse `127.0.0.1` représente la machine locale. Lorsque le client est exécuté sur le PC, cette adresse désigne le PC et non le Raspberry Pi.

## 4. Compiler le client sur le PC

Depuis le dossier du client :

```bash
cd ~/Documents/techleef/embedded-linux-yocto-course/lab1/ipc-tcp/client
gcc -std=c11 -Wall -Wextra -Wpedantic client.c -o client
```

Le fichier exécutable `client` est créé sur le PC.

## 5. Copier le serveur vers le Raspberry Pi

Depuis le dossier du serveur sur le PC :

```bash
cd ~/Documents/techleef/embedded-linux-yocto-course/lab1/ipc-tcp/server
scp server.c roukaya@192.168.1.51:/home/roukaya/
```

La commande `scp` utilise SSH pour copier `server.c` dans le dossier personnel de l'utilisateur `roukaya` sur le Raspberry Pi.

Vérifier le transfert :

```bash
ssh roukaya@192.168.1.51
ls -l /home/roukaya/server.c
```

## 6. Compiler le serveur sur le Raspberry Pi

Après la connexion SSH :

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic server.c -o server
```

Le fichier exécutable `server` est créé sur le Raspberry Pi.

## 7. Lancer le serveur

Sur le Raspberry Pi :

```bash
./server
```

Résultat attendu :

```text
Server listening on 0.0.0.0:2000...
```

Le serveur attend maintenant une connexion sur le port TCP `2000`.

## 8. Lancer le client

Dans un autre terminal, sur le PC :

```bash
cd ~/Documents/techleef/embedded-linux-yocto-course/lab1/ipc-tcp/client
./client
```

Résultat attendu :

```text
Connected to server 192.168.1.51:2000
Client:
```

## 9. Communication TCP

Le fonctionnement est le suivant :

```text
PC                                           RASPBERRY PI

client.c                                     server.c
   │                                             │
   ├── connect(192.168.1.51:2000) ─────────────> │
   │                                             │
   ├── send(message) ──────────────────────────> recv()
   │                                             │
   │ <────────────────────────────────────────── send(réponse)
   │                                             │
   └── recv(réponse)                             │
```

Le serveur doit être lancé avant le client. Sinon, le client affiche :

```text
Cannot connect to the server: Connection refused
```

Cette erreur signifie généralement qu'aucun serveur n'écoute encore sur l'adresse et le port indiqués.

## 10. Résumé des commandes

Sur le PC :

```bash
gcc client.c -o client
scp server.c roukaya@192.168.1.51:/home/roukaya/
ssh roukaya@192.168.1.51
```

Sur le Raspberry Pi :

```bash
gcc server.c -o server
./server
```

Dans un autre terminal sur le PC :

```bash
./client
```
