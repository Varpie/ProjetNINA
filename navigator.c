#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Python.h>

//Run browser as a child process and store it's pid in var child_pid
int main()
{
	pid_t pid;
	//parameters for execvp
	char *parmList[] = {"firefox", "google.com", NULL};

	int a,child_pid;



	if ((pid = fork()) == -1)
		perror("fork failed");
	if (pid == 0) {
		a = execvp("/usr/bin/firefox", parmList);
	} else {
		child_pid = pid;
		//Create command to kill process and execute it with bash
		char command[20] = {0};
		sprintf(command,"kill -9 %d",child_pid);

		sleep(10);
		system(command);


		/*// PyObject est un wrapper Python autour des objets qu'on
		// va échanger enter le C et Python.
		PyObject *retour, *module, *fonction, *arguments;
		char *resultat;

		// Initialisation de l'interpréteur(1 à la fois)
		Py_Initialize();   

		// Import du script. 
		PySys_SetPath("."); // Le dossier en cours n'est pas dans le PYTHON PATH
		module = PyImport_ImportModule("get_html");
		// Récupération de la fonction
		fonction = PyObject_GetAttrString(module, "parse_page");

		// Création d'un PyObject de type string.
		// https://docs.python.org/2/c-api/arg.html#c.Py_BuildValue
		arguments = Py_BuildValue("(s)", "www.google.com"); 

		// Appel de la fonction.
		retour = PyEval_CallObject(fonction, arguments);


		// Conversion du PyObject obtenu en string C
		PyArg_Parse(retour, "s", &resultat);
		printf("Resultat: %s\n", resultat);

		//Important : on ferme l'interpréteur.
		Py_Finalize();*/
	}
return 0;
}
