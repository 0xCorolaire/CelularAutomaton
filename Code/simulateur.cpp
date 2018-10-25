#include "simulateur.h"
#include "automate.h"
#include "etats.h"

template<class T1, class T2>Simulateur<T1,T2>::Simulateur(const T1& a, unsigned int buffer):
    automate(a), etats(nullptr), depart(nullptr), nbMaxEtats(buffer),rang(0) {
    etats = new T2*[nbMaxEtats];
    for (unsigned int i = 0; i < nbMaxEtats; i++) etats[i] = nullptr;
}
template<class T1, class T2>Simulateur<T1,T2>::Simulateur(const T1& a, const T2& dep, unsigned int buffer):
    automate(a), etats(nullptr), depart(&dep), nbMaxEtats(buffer),rang(0) {
    etats = new T2*[nbMaxEtats];
    for (unsigned int i = 0; i < nbMaxEtats; i++) etats[i] = nullptr;
    etats[0] = new T2(dep);
}

template<class T1, class T2>void Simulateur<T1,T2>::build(unsigned int cellule) {
    if (cellule >= nbMaxEtats) throw AutomateException("erreur taille buffer");
    if (etats[cellule] == nullptr) etats[cellule] = new T2;
}

template<class T1, class T2>void Simulateur<T1,T2>::setEtatDepart(const T2& e) {
    depart = &e;
    reset();
}

template<class T1, class T2>void Simulateur<T1,T2>::reset() {
    if (depart==nullptr) throw AutomateException("etat depart indefini");
    build(0);
    *etats[0] = *depart;
    rang = 0;
}

template<class T1, class T2>void Simulateur<T1,T2>::next() {
    if (depart == nullptr) throw AutomateException("etat depart indefini");
    T2 *e = new T2();
    automate.appliquerTransition(*etats[rang%nbMaxEtats],*e);
    rang++;
    etats[rang%nbMaxEtats]= e;
}

template<class T1, class T2>void Simulateur<T1,T2>::run(unsigned int nb_steps) {
    for (unsigned int i = 0; i < nb_steps; i++) next();
}

template<class T1, class T2>const T2& Simulateur<T1,T2>::dernier() const {
    return *etats[rang%nbMaxEtats];
}

template<class T1, class T2>Simulateur<T1,T2>::~Simulateur() {
    for (unsigned int i = 0; i < nbMaxEtats; i++) {
        delete etats[i];
    }
    delete[] etats;
}

template class Simulateur<Automate1D,Etat1D>;
template class Simulateur<Automate2D,Etat2D>;
template class Simulateur<Automate2DFeu,Etat2D>;
template class Simulateur<Automate2DG,Etat2D>;
template class Simulateur<Automate2DBrian,Etat2D>;
