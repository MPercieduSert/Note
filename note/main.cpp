#include <QApplication>

#include "fen_principale_note.h"

int main(int argc, char *argv[]) {
    using namespace note_mps;
    QApplication a(argc, argv);
    auto bdd = std::make_unique<bdd_note>();
    auto *zone_centrale = new zone_centrale_note(new tab_note(*bdd, nullptr));
    fen_principale_note fen(new noyau_note,std::move(bdd),zone_centrale);
    fen.show();
    return a.exec();
}
