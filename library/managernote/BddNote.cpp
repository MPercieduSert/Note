#include "BddNote.h"

using namespace noteMPS;

bool BddNote::delP(idt id, szt idEntity) {
    auto controle = true;
    // Spécifique
    switch (idEntity) {
    case Annee::ID:
        controle = delList<Classe>(Classe::IdAn,id)
                && delList<ClasseGroupe>(ClasseGroupe::IdAn,id);
        break;
    case Classe::ID:
        controle = delList<ClasseEleve>(ClasseEleve::IdClasse,id)
                && delList<ClasseGroupe>(ClasseGroupe::IdClasse,id)
                && delList<ControleEpreuve>(ControleEpreuve::IdClasse,id);
        break;
    case Eleve::ID:
        controle = delList<ClasseEleve>(ClasseEleve::IdEleve,id)
                && delList<ControleEpreuve>(ControleEpreuve::IdEleve,id)
                && delList<EleveGroupe>(EleveGroupe::IdEleve,id)
                && delList<Note>(Note::IdEleve,id);
        break;
    case Etablissement::ID:
        controle = delList<EtablissementNiveau>(EtablissementNiveau::IdEtab,id)
                && delList<EtablissementType>(EtablissementType::IdEtab,id);
        break;
    case Niveau::ID:
        controle = delList<Classe>(Classe::IdNiveau,id)
                && delList<EtablissementNiveau>(EtablissementNiveau::IdNiveau,id)
                && delList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdNiveau,id)
                && delList<FiliationNiveau>(FiliationNiveau::IdPrecedent,id)
                && delList<FiliationNiveau>(FiliationNiveau::IdSuivant,id);
        break;
    case TypeEtablissement::ID:
        controle = delList<EtablissementType>(EtablissementType::IdTpEtab,id)
                && delList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,id);
        break;
    }
    return controle && BddPredef::delP(id,idEntity);
}

bool BddNote::getAutorisationP(idt id, szt idEntity, bmps::autorisation autoris) {
    auto controle = bddMPS::BddPredef::getAutorisationP(id,idEntity,autoris);
    if(controle && autoris == bddMPS::autorisation::Suppr) {
        switch (idEntity) {
        case Annee::ID:
            controle = controle && getAutorisationList<Classe>(autoris,Classe::IdAn,id)
                    && getAutorisationList<ClasseGroupe>(autoris,ClasseGroupe::IdAn,id);
            break;
        case Classe::ID:
            controle = controle && getAutorisationList<ClasseEleve>(autoris,ClasseEleve::IdClasse,id)
                    && getAutorisationList<ClasseGroupe>(autoris,ClasseGroupe::IdClasse,id)
                    && getAutorisationList<ControleEpreuve>(autoris,ControleEpreuve::IdClasse,id);
            break;
        case Eleve::ID:
            controle = controle && getAutorisationList<ClasseEleve>(autoris,ClasseEleve::IdEleve,id)
                    && getAutorisationList<ControleEpreuve>(autoris,ControleEpreuve::IdEleve,id)
                    && getAutorisationList<EleveGroupe>(autoris,EleveGroupe::IdEleve,id)
                    && getAutorisationList<Note>(autoris,Note::IdEleve,id);
            break;
        case Etablissement::ID:
            controle = controle && getAutorisationList<EtablissementType>(autoris,EtablissementType::IdEtab,id)
                    && getAutorisationList<EtablissementNiveau>(autoris,EtablissementNiveau::Id,id);
            break;
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
