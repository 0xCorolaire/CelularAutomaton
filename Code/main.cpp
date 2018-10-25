#include <QApplication>
// #include "autocell.h"
// #include "automate.h"
// #include "etats.h"
#include "choix.h"
// #include "simulateur.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Choix::getInstance().show();
    return app.exec();

}
