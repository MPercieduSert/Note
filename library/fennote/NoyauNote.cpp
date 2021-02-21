#include "NoyauNote.h"

using namespace noteMPS;

//NoyauNote::NoyauNote()
//    : AbstractNoyau() {
//    setAnnee();
//}

void NoyauNote::setAnnee(const Annee & an) {
    donnee_cible dnC;
    dnC.set_iddonnee(donnee::Id_Prog_IdND);
    dnC.set_cible(bddMPS::cibleId::Configuration);
    dnC.set_id_cible(bddMPS::Id_Prog::AnneeCourante);
    Annee anBdd;
    if(bdd().getUnique(dnC)) {
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

void NoyauNote::setConfigByPath(const QString &configPath, QWidget *modalParent)
    {setConfig(std::make_unique<ConfigNote>(configPath), modalParent);}
