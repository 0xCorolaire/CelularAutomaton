#include "choix.h"
#include "autocell.h"

Choix::Choix(QWidget *parent) : QWidget(parent) {

    mode1 = new QPushButton("Automate 1D",this);
    mode1->setFixedSize(150, 50);
    connect(mode1,SIGNAL(clicked()),this,SLOT(goMode1()));

    mode2 = new QPushButton("Jeu de la Vie",this);
    mode2->setFixedSize(150, 50);
    connect(mode2,SIGNAL(clicked()),this,SLOT(goMode2()));
    mode2feu = new QPushButton("Feu de forêt",this);
    mode2feu->setFixedSize(150, 50);
    connect(mode2feu,SIGNAL(clicked()),this,SLOT(goMode2f()));
    mode2griffeath = new QPushButton("Griffeath",this);
    mode2griffeath->setFixedSize(150, 50);
    connect(mode2griffeath,SIGNAL(clicked()),this,SLOT(goMode2g()));
    mode2brian = new QPushButton("Brian's Brain",this);
    mode2brian->setFixedSize(150, 50);
    connect(mode2brian,SIGNAL(clicked()),this,SLOT(goMode2b()));

    layout = new QVBoxLayout;
    layout->addWidget(mode1);
    layout->addWidget(mode2);
    layout->addWidget(mode2feu);
    layout->addWidget(mode2griffeath);
    layout->addWidget(mode2brian);
    setLayout(layout);
}

Choix::~Choix(){}

void Choix::goMode1() {
    AutoCell1D *a = new AutoCell1D();
    a->show();
    this->hide();
}
void Choix::goMode2() {
    AutoCell *a = new AutoCell();
    a->show();
    this->hide();
}
void Choix::goMode2f() {
    AutoCellFeu *a = new AutoCellFeu();
    a->show();
    this->hide();
}
void Choix::goMode2g() {
    AutoCellG *a = new AutoCellG();
    a->show();
    this->hide();
}
void Choix::goMode2b() {
    AutoCellBrian *a = new AutoCellBrian();
    a->show();
    this->hide();
}

Choix::Handler Choix::handler = Handler();

Choix& Choix::getInstance() {
    if (!handler.instance) handler.instance = new Choix;
    return *handler.instance;
}
