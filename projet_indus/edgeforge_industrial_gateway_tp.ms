.TL
EdgeForge Industrial Gateway
.AU
TP complet Yocto pour BeagleBone
.AB
Ce document propose un TP complet pour transformer une formation Yocto en vrai projet industriel sur BeagleBone. Il conserve la demarche du cahier des charges Yocto : structure projet, build, machine, distribution, image, users/groups, SDK, OTA et securite. Il ajoute une application industrielle, un service systemd, du monitoring, une communication MQTT/REST, une partition data et un workflow dev/prod.
.AE

.NH 1
Vision du projet
.LP
Le projet s'appelle
.B "EdgeForge Industrial Gateway" .
Il consiste a construire une distribution Linux embarquee industrielle avec Yocto pour BeagleBone.
.LP
L'application principale s'appelle
.B edgeforge-agent .
Elle lit des donnees machine, les traite localement, les journalise, puis les publie vers un serveur via MQTT ou REST.
.LP
Objectifs principaux :
.IP \(bu 2
demarrer sur BeagleBone avec une image Yocto propre ;
.IP \(bu 2
separer BSP, distribution et application ;
.IP \(bu 2
creer une image de developpement et une image production ;
.IP \(bu 2
integrer une application industrielle lancee par systemd ;
.IP \(bu 2
gerer users, groups et permissions ;
.IP \(bu 2
ajouter monitoring, logs et watchdog ;
.IP \(bu 2
preparer les mises a jour OTA avec RAUC ;
.IP \(bu 2
durcir progressivement la securite.

.NH 1
Architecture fonctionnelle
.LP
.DS
Capteurs / bus industriel
    |
    | GPIO / I2C / SPI / UART / CAN
    v
edgeforge-agent (service systemd)
    |
    +-- traitement local
    +-- logs journald
    +-- stockage data
    +-- publication MQTT ou REST
    +-- health check
    v
Serveur industriel / dashboard / cloud
.DE

.NH 1
Structure du projet
.LP
.DS
edgeforge-gateway/
|-- docker/
|-- kas/
|   `-- kas-beaglebone.yml
|-- meta-edgeforge-bsp/
|-- meta-edgeforge-distro/
|-- meta-edgeforge-prj/
`-- README.md
.DE
.LP
Role des layers :
.IP "meta-edgeforge-bsp" 4
Machine BeagleBone, kernel, device tree, WIC, boot et hardware.
.IP "meta-edgeforge-distro" 4
Politique de distribution : systemd, CVE, licences, features et build info.
.IP "meta-edgeforge-prj" 4
Images, application industrielle, users, packagegroups et services.

.NH 1
Workflow global conserve depuis le PDF Yocto
.LP
La demarche du PDF est conservee :
.IP 1. 4
Project structure
.IP 2. 4
Build avec KAS et Docker
.IP 3. 4
Machine BeagleBone
.IP 4. 4
Distribution Yocto separee
.IP 5. 4
Images dev et production
.IP 6. 4
Users and Groups
.IP 7. 4
SDK
.IP 8. 4
OTA avec RAUC
.IP 9. 4
Securite
.LP
Extension industrielle ajoutee :
.IP \(bu 2
application edgeforge-agent ;
.IP \(bu 2
communication MQTT ou REST ;
.IP \(bu 2
service systemd robuste ;
.IP \(bu 2
watchdog et monitoring ;
.IP \(bu 2
partition data ;
.IP \(bu 2
diagnostic CLI avec edgeforgectl ;
.IP \(bu 2
mode dev/prod.

.bp
.NH 1
TP 0 - Preparation
.NH 2
Objectif
.LP
Creer le dossier du projet et comprendre la separation entre BSP, distro et projet.
.NH 2
Commandes
.DS
mkdir edgeforge-gateway
cd edgeforge-gateway
mkdir docker kas meta-edgeforge-bsp meta-edgeforge-distro meta-edgeforge-prj
touch README.md
.DE
.NH 2
Resultat attendu
.LP
Un projet propre, lisible et pret a recevoir les layers Yocto.

.NH 1
TP 1 - Build avec KAS
.NH 2
Objectif
.LP
Creer un fichier KAS pour recuperer les layers et lancer un build reproductible.
.NH 2
Fichier
.DS
kas/kas-beaglebone.yml
.DE
.NH 2
Layers a integrer
.IP \(bu 2
openembedded-core
.IP \(bu 2
bitbake
.IP \(bu 2
meta-openembedded
.IP \(bu 2
meta-ti ou layer compatible BeagleBone
.IP \(bu 2
meta-edgeforge-bsp
.IP \(bu 2
meta-edgeforge-distro
.IP \(bu 2
meta-edgeforge-prj
.NH 2
Options importantes
.DS
DL_DIR = "/work/downloads"
SSTATE_DIR = "/work/sstate-cache"
INHERIT += "buildhistory"
INHERIT += "create-spdx"
SPDX_PRETTY = "1"
.DE
.NH 2
Commande
.DS
kas build kas/kas-beaglebone.yml
.DE

.NH 1
TP 2 - Docker de build
.NH 2
Objectif
.LP
Construire dans un environnement stable et partageable.
.NH 2
Fichiers
.DS
docker/Dockerfile
docker/docker-run.sh
.DE
.NH 2
A integrer
.IP \(bu 2
dependances Yocto ;
.IP \(bu 2
installation de kas ;
.IP \(bu 2
montage du projet en volume ;
.IP \(bu 2
montage de downloads et sstate-cache ;
.IP \(bu 2
script HTTP local pour exposer le dossier deploy.

.bp
.NH 1
TP 3 - Layer BSP BeagleBone
.NH 2
Objectif
.LP
Creer la couche qui gere la carte cible et le hardware.
.NH 2
Structure
.DS
meta-edgeforge-bsp/
|-- conf/
|   |-- layer.conf
|   `-- machine/beaglebone-edgeforge.conf
|-- recipes-kernel/
`-- wic/
.DE
.NH 2
A faire
.IP \(bu 2
selectionner la machine BeagleBone ;
.IP \(bu 2
controler les MACHINE_FEATURES ;
.IP \(bu 2
preparer le boot ;
.IP \(bu 2
preparer les partitions WIC ;
.IP \(bu 2
preparer la configuration kernel.

.NH 1
TP 4 - Configuration kernel
.NH 2
Objectif
.LP
Activer uniquement les fonctionnalites necessaires a la gateway industrielle.
.NH 2
Fichiers
.DS
meta-edgeforge-bsp/recipes-kernel/linux/linux-%.bbappend
meta-edgeforge-bsp/recipes-kernel/linux/files/edgeforge.cfg
meta-edgeforge-bsp/recipes-kernel/linux/files/industrial-io.cfg
.DE
.NH 2
Fonctionnalites a activer
.IP \(bu 2
GPIO ;
.IP \(bu 2
I2C ;
.IP \(bu 2
SPI si necessaire ;
.IP \(bu 2
UART ;
.IP \(bu 2
CAN si disponible ;
.IP \(bu 2
watchdog ;
.IP \(bu 2
options reseau utiles.

.NH 1
TP 5 - Device Tree et overlays
.NH 2
Objectif
.LP
Activer le materiel necessaire depuis le device tree.
.NH 2
Fichier
.DS
meta-edgeforge-bsp/recipes-kernel/linux/files/edgeforge-industrial-overlay.dts
.DE
.NH 2
Tests attendus
.DS
gpioinfo
i2cdetect -y 1
dmesg | grep -i i2c
.DE

.bp
.NH 1
TP 6 - Distribution industrielle
.NH 2
Objectif
.LP
Creer une distribution Yocto separee, controlee et orientee produit.
.NH 2
Fichier
.DS
meta-edgeforge-distro/conf/distro/edgeforge-distro.conf
.DE
.NH 2
Configuration de base
.DS
DISTRO = "edgeforge-distro"
DISTRO_NAME = "EdgeForge Industrial Linux"
DISTRO_VERSION = "1.0"
INIT_MANAGER = "systemd"
NO_RECOMMENDATIONS = "1"
PACKAGE_CLASSES ?= "package_ipk"
INHERIT += "cve-check"
INHERIT += "image-buildinfo"
.DE
.NH 2
Options industrielles
.IP \(bu 2
controler les licences ;
.IP \(bu 2
activer le suivi CVE ;
.IP \(bu 2
ajouter build information ;
.IP \(bu 2
preparer les variantes dev et prod.

.NH 1
TP 7 - Images dev et production
.NH 2
Objectif
.LP
Separer une image de developpement confortable et une image production minimale.
.NH 2
Fichiers
.DS
meta-edgeforge-prj/recipes-image/images/edgeforge-image-dev.bb
meta-edgeforge-prj/recipes-image/images/edgeforge-image-prod.bb
.DE
.NH 2
Image dev
.IP \(bu 2
SSH actif ;
.IP \(bu 2
debug tools ;
.IP \(bu 2
gdb, strace, i2c-tools, can-utils ;
.IP \(bu 2
dev-pkgs et dbg-pkgs.
.NH 2
Image prod
.IP \(bu 2
paquets minimum ;
.IP \(bu 2
pas de debug inutile ;
.IP \(bu 2
utilisateur non-root ;
.IP \(bu 2
services essentiels uniquement.

.NH 1
TP 8 - Application industrielle edgeforge-agent
.NH 2
Objectif
.LP
Developper l'application metier qui transforme l'image Yocto en vrai produit industriel.
.NH 2
Structure
.DS
meta-edgeforge-prj/recipes-apps/edgeforge-agent/
|-- edgeforge-agent.bb
`-- files/
    |-- edgeforge-agent.py
    |-- edgeforge-agent.service
    `-- edgeforge-agent.conf
.DE
.NH 2
Version 1
.IP \(bu 2
generer des donnees simulees ;
.IP \(bu 2
ecrire les logs dans journald ;
.IP \(bu 2
lancer l'application via systemd ;
.IP \(bu 2
redemarrer automatiquement en cas de crash.
.NH 2
Version 2
.IP \(bu 2
lire une entree GPIO ou I2C ;
.IP \(bu 2
publier via MQTT ;
.IP \(bu 2
charger un fichier de configuration.

.bp
.NH 1
TP 9 - Service systemd industriel
.NH 2
Objectif
.LP
Rendre l'application robuste et demarree automatiquement.
.NH 2
Exemple de service
.DS
[Unit]
Description=EdgeForge Industrial Agent
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=edgeuser
ExecStart=/usr/bin/edgeforge-agent --config /etc/edgeforge/agent.conf
Restart=always
RestartSec=5
WatchdogSec=30

[Install]
WantedBy=multi-user.target
.DE
.NH 2
Tests
.DS
systemctl status edgeforge-agent
journalctl -u edgeforge-agent -f
.DE

.NH 1
TP 10 - Users, groups et permissions
.NH 2
Objectif
.LP
Eviter de lancer les applications industrielles en root.
.NH 2
Utilisateur
.LP
edgeuser
.NH 2
Groupes
.DS
gpio
i2c
spi
dialout
systemd-journal
.DE
.NH 2
Fichier
.DS
meta-edgeforge-prj/recipes-core/users/edgeforge-users.bb
.DE

.NH 1
TP 11 - Packagegroup industriel
.NH 2
Objectif
.LP
Organiser les paquets du projet dans un packagegroup propre.
.NH 2
Fichier
.DS
meta-edgeforge-prj/recipes-core/packagegroups/packagegroup-edgeforge-industrial.bb
.DE
.NH 2
Contenu
.IP \(bu 2
edgeforge-agent ;
.IP \(bu 2
outils hardware ;
.IP \(bu 2
outils reseau ;
.IP \(bu 2
outils monitoring ;
.IP \(bu 2
dependances MQTT ou REST.

.NH 1
TP 12 - Partition data
.NH 2
Objectif
.LP
Separer les donnees industrielles du systeme rootfs.
.NH 2
Fichier
.DS
meta-edgeforge-bsp/wic/edgeforge.wks
.DE
.NH 2
Partitions
.DS
boot
rootfs
data
.DE
.NH 2
Contenu de data
.IP \(bu 2
logs applicatifs ;
.IP \(bu 2
base SQLite ;
.IP \(bu 2
configuration locale ;
.IP \(bu 2
donnees temporaires.

.bp
.NH 1
TP 13 - Communication industrielle
.NH 2
Objectif
.LP
Connecter la gateway au systeme de supervision.
.NH 2
Option recommandee
.LP
MQTT, car il est courant en IoT industriel et simple a tester.
.NH 2
Flux
.DS
edgeforge-agent -> MQTT topic -> broker -> dashboard / serveur
.DE
.NH 2
Topics exemples
.DS
factory/edgeforge/status
factory/edgeforge/sensors
factory/edgeforge/alerts
.DE

.NH 1
TP 14 - Monitoring et diagnostic
.NH 2
Objectif
.LP
Faciliter le diagnostic terrain.
.NH 2
Commande cible
.DS
edgeforgectl status
edgeforgectl sensors
edgeforgectl version
edgeforgectl logs
.DE
.NH 2
Informations a afficher
.IP \(bu 2
etat du service ;
.IP \(bu 2
uptime ;
.IP \(bu 2
temperature CPU ;
.IP \(bu 2
espace disque ;
.IP \(bu 2
version de l'image ;
.IP \(bu 2
derniers logs importants.

.NH 1
TP 15 - SDK
.NH 2
Objectif
.LP
Generer une toolchain pour developper l'application hors Yocto.
.NH 2
Commande
.DS
bitbake edgeforge-image-dev -c populate_sdk
.DE
.NH 2
Test attendu
.IP \(bu 2
installer le SDK ;
.IP \(bu 2
compiler une petite application ;
.IP \(bu 2
integrer ensuite cette application dans une recette Yocto.

.NH 1
TP 16 - OTA avec RAUC
.NH 2
Objectif
.LP
Mettre a jour le systeme comme un produit industriel.
.NH 2
A ajouter
.IP \(bu 2
meta-rauc ;
.IP \(bu 2
rauc ;
.IP \(bu 2
rauc-conf ;
.IP \(bu 2
certificats de developpement ;
.IP \(bu 2
bundle signe.
.NH 2
Fichier
.DS
meta-edgeforge-prj/recipes-image/bundles/edgeforge-bundle.bb
.DE
.NH 2
Tests
.DS
rauc status
rauc install edgeforge-update.raucb
.DE

.bp
.NH 1
TP 17 - Securite industrielle
.NH 2
Objectif
.LP
Durcir progressivement le systeme.
.NH 2
Actions
.IP \(bu 2
desactiver le root login ;
.IP \(bu 2
forcer SSH par cle ;
.IP \(bu 2
lancer les services avec utilisateur non-root ;
.IP \(bu 2
limiter les permissions ;
.IP \(bu 2
suivre les CVE ;
.IP \(bu 2
envisager rootfs read-only ;
.IP \(bu 2
ajouter dm-verity plus tard ;
.IP \(bu 2
chiffrer la partition data plus tard.

.NH 1
TP 18 - Documentation finale
.NH 2
Objectif
.LP
Rendre le projet maintenable et presentable.
.NH 2
README attendu
.DS
1. Presentation
2. Architecture
3. Hardware cible
4. Build avec Docker/KAS
5. Images disponibles
6. Application edgeforge-agent
7. Tests hardware
8. OTA RAUC
9. Securite
10. Roadmap
.DE

.NH 1
Roadmap conseillee
.IP 1. 4
Build minimal BeagleBone
.IP 2. 4
Image dev avec SSH
.IP 3. 4
Distro edgeforge propre
.IP 4. 4
Application simulee edgeforge-agent
.IP 5. 4
Service systemd et logs
.IP 6. 4
User non-root
.IP 7. 4
Lecture GPIO/I2C reelle
.IP 8. 4
Publication MQTT
.IP 9. 4
Image production
.IP 10. 4
Partition data
.IP 11. 4
SDK
.IP 12. 4
RAUC OTA
.IP 13. 4
Securite avancee
.LP
Conseil important : ne pas tout faire en meme temps. Construire une image qui boot, puis ajouter une seule capacite a la fois. Chaque TP doit se terminer par un test clair sur la BeagleBone ou dans l'environnement de build.
