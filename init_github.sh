#!/bin/bash

if [ ${#} = 0 ]; then
    log=$(whoami)
else
    log="${1}"
fi

case "$log" in
    pierre)
        name="Pierre V"
        email="pierre.varlez@gmail.com"
    louis)
        name="Louis Klein"
        email="louis.klein7@gmail.com"
    alix)
        name="Alix Vuagnoux"
        email="alix.vuagnoux@gmail.com"
    etienne)
        name="Etienne Jodry"
        email="Etienne.JODRY@hotmail.fr"
    *)
        echo "Erreur de login"
        exit 0
esac

git config --global user.name $name
git config --global user.email $email

# Caching password for 1 hour
git config --global credential.helper 'cache --timeout=3600'
