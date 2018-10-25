#ifndef AUTOCELL_H
#define AUTOCELL_H

/**
* \file autocell.h
* \brief Déclaration des classes autocell (interface du programme)
* \author Hugo L. Hugo P. Thomas L.
* \date 15 Juin 2018
*/

#include <QtGui>
#include <qobject.h>
#include <iostream>
#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QtXml>
#include <string>
#include <QString>
#include "automate.h"
#include "etats.h"
#include "simulateur.h"
#include "choix.h"

/**
* \class Interface
* \brief Regroupe les élements d'interface graphique pour tout automate à une dimension
*/

class Interface{
protected:
    static const unsigned int size = 625; /*!< Taille de réference de la grille en pixels*/
    unsigned int nbCol = 25; /*!< Nombre de colonnes initial*/

    QHBoxLayout* configuration; /*!< Layout des boutons de configuration de l'automate*/
    QPushButton* aleatoire; /*!< Bouton de remplissage aléatoire de la situation de départ*/
    QLabel* aleat; /*!< Label du bouton aleatoire*/
    QVBoxLayout* aleatc; /*!<Layout du remplissage aleatoire*/
    QTableWidget* depart; /*!< Tableau de la situation de départ*/

    QSpinBox* grilletaille; /*!< Choix de la taille de la grille*/
    QLabel* grillet; /*!< Label du choix de la taille de la grille*/
    QVBoxLayout* grillec; /*!< Layot du choix de la taille de la grille*/

    QHBoxLayout* simulationButtons; /*!< Layout des boutons de simulation*/
    QPushButton* boutonChoix; /*!< Bouton de retour au choix de l'automate*/
    QPushButton* simulation; /*!< Bouton d'execution d'une itération de la simuation*/
    QPushButton* restaurer; /*!< Bouton de restauration du dernier état*/
    QPushButton* raz; /*!< Bouton de réinitialisation de la grille*/

    QVBoxLayout* couche; /*!< Layout de l'interface de l'automate*/

    /**
    * \brief Change la taille de la grille
    * \param a Entier désignant la nouvelle taille de la grille
    */
    virtual void changeTaille(int a) = 0;
    /**
    * \brief Remplit aléatoirement la situation de départ
    */
    virtual void remplissageAleatoire() = 0;
    /**
    * \brief Réinitialise la grille ainsi que les réglages de l'automate
    */
    virtual void resetEtat() = 0;
    /**
    * \brief Réalise une itération de la simulation
    */
    virtual void faireSimulation() = 0;
    /**
    * \brief Permet de retourner à la fenêtre de choix de l'automate
    */
    virtual void retourMenu() = 0;
    /**
    * \brief Activation des cellules au clic de la souris
    */
    virtual void cellActivation(const QModelIndex& index) = 0;
};

/**
* \class Interface2D
* \brief Regroupe les éléments d'interface graphique pour tout automate à 2 dimensions.
*
*/
class Interface2D : public Interface {
protected:

    unsigned int nbLignes = 25; /*!<Nombre de lignes initial*/

    QSpinBox* speedSimulation; /*!<Réglage de la vitesse de la simulation*/
    QLabel* speedSim; /*!< Label du réglage de la vitesse de la simulation*/
    QVBoxLayout* speedSimc; /*!< Layout du réglage de la vitesse de la simulation*/

    QTimer* timer; /*!<Timer pour la simulation de l'automate*/
    unsigned int interval = 500; /*!< Interval entre deux itérations de la simulation de l'automate*/

    QPushButton* simulationPause; /*!<Bouton d'arrêt de la simulation de l'automate*/
    QPushButton* simulationStart; /*!<Bouton de démarrage de la simulation de l'automate*/

    /**
    * \brief Déclenche la simulation en continu de l'automate
    */
    virtual void faireSimulationStart() = 0;
    /**
    * \brief Interrompt la simulation en continu de l'automate
    */
    virtual void faireSimulationPause() = 0;
    /**
    * \brief Restaure le dernier état simulé ainsi que les réglages de l'automate
    */
    virtual void restaurer2D(){qDebug()<<"Restauration non prise en charge";}
    /**
    * \brief Sauvegarde un état de l'automate ainsi que les réglages
    */
    virtual void sauvegarder2D(){qDebug()<<"Sauvegarde non prise en charge";}
    /**
    * \brief Modifie l'interval de temps entre deux itérations de la simulation de l'automate
    */
    virtual void changeSpeed(int i) = 0;

};

/**
* \class AutoCell
* \brief Implémentation du Jeu de la Vie
*/
class AutoCell : public QWidget, private Interface2D {
    Q_OBJECT

    QVBoxLayout* survie; /*!< Layout de la survie d'une cellule*/
    QLabel* surviel; /*!< Label du reglage de la survie d'une cellule*/
    QHBoxLayout* choix_survie; /*!< Layout du réglage du nombre de voisins pour la survie d'une cellule*/
    QLabel* s_min; /*!< Label du mimnimum de voisins pour la survie d'une cellule*/
    QLabel* s_max; /*!<Label du maximum de voisins pour la survie d'une cellule*/
    QSpinBox* s_nbMin; /*!<Réglage du minimum de voisins que doit avoir une cellule pour survivre*/
    QSpinBox* s_nbMax; /*!<Réglage du maximum de voisins que doit avoir une cellule pour survivre*/

    QVBoxLayout* naissance; /*!<Layout de la naissance d'une cellule*/
    QLabel* naissancel; /*!<Lable du réglage de la naissance d'une cellule*/
    QHBoxLayout* choix_naissance; /*!<Layout du réglage du nombre de voisins pour la naissance d'une cellule*/
    QLabel* n_min; /*!<Label du minimum de voisins pour la naissance d'une cellule*/
    QLabel* n_max; /*!<Label du maximum de voisins pour la naissance d'une cellule*/
    QSpinBox* n_nbMin; /*!<Réglage du nombre minimum de voisins que doit avoir une case pour qu'une cellule puisse naître*/
    QSpinBox* n_nbMax; /*!<Réglage du nombre maximum de voisins que doit avoir une case pour qu'une cellule puisse naître*/

    QFrame* line; /*!< Ligne de séparation dans les réglages de l'automate*/
    QSignalMapper* restaurerMapper; /*!<Mapper permettant d'appeler la restauration avec passage d'argument*/
    QHBoxLayout* liste_boutons; /*!< Liste des boutons de restauration*/
    QPushButton* planeur; /*!< Bouton restaurant un fichier de configuration contenant un planeur*/
    QPushButton* clown; /*!< Bouton restaurant un fichier de configuration contenant un clown*/

    static const unsigned int size = 625; /*!< Taille de réference de la grille en pixels*/

public:
    /**
    * \brief Constructeur de l'automate du jeu de la Vie
    */
    explicit AutoCell(QWidget* parent = nullptr);
private slots:
    /**
    * \brief Activation des cellules au clic de la souris
    */
    void cellActivation(const QModelIndex& index);
    /**
    * \brief Réalise une itération de la simulation
    */
    void faireSimulation();
    /**
    * \brief Déclenche la simulation en continu de l'automate
    */
    void faireSimulationStart();
    /**
    * \brief Interrompt la simulation en continu de l'automate
    */
    void faireSimulationPause();
    /**
    * \brief Remplit aléatoirement la situation de départ
    */
    void remplissageAleatoire();
    /**
    * \brief Change la taille de la grille
    * \param a Entier désignant la nouvelle taille de la grille
    */
    void changeTaille(int a);
    /**
    * \brief Restaure le dernier état simulé ainsi que les réglages de l'automate
    */
    void restaurer2D(int nb=0);
    /**
    * \brief Sauvegarde un état de l'automate ainsi que les réglages
    */
    void sauvegarder2D(const Etat2D& e, int nb=0);
    /**
    * \brief Réinitialise la grille ainsi que les réglages de l'automate
    */
    void resetEtat();
    /**
    * \brief Modifie l'interval de temps entre deux itérations de la simulation de l'automate
    */
    void changeSpeed(int i);
    /**
    * brief Actualise les valeurs possibles pour le réglages du nombre de voisins pour la survie/naissance d'une cellule
    */
    void updateMinMax();
    /**
    * \brief Permet de retourner à la fenêtre de choix de l'automate
    */
    void retourMenu();
};


/**
* \class AutoCellFeu
* \brief Implémentation de l'automate feu de forêt
*/
class AutoCellFeu : public QWidget, private Interface2D {
    Q_OBJECT

    static const unsigned int size = 625; /*!< Taille de réference de la grille en pixels*/

public:
    /**
    * \brief Constructeur de l'automate feu de forêt
    */
    explicit AutoCellFeu(QWidget* parent = nullptr);
private slots:
    /**
    * \brief Activation des cellules au clic de la souris
    */
    void cellActivation(const QModelIndex& index);
    /**
    * \brief Réalise une itération de la simulation
    */
    void faireSimulation();
    /**
    * \brief Déclenche la simulation en continu de l'automate
    */
    void faireSimulationStart();
    /**
    * \brief Interrompt la simulation en continu de l'automate
    */
    void faireSimulationPause();
    /**
    * \brief Remplit aléatoirement la situation de départ
    */
    void remplissageAleatoire();
    /**
    * \brief Change la taille de la grille
    * \param a Entier désignant la nouvelle taille de la grille
    */
    void changeTaille(int a);
    /**
    * \brief Restaure le dernier état simulé ainsi que les réglages de l'automate
    */
    void restaurer2D();
    /**
    * \brief Sauvegarde un état de l'automate ainsi que les réglages
    */
    void sauvegarder2D(const Etat2D&);
    /**
    * \brief Réinitialise la grille ainsi que les réglages de l'automate
    */
    void resetEtat();
    /**
    * \brief Modifie l'interval de temps entre deux itérations de la simulation de l'automate
    */
    void changeSpeed(int i);
    /**
    * \brief Permet de retourner à la fenêtre de choix de l'automate
    */
    void retourMenu();
};

/**
* \class AutoCellG
* \brief Implémentation de l'automate Griffeath
*/
class AutoCellG : public QWidget, private Interface2D {
    Q_OBJECT

    unsigned int interval = 100; /*!< Interval entre deux itérations de la simulation de l'automate*/
    static const unsigned int size = 625; /*!< Taille de réference de la grille en pixels*/
    unsigned int nbCol =100; /*!< Nombre de colonnes initial*/
    unsigned int nbLignes = 100; /*!< Nombre de lignes initial*/

public:
    /**
    * \brief Constructeur de l'automate Griffeath
    */
    explicit AutoCellG(QWidget* parent = nullptr);
private slots:
    /**
    * \brief Activation des cellules au clic de la souris
    */
    void cellActivation(const QModelIndex& index);
    /**
    * \brief Réalise une itération de la simulation
    */
    void faireSimulation();
    /**
    * \brief Déclenche la simulation en continu de l'automate
    */
    void faireSimulationStart();
    /**
    * \brief Interrompt la simulation en continu de l'automate
    */
    void faireSimulationPause();
    /**
    * \brief Remplit aléatoirement la situation de départ
    */
    void remplissageAleatoire();
    /**
    * \brief Change la taille de la grille
    * \param a Entier désignant la nouvelle taille de la grille
    */
    void changeTaille(int a);
    /**
    * \brief Restaure le dernier état simulé ainsi que les réglages de l'automate
    */
    void restaurer2D();
    /**
    * \brief Sauvegarde un état de l'automate ainsi que les réglages
    */
    void sauvegarder2D(const Etat2D&);
    /**
    * \brief Réinitialise la grille ainsi que les réglages de l'automate
    */
    void resetEtat();
    /**
    * \brief Modifie l'interval de temps entre deux itérations de la simulation de l'automate
    */
    void changeSpeed(int i);
    /**
    * \brief Permet de retourner à la fenêtre de choix de l'automate
    */
    void retourMenu();
};

/**
* \class AutoCellBrian
* \brief Implémentation de l'automate Brian's Brain
*/
class AutoCellBrian : public QWidget, private Interface2D {
    Q_OBJECT

    unsigned int interval = 100; /*!< Interval entre deux itérations de la simulation de l'automate*/
    static const unsigned int size = 625; /*!< Taille de réference de la grille en pixels*/
    unsigned int nbCol =100; /*!< Nombre de colonnes initial*/
    unsigned int nbLignes = 100; /*!< Nombre de lignes initial*/

public:
    /**
    * \brief Constructeur de l'automate Brian's Brain
    */
    explicit AutoCellBrian(QWidget* parent = nullptr);
private slots:
    /**
    * \brief Activation des cellules au clic de la souris
    */
    void cellActivation(const QModelIndex& index);
    /**
    * \brief Réalise une itération de la simulation
    */
    void faireSimulation();
    /**
    * \brief Déclenche la simulation en continu de l'automate
    */
    void faireSimulationStart();
    /**
    * \brief Interrompt la simulation en continu de l'automate
    */
    void faireSimulationPause();
    /**
    * \brief Remplit aléatoirement la situation de départ
    */
    void remplissageAleatoire();
    /**
    * \brief Change la taille de la grille
    * \param a Entier désignant la nouvelle taille de la grille
    */
    void changeTaille(int a);
    /**
    * \brief Restaure le dernier état simulé ainsi que les réglages de l'automate
    */
    void restaurer2D();
    /**
    * \brief Sauvegarde un état de l'automate ainsi que les réglages
    */
    void sauvegarder2D(const Etat2D&);
    /**
    * \brief Réinitialise la grille ainsi que les réglages de l'automate
    */
    void resetEtat();
    /**
    * \brief Modifie l'interval de temps entre deux itérations de la simulation de l'automate
    */
    void changeSpeed(int i);
    /**
    * \brief Permet de retourner à la fenêtre de choix de l'automate
    */
    void retourMenu();
};

/**
* \class AutoCell1D
* \brief Implémentation d'un automate à une dimension
*/
class AutoCell1D : public QWidget, public Interface {
    Q_OBJECT

    static const unsigned int size = 625; /*!< Taille de réference de la grille en pixels*/

    QSpinBox* num; /*!< Réglage du numéro décimal de l'automate*/
    QLabel* numl; /*!<Lable du réglage du numéro décimal de l'automate*/
    QVBoxLayout* numc; /*!<Layout du réglage du numéro décimal de l'automate*/

    QLineEdit* numeroBit[8]; /*!<Réglage de la valeur de chaque bit du numéro binaire de l'automate*/
    QLabel* numeroBitl[8]; /*!<Label de la règle correspondant à chaque bit du numéro de l'automate*/
    QVBoxLayout* bitc[8]; /*!< Layout de chaque bit correspondant au numéro binaire de l'automate*/
    QIntValidator* zeroOneValidator; /*!<Valide que chaque bit ait une valeur de 0 ou 1*/

    QVBoxLayout* regleAleatoirec; /*!<Layout de la règle aléatoire de l'automate*/
    QLabel* regleAleatoirel; /*!<Label du réglage aléatoire de l'automate*/
    QPushButton* btnRegleAleatoire; /*!<Bouton du réglage aléatoire de l'automate*/

    QTableWidget* etats; /*!<Tableaux des états générés par l'automate*/

public:
    /**
    * \brief Constructeur de l'automate à une dimension
    */
    explicit AutoCell1D(QWidget* parent = nullptr);
private slots:
    /**
    * \brief Synchronise l'affichage entre le numéro décimal et le numéro binaire de l'automate
    */
    void synchronizeNumToNumBit(int i);
    /**
    * \brief Synchronise l'affichage entre le numéro binaire et le numéro décimal de l'automate
    */
    void synchronizeNumBitToNum(const QString& s);
    /**
    * \brief Change la taille de la grille
    * \param a Entier désignant la nouvelle taille de la grille
    */
    void changeTaille(int a);
    /**
    * \brief Activation des cellules au clic de la souris
    */
    void cellActivation(const QModelIndex& index);
    /**
    * \brief Remplit aléatoirement la situation de départ
    */
    void remplissageAleatoire();
    /**
    * \brief Définit aléatoirement une règle pour l'automate
    */
    void regleAleatoire();
    /**
    * \brief Réalise la simulation de l'automate jusqu'au remplissage de la grille
    */
    void faireSimulation();
    /**
    * \brief Sauvegarde le dernier état généré lors de la simulation ainsi que les réglages de l'automate
    */
    void sauvegarder1D(const Etat1D& e);
    /**
    * \brief Restaure l'état sauvegarde ainsi que les réglages de l'automate
    */
    void restaurer1D();
    /**
    * \brief Réinitialise la grille ainsi que les réglages de l'automate
    */
    void resetEtat();
    /**
    * \brief Permet de retourner à la fenêtre de choix de l'automate
    */
    void retourMenu();
};

#endif // AUTOCELL_H
