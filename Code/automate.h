#ifndef AUTOMATE
#define AUTOMATE

/**
* \file automate.h
* \brief Déclaration des classes automate (gestion des transitions)
* \author Hugo L. Hugo P. Thomas L.
* \date 15 Juin 2018
*/

#include <string>
#include <iostream>
#include "etats.h"

/*============================================================================*/
/*                              Exceptions&Transition                         */
/*============================================================================*/

/**
* \class AutomateException
* \brief Gestion des erreurs
*/
class AutomateException {
public:
    /**
    * \brief Constructeur de la classe AutomateException
    * \param message Chaîne contenant les informations sur l'erreu*/
    AutomateException(const std::string& message) :info(message) {}
    /**
    * \brief Permet d'obtenir l'information sur l'erreur
    * \return Chaîne de caractère contenant info
    */
    std::string getInfo() const { return info; }
private:
    std::string info; /*!<Chaîne de caractère contenant l'information sur l'erreur*/
};

/**
* \brief COnvertir une chaîne de caractère représentant des bits en integer
*/
short unsigned int NumBitToNum(const std::string& num);
/**
* \brief Convertir un integer en chaîne de bits
*/
std::string NumToNumBit(short unsigned int num);

/*============================================================================*/
/*                                   Automates                                */
/*============================================================================*/
/**
* \class AutomateMere
* \brief Classe abstraite à utiliser pour générer des automates, application des transition entre chaque état.
*/
template<class T>
class AutomateMere
{
public:
    AutomateMere(){}
    virtual void appliquerTransition(const T& , T&) const = 0; //Virtuelle utilisé par chacun des types d'automates (1D,2D)
};

/**
* \class Automate1D
* \brief Impémentation d'AutomateMere permettant de gérer un automate à une dimension
*/
class Automate1D : public AutomateMere<Etat1D> {
private:
    friend class AutomateManager;
    unsigned short int numero; /*!< Numero de l'automate*/
    std::string numeroBit; /*!< Chaîne de caractère des bits correspondant au numéro de l'automate*/
    /**
    * \brief Constructeur     void setMaxRegle(unsigned int max){maxRegle=max;}
    void setMaxRegle(unsigned int max){maxRegle=max;}
de la classe Automate1D
    * \param num Entier correspondant à la règle de l'automate
    */
    Automate1D(unsigned short int num);
    /**
    * \brief Constructeuer de la classe Automate1D
    * \param Chaîne de caractère correspondant à la règle de l'automate
    */
    Automate1D(const std::string& numBit);
    /**
    * \brief Destructeur de la classe Automate1D
    */
    virtual ~Automate1D(){}
    /**
    * \ Constructeur par recopie de la classe Automate1D
    */
    Automate1D(const Automate1D& a);
public:
    /**
    * \brief Obtenir le numéro de l'automate
    * \return Numéro décimal de l'automate
    */
    unsigned short int getNumero() const {return numero;}
    /**
    * \brief Obtenir le numéro de l'automate
    * \return Chaîne binaire de l'automate
    */
    const std::string getNumeroBit() const {return numeroBit;}
    /**
    * \brief Appliquer la transition correcpondant à l'automate
    * \param dep Etat de départ
    * \param dest Etat généré conformément au numéro de l'automate
    */
    void appliquerTransition(const Etat1D& dep, Etat1D& dest) const;
};
/**
* \class Automate2D
* \brief Impémentation d'AutomateMere permettant de gérer un automate du jeu de la vie
*/
class Automate2D : public AutomateMere<Etat2D> {
private:
    unsigned int min_survie; /*!< Nombre de voisins minimum pour la survie d'une cellule*/
    unsigned int max_survie; /*!< Nombre de voisins maximum pour la survie d'une cellule*/
    unsigned int min_naissance; /*!< Nombre de voisins minimum pour la naissance d'une cellule*/
    unsigned int max_naissance; /*!< Nombre de voisins maximum pour la naissance d'une cellule*/
public:
    /**
    * \brief Constructeur de l'automate du jeu de la vie
    * \param s_min Nombre de voisins minimum pour la survie d'une cellule (2 par défaut)
    * \param s_max Nombre de voisins maximum pour la survie d'une cellule (3 par défaut)
    * \param n_min Nombre de voisins minimum pour la naissance d'une cellule (3 par défaut)
    * \param n_max Nombre de voisins maximum pour la naissance d'une cellule (3 par défaut)
    * \param nbEtat Nombre d'états que l'automate devra gérer (2 par défaut)
    */
    Automate2D(unsigned int s_min=2,unsigned int s_max=3,unsigned int n_min=3, unsigned int n_max=3);
    /**
    * \brief Constructeur par recopie de la classe Automate2D
    */
    Automate2D(const Automate2D& a);
    /**
    * \brief Destructeur de la classe Automate2D
    */
    ~Automate2D(){}
    /**
    * \brief Renvoie le nombre minimal de voisins pour la survie d'une cellule
    */
    unsigned int getMinSurvie() const {return min_survie;}
    /**
    * \brief Renvoie le nombre maximal de voisins pour la survie d'une cellule
    */
    unsigned int getMaxSurvie() const {return max_survie;}
    /**
    * \brief Renvoie le nombre minimal de voisins pour la naissance d'une cellule
    */
    unsigned int getMinNaissance() const {return min_naissance;}
    /**
    * \brief Renvoie le nombre maximal de voisins pour la naissance d'une cellule
    */
    unsigned int getMaxNaissance() const {return max_naissance;}
    /**
    * \brief Définir le nombre minimal de voisins pour la survie d'une cellule
    * \param s_min Nombre de voisins minimum pour la survie d'une cellule
    */
    void setMinSurvie(unsigned int s_min){min_survie=s_min;}
    /**
    * \brief Définir le nombre maximal de voisins pour la survie d'une cellule
    * \param s_max Nombre de voisins maximum pour la survie d'une cellule
    */
    void setMaxSurvie(unsigned int s_max){max_survie=s_max;}
    /**
    * \brief Définir le nombre minimal de voisins pour la naissnace d'une cellule
    * \param n_min Nombre de voisins minimum pour la naissance d'une cellule
    */
    void setMinNaissance(unsigned int n_min){min_naissance=n_min;}
    /**
    * \brief Définir le nombre maximal de voisins pour la naissnace d'une cellule
    * \param n_max Nombre de voisins maximum pour la naissance d'une cellule
    */
    void setMaxNaissance(unsigned int n_max){max_naissance=n_max;}
    /**
    * \brief Applique la transition du jeu de la vie, conformément aux règles définies.
    * \param dep Référence vers l'état de départ de la simulation
    * \param dest Référence vers l'état généré après applicationd de la transision
    */
    void appliquerTransition(const Etat2D& dep, Etat2D& dest) const;
    /**
    * \brief Détermine le nombre de cellules vivantes autour d'une case
    * \param i Indice de la ligne de la case
    * \param j Indice de la colonne de la case
    * \param e Etat dans lequel il faut compter les voisins d'une case
    * \return Nombre de voisins actifs
    */
    unsigned int nbVoisinActif(unsigned int i,unsigned int j, Etat2D const& e) const;
};
/**
* \class Automate2DFeu
* \brief Impémentation d'AutomateMere permettant de gérer un automate du feu de forêt
*/
class Automate2DFeu : public AutomateMere<Etat2D> {
public:
    /**
    * \brief Constructeur de la calsse Automate2DFeu
    */
    Automate2DFeu(){}
    /**
    * \brief Destructeur de la classe Automate2DFeu
    */
    ~Automate2DFeu(){}
    /**
    * \brief Applique la transition correspondant à l'automate du feu de forêt
    * \param dep Etat initial
    * \param dest Etat après application de la transition
    */
    void appliquerTransition(const Etat2D& dep, Etat2D& dest) const;
    /**
    * \brief Coqmpte le nombre de voisins d'une case selon l'automate du feu de forêt
    * \param i Indice de la ligne de la case
    * \param j Indice de la colonne de la case
    * \param e Etat considéré
    */
    unsigned int nbVoisins(unsigned int i,unsigned int j, Etat2D const& e) const;
};
/**
* \class Automate2DG
* \brief Impémentation d'AutomateMere permettant de gérer un automate Griffeath
*/
class Automate2DG : public AutomateMere<Etat2D> {
public:
    /**
    * \brief Constructeur de la calsse Automate2DG
    */
    Automate2DG(){}
    /**
    * \brief Destructeur de la classe Automate2DFeu
    */
    ~Automate2DG(){}
    /**
    * \brief Applique la transition correspondant à l'automate Griffeath
    * \param dep Etat initial
    * \param dest Etat après application de la transition
    */
    void appliquerTransition(const Etat2D& dep, Etat2D& dest) const;
    /**
    * \brief Coqmpte le nombre de voisins d'une case selon l'automate du feu de forêt
    * \param i Indice de la ligne de la case
    * \param j Indice de la colonne de la case
    * \param e Etat considéré
    */
    unsigned int nbVoisins(unsigned int i,unsigned int j, Etat2D const& e, unsigned int etat_cellule) const;
};

/**
* \class Automate2DBrian
* \brief Impémentation d'AutomateMere permettant de gérer un automate Brian's Brain
*/
class Automate2DBrian : public AutomateMere<Etat2D> {
public:
    /**
    * \brief Constructeur de la calsse Automate2DBrian
    */
    Automate2DBrian(){}
    /**
    * \brief Destructeur de la classe Automate2DFeu
    */
    ~Automate2DBrian(){}
    /**
    * \brief Applique la transition correspondant à l'automate Brian's Brain
    * \param dep Etat initial
    * \param dest Etat après application de la transition
    */
    void appliquerTransition(const Etat2D& dep, Etat2D& dest) const;
    /**
    * \brief Coqmpte le nombre de voisins d'une case selon l'automate du feu de forêt
    * \param i Indice de la ligne de la case
    * \param j Indice de la colonne de la case
    * \param e Etat considéré
    */
    unsigned int nbVoisins(unsigned int i,unsigned int j, Etat2D const& e) const;
};


/*============================================================================*/
/*                                 Automate Manager                           */
/*============================================================================*/
class AutomateManager {
    Automate1D* automates[256]; /*!<Tableau de toutes les configurations possibles de l'automate*/
    /**
    * \brief Constructeur de la classe AutomateManager
    */
    AutomateManager();
    /**
    * \brief Destructeur de la classe AutomateManager
    */
    ~AutomateManager();
    /**
    * \brief Constructeur par recopie de la classe AutomateManager
    * \param a AutomateManager à recopier
    */
    AutomateManager(const AutomateManager& a);
    /**
    * \brief Surcharge de l'opérateur d'affectation de la classe AutomateManager
    * \param a AutomateManager à copier
    */
    AutomateManager& operator=(const AutomateManager& a);
    /**
    * \brief Gestion de l'instance unique d'AutomateManager
    */
    struct Handler {
        AutomateManager* instance; /*!< Pointeur vers l'instance unique d'AutomateManager*/
        /**
        * \brief Constructeur de la classe Handler
        */
        Handler():instance(nullptr) {}
        /**
        * \brief Destructeur de la classe handler, détruit l'instance unique d'AutomateManager
        */
        ~Handler() { delete instance;  }
    };
    static Handler handler; /*!<Handler de la classe AutomateManager*/
public:
    /**
    * \brief Obtenir un automate
    * \param num Entier correspondant à l'automate demandé
    */
    const Automate1D& getAutomate(unsigned short int num);
    /**
    * \brief Obtenir un automate
    * \param Chaîne de bits correspondant à l'automate demandé
    */
    const Automate1D& getAutomate(const std::string& num);
    /**
    * \brief Obtenir un AutomateManager
    */
    static AutomateManager& getAutomateManager();
    /**
    * \brief Liberer l'getAutomateManager
    */
    static void libererAutomateManager();
};


/*============================================================================*/
/*                                  Cout surcharge <<                         */
/*============================================================================*/

/**
* \brief Surcharge de l'opérateur de flux de l'automate1D
*/
std::ostream& operator<<(std::ostream& f, const Automate1D& t);

#endif // AUTOMATE
