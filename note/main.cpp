#include <QApplication>

#include "FenPrincipaleNote.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    noteMPS::FenPrincipaleNote fen;
    fen.show();
    return a.exec();
}
