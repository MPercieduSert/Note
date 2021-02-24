#include "noyau_note.h"

using namespace note_mps;

void noyau_note::set_annee(const annee & an) {
    donnee_cible dnC;
    dnC.set_id_donnee(donnee::Id_Prog_IdND);
    dnC.set_cible(mps::b2d::cible_id::Configuration);
    dnC.set_id_cible(mps::b2d::Id_Prog::Annee_Courante);
    annee anBdd;
    if(bdd().get_unique(dnC)) {
        anBdd.set_id(dnC.valeur().toUInt());
        bdd().get(anBdd);
    }
    if(!an.is_new() && an.id() != anBdd.id()) {
        dnC.set_valeur(an.id());
        bdd().save(dnC);
        anBdd.set_id(an.id());
        bdd().get(anBdd);
    }
    m_annee = anBdd;
}

void noyau_note::set_config_by_path(const QString &config_path, QWidget *modal_parent)
    {set_config(std::make_unique<config_note>(config_path), modal_parent);}
