#ifndef CHOIX_H
#define CHOIX_H
/**
* \file choix.h
* \brief Menu du programme : choix de l'automate cellulaire à afficher
* \author Hugo L. Hugo P. Thomas L.
* \date 15 Juin 2018
*/
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

/**
* \class Choix
* \brief Fenêtre de choix de l'automate à afficher
*/
class Choix : public QWidget{
    Q_OBJECT

    QVBoxLayout *layout; /*!< Layout contenant les boutons vers les automates*/
    QPushButton *mode1; /*!< Bouton de l'automate à une dimension*/
    QPushButton *mode2; /*!< Bouton de l'automate du jeu de la vie*/
    QPushButton *mode2feu; /*!< Bouton de l'automate feu de forêt*/
    QPushButton *mode2griffeath; /*!< Bouton de l'automate Griffeath*/
    QPushButton *mode2brian; /*!< Bouton de l'automate Biran's Brain*/
    // QPushButton *restorePrevious;

//    static Choix instance;
    Choix(const Choix& c);
    /**
    * \brief Surcharge de l'opérateur =, privé pour empécher la copie de Choix
    */
    Choix& operator=(const Choix& c);
    /**
    * \brief Constructeur de la classe Choix, privé pour empécher l'instanciation par l'utilisateur
    */
    Choix(QWidget* parent = nullptr);
    /**
    * \brief Destructeur de la classe Choix, privé pour empécher la destruction par l'utilisateur
    */
    ~Choix();
    /**
    * \class Handler
    * \brief Gestion de l'instance unique de Choix
    */
    struct Handler {
        Choix* instance; /*!< Pointeur vers une instance de la classe Choix*/
        /**
        * \brief Constructeur de la classe Handler, initialise un pointeur nul d'instance
        */
        Handler():instance(nullptr) {}
        /**
        * \brief Destructeur de la Classe Handler, détruit également l'unique instance de la classe Choix
        */
        ~Handler() { delete instance;  }
    };
    static Handler handler;

public:
    /**
    * \brief Retourne une instance de la classe Choix*/
    static Choix& getInstance();
private slots:
    /**
    * \brief Créé une instance d'AutoCell1D et en affiche son interface.
    */
    void goMode1();
    /**
    * \brief Créé une instance d'AutoCell2D et en affiche son interface.
    */
    void goMode2();
    /**
    * \brief Créé une instance d'AutoCellFeu et en affiche son interface.
    */
    void goMode2f();
    /**
    * \brief Créé une instance d'AutoCellG et en affiche son interface.
    */
    void goMode2g();
    /**
    * \brief Créé une instance d'AutoCellBrian et en affiche son interface.
    */
    void goMode2b();
};

#endif // CHOIX_H
