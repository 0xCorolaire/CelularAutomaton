#ifndef ETATS_H_INCLUDED
#define ETATS_H_INCLUDED

/**
* \file etats.h
* \brief Déclaration des classes Etats (qui représentent l'état d'un automate a un moment donné)
* \author Hugo L. Hugo P. Thomas L.
* \date 16 Juin 2018
*/
#include <string>
#include <iostream>

/*============================================================================*/
/*                                   Etat                                     */
/*============================================================================*/
/* Classe abstraite */

/**
 * \class EtatMere
 * \brief Classe abstraite qui définit le comportement d'un état
 */
class EtatMere {
protected:
    /**
     * \brief nbCol Largeur de la matrice
     */
    unsigned int nbCol;
public:
    /**
     * \brief EtatMere constructeur
     * \param nbCol Nombre de colonnes de l'état, 25 de base
     */
    EtatMere(unsigned int nbCol=25): nbCol(nbCol){}
    virtual ~EtatMere(){nbCol=0;}
    /**
     * \brief getNbCol
     * \return Nombre de colonnes de l'état
     */
    unsigned int getNbCol() const {return nbCol;}
    /**
     * \brief getNbLignes méthode virtuelle pure
     * \return Nombre de lignes de l'état
     */
    virtual unsigned int getNbLignes() const = 0;
    /**
     * \brief setNbCol set le nombre de colonnes
     * \param nbC Nombre de colonnes
     */
    void setNbCol(unsigned int nbC){nbCol = nbC;}
};

/* Etat 1D*/
/**
 * \class Etat1D
 * \brief La classe Etat1D représente un état à un instant donné d'un automate cellulaire à une dimension
 */
class Etat1D : public EtatMere {
    /**
     * \brief valeur Tableau de booleens qui représente un état sur une ligne
     */
    bool* valeur;
public:
    /**
     * \brief Constructeur vide
     */
    Etat1D():EtatMere(nbCol),valeur(nullptr){}
    /**
     * \brief Constructeur prenant en paramètre le nombre de colonnes de l'état et initialisant le tableau de valeur
     * \param nbCol Nombre de colonnes de l'état
     */
    Etat1D(unsigned int nbCol):EtatMere(nbCol),valeur(new bool [nbCol]) {
        for (unsigned int i = 0; i < nbCol; i++) valeur[i] = false;
    }
    /**
     * \brief Constructeur par recopie d'un Etat1D
     * \param e Etat à recopier
     */
    Etat1D(const Etat1D& e):EtatMere(e.nbCol),valeur(new bool[e.nbCol]) {
        for (unsigned int i = 0; i < nbCol; i++) valeur[i] = e.valeur[i];
    }

    ~Etat1D(){delete[] valeur;}
    unsigned int getNbLignes() const {return 1;}
    unsigned int getNbCol() const{return nbCol;}
    bool getCellule(unsigned int i) const;
    void setCellule(unsigned int i, bool val);
    Etat1D& operator=(const Etat1D& etat);
};

/* Etat2D */
/**
 * \class Etat2D
 * \brief La classe Etat2D représente un état à un instant donné d'un automate cellulaire en deux dimensions
 */
class Etat2D : public EtatMere {
    /**
     * \brief nbLignes Hauteur de la matrice
     */
    unsigned int nbLignes;
    /**
     * \brief valeur Tableau de tableaux d'entiers de dimensions nbCol et nbLignes. La valeur de l'entier représente l'état de la cellule.
     */
    unsigned int** valeur;
public:
    /**
     * \brief Constructeur vide d'un état 2D, avec nbCol=25 et nbLignes=25
     */
    Etat2D():EtatMere(nbCol),nbLignes(25), valeur(nullptr){}
    /**
     * \brief Constructeur prenant la largeur et la longueur de l'état en paramètres. Il initialise un tablea
     * \param nbCol Largeur de la matrice
     * \param nbLignes Hauteur de la matrice
     */
    Etat2D(unsigned int nbCol, unsigned int nbLignes=25):EtatMere(nbCol),nbLignes(nbLignes),valeur(new unsigned int* [nbCol]) {
        for(unsigned int i=0; i<nbLignes; i++){
            valeur[i] = new unsigned int [nbLignes];
        }
        for(unsigned int i = 0; i < nbCol; ++i)
            for(unsigned int j = 0; j < nbLignes; ++j)
                valeur[i][j] = 0;
    }
    /**
     * \brief Constructeur par recopie
     * \param e Etat à recopier
     */
    Etat2D(const Etat2D& e):EtatMere(e.nbCol),nbLignes(e.nbLignes),valeur(new unsigned int* [e.nbCol]) {
        for(unsigned int i=0; i<nbCol; i++){
            valeur[i] = new unsigned int [e.nbLignes];
        }
        for(unsigned int i = 0; i < e.nbCol; ++i)
            for(unsigned int j = 0; j < e.nbLignes; ++j)
                valeur[i][j] = e.valeur[i][j];
    }
    unsigned int getNbLignes() const {return nbLignes;}
    // unsigned int getNbCol() const {return nbCol;}
    void setnbLignes(unsigned int nbL) {nbLignes = nbL;}
    unsigned int getCellule(unsigned int i,unsigned int j) const {return valeur[i][j];}
    void setCellule(unsigned int i, unsigned int j, unsigned int val);
};



/*============================================================================*/
/*                                   Cout << surcharge                        */
/*============================================================================*/

std::ostream& operator<<(std::ostream& f, const Etat1D& e);
std::ostream& operator<<(std::ostream& f, const Etat2D& e);
#endif
