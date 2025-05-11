# Code du Petit Actionneur Mobile Indépendant d'EsialRobotik 
Destination du code : ESP32 C3 Zero selon l'implantation électronique suivante : https://github.com/EsialRobotik/Electronique/blob/master/AsservPami/asserv-pami-board.pdf

# Utilisation de la PAMI
A la mise sous tension, la PAMI attend l'insertion de la tirette.
A l'insertion de la tirette, la PAMI charge la liste d'instruction qui correspond à son numéro, déterminé par le SRF08 qu'elle contient.
Au retrait de la tirette, un compte à rebours de 85 secondes est lancé et les oreilles commencent à osciller.
Au bout de compte à rebours, la PAMI exécute sa liste d'instruction.
Si la liste d'instruction n'est pas terminé au bout de 15 secondes, l'exécution de la liste s'interromp et la PAMI s'immobilise.
Les oreilles continuent à osciller jusqu'à la mise hors tension de la PAMI

# Liaison série avec un PC
Il est possible d'envoyer des commandes à la PAMI à travers la liaison série du connecteur USB-C relié à un oridnateur.
Une commande est constituée d'un caractère suivi éventuellement d'autres caractères qui constituent ses paramètres.
Concrètement, envoyer sur la liaison série en une fois "\<commande>\<argument>\n" :
| Commande | Argument | Description |
|--|--|--|
| c | *aucun* | Active ou désactive le mode test en continu du SRF. Environ 15 mesures secondes |
| e | *aucun* | Active ou désactive le mode test en continu des oreilles |
| i | *aucun* | Scanne le Bus I2C et affiche les adresse vivantes |
| s | *aucun* | Teste le SRF : affiche son adresse s'il est connecté et effectue une mesure de distance et l'affiche |
| t | *aucun* | Active ou désactive le mode test de la PAMI. Renvoie sur la liaison série "Tests ON" ou "Tests OFF" |
