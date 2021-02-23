#include "NoyauNote.h"

using namespace noteMPS;

//NoyauNote::NoyauNote()
//    : abstract_noyau() {
//    setAnnee();
//}

void NoyauNote::setAnnee(const Annee & an) {
    donnee_cible dnC;
    dnC.set_iddonnee(donnee::Id_Prog_IdND);
    dnC.set_cible(b2d::cible_id::Configuration);
    dnC.set_id_cible(b2d::Id_Prog::AnneeCourante);
    Annee anBdd;
    if(bdd().get_unique(dnC)) {
        anBdd.set_id(dnC.valeur().toUInt());
        bdd().get(anBdd);
    }
    if(!an.is_new() && an.id() != anBdd.id()) {
        dnC.setValeur(an.id());
        bdd().save(dnC);
        anBdd.set_id(an.id());
        bdd().get(anBdd);
    }
    m_annee = anBdd;
}

void NoyauNote::set_config_by_path(const QString &config_path, QWidget *modal_parent)
    {set_config(std::make_unique<ConfigNote>(config_path), modal_parent);}
