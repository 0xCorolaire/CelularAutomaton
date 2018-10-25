#include <iostream>
#include "etats.h"
#include "automate.h"

/*============================================================================*/
/*                                   Etats                                    */
/*============================================================================*/

/* Etat 1D */
bool Etat1D::getCellule(unsigned int i) const {
    if (i >= nbCol) throw AutomateException("Erreur Cellule");
    return valeur[i];
}
void Etat1D::setCellule(unsigned int i, bool val) {
    if (i >= nbCol) throw AutomateException("Erreur Cellule");
    valeur[i] = val;
}
Etat1D& Etat1D::operator=(const Etat1D& e) {
    if (this != &e) {
        if (nbCol != e.nbCol) {
            bool* newvaleur = new bool[e.nbCol];
            for (unsigned int i = 0; i < e.nbCol; i++) newvaleur[i] = e.valeur[i];
            bool* old = valeur;
            valeur = newvaleur;
            nbCol = e.nbCol;
            delete[] old;
        }else for (unsigned int i = 0; i < e.nbCol; i++) valeur[i] = e.valeur[i];
    }
    return *this;
}

/* Etat 2D */
void Etat2D::setCellule(unsigned int i, unsigned int j, unsigned int val){
    if(i >= nbCol||j >= nbLignes) throw AutomateException("Erreur Cellule");
    valeur[i][j] = val;
}


/* Ostream affichage en cout */
std::ostream& operator<<(std::ostream& f, const Etat1D& e) {
    for (unsigned int i = 0; i < e.getNbCol(); i++)
        if (e.getCellule(i)) f << char(178); else f << " ";
    return f;
}

std::ostream& operator<<(std::ostream& f, const Etat2D& e) {
    for(unsigned int i=0;i<(e.getNbCol());i++)
        for (unsigned int j=0;j<(e.getNbLignes());j++)
            if (e.getCellule(i,j)) f << char(178); else f << " ";
    return f;
}
