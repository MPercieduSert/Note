#include "BddNote.h"

using namespace noteMPS;

bool BddNote::delP(idt id, szt idEntity) {
    auto controle = true;
    // Spécifique
    switch (idEntity) {
    case Niveau::ID:
        controle = delList<Classe>(Classe::IdNiveau,id)
                && delList<FiliationNiveau>(FiliationNiveau::IdPrecedent,id)
                && delList<FiliationNiveau>(FiliationNiveau::IdSuivant,id)
                && delList<EtablissementNiveau>(EtablissementNiveau::IdNiveau,id)
                && delList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdNiveau,id);
        break;
    case TypeEtablissement::ID:
        controle = delList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,id)
                && delList<EtablissementType>(EtablissementType::IdTpEtab,id);
        break;
    }
    return controle && BddPredef::delP(id,idEntity);
}

bool BddNote::getAutorisationP(idt id, szt idEntity, bmps::autorisation autoris) {
    auto controle = bddMPS::BddPredef::getAutorisationP(id,idEntity,autoris);
    if(controle && autoris == bddMPS::autorisation::Suppr) {
        switch (idEntity) {
            case Niveau::ID:
                controle = controle && getAutorisationList<Classe>(autoris,Classe::IdNiveau,id)
                                    && getAutorisationList<FiliationNiveau>(autoris,FiliationNiveau::IdPrecedent,id)
                                    && getAutorisationList<FiliationNiveau>(autoris,FiliationNiveau::IdSuivant,id)
                                    && getAutorisationList<EtablissementNiveau>(autoris,EtablissementNiveau::IdNiveau,id)
                                    && getAutorisationList<NiveauTypeEtablissement>(autoris,NiveauTypeEtablissement::IdNiveau,id);
                break;
            case TypeEtablissement::ID:
                controle = controle && getAutorisationList<NiveauTypeEtablissement>(autoris, NiveauTypeEtablissement::IdTpEtab,id)
                                    && getAutorisationList<EtablissementType>(autoris, EtablissementType::IdTpEtab,id);
                break;
        }
    }
    return controle;
}

void BddNote::listeMiseAJourBdd(int version) {
    BddPredef::listeMiseAJourBdd(version);
    switch (version) {
    case bmps::bddVersion::InitialePredef:
        creerTable<Annee>();
        creerTable<Bareme>();
        creerTable<TypeEtablissement>();
        creerTable<Etablissement>();
        creerTable<EtablissementType>();
        creerTable<Niveau>();
        creerTable<Classe>();
        creerTable<Eleve>();
        creerTable<ClasseEleve>();
        creerTable<Groupe>();
        creerTable<ClasseGroupe>();
        creerTable<TypeControle>();
        creerTable<Controle>();
        creerTable<Enonce>();
        creerTable<Epreuve>();
        creerTable<ControleEpreuve>();
        creerTable<EleveGroupe>();
        creerTable<Point>();
        creerTable<EnoncePoint>();
        creerTable<EtablissementNiveau>();
        creerTable<FiliationNiveau>();
        creerTable<NiveauTypeEtablissement>();
        creerTable<Note>();
        creerTable<Valide>();
        m_manager->saveVersion(bmps::bddVersion::InitialeNote);
    [[clang::fallthrough]];

    case bmps::bddVersion::InitialeNote:
        save(Type(bmps::idProg::EtudeType,"Étude","Étude"));
        m_manager->saveVersion(bmps::bddVersion::IdProg001Note);
    }
}
