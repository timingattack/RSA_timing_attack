# RSA_timing_attack
un projet TER « Démonstrateur d’attaque temporelle sur le RSA »

Il comporte 2 versions pour la timing attack.

La première qui est dans la racine du projet consiste à effectuer l'attaque sur tout les bits de l'exposant secret d en même temps, la seconde qui se trouve dans le dossier timing_attack_bit_a_bit effectue l'attaque sur l'exposant d bit à bit. Les deux versions marchent sauf que la seconde est beaucoup plus longue en terme d'execution puisqu'elle fait l'attaque sur chaque bit.

# Aides
pour compiler le RSA :
```shell
$make
```
- Lors de l'execution du programme vous pouvez choisir :

# Le mode RSA
	* Tapez 1 pour RSA avec square and multiply 
	* Tapez 2 pour RSA avec Montgomery 

# La taille du module RSA :
	* Tapez 1 pour 1024 bits  
	* Tapez 2 pour 2048 bits 
	* Tapez 3 pour 3072 bits 
	* Tapez 4 pour 4096 bits 

# Le nombre maximum de messages aléatoitres que vous voulez génerer 
	* Tapez le nombre maximum de messages souhaités

# Avec padding PKCS#1 v1.5
	* Tapez 1 pour oui (avec padding)
	* Tapez 2 pour non (sans padding)

Vous aurez par la suite les details de vos choix et les messages chiffrés et déchiffrés avec le mode RSA que vous avez choisi.

si vous choisissez le mode 2 RSA avec montgomery vous aurez en plus la Timing Attack avec le d secret reconstituer.

