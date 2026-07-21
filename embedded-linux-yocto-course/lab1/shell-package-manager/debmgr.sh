#!/bin/bash

set -o nounset

usage()
{
    printf '%s\n' \
        "Usage: $0 {info|install|remove|create|help} ARGUMENT" \
        "" \
        "  info PACKAGE       Afficher les informations d'un paquet" \
        "  install PACKAGE    Installer un paquet ou un fichier .deb" \
        "  remove PACKAGE     Supprimer un paquet installé" \
        "  create DIRECTORY   Créer un paquet .deb depuis un dossier" \
        "  help               Afficher cette aide"
}

require_command()
{
    if ! command -v "$1" >/dev/null 2>&1; then
        printf '[ERROR] Commande requise introuvable : %s\n' "$1" >&2
        exit 1
    fi
}

require_argument()
{
    if [[ -z "${2:-}" ]]; then
        printf '[ERROR] Argument manquant pour la commande %s.\n' "$1" >&2
        usage >&2
        exit 2
    fi
}

action="${1:-help}"
argument="${2:-}"

case "$action" in
    info)
        require_argument "$action" "$argument"
        require_command dpkg-query
        dpkg-query --show --showformat='Paquet: ${binary:Package}\nVersion: ${Version}\nArchitecture: ${Architecture}\nStatut: ${db:Status-Status}\n' "$argument"
        ;;
    install)
        require_argument "$action" "$argument"
        if [[ "$argument" == *.deb ]]; then
            require_command dpkg
            sudo dpkg --install "$argument"
        else
            require_command apt-get
            sudo apt-get install "$argument"
        fi
        ;;
    remove)
        require_argument "$action" "$argument"
        require_command apt-get
        sudo apt-get remove "$argument"
        ;;
    create)
        require_argument "$action" "$argument"
        require_command dpkg-deb
        if [[ ! -d "$argument" ]]; then
            printf '[ERROR] Dossier introuvable : %s\n' "$argument" >&2
            exit 2
        fi
        dpkg-deb --build "$argument"
        ;;
    help|-h|--help)
        usage
        ;;
    *)
        printf '[ERROR] Action inconnue : %s\n' "$action" >&2
        usage >&2
        exit 2
        ;;
esac
