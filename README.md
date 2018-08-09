# `tone`

Synthétiseur de notes pures avec sortie WAV, développé dans le cadre de l'exposé **Texte, images et sons : au cœur du numérique** du Palais de la Découverte.

## Description

Le programme fourni génère une série de [notes pures(en)](https://en.wikipedia.org/wiki/Pure_tone) sous la forme d'une série d'échantillons à une fréquence fixe (par défaut, 48KHz). Les données résultantes sont écrites dans un fichier au format [Waveform Audio(en)](https://en.wikipedia.org/wiki/WAV), en incluant les informations nécessaires dans l'en-tête du fichier.

# Déploiement

Le programme fourni est conçu pour un système GNU/Linux ([Debian(fr)](https://www.debian.org/index.fr.html)), mais peut être aisément porté sur d'autres systèmes.

Le fichier `Makefile` fourni permet de compiler le programme C. Celui-ci génère un fichier WAV standard.

## Paquetages nécessaires (Debian Stretch)
- `gcc`
- `make`
- `alsa-utils` (installé par défaut, sinon tout autre lecteur WAV)

## Usage
```
git clone https://github.com/universcience/tone
cd tone
make
./tone
aplay out.wav
```

# Licence

Ceci est un [logiciel libre(fr)](https://www.gnu.org/philosophy/free-sw.fr.html), distribué sous les termes de la licence GNU GPL Affero. Les termes exacts de cette licence sont disponibles dans le fichier `LICENSE`.

En résumé, vous êtes libres de télécharger ce logiciel, de l'utiliser et de le modifier sans restrictions, ainsi que de le redistribuer sous les mêmes termes, avec ou sans modifications. 
