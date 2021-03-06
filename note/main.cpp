#include <QApplication>

#include "FenPrincipaleNote.h"

int main(int argc, char *argv[])
{
    using namespace noteMPS;
    QApplication a(argc, argv);
    auto bdd = std::make_unique<BddNote>();
    auto * zoneCentrale = new ZoneCentraleNote(new TabNote(*bdd, nullptr));
    FenPrincipaleNote fen(new NoyauNote,std::move(bdd),zoneCentrale);
    fen.show();
    return a.exec();
}
