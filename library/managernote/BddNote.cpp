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
        controle = delList<ClasseEleve>(ClasseEleve::IdClasse,id);
                //&& delList<ClasseGroupe>(ClasseGroupe::IdClasse,id)
                //&& delList<ControleEpreuve>(ControleEpreuve::IdClasse,id);
        break;
    case Eleve::ID:
        controle = delList<ClasseEleve>(ClasseEleve::IdEleve,id)
                //&& delList<ControleEpreuve>(ControleEpreuve::IdEleve,id)
                && delList<EleveGroupe>(EleveGroupe::IdEleve,id)
                && delList<Note>(Note::IdEleve,id);
        break;
    case Etablissement::ID:
        controle = delList<EtablissementNiveau>(EtablissementNiveau::IdEtab,id)
                && delList<EtablissementType>(EtablissementType::IdEtab,id)
                && delList<Classe>(Classe::IdEtab,id);
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

bool BddNote::testAutorisationP(idt id, szt idEntity, flag autoris) {
    auto controle = bddMPS::BddPredef::testAutorisationP(id,idEntity,autoris);
    if(controle && autoris & bddMPS::Suppr) {
        switch (idEntity) {
        case Annee::ID:
            controle = controle && testAutorisationList<Classe>(autoris,Classe::IdAn,id)
                    && testAutorisationList<ClasseGroupe>(autoris,ClasseGroupe::IdAn,id);
            break;
        case Classe::ID:
            controle = controle && testAutorisationList<ClasseEleve>(autoris,ClasseEleve::IdClasse,id);
                    //&& testAutorisationList<ClasseGroupe>(autoris,ClasseGroupe::IdClasse,id)
                    //&& testAutorisationList<ControleEpreuve>(autoris,ControleEpreuve::IdClasse,id);
            break;
        case Eleve::ID:
            controle = controle && testAutorisationList<ClasseEleve>(autoris,ClasseEleve::IdEleve,id)
                    //&& testAutorisationList<ControleEpreuve>(autoris,ControleEpreuve::IdEleve,id)
                    && testAutorisationList<EleveGroupe>(autoris,EleveGroupe::IdEleve,id)
                    && testAutorisationList<Note>(autoris,Note::IdEleve,id);
            break;
        case Etablissement::ID:
            controle = controle && testAutorisationList<Classe>(autoris,Classe::IdEtab,id)
                    && testAutorisationList<EtablissementType>(autoris,EtablissementType::IdEtab,id)
                    && testAutorisationList<EtablissementNiveau>(autoris,EtablissementNiveau::Id,id);
            break;
        case Niveau::ID:
            controle = controle && testAutorisationList<Classe>(autoris,Classe::IdNiveau,id)
                    && testAutorisationList<FiliationNiveau>(autoris,FiliationNiveau::IdPrecedent,id)
                    && testAutorisationList<FiliationNiveau>(autoris,FiliationNiveau::IdSuivant,id)
                    && testAutorisationList<EtablissementNiveau>(autoris,EtablissementNiveau::IdNiveau,id)
                    && testAutorisationList<NiveauTypeEtablissement>(autoris,NiveauTypeEtablissement::IdNiveau,id);
            break;
        case TypeEtablissement::ID:
            controle = controle && testAutorisationList<NiveauTypeEtablissement>(autoris, NiveauTypeEtablissement::IdTpEtab,id)
                    && testAutorisationList<EtablissementType>(autoris, EtablissementType::IdTpEtab,id);
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
        creerTable<TypeEtablissement>();
        creerTable<Etablissement>();
        creerTable<EtablissementType>();
        creerTable<Niveau>();
        creerTable<Classe>();
        creerTable<Eleve>();
        creerTable<ClasseEleve>();
        creerTable<EtablissementNiveau>();
        creerTable<FiliationNiveau>();
        creerTable<NiveauTypeEtablissement>();
        m_manager->saveVersion(bmps::bddVersion::InitialeNote);
    [[clang::fallthrough]];

    case bmps::bddVersion::InitialeNote: {
        Type etudeTp;
        etudeTp.setNom("Étude");
        etudeTp.setNc("Étude");
        etudeTp.setIdProg(bmps::idProg::EtudeType);
        save(etudeTp,bmps::Suppr);
        TypePermission etudePerm;
        etudePerm.setIdType(etudeTp.id());
        etudePerm.setCible(cible<Niveau>());
        etudePerm.setCode(bmps::code::Visible);
        save(etudePerm,bmps::Suppr);
        m_manager->saveVersion(bmps::bddVersion::IdProg001Note);
    }
    [[clang::fallthrough]];

    case bmps::bddVersion::IdProg001Note:{
        Type configType;
        configType.setNom("Configuration");
        configType.setNc("config");
        configType.setIdProg(bmps::idProg::ConfigurationType);
        save(configType,bmps::Suppr);
        TypePermission configPerm;
        configPerm.setIdType(configType.id());
        configPerm.setCible(cible<Donnee>());
        configPerm.setCode(bmps::code::Visible);
        save(configPerm);
        Type defValType;
        defValType.setNom("Valeur par defaut");
        defValType.setNc("defaut");
        defValType.setIdProg(bmps::idProg::DefaultValueType);
        defValType.setParent(configType.id());
        save(defValType,bmps::Suppr);
        TypePermission defValPerm;
        defValPerm.setIdType(defValType.id());
        defValPerm.setCible(cible<Donnee>());
        defValPerm.setCode(bmps::code::Visible | bmps::code::Attribuable);
        save(defValPerm);
        conteneurMPS::tree<Donnee> tree;
        auto iter = tree.begin();
        iter->setNom("Configuration");
        iter->setType(configType.id());
        iter->setTpVal(donnee::NoDonnee);
        iter->setIdProg(donnee::ConfigurationId);
        iter = tree.push_back(iter);
        iter->setNom("Valeurs par defaut");
        iter->setType(configType.id());
        iter->setTpVal(donnee::NoDonnee);
        iter->setIdProg(donnee::DefaultValueId);
        iter = tree.push_back(iter);
        iter->setNom("Date par defaut");
        iter->setType(defValType.id());
        iter->setTpVal(donnee::Date);
        iter->setIdProg(donnee::DefaultDateId);
        save(tree,bmps::WithoutDelete);
        DonneeCard dnCard;
        dnCard.setIdDonnee(iter->id());
        dnCard.setCible(cible<Classe>());
        dnCard.setCard(donnee::NbrDefaultDateClasse);
        dnCard.setExact(donnee::Exact);
        save(dnCard);
        m_manager->saveVersion(bmps::bddVersion::IdProg002Note);
    }
    }
    //creerTable<Bareme>();
    //creerTable<Groupe>();
    //creerTable<ClasseGroupe>();
    //creerTable<TypeControle>();
    //creerTable<Controle>();
    //creerTable<Enonce>();
    //creerTable<Epreuve>();
    //creerTable<ControleEpreuve>();
    //creerTable<EleveGroupe>();
    //creerTable<Point>();
    //creerTable<EnoncePoint>();
    //creerTable<Note>();
    //creerTable<Valide>();
}
