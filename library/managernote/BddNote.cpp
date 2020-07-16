#include "BddNote.h"

using namespace noteMPS;

bool BddNote::delP(idt id, szt idEntity) {
    auto controle = true;
    // Spécifique
    switch (idEntity) {
    case Annee::ID:
        controle = delList<Classe>(Classe::IdAn,id);
        break;
    case Classe::ID:
        controle = delList<ClasseEleve>(ClasseEleve::IdClasse,id);
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
    case Groupe::ID:
        controle = delList<EleveGroupe>(EleveGroupe::IdGroupe,id);
                //&& delList<ControleEpreuve>(ControleEpreuve::IdGroupe,id);
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
            controle = controle && testAutorisationList<Classe>(autoris,Classe::IdAn,id);
            break;
        case Classe::ID:
            controle = controle && testAutorisationList<ClasseEleve>(autoris,ClasseEleve::IdClasse,id);
                    //&& testAutorisationList<ControleEpreuve>(autoris,ControleEpreuve::IdClasse,id);
            break;
        case Eleve::ID:
            controle = controle && testAutorisationList<ClasseEleve>(autoris,ClasseEleve::IdEleve,id)
                    //&& testAutorisationList<ControleEpreuve>(autoris,ControleEpreuve::IdEleve,id)
                    && testAutorisationList<EleveGroupe>(autoris,EleveGroupe::IdEleve,id)
                    && testAutorisationList<Note>(autoris,Note::IdEleve,id);
            break;
        case Groupe::ID:
            controle = controle //&& testAutorisationList<ControleEpreuve>(autoris,ControleEpreuve::IdGroupe,id)
                    && testAutorisationList<EleveGroupe>(autoris,EleveGroupe::IdGroupe,id);
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

void BddNote::listeMiseAJourBdd(int version, idt type) {
    BddPredef::listeMiseAJourBdd(version,type);
    if(type == bmps::bddVersion::NoteType) {
        switch (version) {
        case bmps::bddVersion::Initiale:
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
            creerTable<Groupe>();
            creerTable<EleveGroupe>();
            Type etudeTp;
            etudeTp.setNom("Étude");
            etudeTp.setNc("Étude");
            etudeTp.setRef("etude_root_tp");
            save(etudeTp,bmps::Suppr);
            TypePermission etudePerm;
            etudePerm.setIdType(etudeTp.id());
            etudePerm.setCible(cible<Niveau>());
            etudePerm.setCode(bmps::code::Visible);
            save(etudePerm,bmps::Suppr);
            DonneeCard dnCard;
            dnCard.setIdDonnee(refToId<Donnee>("date_defaut_dn"));
            dnCard.setCible(cible<Classe>());
            dnCard.setCard(donnee::NbrDefaultDateClasse);
            dnCard.setExact(donnee::Exact);
            save(dnCard);
            Type grType;
            grType.setNom("Groupe");
            grType.setNc("Gr");
            grType.setRef("groupe_root_tp");
            save(grType);
            TypePermission grPerm;
            grPerm.setIdType(grType.id());
            grPerm.setCible(cible<Groupe>());
            grPerm.setCode(bmps::code::Visible);
            save(grPerm);
            m_manager->saveVersion(bmps::bddVersion::Creation,bmps::bddVersion::NoteType);
        }
        //creerTable<Bareme>();
        //creerTable<TypeControle>();
        //creerTable<Controle>();
        //creerTable<Enonce>();
        //creerTable<Epreuve>();
        //creerTable<ControleEpreuve>();
        //creerTable<Point>();
        //creerTable<EnoncePoint>();
        //creerTable<Note>();
        //creerTable<Valide>();
    }
}
