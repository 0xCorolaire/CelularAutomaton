#include "autocell.h"

AutoCell::AutoCell(QWidget* parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(faireSimulation()));

    configuration = new QHBoxLayout;

    survie = new QVBoxLayout;
    surviel = new QLabel("Voisins survie");
    survie->addWidget(surviel);
    s_min = new QLabel("Min");
    s_max = new QLabel("Max");
    s_nbMin = new QSpinBox(this);
    s_nbMin->setValue(2);
    s_nbMax = new QSpinBox;
    s_nbMax->setValue(3);
    s_nbMin->setRange(0,s_nbMax->value());
    s_nbMax->setRange(s_nbMin->value(),8);
    choix_survie = new QHBoxLayout;
    choix_survie->addWidget(s_min);
    choix_survie->addWidget(s_nbMin);
    choix_survie->addWidget(s_max);
    choix_survie->addWidget(s_nbMax);
    survie->addLayout(choix_survie);

    naissance = new QVBoxLayout;
    naissancel = new QLabel("Voisins naissance");
    naissance->addWidget(naissancel);
    n_min = new QLabel("Min");
    n_max = new QLabel("Max");
    n_nbMin = new QSpinBox(this);
    n_nbMin->setValue(3);
    n_nbMax = new QSpinBox;
    n_nbMax->setValue(3);
    n_nbMin->setRange(0,n_nbMax->value());
    n_nbMax->setRange(n_nbMin->value(),8);
    choix_naissance = new QHBoxLayout;
    choix_naissance->addWidget(n_min);
    choix_naissance->addWidget(n_nbMin);
    choix_naissance->addWidget(n_max);
    choix_naissance->addWidget(n_nbMax);
    naissance->addLayout(choix_naissance);

    connect(s_nbMin, SIGNAL(valueChanged(int)),this,SLOT(updateMinMax()));
    connect(s_nbMax, SIGNAL(valueChanged(int)),this,SLOT(updateMinMax()));
    connect(n_nbMin, SIGNAL(valueChanged(int)),this,SLOT(updateMinMax()));
    connect(n_nbMax, SIGNAL(valueChanged(int)),this,SLOT(updateMinMax()));

    grilletaille = new QSpinBox(this);
    grilletaille->setRange(1,150);
    grilletaille->setValue(nbCol);
    grillet = new QLabel("Taille de la grille",this);
    connect(grilletaille,SIGNAL(valueChanged(int)),this,SLOT(changeTaille(int)));

    grillec = new QVBoxLayout;
    grillec->addWidget(grillet);
    grillec->addWidget(grilletaille);

    speedSimulation = new QSpinBox(this);
    speedSimulation->setRange(interval/10,interval*2);
    speedSimulation->setValue(interval);
    speedSimulation->setSingleStep(interval/10);
    speedSim = new QLabel("Vitesse");
    connect(speedSimulation,SIGNAL(valueChanged(int)),this,SLOT(changeSpeed(int)));

    speedSimc = new QVBoxLayout;
    speedSimc->addWidget(speedSim);
    speedSimc->addWidget(speedSimulation);

    configuration->addLayout(survie);
    configuration->addLayout(naissance);
    configuration->addLayout(grillec);
    configuration->addLayout(speedSimc);

    depart = new QTableWidget(nbCol,nbLignes,this);
    depart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    depart->setFocusPolicy(Qt::NoFocus);
    depart->setSelectionMode(QAbstractItemView::NoSelection);

    depart->setFixedSize(size-size%nbCol,size-size%nbLignes);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);


    line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);

    liste_boutons = new QHBoxLayout;
    aleatoire = new QPushButton("Remplissage Aléatoire",this);
    planeur = new QPushButton("Planeur",this);
    clown = new QPushButton("Clown",this);

    restaurerMapper = new QSignalMapper(this);

    connect(aleatoire,SIGNAL(clicked()),this,SLOT(remplissageAleatoire()));
    connect (planeur, SIGNAL(clicked()), restaurerMapper, SLOT(map()));
    connect (clown, SIGNAL(clicked()), restaurerMapper, SLOT(map()));

    restaurerMapper->setMapping(planeur, 11);
    restaurerMapper->setMapping(clown, 12);

    connect(restaurerMapper, SIGNAL(mapped(int)), this, SLOT(restaurer2D(int)));

    liste_boutons->addWidget(aleatoire);
    liste_boutons->addWidget(planeur);
    liste_boutons->addWidget(clown);

    for(unsigned int i=0; i<nbCol; i++){
        depart->setColumnWidth(i,size/nbLignes);
        depart->setRowHeight(i,size/nbLignes);
        for(unsigned int j=0; j<nbLignes; j++){
            depart->setItem(i,j,new QTableWidgetItem(""));
            depart->item(i,j)->setBackgroundColor("white");
            depart->item(i,j)->setTextColor("white");
        }
    }

    connect(depart,SIGNAL(clicked(const QModelIndex& )),this,SLOT(cellActivation(const QModelIndex)));

    simulationButtons = new QHBoxLayout;

    boutonChoix = new QPushButton("Menu",this);
    simulationButtons->addWidget(boutonChoix);
    simulation =     new QPushButton("Simulation",this);
    simulationButtons->addWidget(simulation);
    simulationStart = new QPushButton("Start",this);
    simulationButtons->addWidget(simulationStart);
    simulationPause = new QPushButton("Pause",this);
    simulationButtons->addWidget(simulationPause);
    restaurer = new QPushButton("Restaurer",this);
    simulationButtons->addWidget(restaurer);
    raz = new QPushButton("Reset",this);
    simulationButtons->addWidget(raz);

    connect(boutonChoix,SIGNAL(clicked()),this,SLOT(retourMenu()));
    connect(simulation,SIGNAL(clicked()),this,SLOT(faireSimulation()));
    connect(simulationStart,SIGNAL(clicked()),this,SLOT(faireSimulationStart()));
    connect(simulationPause,SIGNAL(clicked()),this,SLOT(faireSimulationPause()));
    connect(restaurer,SIGNAL(clicked()),this,SLOT(restaurer2D()));
    connect(raz, SIGNAL(clicked()),this,SLOT(resetEtat()));

    couche = new QVBoxLayout;
    couche->addLayout(configuration);
    couche->addWidget(line);
    couche->addLayout(liste_boutons);
    couche->addWidget(depart);
    couche->addLayout(simulationButtons);

    setLayout(couche);
}

void AutoCell::cellActivation(const QModelIndex& index){
    if (depart->item(index.row(),index.column())->text()==""){
        //elle est désactivée -> il faut l'activer
        depart->item(index.row(),index.column())->setText("_");
        depart->item(index.row(),index.column())->setBackgroundColor("black");
        depart->item(index.row(),index.column())->setTextColor("black");
    }else{
        //elle est activée -> il faut la désactiver
        depart->item(index.row(),index.column())->setText("");
        depart->item(index.row(),index.column())->setBackgroundColor("white");
        depart->item(index.row(),index.column())->setTextColor("white");
    }
}

void AutoCell::faireSimulation(){
    Etat2D e(nbCol,nbLignes);
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            if (depart->item(i,j)->text()!="")
                e.setCellule(i,j,true);
        }
    }
    const Automate2D* A= new Automate2D(s_nbMin->value(),s_nbMax->value(),n_nbMin->value(),n_nbMax->value());
    Simulateur<Automate2D,Etat2D> S(*A,e);
    S.next();
    const Etat2D& d=S.dernier();
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            if (d.getCellule(i,j)){
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("black");
                depart->item(i,j)->setTextColor("black");
            }else{
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("white");
                depart->item(i,j)->setTextColor("white");
            }
        }
        if(i==nbCol-1) sauvegarder2D(d,0);
    }
}

void AutoCell::changeTaille(int a) {
    if(depart == nullptr) throw "depart must be defined before";
    else
    {
        nbCol=a;
        nbLignes=a;
        depart->setFixedSize(size-size%a,size-size%a);
        depart->setColumnCount(a);
        depart->setRowCount(a);
        for(int i=0;i<a;i++){
            for(int j=0;j<a;j++){
                if(depart->item(i,j) == nullptr){
                    QTableWidgetItem* a = new QTableWidgetItem ("");
                    a->setFlags(Qt::NoItemFlags);
                    a->setFlags(Qt::ItemIsEnabled);
                    depart->setItem(i, j, a);
                }
            }
            depart->setColumnWidth(i,size/nbCol);
            depart->setRowHeight(i,size/nbLignes);
        }
    }
}

void AutoCell::remplissageAleatoire() {
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            int v = std::rand() % 2;
            if(v){
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("black");
                depart->item(i,j)->setTextColor("black");
            }else {
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("white");
                depart->item(i,j)->setTextColor("white");
            }
        }
    }
}

void AutoCell::changeSpeed(int i){
    interval = i;
    AutoCell::timer->setInterval(interval);
}

void AutoCell::faireSimulationStart(){
    AutoCell::timer->start(interval);
}
void AutoCell::faireSimulationPause(){
    AutoCell::timer->stop();
}

void AutoCell::resetEtat(){
    s_nbMin->setValue(2);
    s_nbMax->setValue(3);
    n_nbMin->setValue(3);
    n_nbMax->setValue(3);
    AutoCell::timer->stop();
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            depart->item(i,j)->setText("");
            depart->item(i,j)->setBackgroundColor("white");
            depart->item(i,j)->setTextColor("white");
        }
    }
}

void AutoCell::updateMinMax(){
    s_nbMin->setRange(0,s_nbMax->value());
    s_nbMax->setRange(s_nbMin->value(),8);
    n_nbMin->setRange(0,n_nbMax->value());
    n_nbMax->setRange(n_nbMin->value(),8);
}

void AutoCell::sauvegarder2D(const Etat2D& e, int nb){
    QDomDocument sauvegarde;
    QDomElement root = sauvegarde.createElement("sauvegarde");
    sauvegarde.appendChild(root);

    QDomElement automate = sauvegarde.createElement("automate");
    automate.setAttribute("grilletaille", grilletaille->value());
    automate.setAttribute("min_survie",s_nbMin->value());
    automate.setAttribute("max_survie",s_nbMax->value());
    automate.setAttribute("min_naissance",n_nbMin->value());
    automate.setAttribute("max_naissance",n_nbMax->value());
    root.appendChild(automate);

    QDomElement etat = sauvegarde.createElement("etat");
    etat.setAttribute("nbCol",nbCol);
    etat.setAttribute("nbLignes",nbLignes);
    root.appendChild(etat);

    for (unsigned int i=0; i<nbCol;++i){
        QDomElement col = sauvegarde.createElement("ligne");
        col.setAttribute("num",i);
        for(unsigned int j=0; j<nbLignes;++j){
            QDomElement cellule = sauvegarde.createElement("col");
            cellule.setAttribute("num",j);
            cellule.setAttribute("value",e.getCellule(i,j));
            col.appendChild(cellule);

        }
        etat.appendChild(col);
    }

    std::string path = "../Code/Sauvegardes/jeudelavie_";
    std::string num = std::to_string(nb);
    std::string extension = ".xml";

    QString doc = QString::fromStdString(path)+QString::fromStdString(num)+QString::fromStdString(extension);
    QFile file(doc);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() <<"Open the file for writing failed";
    }
    QTextStream stream(&file);
    stream<<sauvegarde.toString();
    file.close();
}

void AutoCell::restaurer2D(int nb){
    QDomDocument dom("xml");
    std::string path;
    if(nb>=10) path = "../Code/Sauvegardes/jeudelavie_config_";
    else path = "../Code/Sauvegardes/jeudelavie_";
    std::string num;
    if(nb>=10){
        nb=nb%10;
    }
    num = std::to_string(nb);
    std::string extension = ".xml";

    QString doc = QString::fromStdString(path)+QString::fromStdString(num)+QString::fromStdString(extension);
    QFile xml_doc(doc);

    if(!xml_doc.open(QIODevice::ReadOnly))
    {
        qDebug() << "Erreur à l'ouverture du fichier";
        return;
    }
    if(!dom.setContent(&xml_doc)){
        xml_doc.close();
        qDebug() << "Impossible d'ouvrir le fichier XML";
        return;
    }
    QDomElement dom_element = dom.documentElement();
    QDomNode n = dom_element.firstChild();
    QDomElement element = n.toElement();
    grilletaille->setValue(element.attribute("grilletaille").toUInt());
    s_nbMin->setValue(element.attribute("min_survie").toInt());
    s_nbMax->setValue(element.attribute("max_survie").toInt());
    n_nbMin->setValue(element.attribute("min_naissance").toInt());
    n_nbMax->setValue(element.attribute("max_naissance").toInt());

    n=n.nextSibling();
    QDomElement etat;
    etat = n.toElement();
    QDomNode col = etat.firstChild();
    for(unsigned int i=0; i<(unsigned int)grilletaille->value();++i){
        QDomNode n_cel = col.firstChild();
        for(unsigned int j=0; j<(unsigned int)grilletaille->value();++j){
            QDomElement e_cel = n_cel.toElement();
            if(e_cel.attribute("value").toUInt()){
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("black");
                depart->item(i,j)->setTextColor("black");
            }else {
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("white");
                depart->item(i,j)->setTextColor("white");
            }
            n_cel = n_cel.nextSibling();
        }

        col = col.nextSibling();
    }
}

void AutoCell::retourMenu() {
    this->deleteLater();
    Choix::getInstance().show();
}


AutoCellFeu::AutoCellFeu(QWidget* parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(faireSimulation()));

    configuration = new QHBoxLayout;

    aleatoire = new QPushButton("Remplir",this);
    aleat = new QLabel("Remplissage Aléatoire");
    connect(aleatoire,SIGNAL(clicked()),this,SLOT(remplissageAleatoire()));

    aleatc = new QVBoxLayout;
    aleatc->addWidget(aleat);
    aleatc->addWidget(aleatoire);

    grilletaille = new QSpinBox(this);
    grilletaille->setRange(1,150);
    grilletaille->setValue(nbCol);
    grillet = new QLabel("Taille de la grille",this);
    connect(grilletaille,SIGNAL(valueChanged(int)),this,SLOT(changeTaille(int)));

    grillec = new QVBoxLayout;
    grillec->addWidget(grillet);
    grillec->addWidget(grilletaille);

    speedSimulation = new QSpinBox(this);
    speedSimulation->setRange(interval/10,interval*2);
    speedSimulation->setValue(interval);
    speedSimulation->setSingleStep(interval/10);
    speedSim = new QLabel("Vitesse");
    connect(speedSimulation,SIGNAL(valueChanged(int)),this,SLOT(changeSpeed(int)));

    speedSimc = new QVBoxLayout;
    speedSimc->addWidget(speedSim);
    speedSimc->addWidget(speedSimulation);

    configuration->addLayout(grillec);
    configuration->addLayout(aleatc);
    configuration->addLayout(speedSimc);


    depart = new QTableWidget(nbCol,nbLignes,this);
    depart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    depart->setFocusPolicy(Qt::NoFocus);
    depart->setSelectionMode(QAbstractItemView::NoSelection);
    depart->setShowGrid(false);
    depart->setFixedSize(size-size%nbCol,size-size%nbLignes);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);

    for(unsigned int i=0; i<nbCol; i++){

        for(unsigned int j=0; j<nbLignes; j++){
            depart->setColumnWidth(i,size/nbCol);
            depart->setRowHeight(i,size/nbLignes);
            depart->setItem(i,j,new QTableWidgetItem("="));
            depart->item(i,j)->setBackgroundColor("green");
            depart->item(i,j)->setTextColor("green");
        }
    }

    connect(depart,SIGNAL(clicked(const QModelIndex& )),this,SLOT(cellActivation(const QModelIndex)));

    simulationButtons = new QHBoxLayout;

    boutonChoix = new QPushButton("Menu",this);
    simulationButtons->addWidget(boutonChoix);
    simulation = new QPushButton("Simulation",this);
    simulationButtons->addWidget(simulation);
    simulationStart = new QPushButton("Start",this);
    simulationButtons->addWidget(simulationStart);
    simulationPause = new QPushButton("Pause",this);
    simulationButtons->addWidget(simulationPause);
    restaurer = new QPushButton("Restaurer",this);
    simulationButtons->addWidget(restaurer);
    raz = new QPushButton("Reset",this);
    simulationButtons->addWidget(raz);

    connect(simulation,SIGNAL(clicked()),this,SLOT(faireSimulation()));
    connect(simulationStart,SIGNAL(clicked()),this,SLOT(faireSimulationStart()));
    connect(simulationPause,SIGNAL(clicked()),this,SLOT(faireSimulationPause()));
    connect(restaurer,SIGNAL(clicked()),this,SLOT(restaurer2D()));
    connect(raz, SIGNAL(clicked()),this,SLOT(resetEtat()));
    connect(boutonChoix,SIGNAL(clicked()),this,SLOT(retourMenu()));

    couche = new QVBoxLayout;
    couche->addLayout(configuration);
    couche->addWidget(depart);
    couche->addLayout(simulationButtons);

    setLayout(couche);
}

void AutoCellFeu::cellActivation(const QModelIndex& index){
    if (depart->item(index.row(),index.column())->text()==""){
        //elle est désactivée -> il faut l'activer
        depart->item(index.row(),index.column())->setText("_");
        depart->item(index.row(),index.column())->setBackgroundColor("grey");
        depart->item(index.row(),index.column())->setTextColor("grey");
    }else if(depart->item(index.row(),index.column())->text()=="_"){
        depart->item(index.row(),index.column())->setText("-");
        depart->item(index.row(),index.column())->setBackgroundColor("red");
        depart->item(index.row(),index.column())->setTextColor("red");
    }else if(depart->item(index.row(),index.column())->text()=="-"){
        depart->item(index.row(),index.column())->setText("=");
        depart->item(index.row(),index.column())->setBackgroundColor("green");
        depart->item(index.row(),index.column())->setTextColor("green");
    }else if(depart->item(index.row(),index.column())->text()=="="){
        //elle est activée -> il faut la désactiver
        depart->item(index.row(),index.column())->setText("");
        depart->item(index.row(),index.column())->setBackgroundColor("white");
        depart->item(index.row(),index.column())->setTextColor("white");
    }
}

void AutoCellFeu::faireSimulation(){
    Etat2D e(nbCol,nbLignes);
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            if (depart->item(i,j)->text()==""){
                e.setCellule(i,j,0);
            }else if(depart->item(i,j)->text()=="_"){
                e.setCellule(i,j,1);
            }else if(depart->item(i,j)->text()=="-"){
                e.setCellule(i,j,2);
            }else if(depart->item(i,j)->text()=="="){
                e.setCellule(i,j,3);
            }
        }
    }
    const Automate2DFeu* A= new Automate2DFeu;
    Simulateur<Automate2DFeu,Etat2D> S(*A,e,4);
    S.next();
    const Etat2D& d=S.dernier();
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            switch(d.getCellule(i,j)){
            case 1 :
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("grey");
                depart->item(i,j)->setTextColor("grey");
                break;
            case 2 :
                depart->item(i,j)->setText("-");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
                break;
            case 3 :
                depart->item(i,j)->setText("=");
                depart->item(i,j)->setBackgroundColor("green");
                depart->item(i,j)->setTextColor("green");
                break;
            default:
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("white");
                depart->item(i,j)->setTextColor("white");
                break;
            }
        }
        if(i==nbCol-1) sauvegarder2D(d);
    }
}

void AutoCellFeu::changeTaille(int a) {
    if(depart == nullptr) throw "depart must be defined before";
    else
    {
        nbCol=a;
        nbLignes=a;
        depart->setFixedSize(size-size%a,size-size%a);
        depart->setColumnCount(a);
        depart->setRowCount(a);
        depart->setShowGrid(false);
        for(int i=0;i<a;i++){
            for(int j=0;j<a;j++){
                if(depart->item(i,j) == nullptr){
                    QTableWidgetItem* a = new QTableWidgetItem ("");
                    a->setFlags(Qt::NoItemFlags);
                    a->setFlags(Qt::ItemIsEnabled);
                    depart->setItem(i, j, a);
                }
            }
            depart->setColumnWidth(i,size/nbCol);
            depart->setRowHeight(i,size/nbLignes);
        }
    }

}

void AutoCellFeu::remplissageAleatoire() {
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            int v = std::rand() % 3;
            if(v==0){
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("white");
                depart->item(i,j)->setTextColor("white");
            }else {
                depart->item(i,j)->setText("=");
                depart->item(i,j)->setBackgroundColor("green");
                depart->item(i,j)->setTextColor("green");
            }
        }
    }
}

void AutoCellFeu::changeSpeed(int i){
    interval = i;
    AutoCellFeu::timer->setInterval(interval);
}

void AutoCellFeu::faireSimulationStart(){
    AutoCellFeu::timer->start(interval);
}
void AutoCellFeu::faireSimulationPause(){
    AutoCellFeu::timer->stop();
}
void AutoCellFeu::resetEtat(){
    AutoCellFeu::timer->stop();
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            depart->item(i,j)->setText("=");
            depart->item(i,j)->setBackgroundColor("green");
            depart->item(i,j)->setTextColor("green");
        }
    }
}
void AutoCellFeu::sauvegarder2D(const Etat2D& e){
    QDomDocument sauvegarde;
    QDomElement root = sauvegarde.createElement("sauvegarde");
    sauvegarde.appendChild(root);

    QDomElement automate = sauvegarde.createElement("automate");
    automate.setAttribute("grilletaille", grilletaille->value());
    root.appendChild(automate);

    QDomElement etat = sauvegarde.createElement("etat");
    etat.setAttribute("nbCol",nbCol);
    etat.setAttribute("nbLignes",nbLignes);
    root.appendChild(etat);

    for (unsigned int i=0; i<nbCol;++i){
        QDomElement col = sauvegarde.createElement("ligne");
        col.setAttribute("num",i);
        for(unsigned int j=0; j<nbLignes;++j){
            QDomElement cellule = sauvegarde.createElement("col");
            cellule.setAttribute("num",j);
            cellule.setAttribute("value",e.getCellule(i,j));
            col.appendChild(cellule);

        }
        etat.appendChild(col);
    }

    QFile file("../Code/Sauvegardes/feu.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() <<"Open the file for writing failed";
    }
    QTextStream stream(&file);
    stream<<sauvegarde.toString();
    file.close();
    qDebug() << "Sauvegarde effectuée";
}

void AutoCellFeu::restaurer2D(){
    QDomDocument dom("xml");
    QFile xml_doc("../Code/Sauvegardes/feu.xml");

    if(!xml_doc.open(QIODevice::ReadOnly))
    {
        qDebug() << "Erreur à l'ouverture du fichier";
        return;
    }
    if(!dom.setContent(&xml_doc)){
        xml_doc.close();
        qDebug() << "Impossible d'ouvrir le fichier XML";
        return;
    }
    QDomElement dom_element = dom.documentElement();
    QDomNode n = dom_element.firstChild();
    QDomElement element = n.toElement();
    grilletaille->setValue(element.attribute("grilletaille").toUInt());
    n=n.nextSibling();
    QDomElement etat;
    etat = n.toElement();
    QDomNode col = etat.firstChild();
    for(unsigned int i=0; i<(unsigned int)grilletaille->value();++i){
        QDomNode n_cel = col.firstChild();
        for(unsigned int j=0; j<(unsigned int)grilletaille->value();++j){
            QDomElement e_cel = n_cel.toElement();
            switch(e_cel.attribute("value").toUInt()){
            case 1 :
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("grey");
                depart->item(i,j)->setTextColor("grey");
                break;
            case 2 :
                depart->item(i,j)->setText("-");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
                break;
            case 3 :
                depart->item(i,j)->setText("=");
                depart->item(i,j)->setBackgroundColor("green");
                depart->item(i,j)->setTextColor("green");
                break;
            default:
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("white");
                depart->item(i,j)->setTextColor("white");
                break;
            }
            n_cel = n_cel.nextSibling();
        }

        col = col.nextSibling();
    }
}

void AutoCellFeu::retourMenu() {
    this->deleteLater();
    Choix::getInstance().show();
}

AutoCellG::AutoCellG(QWidget* parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(faireSimulation()));

    configuration = new QHBoxLayout;

    aleatoire = new QPushButton("Remplir",this);
    aleat = new QLabel("Remplissage Aléatoire");
    connect(aleatoire,SIGNAL(clicked()),this,SLOT(remplissageAleatoire()));

    aleatc = new QVBoxLayout;
    aleatc->addWidget(aleat);
    aleatc->addWidget(aleatoire);

    grilletaille = new QSpinBox(this);
    grilletaille->setRange(1,200);
    grilletaille->setValue(nbCol);
    grillet = new QLabel("Taille de la grille",this);
    connect(grilletaille,SIGNAL(valueChanged(int)),this,SLOT(changeTaille(int)));

    grillec = new QVBoxLayout;
    grillec->addWidget(grillet);
    grillec->addWidget(grilletaille);

    speedSimulation = new QSpinBox(this);
    speedSimulation->setRange(interval/10,interval*2);
    speedSimulation->setValue(interval);
    speedSimulation->setSingleStep(interval/10);
    speedSim = new QLabel("Vitesse");
    connect(speedSimulation,SIGNAL(valueChanged(int)),this,SLOT(changeSpeed(int)));

    speedSimc = new QVBoxLayout;
    speedSimc->addWidget(speedSim);
    speedSimc->addWidget(speedSimulation);

    configuration->addLayout(grillec);
    configuration->addLayout(aleatc);
    configuration->addLayout(speedSimc);

    depart = new QTableWidget(nbCol,nbLignes,this);
    depart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    depart->setFocusPolicy(Qt::NoFocus);
    depart->setSelectionMode(QAbstractItemView::NoSelection);
    depart->setShowGrid(false);
    depart->setFixedSize(size-size%nbCol,size-size%nbLignes);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);

    for(unsigned int i=0; i<nbCol; i++){

        depart->setRowHeight(i,size/nbLignes);
        for(unsigned int j=0; j<nbLignes; j++){
            depart->setColumnWidth(j,size/nbLignes);
            depart->setItem(i,j,new QTableWidgetItem("="));
            depart->item(i,j)->setBackgroundColor("magenta");
            depart->item(i,j)->setTextColor("magenta");
        }
    }

    connect(depart,SIGNAL(clicked(const QModelIndex& )),this,SLOT(cellActivation(const QModelIndex)));

    simulationButtons = new QHBoxLayout;

    boutonChoix = new QPushButton("Menu",this);
    simulationButtons->addWidget(boutonChoix);
    simulation = new QPushButton("Simulation",this);
    simulationButtons->addWidget(simulation);
    simulationStart = new QPushButton("Start",this);
    simulationButtons->addWidget(simulationStart);
    simulationPause = new QPushButton("Pause",this);
    simulationButtons->addWidget(simulationPause);
    restaurer = new QPushButton("Restaurer",this);
    simulationButtons->addWidget(restaurer);
    raz = new QPushButton("Reset",this);
    simulationButtons->addWidget(raz);

    connect(simulation,SIGNAL(clicked()),this,SLOT(faireSimulation()));
    connect(simulationStart,SIGNAL(clicked()),this,SLOT(faireSimulationStart()));
    connect(simulationPause,SIGNAL(clicked()),this,SLOT(faireSimulationPause()));
    connect(restaurer,SIGNAL(clicked()),this,SLOT(restaurer2D()));
    connect(raz, SIGNAL(clicked()),this,SLOT(resetEtat()));
    connect(boutonChoix,SIGNAL(clicked()),this,SLOT(retourMenu()));

    couche = new QVBoxLayout;
    couche->addLayout(configuration);
    couche->addWidget(depart);
    couche->addLayout(simulationButtons);

    setLayout(couche);
}

void AutoCellG::cellActivation(const QModelIndex& index){
    if (depart->item(index.row(),index.column())->text()==""){
        //elle est désactivée -> il faut l'activer
        depart->item(index.row(),index.column())->setText("_");
        depart->item(index.row(),index.column())->setBackgroundColor("darkRed");
        depart->item(index.row(),index.column())->setTextColor("darkRed");
    }else if(depart->item(index.row(),index.column())->text()=="_"){
        depart->item(index.row(),index.column())->setText("-");
        depart->item(index.row(),index.column())->setBackgroundColor("darkMagenta");
        depart->item(index.row(),index.column())->setTextColor("darkMagenta");
    }else if(depart->item(index.row(),index.column())->text()=="-"){
        depart->item(index.row(),index.column())->setText("=");
        depart->item(index.row(),index.column())->setBackgroundColor("magenta");
        depart->item(index.row(),index.column())->setTextColor("magenta");
    }else if(depart->item(index.row(),index.column())->text()=="="){
        //elle est activée -> il faut la désactiver
        depart->item(index.row(),index.column())->setText("");
        depart->item(index.row(),index.column())->setBackgroundColor("red");
        depart->item(index.row(),index.column())->setTextColor("red");
    }
}

void AutoCellG::faireSimulation(){
    Etat2D e(nbCol,nbLignes);
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            if (depart->item(i,j)->text()==""){
                e.setCellule(i,j,0);
            }else if(depart->item(i,j)->text()=="_"){
                e.setCellule(i,j,1);
            }else if(depart->item(i,j)->text()=="-"){
                e.setCellule(i,j,2);
            }else if(depart->item(i,j)->text()=="="){
                e.setCellule(i,j,3);
            }
        }
    }
    const Automate2DG* A= new Automate2DG;
    Simulateur<Automate2DG,Etat2D> S(*A,e,4);
    S.next();
    const Etat2D& d=S.dernier();
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            switch(d.getCellule(i,j)){
            case 1 :
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("darkRed");
                depart->item(i,j)->setTextColor("darkRed");
                break;
            case 2 :
                depart->item(i,j)->setText("-");
                depart->item(i,j)->setBackgroundColor("darkMagenta");
                depart->item(i,j)->setTextColor("darkMagenta");
                break;
            case 3 :
                depart->item(i,j)->setText("=");
                depart->item(i,j)->setBackgroundColor("magenta");
                depart->item(i,j)->setTextColor("magenta");
                break;
            case 0 :
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
                break;
            }
        }
        if(i==nbCol-1) sauvegarder2D(d);
    }
}

void AutoCellG::changeTaille(int a) {
    if(depart == nullptr) throw "depart must be defined before";
    else
    {
        nbCol=a;
        nbLignes=a;
        depart->setFixedSize(size-size%a,size-size%a);
        depart->setColumnCount(a);
        depart->setRowCount(a);
        depart->setShowGrid(false);
        for(int i=0;i<a;i++){
            for(int j=0;j<a;j++){
                if(depart->item(i,j) == nullptr){
                    QTableWidgetItem* a = new QTableWidgetItem ("");
                    a->setFlags(Qt::NoItemFlags);
                    a->setFlags(Qt::ItemIsEnabled);
                    depart->setItem(i, j, a);
                }
            }
            depart->setColumnWidth(i,size/nbLignes);
            depart->setRowHeight(i,size/nbCol);
        }
    }
}

void AutoCellG::remplissageAleatoire() {
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            int v = std::rand() % 4;
            if(v==0){
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
            }else if(v==3) {
                depart->item(i,j)->setText("=");
                depart->item(i,j)->setBackgroundColor("magenta");
                depart->item(i,j)->setTextColor("magenta");
            }else if(v==2) {
                depart->item(i,j)->setText("-");
                depart->item(i,j)->setBackgroundColor("darkMagenta");
                depart->item(i,j)->setTextColor("darkMagenta");
            }else if(v==1) {
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("darkRed");
                depart->item(i,j)->setTextColor("darkRed");
            }
        }
    }
}

void AutoCellG::changeSpeed(int i){
    interval = i;
    AutoCellG::timer->setInterval(interval);
}

void AutoCellG::faireSimulationStart(){
    AutoCellG::timer->start(interval);
}
void AutoCellG::faireSimulationPause(){
    AutoCellG::timer->stop();
}
void AutoCellG::resetEtat(){
    AutoCellG::timer->stop();
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            depart->item(i,j)->setText("");
            depart->item(i,j)->setBackgroundColor("red");
            depart->item(i,j)->setTextColor("red");
        }
    }
}
void AutoCellG::sauvegarder2D(const Etat2D& e){
    QDomDocument sauvegarde;
    QDomElement root = sauvegarde.createElement("sauvegarde");
    sauvegarde.appendChild(root);

    QDomElement automate = sauvegarde.createElement("automate");
    automate.setAttribute("grilletaille", grilletaille->value());
    root.appendChild(automate);

    QDomElement etat = sauvegarde.createElement("etat");
    etat.setAttribute("nbCol",nbCol);
    etat.setAttribute("nbLignes",nbLignes);
    root.appendChild(etat);

    for (unsigned int i=0; i<nbCol;++i){
        QDomElement col = sauvegarde.createElement("ligne");
        col.setAttribute("num",i);
        for(unsigned int j=0; j<nbLignes;++j){
            QDomElement cellule = sauvegarde.createElement("col");
            cellule.setAttribute("num",j);
            cellule.setAttribute("value",e.getCellule(i,j));
            col.appendChild(cellule);

        }
        etat.appendChild(col);
    }

    QFile file("../Code/Sauvegardes/G.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() <<"Open the file for writing failed";
    }
    QTextStream stream(&file);
    stream<<sauvegarde.toString();
    file.close();
    qDebug() << "Sauvegarde effectuée";
}

void AutoCellG::restaurer2D(){
    QDomDocument dom("xml");
    QFile xml_doc("../Code/Sauvegardes/G.xml");

    if(!xml_doc.open(QIODevice::ReadOnly))
    {
        qDebug() << "Erreur à l'ouverture du fichier";
        return;
    }
    if(!dom.setContent(&xml_doc)){
        xml_doc.close();
        qDebug() << "Impossible d'ouvrir le fichier XML";
        return;
    }
    QDomElement dom_element = dom.documentElement();
    QDomNode n = dom_element.firstChild();
    QDomElement element = n.toElement();
    grilletaille->setValue(element.attribute("grilletaille").toUInt());
    n=n.nextSibling();
    QDomElement etat;
    etat = n.toElement();
    QDomNode col = etat.firstChild();
    for(unsigned int i=0; i<(unsigned int)grilletaille->value();++i){
        QDomNode n_cel = col.firstChild();
        for(unsigned int j=0; j<(unsigned int)grilletaille->value();++j){
            QDomElement e_cel = n_cel.toElement();
            switch(e_cel.attribute("value").toUInt()){
            case 1 :
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("darkRed");
                depart->item(i,j)->setTextColor("darkRed");
                break;
            case 2 :
                depart->item(i,j)->setText("-");
                depart->item(i,j)->setBackgroundColor("darkMagenta");
                depart->item(i,j)->setTextColor("darkMagenta");
                break;
            case 3 :
                depart->item(i,j)->setText("=");
                depart->item(i,j)->setBackgroundColor("magenta");
                depart->item(i,j)->setTextColor("magenta");
                break;
            case 0 :
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
                break;
            }
            n_cel = n_cel.nextSibling();
        }

        col = col.nextSibling();
    }
}

void AutoCellG::retourMenu() {
    this->deleteLater();
    Choix::getInstance().show();
}


/* brian */

AutoCellBrian::AutoCellBrian(QWidget* parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(faireSimulation()));

    configuration = new QHBoxLayout;

    aleatoire = new QPushButton("Remplir",this);
    aleat = new QLabel("Remplissage Aléatoire");
    connect(aleatoire,SIGNAL(clicked()),this,SLOT(remplissageAleatoire()));

    aleatc = new QVBoxLayout;
    aleatc->addWidget(aleat);
    aleatc->addWidget(aleatoire);

    grilletaille = new QSpinBox(this);
    grilletaille->setRange(1,200);
    grilletaille->setValue(nbCol);
    grillet = new QLabel("Taille de la grille",this);
    connect(grilletaille,SIGNAL(valueChanged(int)),this,SLOT(changeTaille(int)));

    grillec = new QVBoxLayout;
    grillec->addWidget(grillet);
    grillec->addWidget(grilletaille);

    speedSimulation = new QSpinBox(this);
    speedSimulation->setRange(interval/10,interval*2);
    speedSimulation->setValue(interval);
    speedSimulation->setSingleStep(interval/10);
    speedSim = new QLabel("Vitesse");
    connect(speedSimulation,SIGNAL(valueChanged(int)),this,SLOT(changeSpeed(int)));

    speedSimc = new QVBoxLayout;
    speedSimc->addWidget(speedSim);
    speedSimc->addWidget(speedSimulation);

    configuration->addLayout(grillec);
    configuration->addLayout(aleatc);
    configuration->addLayout(speedSimc);

    depart = new QTableWidget(nbCol,nbLignes,this);
    depart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    depart->setFocusPolicy(Qt::NoFocus);
    depart->setSelectionMode(QAbstractItemView::NoSelection);
    depart->setShowGrid(false);
    depart->setFixedSize(size-size%nbCol,size-size%nbLignes);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);

    for(unsigned int i=0; i<nbCol; i++){

        depart->setRowHeight(i,size/nbLignes);
        for(unsigned int j=0; j<nbLignes; j++){
            depart->setColumnWidth(j,size/nbLignes);
            depart->setItem(i,j,new QTableWidgetItem(""));
            depart->item(i,j)->setBackgroundColor("black");
            depart->item(i,j)->setTextColor("black");
        }
    }

    connect(depart,SIGNAL(clicked(const QModelIndex& )),this,SLOT(cellActivation(const QModelIndex)));

    simulationButtons = new QHBoxLayout;

    boutonChoix = new QPushButton("Menu",this);
    simulationButtons->addWidget(boutonChoix);
    simulation = new QPushButton("Simulation",this);
    simulationButtons->addWidget(simulation);
    simulationStart = new QPushButton("Start",this);
    simulationButtons->addWidget(simulationStart);
    simulationPause = new QPushButton("Pause",this);
    simulationButtons->addWidget(simulationPause);
    restaurer = new QPushButton("Restaurer",this);
    simulationButtons->addWidget(restaurer);
    raz = new QPushButton("Reset",this);
    simulationButtons->addWidget(raz);

    connect(simulation,SIGNAL(clicked()),this,SLOT(faireSimulation()));
    connect(simulationStart,SIGNAL(clicked()),this,SLOT(faireSimulationStart()));
    connect(simulationPause,SIGNAL(clicked()),this,SLOT(faireSimulationPause()));
    connect(restaurer,SIGNAL(clicked()),this,SLOT(restaurer2D()));
    connect(raz, SIGNAL(clicked()),this,SLOT(resetEtat()));
    connect(boutonChoix,SIGNAL(clicked()),this,SLOT(retourMenu()));

    couche = new QVBoxLayout;
    couche->addLayout(configuration);
    couche->addWidget(depart);
    couche->addLayout(simulationButtons);

    setLayout(couche);
}

void AutoCellBrian::cellActivation(const QModelIndex& index){
    if (depart->item(index.row(),index.column())->text()==""){
        //elle est désactivée -> il faut l'activer
        depart->item(index.row(),index.column())->setText("_");
        depart->item(index.row(),index.column())->setBackgroundColor("red");
        depart->item(index.row(),index.column())->setTextColor("red");
    }else if(depart->item(index.row(),index.column())->text()=="_"){
        depart->item(index.row(),index.column())->setText("-");
        depart->item(index.row(),index.column())->setBackgroundColor("yellow");
        depart->item(index.row(),index.column())->setTextColor("yellow");
    }else if(depart->item(index.row(),index.column())->text()=="-"){
        //elle est activée -> il faut la désactiver
        depart->item(index.row(),index.column())->setText("");
        depart->item(index.row(),index.column())->setBackgroundColor("black");
        depart->item(index.row(),index.column())->setTextColor("black");
    }
}

void AutoCellBrian::faireSimulation(){
    Etat2D e(nbCol,nbLignes);
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            if (depart->item(i,j)->text()==""){
                e.setCellule(i,j,0);
            }else if(depart->item(i,j)->text()=="_"){
                e.setCellule(i,j,1);
            }else if(depart->item(i,j)->text()=="-"){
                e.setCellule(i,j,2);
            }
        }
    }
    const Automate2DBrian* A= new Automate2DBrian;
    Simulateur<Automate2DBrian,Etat2D> S(*A,e,3);
    S.next();
    const Etat2D& d=S.dernier();
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            switch(d.getCellule(i,j)){
            case 1 :
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
                break;
            case 2 :
                depart->item(i,j)->setText("-");
                depart->item(i,j)->setBackgroundColor("yellow");
                depart->item(i,j)->setTextColor("yellow");
                break;
            case 0 :
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("black");
                depart->item(i,j)->setTextColor("black");
                break;
            }
        }
        if(i==nbCol-1) sauvegarder2D(d);
    }
}

void AutoCellBrian::changeTaille(int a) {
    if(depart == nullptr) throw "depart must be defined before";
    else
    {
        nbCol=a;
        nbLignes=a;
        depart->setFixedSize(size-size%a,size-size%a);
        depart->setColumnCount(a);
        depart->setRowCount(a);
        depart->setShowGrid(false);
        for(int i=0;i<a;i++){
            for(int j=0;j<a;j++){
                if(depart->item(i,j) == nullptr){
                    QTableWidgetItem* a = new QTableWidgetItem ("");
                    a->setFlags(Qt::NoItemFlags);
                    a->setFlags(Qt::ItemIsEnabled);
                    depart->setItem(i, j, a);
                    depart->item(i,j)->setBackgroundColor("black");
                }
            }
            depart->setColumnWidth(i,size/nbLignes);
            depart->setRowHeight(i,size/nbCol);
        }
    }
}

void AutoCellBrian::remplissageAleatoire() {
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            int v = std::rand() % 3;
            if(v==0){
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("black");
                depart->item(i,j)->setTextColor("black");
            }else if(v==2) {
                depart->item(i,j)->setText("-");
                depart->item(i,j)->setBackgroundColor("yellow");
                depart->item(i,j)->setTextColor("yellow");
            }else if(v==1) {
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
            }
        }
    }
}

void AutoCellBrian::changeSpeed(int i){
    interval = i;
    AutoCellBrian::timer->setInterval(interval);
}

void AutoCellBrian::faireSimulationStart(){
    AutoCellBrian::timer->start(interval);
}
void AutoCellBrian::faireSimulationPause(){
    AutoCellBrian::timer->stop();
}
void AutoCellBrian::resetEtat(){
    AutoCellBrian::timer->stop();
    for(unsigned int i=0; i<nbCol; i++){
        for(unsigned int j=0; j<nbLignes; j++){
            depart->item(i,j)->setText("");
            depart->item(i,j)->setBackgroundColor("black");
            depart->item(i,j)->setTextColor("black");
        }
    }
}
void AutoCellBrian::sauvegarder2D(const Etat2D& e){
    QDomDocument sauvegarde;
    QDomElement root = sauvegarde.createElement("sauvegarde");
    sauvegarde.appendChild(root);

    QDomElement automate = sauvegarde.createElement("automate");
    automate.setAttribute("grilletaille", grilletaille->value());
    root.appendChild(automate);

    QDomElement etat = sauvegarde.createElement("etat");
    etat.setAttribute("nbCol",nbCol);
    etat.setAttribute("nbLignes",nbLignes);
    root.appendChild(etat);

    for (unsigned int i=0; i<nbCol;++i){
        QDomElement col = sauvegarde.createElement("ligne");
        col.setAttribute("num",i);
        for(unsigned int j=0; j<nbLignes;++j){
            QDomElement cellule = sauvegarde.createElement("col");
            cellule.setAttribute("num",j);
            cellule.setAttribute("value",e.getCellule(i,j));
            col.appendChild(cellule);

        }
        etat.appendChild(col);
    }

    QFile file("../Code/Sauvegardes/brian.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() <<"Open the file for writing failed";
    }
    QTextStream stream(&file);
    stream<<sauvegarde.toString();
    file.close();
    qDebug() << "Sauvegarde effectuée";
}

void AutoCellBrian::restaurer2D(){
    QDomDocument dom("xml");
    QFile xml_doc("../Code/Sauvegardes/brian.xml");

    if(!xml_doc.open(QIODevice::ReadOnly))
    {
        qDebug() << "Erreur à l'ouverture du fichier";
        return;
    }
    if(!dom.setContent(&xml_doc)){
        xml_doc.close();
        qDebug() << "Impossible d'ouvrir le fichier XML";
        return;
    }
    QDomElement dom_element = dom.documentElement();
    QDomNode n = dom_element.firstChild();
    QDomElement element = n.toElement();
    grilletaille->setValue(element.attribute("grilletaille").toUInt());
    n=n.nextSibling();
    QDomElement etat;
    etat = n.toElement();
    QDomNode col = etat.firstChild();
    for(unsigned int i=0; i<(unsigned int)grilletaille->value();++i){
        QDomNode n_cel = col.firstChild();
        for(unsigned int j=0; j<(unsigned int)grilletaille->value();++j){
            QDomElement e_cel = n_cel.toElement();
            switch(e_cel.attribute("value").toUInt()){
            case 1 :
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
                break;
            case 2 :
                depart->item(i,j)->setText("-");
                depart->item(i,j)->setBackgroundColor("yellow");
                depart->item(i,j)->setTextColor("yellow");
                break;
            case 0 :
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("black");
                depart->item(i,j)->setTextColor("black");
                break;
            }
            n_cel = n_cel.nextSibling();
        }

        col = col.nextSibling();
    }
}

void AutoCellBrian::retourMenu() {
    this->deleteLater();
    Choix::getInstance().show();
}


//Automate 1D

AutoCell1D::AutoCell1D(QWidget* parent) : QWidget(parent)
{
    configuration = new QHBoxLayout;

    grilletaille = new QSpinBox(this);
    grilletaille->setRange(1,50);
    grilletaille->setValue(nbCol);
    grillet = new QLabel("Taille de la grille",this);
    connect(grilletaille,SIGNAL(valueChanged(int)),this,SLOT(changeTaille(int)));

    grillec = new QVBoxLayout;
    grillec->addWidget(grillet);
    grillec->addWidget(grilletaille);

    configuration->addLayout(grillec);

    num = new QSpinBox(this);
    num->setRange(0,255);
    num->setValue(0);

    numl = new QLabel("Numéro",this);

    numc = new QVBoxLayout;
    numc->addWidget(numl);
    numc->addWidget(num);

    configuration->addLayout(numc);


    zeroOneValidator = new QIntValidator(this);
    zeroOneValidator->setRange(0,1);

    for (unsigned int i=0; i<8; i++){
        numeroBit[i] =  new QLineEdit(this);
        numeroBit[i]->setFixedWidth(20);
        numeroBit[i]->setMaxLength(1);
        numeroBit[i]->setText("0");
        numeroBit[i]->setValidator(zeroOneValidator);

        numeroBitl[i] = new QLabel(this);
        bitc[i]= new QVBoxLayout;
        bitc[i]->addWidget(numeroBitl[i]);
        bitc[i]->addWidget(numeroBit[i]);
        configuration->addLayout(bitc[i]);

        connect(numeroBit[i],SIGNAL(textChanged(QString)),this,SLOT(synchronizeNumBitToNum(QString)));

    }

    regleAleatoirec = new QVBoxLayout;
    regleAleatoirel = new QLabel("Règle",this);
    btnRegleAleatoire = new QPushButton("Aléatoire",this);
    connect(btnRegleAleatoire,SIGNAL(clicked()),this,SLOT(regleAleatoire()));
    regleAleatoirec->addWidget(regleAleatoirel);
    regleAleatoirec->addWidget(btnRegleAleatoire);
    configuration->addLayout(regleAleatoirec);

    aleatc = new QVBoxLayout;
    aleat = new QLabel("Départ",this);
    aleatoire = new QPushButton("Remplir",this);
    connect(aleatoire,SIGNAL(clicked()),this,SLOT(remplissageAleatoire()));
    aleatc->addWidget(aleat);
    aleatc->addWidget(aleatoire);
    configuration->addLayout(aleatc);

    numeroBitl[0]->setText("111");
    numeroBitl[1]->setText("110");
    numeroBitl[2]->setText("101");
    numeroBitl[3]->setText("100");
    numeroBitl[4]->setText("011");
    numeroBitl[5]->setText("010");
    numeroBitl[6]->setText("001");
    numeroBitl[7]->setText("000");

    connect(num,SIGNAL(valueChanged(int)),this,SLOT(synchronizeNumToNumBit(int)));
    depart = new QTableWidget(1,nbCol,this);

    depart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    depart->setFocusPolicy(Qt::NoFocus);
    depart->setSelectionMode(QAbstractItemView::NoSelection);

    depart->setFixedSize(size-size%nbCol,size/nbCol);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);
    depart->setColumnCount(nbCol);


    couche = new QVBoxLayout;
    couche->addLayout(configuration);
    couche->addWidget(depart);

    for (unsigned int i=0; i<nbCol; i++){
        depart->setItem(0,i,new QTableWidgetItem(""));
        depart->item(0,i)->setBackgroundColor("white");
        depart->item(0,i)->setTextColor("white");
        depart->setColumnWidth(i,size/nbCol);
    }

    connect(depart,SIGNAL(clicked(const QModelIndex& )),this,SLOT(cellActivation(const QModelIndex)));

    etats = new QTableWidget(nbCol,nbCol,this);
    etats->setFocusPolicy(Qt::NoFocus);
    etats->setSelectionMode(QAbstractItemView::NoSelection);
    couche->addWidget(etats);
    etats->setFixedSize(size-size%nbCol,size-size%nbCol);

    etats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    etats->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    etats->horizontalHeader()->setVisible(false);
    etats->verticalHeader()->setVisible(false);
    etats->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(unsigned int i=0; i<nbCol; i++){
        etats->setColumnWidth(i,size/nbCol);
        etats->setRowHeight(i,size/nbCol);
        for(unsigned int j=0; j<nbCol; j++){
            etats->setItem(j,i,new QTableWidgetItem(""));
            etats->item(j,i)->setBackgroundColor("white");
            etats->item(j,i)->setTextColor("white");
        }
    }
    simulationButtons = new QHBoxLayout;

    boutonChoix = new QPushButton("Menu",this);
    simulationButtons->addWidget(boutonChoix);
    connect(boutonChoix,SIGNAL(clicked()),this,SLOT(retourMenu()));

    simulation = new QPushButton("Simulation",this);
    simulationButtons->addWidget(simulation);
    connect(simulation,SIGNAL(clicked()),this,SLOT(faireSimulation()));

    restaurer = new QPushButton("Restaurer Dernier",this);
    simulationButtons->addWidget(restaurer);
    connect(restaurer,SIGNAL(clicked()),this,SLOT(restaurer1D()));

    raz = new QPushButton("Reset",this);
    simulationButtons->addWidget(raz);
    connect(raz, SIGNAL(clicked()),this,SLOT(resetEtat()));

    couche->addLayout(simulationButtons);
    setLayout(couche);

}

void AutoCell1D::synchronizeNumToNumBit(int i){
    std::string numbit = NumToNumBit(i);
    for (unsigned int i=0; i<8; i++)
        numeroBit[i]->setText(QString(numbit[i]));
}

void AutoCell1D::synchronizeNumBitToNum(const QString& s){
    if (s=="") return;
    std::string str;
    for(unsigned int i=0; i<8; i++)
        str+=numeroBit[i]->text().toStdString();
    int i = NumBitToNum(str);
    num->setValue(i);
}


void AutoCell1D::remplissageAleatoire() {
    for(unsigned int i = 0; i < nbCol; i ++) {
        int v = std::rand() % 2;
        if(v){
            depart->item(0,i)->setText("_");
            depart->item(0,i)->setBackgroundColor("black");
            depart->item(0,i)->setTextColor("black");
        }else {
            depart->item(0,i)->setText("");
            depart->item(0,i)->setBackgroundColor("white");
            depart->item(0,i)->setTextColor("white");
        }
    }
}

void AutoCell1D::regleAleatoire() {
    int v = std::rand() % 256;
    num->setValue(v);
}

void AutoCell1D::cellActivation(const QModelIndex& index){
    if (depart->item(0,index.column())->text()==""){
        //elle est désactivée -> il faut l'activer
        depart->item(0,index.column())->setText("_");
        depart->item(0,index.column())->setBackgroundColor("black");
        depart->item(0,index.column())->setTextColor("black");
    }else{
        //elle est activée -> il faut la désactiver
        depart->item(0,index.column())->setText("");
        depart->item(0,index.column())->setBackgroundColor("white");
        depart->item(0,index.column())->setTextColor("white");
    }
}

void AutoCell1D::faireSimulation(){
    Etat1D e(nbCol);
    for(unsigned int i=0; i<nbCol; i++){
        if (depart->item(0,i)->text()!="")
            e.setCellule(i,true);
    }
    const Automate1D& A = AutomateManager::getAutomateManager().getAutomate(num->value());
    Simulateur<Automate1D,Etat1D> S(A,e);
    for(unsigned int i=0; i<nbCol; i++){
        S.next();
        const Etat1D& d=S.dernier();
        for(unsigned int j=0; j<nbCol; j++){
            if (d.getCellule(j)){
                etats->item(i,j)->setText("_");
                etats->item(i,j)->setBackgroundColor("black");
                etats->item(i,j)->setTextColor("black");
            }else{
                etats->item(i,j)->setText("");
                etats->item(i,j)->setBackgroundColor("white");
                etats->item(i,j)->setTextColor("white");
            }
        }
        if(i==nbCol-1) sauvegarder1D(d);
    }
}

void AutoCell1D::sauvegarder1D(const Etat1D& e){
    QDomDocument sauvegarde;
    QDomElement root = sauvegarde.createElement("sauvegarde");
    sauvegarde.appendChild(root);

    QDomElement automate = sauvegarde.createElement("automate");
    automate.setAttribute("numero",num->value());
    root.appendChild(automate);

    QDomElement etat = sauvegarde.createElement("etat");
    etat.setAttribute("nbCol",nbCol);
    etat.setAttribute("nbLignes",1);
    root.appendChild(etat);

    for (unsigned int i=0; i<nbCol;++i){
        QDomElement cellule = sauvegarde.createElement("cellule");
        cellule.setAttribute("col",i);
        cellule.setAttribute("value", e.getCellule(i));
        etat.appendChild(cellule);
    }

    QFile file("../Code/Sauvegardes/1D.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() <<"Open the file for writing failed";
    }
    QTextStream stream(&file);
    stream<<sauvegarde.toString();
    file.close();
    qDebug() << "Sauvegarde effectuée";
}

void AutoCell1D::restaurer1D(){
    QDomDocument dom("xml");
    QFile xml_doc("../Code/Sauvegardes/1D.xml");

    if(!xml_doc.open(QIODevice::ReadOnly))
    {
        qDebug() << "Erreur à l'ouverture du fichier";
        return;
    }
    if(!dom.setContent(&xml_doc)){
        xml_doc.close();
        qDebug() << "Impossible d'ouvrir le fichier XML";
        return;
    }

    QDomElement dom_element = dom.documentElement();
    QDomNode n = dom_element.firstChild();
    QDomElement element = n.toElement();
    num->setValue(element.attribute("numero").toUInt());
    n=n.nextSibling();
    QDomElement ligne;
    ligne = n.toElement();
    nbCol = ligne.attribute("nbCol").toUInt();
    grilletaille->setValue(nbCol);
    QDomNode n_cel = ligne.firstChild();
    for(unsigned int i=0; i<nbCol;++i){
        QDomElement e_cel = n_cel.toElement();
        if(e_cel.attribute("value").toUInt()){
            depart->item(0,i)->setText("_");
            depart->item(0,i)->setBackgroundColor("black");
            depart->item(0,i)->setTextColor("black");
        }else {
            depart->item(0,i)->setText("");
            depart->item(0,i)->setBackgroundColor("white");
            depart->item(0,i)->setTextColor("white");
        }

        n_cel = n_cel.nextSibling();


    }
}

void AutoCell1D::resetEtat(){
    num->setValue(0);
    for (unsigned int i=0; i<nbCol; i++){
        depart->item(0,i)->setBackgroundColor("white");
        depart->item(0,i)->setTextColor("white");
        for(unsigned int j=0; j<nbCol; j++){
            etats->item(j,i)->setBackgroundColor("white");
            etats->item(j,i)->setTextColor("white");
        }
    }
}

void AutoCell1D::retourMenu() {
    this->deleteLater();
    Choix::getInstance().show();
}

void AutoCell1D::changeTaille(int a){
    if(depart == nullptr) throw "depart must be defined before";
    else
    {
        nbCol=a;
        depart->setFixedSize(size-size%a,size/a);
        depart->setColumnCount(a);
        etats->setFixedSize(size-size%nbCol,size-size%nbCol);
        etats->setColumnCount(a);
        etats->setRowCount(a);
        for(int i=0;i<a;i++){
            depart->setColumnWidth(i,size/a);
            depart->setRowHeight(i,size/a);
            if(depart->item(0,i) == nullptr){
                QTableWidgetItem* a = new QTableWidgetItem ("");
                a->setFlags(Qt::NoItemFlags);
                a->setFlags(Qt::ItemIsEnabled);
                depart->setItem(0, i, a);
                depart->item(0,i)->setText("");
                depart->item(0,i)->setBackgroundColor("white");
                depart->item(0,i)->setTextColor("white");
            }
        }
        for(unsigned int i=0; i<nbCol; i++){
            etats->setColumnWidth(i,size/nbCol);
            etats->setRowHeight(i,size/nbCol);
            for(unsigned int j=0; j<nbCol; j++){
                if(etats->item(j,i) == nullptr){
                    QTableWidgetItem* a = new QTableWidgetItem ("");
                    a->setFlags(Qt::NoItemFlags);
                    a->setFlags(Qt::ItemIsEnabled);
                    etats->setItem(j, i, a);
                    etats->item(j,i)->setText("");
                    etats->item(j,i)->setBackgroundColor("white");
                    etats->item(j,i)->setTextColor("white");
                }
            }
        }
    }
}
