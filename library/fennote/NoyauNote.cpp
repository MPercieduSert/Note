#include "NoyauNote.h"

using namespace noteMPS;

//NoyauNote::NoyauNote()
//    : AbstractNoyau() {
//    setAnnee();
//}

void NoyauNote::setAnnee(const Annee & an) {
    DonneeCible dnC;
    dnC.setIdDonnee(Donnee::IdProgIdND);
    dnC.setCible(bddMPS::cibleId::Configuration);
    dnC.setIdCible(bddMPS::idProg::AnneeCourante);
    Annee anBdd;
    if(bdd().getUnique(dnC)) {
        anBdd.setId(dnC.valeur().toUInt());
        bdd().get(anBdd);
    }
    if(!an.isNew() && an.id() != anBdd.id()) {
        dnC.setValeur(an.id());
        bdd().save(dnC);
        anBdd.setId(an.id());
        bdd().get(anBdd);
    }
    m_annee = anBdd;
}

void NoyauNote::setConfigByPath(const QString &configPath, QWidget *modalParent)
    {setConfig(std::make_unique<ConfigNote>(configPath), modalParent);}
