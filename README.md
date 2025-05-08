# Code du Petit Actionneur Mobile Indépendant d'EsialRobotik 
Destination du code : ESP32 C3 Zero selon l'implantation électronique suivante : https://github.com/EsialRobotik/Electronique/blob/master/AsservPami/asserv-pami-board.pdf

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
