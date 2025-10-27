# Euh y'a rien ici

Ah si, pour compiler fait `gcc -Wall main.c board.c game.c computer.c digraph.c ../utils.c rss.o -o othello.exe` dans un terminal compatible :)

Le `rss.o` applique l'icône au fichier lors de la compilation. Si tu veux changer d'icône, télécharge une image, convertis la en icône (.ico) et ensuite change le chemin d'accès vers l'icône dans le fichier `rss.rc`, puis entrer dans le terminal `windres rss.rc -o rss.o`.

Version : `gcc.exe (Rev2, Built by MSYS2 project) 14.2.0`.


# Installation pour windows
- Terminal compatible : [MSYS2 UCRT64](https://www.msys2.org/)
- Dans le terminal : `pacman -S mingw-w64-x86_64-gcc` pour installer gcc (`-S` et `-Q` sont "identiques", perso j'ai mis les deux, ça fonctionne dans tous les cas je crois)
- Ensuite pour mettre `gcc` au PATH, entrer les commandes sucessives :
	- `nano ~/.bashrc` et rajoute à la fin du fichier la ligne `export PATH=/mingw64/bin:$PATH`
	- `source ~/.bashrc`
	- Tu peux vérifier l'installation avec `gcc --version` n'importe où pour voir s'il est bien au PATH. Si ça ne marche pas bah tant pis :)
- Amuse toi bien
