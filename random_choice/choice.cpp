#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

int main() {

  ifstream fichier("../word_list/word_list.txt", ios::in);
  if(fichier) {
    string ligne;
    list<string> l;
    while(getline(fichier, ligne))
    {
      l.push_back(ligne);
    }
    for(vector<string>::iterator i = l.begin(); i != l.end(); ++i) {
      cout << *i << "\n";
    }
    fichier.close();
  } else
    cerr << "Impossible d'ouvrir le fichier !" << endl;
  return 0;
}
