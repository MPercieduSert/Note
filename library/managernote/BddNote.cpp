#include "BddNote.h"

using namespace noteMPS;
bool BddNote::del_v(idt id, entidt id_entity) {
    auto controle = true;
    // Spécifique
    switch (id_entity) {
    case Annee::ID:
        controle = del_list<Classe>(Classe::IdAn,id);
        break;
    case Classe::ID:
        controle = del_list<ClasseEleve>(ClasseEleve::IdClasse,id);
                //&& del_list<ControleEpreuve>(ControleEpreuve::IdClasse,id);
        break;
    case Eleve::ID:
        controle = del_list<ClasseEleve>(ClasseEleve::IdEleve,id)
                //&& del_list<ControleEpreuve>(ControleEpreuve::IdEleve,id)
                && del_list<Eleve_groupe>(Eleve_groupe::IdEleve,id)
                && del_list<Note>(Note::IdEleve,id);
        break;
    case Etablissement::ID:
        controle = del_list<EtablissementNiveau>(EtablissementNiveau::IdEtab,id)
                && del_list<EtablissementType>(EtablissementType::IdEtab,id)
                && del_list<Classe>(Classe::IdEtab,id);
        break;
    case Groupe::ID:
        controle = del_list<Eleve_groupe>(Eleve_groupe::Id_groupe,id);
                //&& del_list<ControleEpreuve>(ControleEpreuve::Id_groupe,id);
        break;
    case Niveau::ID:
        controle = del_list<Classe>(Classe::IdNiveau,id)
                && del_list<EtablissementNiveau>(EtablissementNiveau::IdNiveau,id)
                && del_list<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdNiveau,id)
                && del_list<FiliationNiveau>(FiliationNiveau::IdPrecedent,id)
                && del_list<FiliationNiveau>(FiliationNiveau::IdSuivant,id);
        break;
    case TypeControle::ID:
        controle = del_list<Controle>(Controle::id_type,id);
        break;
    case TypeEtablissement::ID:
        controle = del_list<EtablissementType>(EtablissementType::IdTpEtab,id)
                && del_list<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,id);
        break;
    }
    controle = controle && del_list<Controle>(Controle::Cible,cible(id_entity),
                                             Controle::Id_Cible,id);
    return controle && bdd_predef::del_v(id,id_entity);
}

bool BddNote::test_autorisation_v(idt id, entidt id_entity, flag autoris) {
    auto controle = b2d::bdd_predef::test_autorisation_v(id,id_entity,autoris);
    if(controle && autoris & b2d::Suppr) {
        //Cible
        controle = controle && test_autorisation_list<Controle>(autoris,
                                                              Controle::Cible,cible(id_entity),
                                                              Controle::Id_Cible,id);
        if(controle){
            switch (id_entity) {
            case Annee::ID:
                controle = controle && test_autorisation_list<Classe>(autoris,Classe::IdAn,id);
                break;
            case Classe::ID:
                controle = controle && test_autorisation_list<ClasseEleve>(autoris,ClasseEleve::IdClasse,id);
                        //&& test_autorisation_list<ControleEpreuve>(autoris,ControleEpreuve::IdClasse,id);
                break;
            case Eleve::ID:
                controle = controle && test_autorisation_list<ClasseEleve>(autoris,ClasseEleve::IdEleve,id)
                        //&& test_autorisation_list<ControleEpreuve>(autoris,ControleEpreuve::IdEleve,id)
                        && test_autorisation_list<Eleve_groupe>(autoris,Eleve_groupe::IdEleve,id)
                        && test_autorisation_list<Note>(autoris,Note::IdEleve,id);
                break;
            case Groupe::ID:
                controle = controle //&& test_autorisation_list<ControleEpreuve>(autoris,ControleEpreuve::Id_groupe,id)
                        && test_autorisation_list<Eleve_groupe>(autoris,Eleve_groupe::Id_groupe,id);
                break;
            case Etablissement::ID:
                controle = controle && test_autorisation_list<Classe>(autoris,Classe::IdEtab,id)
                        && test_autorisation_list<EtablissementType>(autoris,EtablissementType::IdEtab,id)
                        && test_autorisation_list<EtablissementNiveau>(autoris,EtablissementNiveau::Id,id);
                break;
            case Niveau::ID:
                controle = controle && test_autorisation_list<Classe>(autoris,Classe::IdNiveau,id)
                        && test_autorisation_list<FiliationNiveau>(autoris,FiliationNiveau::IdPrecedent,id)
                        && test_autorisation_list<FiliationNiveau>(autoris,FiliationNiveau::IdSuivant,id)
                        && test_autorisation_list<EtablissementNiveau>(autoris,EtablissementNiveau::IdNiveau,id)
                        && test_autorisation_list<NiveauTypeEtablissement>(autoris,NiveauTypeEtablissement::IdNiveau,id);
                break;
            case TypeControle::ID:
                controle = controle && test_autorisation_list<Controle>(autoris,Controle::id_type,id);
                break;
            case TypeEtablissement::ID:
                controle = controle && test_autorisation_list<NiveauTypeEtablissement>(autoris, NiveauTypeEtablissement::IdTpEtab,id)
                        && test_autorisation_list<EtablissementType>(autoris, EtablissementType::IdTpEtab,id);
                break;
            }
        }
    }
    return controle;
}

enumt BddNote::str_id_to_enum(const QString & str, idt id_entity, QString &controle) const noexcept {
    switch (id_entity) {
    case donnee_card::ID:
        if(str == "NbrDefaultDateClasse")
            return donnee::NbrDefaultDateClasse;
        break;
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
        if(str == "Horaire_groupe")
            return Controle::Horaire_groupe;
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
    }
    return bdd_predef::str_id_to_enum(str,id_entity,controle);
}

void BddNote::listeMiseAJourBdd(int version, idt type) {
    bdd_predef::listeMiseAJourBdd(version,type);
    if(type == b2d::bdd_version::NoteType) {
        switch (version) {
        case b2d::bdd_version::Initiale: {
            creer_table<Annee>();
            creer_table<TypeEtablissement>();
            creer_table<Etablissement>();
            creer_table<EtablissementType>();
            creer_table<Niveau>();
            creer_table<Classe>();
            creer_table<Eleve>();
            creer_table<ClasseEleve>();
            creer_table<EtablissementNiveau>();
            creer_table<FiliationNiveau>();
            creer_table<NiveauTypeEtablissement>();
            creer_table<Groupe>();
            creer_table<Eleve_groupe>();
            creer_table<TypeControle>();
            creer_table<Controle>();
            creer_table<Exercice>();
            m_manager->save_version(b2d::bdd_version::Creation,b2d::bdd_version::NoteType);
        }
        }
        //creer_table<Bareme>();
        //creer_table<Enonce>();
        //creer_table<Epreuve>();
        //creer_table<ControleEpreuve>();
        //creer_table<Point>();
        //creer_table<EnoncePoint>();
        //creer_table<Note>();
        //creer_table<_valide>();
    }
}
