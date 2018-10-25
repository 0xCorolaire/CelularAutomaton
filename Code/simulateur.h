#ifndef _SIMULATEUR_H
#define _SIMULATEUR_H

/**
* \file simulateur.h
* \brief Déclaration de la classe Simulateur (gestion des états générés)
* \author Hugo L. Hugo P. Thomas L.
* \date 16 Juin 2018
*/

#include <string>
#include <iostream>
#include "etats.h"
#include "automate.h"

class Etat1D;
template<class T>class AutomateMere;
template<class T1, class T2> class Simulateur;
class AutomateException;

// T1 => Automate 1D | 2D
// T2 => Etat 1D | 2D
template<class T1, class T2>
class Simulateur {
    const T1& automate; /*!< Automate du simulateur*/
    T2** etats; /*!< Etats du simulateur*/
    const T2* depart; /*!< Etat de départ du simulateur*/
    const unsigned int nbMaxEtats; /*!< Taille du buffer*/
    unsigned int rang; /*!< Rand du dernier état généré*/
    /**
    * \brief Création d'un nouvel état au rang indiqué, si besoin
    * \param c Indice considéré
    */
    void build(unsigned int c);
    /**
    * \brief Constructeur par recopie de la classe simulateur
    * \param s Simulateur à recopier
    */
    Simulateur(const Simulateur& s);
    /**
    * \brief Surcharge de l'opérateur d'affectation
     * \param s Simulateur à affecter
     * \return Simulateur
     */
    Simulateur& operator=(const Simulateur& s);
public:
    /**
    * \brief Constructeur de la classe Simulateur
    * \param a Automate
    * \param buffer Buffer
    */
    Simulateur(const T1& a,unsigned int buffer=2);
    /**
    * \brief Constructeur de la classe Simulateur avec état de départ
    * \param a Automate
    * \param dep Etat de départ
    * \param buffer Buffer
    */
    Simulateur(const T1& a, const T2& dep, unsigned int buffer=2);
    /**
    * \brief Initialisation de l'état de départ du simulateur
    */
    void setEtatDepart(const T2& e);
    /**
    * \brief Génère un certain nombre d'états.
    */
    void run(unsigned int nbSteps);
    /**
    * \brief Génère le prochain état
    */
    void next();
    /**
    * \brief Retourne le dernier état généré
    * \return Dernier état généré
    */
    const T2& dernier() const;
    /**
    * \brief Retourne le rang du dernier état généré
    * \return rang du dernier état généré
    */
    unsigned int getRangDernier() const { return rang; }
    /**
    * \brief Revenir à l'état de départ
    */
    void reset();
    /**
    * \brief Destructeur de la classe Simulateur
    */
    ~Simulateur();
};

#endif
