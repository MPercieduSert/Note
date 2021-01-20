#include "BddNote.h"

using namespace noteMPS;

flag BddNote::code(idt idEntity, const QString & str) const {
    switch (idEntity) {
    case Controle::ID:
        if(str == "Appreciation")
            return Controle::Appreciation;
        if(str == "Barre")
            return Controle::Barre;
        if(str == "Capacites")
            return Controle::Capacites;
        if(str == "Classement")
            return Controle::Classement;
        if(str == "Commentaire")
            return Controle::Commentaire;
        if(str == "Competences")
            return Controle::Competences;
        if(str == "Courbe")
            return Controle::Courbe;
        if(str == "Depassement")
            return Controle::Depassement;
        if(str == "HoraireEleve")
            return Controle::HoraireEleve;
        if(str == "HoraireGroupe")
            return Controle::HoraireGroupe;
        if(str == "Lettre")
            return Controle::Lettre;
        if(str == "Note")
            return Controle::Note;
        if(str == "SujetEleve")
            return Controle::SujetEleve;
        if(str == "SujetGroupe")
            return Controle::SujetGroupe;
        break;
    case TypeControle::ID:
        if(str == "Appreciation")
            return TypeControle::Appreciation;
        if(str == "AppreciationModifiable")
            return TypeControle::AppreciationModifiable;
        if(str == "Barre")
            return TypeControle::Barre;
        if(str == "BarreModifiable")
            return TypeControle::BarreModifiable;
        if(str == "Capacites")
            return TypeControle::Capacites;
        if(str == "CapacitesModifiable")
            return TypeControle::CapacitesModifiable;
        if(str == "Categorie")
            return TypeControle::Categorie;
        if(str == "Classement")
            return TypeControle::Classement;
        if(str == "ClassementModifiable")
            return TypeControle::ClassementModifiable;
        if(str == "Commentaire")
            return TypeControle::Commentaire;
        if(str == "CommentaireModifiable")
            return TypeControle::CommentaireModifiable;
        if(str == "Competences")
            return TypeControle::Competences;
        if(str == "CompetencesModifiable")
            return TypeControle::CompetencesModifiable;
        if(str == "Courbe")
            return TypeControle::Courbe;
        if(str == "CourbeModifiable")
            return TypeControle::CourbeModifiable;
        if(str == "DecimaleModifiable")
            return TypeControle::DecimaleModifiable;
        if(str == "Depassement")
            return TypeControle::Depassement;
        if(str == "DepassementModifiable")
            return TypeControle::DepassementModifiable;
        if(str == "Lettre")
            return TypeControle::Lettre;
        if(str == "MinimaModifiable")
            return TypeControle::MinimaModifiable;
        if(str == "Note")
            return TypeControle::Note;
        if(str == "NoteModifiable")
            return TypeControle::NoteModifiable;
        if(str == "TotalModifiable")
            return TypeControle::TotalModifiable;
        if(str == "TypeNoteModifiable")
            return TypeControle::TypeNoteModifiable;
        break;
    default:
        return BddPredef::code(idEntity,str);
    }
    return bddMPS::code::Invalide;
}

bool BddNote::delP(idt id, entidt idEntity) {
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
    case TypeControle::ID:
        controle = delList<Controle>(Controle::IdType,id);
        break;
    case TypeEtablissement::ID:
        controle = delList<EtablissementType>(EtablissementType::IdTpEtab,id)
                && delList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,id);
        break;
    }
    controle = controle && delList<Controle>(Controle::Cible,cible(idEntity),
                                             Controle::IdCible,id);
    return controle && BddPredef::delP(id,idEntity);
}

bool BddNote::testAutorisationP(idt id, entidt idEntity, flag autoris) {
    auto controle = bddMPS::BddPredef::testAutorisationP(id,idEntity,autoris);
    if(controle && autoris & bddMPS::Suppr) {
        //Cible
        controle = controle && testAutorisationList<Controle>(autoris,
                                                              Controle::Cible,cible(idEntity),
                                                              Controle::IdCible,id);
        if(controle){
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
            case TypeControle::ID:
                controle = controle && testAutorisationList<Controle>(autoris,Controle::IdType,id);
                break;
            case TypeEtablissement::ID:
                controle = controle && testAutorisationList<NiveauTypeEtablissement>(autoris, NiveauTypeEtablissement::IdTpEtab,id)
                        && testAutorisationList<EtablissementType>(autoris, EtablissementType::IdTpEtab,id);
                break;
            }
        }
    }
    return controle;
}

int BddNote::strToEnum(idt idEntity, const QString & str) const {
    switch (idEntity) {
    case DonneeCard::ID:
        if(str == "NbrDefaultDateClasse")
            return donnee::NbrDefaultDateClasse;
        break;
    }
    return BddPredef::strToEnum(idEntity,str);
}

void BddNote::listeMiseAJourBdd(int version, idt type) {
    BddPredef::listeMiseAJourBdd(version,type);
    if(type == bmps::bddVersion::NoteType) {
        switch (version) {
        case bmps::bddVersion::Initiale: {
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
            m_manager->saveVersion(bmps::bddVersion::Creation,bmps::bddVersion::NoteType);
        }
        [[clang::fallthrough]];
        case bmps::bddVersion::Creation:
            creerTable<TypeControle>();
            m_manager->saveVersion(bmps::bddVersion::TypeControleCreation,bmps::bddVersion::NoteType);
        [[clang::fallthrough]];
        case bmps::bddVersion::TypeControleCreation:
            creerTable<Controle>();
            m_manager->saveVersion(bmps::bddVersion::ControleCreation,bmps::bddVersion::NoteType);
        }
        //creerTable<Bareme>();
        //creerTable<Enonce>();
        //creerTable<Epreuve>();
        //creerTable<ControleEpreuve>();
        //creerTable<Point>();
        //creerTable<EnoncePoint>();
        //creerTable<Note>();
        //creerTable<Valide>();
    }
}
