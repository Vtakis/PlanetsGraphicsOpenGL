Εργασία - Γραφικά Ι
2019-2020: Πλανήτες

ΒΛΑΣΣΗΣ ΠΑΝΑΓΙΩΤΗΣ
ΑΜ: 1115201400022


Πλατφόρμα υλοποίησης: Microsoft Visual Studio 2019
Περιβάλλον υλοποίησης: Windows 10 Education
Γλώσσες υλοποίησης: OpenGL 3.3/C++

Αρχεία: 
	planets.cpp , glad.c
(Για την δημιουργία των Shaders: lamps.fs, lamps.vs, lighting_maps.fs, lighting_maps.vs)
(Για την δημιουργία των Models: Globe.obj, planet.obj Τα αρχεία βρίσκονται μέσα στον φάκελο resources)

/*******************************************************************************/

Ρυθμίσεις για Configuration properties του project:

•	Εισαγωγή path για Include Directories και Library Directories
•	Στο C/C++/Preprocessor εισάγουμε επιπλέον στα Preprocessor Definitions το ακόλουθο:
◦	_CRT_SECURE_NO_WARNINGS
•	Στο Linker/Input εισάγουμε στα Additional Dependencies τα ακόλουθα:
◦	glfw3.lib
◦	opengl32.lib
◦	assimp.lib

Επιπλέον:
	Πάμε στον φάκελο των Library Directories και στον υποφάκελο leanopengl, τροποποιούμε το αρχείο κεφαλίδας root_directory.h ως εξής:
	Στη μεταβλητή logl_root αναθέτουμε το απόλυτο μονοπάτι που βρίσκονται τα περιεχόμενα του φακέλου του project.

/*******************************************************************************/

	Η εργασία υλοποιήθηκε με βάση όσα διδάχθηκαν στο εργαστήριο του μαθήματος καθώς και βάση της ιστοσελίδας learnopengl.com.
	Για την μοντελοποίηση των πλανητών χρησιμοποιήθηκαν τα βοηθητικά αρχεία που δόθηκαν, για τον Ήλιο/Σελήνη το μοντέλο planet (planet.obj) και για τη Γη το μοντέλο Earth (Globe.obj).



