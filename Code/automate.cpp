#include "automate.h"
#include "etats.h"


/*============================================================================*/
/*                              Exceptions&Transition                         */
/*============================================================================*/

short unsigned int NumBitToNum(const std::string& num) {
    if (num.size() != 8) throw AutomateException("Numero d'automate indefini");
    int puissance = 1;
    short unsigned int numero = 0;
    for (int i = 7; i >= 0; i--) {
        if (num[i] == '1') numero += puissance;
        else if (num[i] != '0') throw AutomateException("Numero d'automate indefini");
        puissance *= 2;
    }
    return numero;
}
std::string NumToNumBit(short unsigned int num) {
    std::string numeroBit;
    if (num > 256) throw AutomateException("Numero d'automate indefini");
    unsigned short int p = 128;
    int i = 7;
    while (i >= 0) {
        if (num >= p) {
            numeroBit.push_back('1');
            num -= p;
        }
        else { numeroBit.push_back('0'); }
        i--;
        p = p / 2;
    }
    return numeroBit;
}

/*============================================================================*/
/*                                   Automates                                */
/*============================================================================*/

/* Automate 1D */

Automate1D::Automate1D(unsigned short int num):numero(num),numeroBit(NumToNumBit(num)){}
Automate1D::Automate1D(const std::string& numBit):numero(NumBitToNum(numBit)),numeroBit(numBit){}
Automate1D::Automate1D(const Automate1D& a):numero(a.numero),numeroBit(a.numeroBit){}
void Automate1D::appliquerTransition(const Etat1D& dep, Etat1D& dest) const {
    if(dep.getNbCol()!=dest.getNbCol())
        dest=dep;
    for(unsigned int i=0;i<dep.getNbCol();i++){
        unsigned short int conf=0;
        if(i > 0) conf+=dep.getCellule(i - 1) * 4;
        conf+=dep.getCellule(i)*2;
        if(i < dep.getNbCol()-1) conf+=dep.getCellule(i+1);
        dest.setCellule(i, numeroBit[7-conf]-'0');
    }
}

/* Automate 2D */
Automate2D::Automate2D(unsigned int s_min,unsigned int s_max,unsigned int n_min, unsigned int n_max):min_survie(s_min),max_survie(s_max),min_naissance(n_min),max_naissance(n_max){}
unsigned int Automate2D::nbVoisinActif(unsigned int i,unsigned int j, Etat2D const& e) const {
    unsigned int nb=0;
    unsigned int nbCol=e.getNbCol();
    unsigned int nbLignes=e.getNbLignes();
    int ainf,asup,binf,bsup;
    if(i==0){
        ainf=0;asup=1;
    }else if(i==(nbLignes-1)){
        ainf=-1;asup=0;
    }else{
        ainf=-1;asup=1;
    }
    if(j==0){
        binf=0;bsup=1;
    }else if(j==(nbCol-1)){
        binf=-1;bsup=0;
    }else{
        binf=-1;bsup=1;
    }
    for(int a=ainf;a<=asup;a++){
        for(int b=binf;b<=bsup;b++){
            if(e.getCellule((i+a+nbLignes)%nbLignes,(j+b+nbCol)%nbCol)) ++nb;
        }
    }
    if (e.getCellule(i,j) == true) nb--;
    return nb;
}

void Automate2D::appliquerTransition(const Etat2D& dep, Etat2D& dest) const{
    if((dep.getNbCol() != dest.getNbCol()) || (dep.getNbLignes() != dest.getNbLignes())) dest=dep;
    unsigned int nbCol = dep.getNbCol();
    unsigned int nbLignes = dep.getNbLignes();
    unsigned int nbActif[nbCol*nbLignes];
    unsigned int k=0;
    for(unsigned int i=0;i<nbLignes;i++){
        for(unsigned int j=0; j<nbCol; j++){
            nbActif[k++] = nbVoisinActif(i,j,dep);
        }
    }
    k=0;
    for(unsigned int i=0;i<nbLignes;i++){
        for(unsigned int j=0; j<nbCol; j++){
            if(dep.getCellule(i,j)){
                if (nbActif[k]>=min_survie && nbActif[k]<=max_survie){
                    dest.setCellule(i,j,true);
                }else{
                    dest.setCellule(i,j,false);
                }
            }else{
                if (nbActif[k]>=min_naissance && nbActif[k]<=max_naissance){
                    dest.setCellule(i,j,true);
                }else{
                    dest.setCellule(i,j,false);
                }
            }
            k++;
        }
    }
}
/* Automate test */
unsigned int Automate2DFeu::nbVoisins(unsigned int i,unsigned int j, Etat2D const& e) const{
    unsigned int nb=0;
    unsigned int nbCol=e.getNbCol();
    unsigned int nbLignes=e.getNbLignes();
    // on utilise le voisinage de Von Neuman ( 4 voisins )
    if (e.getCellule((i-1+nbLignes)%nbLignes,(j+nbCol)%nbCol) == 2 && i!=0) ++nb; //base
    if (e.getCellule((i+1+nbLignes)%nbLignes,(j+nbCol)%nbCol) == 2 && i!=(nbLignes-1)) ++nb;
    if (e.getCellule((i+nbLignes)%nbLignes,(j-1+nbCol)%nbCol) == 2 && j!=0) ++nb; //Droite
    if (e.getCellule((i+nbLignes)%nbLignes,(j+1+nbCol)%nbCol) == 2 && j!=(nbCol-1)) ++nb;
    return nb;
}

void Automate2DFeu::appliquerTransition(const Etat2D& dep, Etat2D& dest) const{
    if((dep.getNbCol() != dest.getNbCol()) || (dep.getNbLignes() != dest.getNbLignes())) dest=dep;
    unsigned int nbCol = dep.getNbCol();
    unsigned int nbLignes = dep.getNbLignes();
    unsigned int nbFeu[nbCol*nbLignes];
    unsigned int k=0;
    for(unsigned int i=0;i<nbLignes;i++){
        for(unsigned int j=0; j<nbCol; j++){
            nbFeu[k++] = nbVoisins(i,j,dep);
        }
    }
    k=0;
    /*
    une cellule Vide reste Vide
    une cellule Cendre reste Cendre
    une cellule Feu devient Cendre
    une cellule Arbre
    devient Feu si un de ses voisins est en Feu
    reste Arbre sinon*/
    for(unsigned int i=0;i<nbLignes;i++){
        for(unsigned int j=0; j<nbCol; j++){
            switch(dep.getCellule(i,j)){
            case 1 :
                if(nbFeu[k]>=1){
                    dest.setCellule(i,j,1);
                }else{
                    dest.setCellule(i,j,0);
                }
                break;
            case 2 :
                dest.setCellule(i,j,1);
                break;
            case 3 :
                if (nbFeu[k]>=1){
                    dest.setCellule(i,j,2);
                }else{
                    dest.setCellule(i,j,3);
                }
                break;
            default :
                dest.setCellule(i,j,0);
                break;
            }
            k++;
        }
    }
}

/* Automate Griffeath */
unsigned int Automate2DG::nbVoisins(unsigned int i,unsigned int j, Etat2D const& e, unsigned int etat_cellule) const{
    unsigned int nb=0;
    unsigned int nbCol=e.getNbCol();
    unsigned int nbLignes=e.getNbLignes();
    for(int a=-1;a<=1;a++){
        for(int b=-1;b<=1;b++){
            if(e.getCellule((i+a+nbCol)%nbCol,(j+b+nbLignes)%nbLignes) == etat_cellule) ++nb;
        }
    }
    if (e.getCellule(i,j) == etat_cellule) nb--;
    return nb;
}

void Automate2DG::appliquerTransition(const Etat2D& dep, Etat2D& dest) const{
    if((dep.getNbCol() != dest.getNbCol()) || (dep.getNbLignes() != dest.getNbLignes())) dest=dep;
    unsigned int nbCol = dep.getNbCol();
    unsigned int nbLignes = dep.getNbLignes();
    unsigned int nbColor0[nbCol*nbLignes];
    unsigned int nbColor1[nbCol*nbLignes];
    unsigned int nbColor2[nbCol*nbLignes];
    unsigned int nbColor3[nbCol*nbLignes];
    unsigned int k=0,l=0,m=0,n=0;
    for(unsigned int i=0;i<nbCol;i++){
        for(unsigned int j=0; j<nbLignes; j++){
            switch(dep.getCellule(i,j)){
            case 0:
                nbColor1[k++] = nbVoisins(i,j,dep,1);
                break;
            case 1:
                nbColor2[l++] = nbVoisins(i,j,dep,2);
                break;
            case 2:
                nbColor3[m++] = nbVoisins(i,j,dep,3);
                break;
            case 3:
                nbColor0[n++] = nbVoisins(i,j,dep,0);
                break;
            default:
                break;
            }
        }
    }
    k=0;
    l=0;
    m=0;
    n=0;
    for(unsigned int i=0;i<nbCol;i++){
        for(unsigned int j=0; j<nbLignes; j++){
            switch(dep.getCellule(i,j)){
            case 1 :
                if(nbColor2[l++]>=3){
                    dest.setCellule(i,j,2);
                }else{
                    dest.setCellule(i,j,1);
                }
                break;
            case 2 :
                if(nbColor3[m++]>=3){
                    dest.setCellule(i,j,3);
                }else{
                    dest.setCellule(i,j,2);
                }
                break;
            case 3 :
                if (nbColor0[n++]>=3){
                    dest.setCellule(i,j,0);
                }else{
                    dest.setCellule(i,j,3);
                }
                break;
            case 0 :
                if(nbColor1[k++]>=3){
                    dest.setCellule(i,j,1);
                }else{
                    dest.setCellule(i,j,0);
                }
                break;
            }
        }
    }
}
/* Brian Brain */
 unsigned int Automate2DBrian::nbVoisins(unsigned int i,unsigned int j, Etat2D const& e) const{
     unsigned int nb=0;
     unsigned int nbCol=e.getNbCol();
     unsigned int nbLignes=e.getNbLignes();
     for(int a=-1;a<=1;a++){
         for(int b=-1;b<=1;b++){
             if(e.getCellule((i+a+nbCol)%nbCol,(j+b+nbLignes)%nbLignes) == 1) ++nb;
         }
     }
     if (e.getCellule(i,j) == 1) nb--;
     return nb;
 }

 void Automate2DBrian::appliquerTransition(const Etat2D& dep, Etat2D& dest) const{
     if((dep.getNbCol() != dest.getNbCol()) || (dep.getNbLignes() != dest.getNbLignes())) dest=dep;
     unsigned int nbCol = dep.getNbCol();
     unsigned int nbLignes = dep.getNbLignes();
     unsigned int nbFiring[nbCol*nbLignes];
     unsigned int k=0;
     for(unsigned int i=0;i<nbCol;i++){
         for(unsigned int j=0; j<nbLignes; j++){
           nbFiring[k++] = nbVoisins(i,j,dep);
         }
     }
     k=0;
     for(unsigned int i=0;i<nbCol;i++){
         for(unsigned int j=0; j<nbLignes; j++){
             switch(dep.getCellule(i,j)){
             case 0 :
                 if(nbFiring[k]==2){
                     dest.setCellule(i,j,1);
                 }else{
                     dest.setCellule(i,j,0);
                 }
                 break;
             case 1 :
                     dest.setCellule(i,j,2);
                 break;
             case 2 :
                     dest.setCellule(i,j,0);
                 break;
             }
             k++;
         }
     }

}


/*============================================================================*/
/*                                 Automate Manager                           */
/*============================================================================*/


AutomateManager::AutomateManager() {
    for (unsigned int i = 0; i < 256; i++) automates[i] = nullptr;
}

AutomateManager::~AutomateManager() {
    for (unsigned int i = 0; i < 256; i++) delete automates[i];
}

AutomateManager::Handler AutomateManager::handler = Handler();

AutomateManager& AutomateManager::getAutomateManager() {
    if (!handler.instance) handler.instance = new AutomateManager;
    return *handler.instance;
}

void AutomateManager::libererAutomateManager() {
    delete handler.instance;
    handler.instance = nullptr;
}

const Automate1D& AutomateManager::getAutomate(unsigned short int num) {
    if (!automates[num]) automates[num] = new Automate1D(num);
    return *automates[num];
}
const Automate1D& AutomateManager::getAutomate(const std::string& numBit) {
    return getAutomate(NumBitToNum(numBit));
}



/*============================================================================*/
/*                                  Cout surcharge <<                         */
/*============================================================================*/
std::ostream& operator<<(std::ostream& f, const Automate1D& A) {
    f << A.getNumero() << " : " << A.getNumeroBit() << "\n";
    return f;
}
