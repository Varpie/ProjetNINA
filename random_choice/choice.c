#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

int main() {
  FILE* file = fopen("adress_list.txt", "r+");

  char *line = NULL;
  char *tab[1023];
  int tailleTab = 0;

  line = malloc(1023);

  if(line == NULL)
    return(EXIT_FAILURE);

  while((fgets(line, 1023, file)) != NULL ) {
    if(line[0] != '#' && line[0] != '\n') {
      tab[tailleTab] = line;
      line = malloc(1023);
      tailleTab++;
    }
  }

  FILE* file2 = fopen("hist.txt", "w+");

  int i;
  srand(time(NULL));
  for(i=0;i<4;i++) {
    int n = rand()%tailleTab;
    char commande2[255] = "firefox ";
    strcat(commande2, tab[n]);
    system(commande2);
    fputs(tab[n], file2);
  }
  fclose(file2);
  fclose(file);
  free(line);
}
