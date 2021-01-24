# OBJ2
Cette mini-lib que j'ai codée en C++ permet de charger des fichiers .obj et de faire le rendu en OpenGL 3.x

La fonction OBJ2_LoadOBJ() prend en paramètres :

- Le chemin vers le fichier .obj
- Un pointeur sur une instance de OBJ2_VBO représentant le VBO
- Trois flottants représentant les coordonnées où positionner l'objet

La fonction OBJ2_DrawVBO() prend en paramètres :

- Un pointeur sur l'instance de OBJ2_VBO
- Votre shader
