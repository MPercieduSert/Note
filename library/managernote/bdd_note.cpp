#include "bdd_note.h"

using namespace note_mps;

bool bdd_note::del_v(idt id, entidt id_entity) {
    auto ctr = true;
    // Spécifique
    switch (id_entity) {
    case annee::ID:
        ctr = del_list<classe>(classe::Id_An,id);
        break;
    case classe::ID:
        ctr = del_list<classe_eleve>(classe_eleve::Id_Classe,id);
                //&& del_list<ctr_epreuve>(controle_epreuve::Id_Classe,id);
        break;
    case eleve::ID:
        ctr = del_list<classe_eleve>(classe_eleve::Id_Eleve,id)
                //&& del_list<controle_epreuve>(controle_epreuve::Id_Eleve,id)
                && del_list<eleve_groupe>(eleve_groupe::Id_Eleve,id)
                && del_list<note>(note::Id_Eleve,id);
        break;
    case etablissement::ID:
        ctr = del_list<etablissement_niveau>(etablissement_niveau::Id_Etab,id)
                && del_list<etablissement_type>(etablissement_type::Id_Etab,id)
                && del_list<classe>(classe::Id_Etab,id);
        break;
    case groupe::ID:
        ctr = del_list<eleve_groupe>(eleve_groupe::Id_Groupe,id);
                //&& del_list<controle_epreuve>(controle_epreuve::Id_Groupe,id);
        break;
    case niveau::ID:
        ctr = del_list<classe>(classe::Id_Niveau,id)
                && del_list<etablissement_niveau>(etablissement_niveau::Id_Niveau,id)
                && del_list<niveau_type_etablissement>(niveau_type_etablissement::Id_Niveau,id)
                && del_list<filiation_niveau>(filiation_niveau::Id_Precedent,id)
                && del_list<filiation_niveau>(filiation_niveau::Id_Suivant,id);
        break;
    case type_controle::ID:
        ctr = del_list<controle>(controle::Id_Type,id);
        break;
    case type_etablissement::ID:
        ctr = del_list<etablissement_type>(etablissement_type::Id_Tp_Etab,id)
                && del_list<niveau_type_etablissement>(niveau_type_etablissement::Id_Tp_Etab,id);
        break;
    }
    ctr = ctr && del_list<controle>(controle::Cible,cible(id_entity),
                                             controle::Id_Cible,id);
    return ctr && bdd_predef::del_v(id,id_entity);
}

bool bdd_note::test_autorisation_v(idt id, entidt id_entity, flag autoris) {
    auto ctr = mps::b2d::bdd_predef::test_autorisation_v(id,id_entity,autoris);
    if(ctr && autoris & mps::b2d::Suppr) {
        //Cible
        ctr = ctr && test_autorisation_list<controle>(autoris,
                                                              controle::Cible,cible(id_entity),
                                                              controle::Id_Cible,id);
        if(ctr){
            switch (id_entity) {
            case annee::ID:
                ctr = ctr && test_autorisation_list<classe>(autoris,classe::Id_An,id);
                break;
            case classe::ID:
                ctr = ctr && test_autorisation_list<classe_eleve>(autoris,classe_eleve::Id_Classe,id);
                        //&& test_autorisation_list<controle_epreuve>(autoris,controle_epreuve::Id_Classe,id);
                break;
            case eleve::ID:
                ctr = ctr && test_autorisation_list<classe_eleve>(autoris,classe_eleve::Id_Eleve,id)
                        //&& test_autorisation_list<controle_epreuve>(autoris,controle_epreuve::Id_Eleve,id)
                        && test_autorisation_list<eleve_groupe>(autoris,eleve_groupe::Id_Eleve,id)
                        && test_autorisation_list<note>(autoris,note::Id_Eleve,id);
                break;
            case groupe::ID:
                ctr = ctr //&& test_autorisation_list<controle_epreuve>(autoris,controle_epreuve::Id_Groupe,id)
                        && test_autorisation_list<eleve_groupe>(autoris,eleve_groupe::Id_Groupe,id);
                break;
            case etablissement::ID:
                ctr = ctr && test_autorisation_list<classe>(autoris,classe::Id_Etab,id)
                        && test_autorisation_list<etablissement_type>(autoris,etablissement_type::Id_Etab,id)
                        && test_autorisation_list<etablissement_niveau>(autoris,etablissement_niveau::Id,id);
                break;
            case niveau::ID:
                ctr = ctr && test_autorisation_list<classe>(autoris,classe::Id_Niveau,id)
                        && test_autorisation_list<filiation_niveau>(autoris,filiation_niveau::Id_Precedent,id)
                        && test_autorisation_list<filiation_niveau>(autoris,filiation_niveau::Id_Suivant,id)
                        && test_autorisation_list<etablissement_niveau>(autoris,etablissement_niveau::Id_Niveau,id)
                        && test_autorisation_list<niveau_type_etablissement>(autoris,niveau_type_etablissement::Id_Niveau,id);
                break;
            case type_controle::ID:
                ctr = ctr && test_autorisation_list<controle>(autoris,controle::Id_Type,id);
                break;
            case type_etablissement::ID:
                ctr = ctr && test_autorisation_list<niveau_type_etablissement>(autoris, niveau_type_etablissement::Id_Tp_Etab,id)
                        && test_autorisation_list<etablissement_type>(autoris, etablissement_type::Id_Tp_Etab,id);
                break;
            }
        }
    }
    return ctr;
}

enumt bdd_note::str_id_to_enum(const QString & str, idt id_entity, QString &ctr) const noexcept {
    switch (id_entity) {
    case donnee_card::ID:
        if(str == "nbr_default_date_classe")
            return donnee_mps::Nbr_Default_Date_Classe;
        break;
    case controle::ID:
        if(str == "appreciation")
            return controle::Appreciation;
        if(str == "barre")
            return controle::Barre;
        if(str == "capacites")
            return controle::Capacites;
        if(str == "classement")
            return controle::Classement;
        if(str == "commentaire")
            return controle::Commentaire;
        if(str == "competences")
            return controle::Competences;
        if(str == "courbe")
            return controle::Courbe;
        if(str == "depassement")
            return controle::Depassement;
        if(str == "horaire_eleve")
            return controle::Horaire_eleve;
        if(str == "horaire_groupe")
            return controle::Horaire_groupe;
        if(str == "lettre")
            return controle::Lettre;
        if(str == "Note")
            return controle::Note;
        if(str == "sujet_eleve")
            return controle::Sujet_eleve;
        if(str == "sujet_groupe")
            return controle::Sujet_Groupe;
        break;
    case type_controle::ID:
        if(str == "appreciation")
            return type_controle::Appreciation;
        if(str == "appreciation_modifiable")
            return type_controle::Appreciation_Modifiable;
        if(str == "barre")
            return type_controle::Barre;
        if(str == "barre_modifiable")
            return type_controle::Barre_Modifiable;
        if(str == "capacites")
            return type_controle::Capacites;
        if(str == "capacites_modifiable")
            return type_controle::Capacites_Modifiable;
        if(str == "categorie")
            return type_controle::Categorie;
        if(str == "classement")
            return type_controle::Classement;
        if(str == "classement_modifiable")
            return type_controle::Classement_Modifiable;
        if(str == "commentaire")
            return type_controle::Commentaire;
        if(str == "commentaire_modifiable")
            return type_controle::Commentaire_Modifiable;
        if(str == "competences")
            return type_controle::Competences;
        if(str == "competences_modifiable")
            return type_controle::Competences_Modifiable;
        if(str == "courbe")
            return type_controle::Courbe;
        if(str == "courbe_modifiable")
            return type_controle::Courbe_Modifiable;
        if(str == "decimale_modifiable")
            return type_controle::Decimale_Modifiable;
        if(str == "depassement")
            return type_controle::Depassement;
        if(str == "depassement_modifiable")
            return type_controle::Depassement_Modifiable;
        if(str == "lettre")
            return type_controle::Lettre;
        if(str == "minima_modifiable")
            return type_controle::Minima_Modifiable;
        if(str == "note")
            return type_controle::Note;
        if(str == "note_modifiable")
            return type_controle::Note_Modifiable;
        if(str == "total_modifiable")
            return type_controle::Total_Modifiable;
        if(str == "type_note_nodifiable")
            return type_controle::Type_Note_Modifiable;
        break;
    }
    return bdd_predef::str_id_to_enum(str,id_entity,ctr);
}

void bdd_note::liste_mise_a_jour_bdd(int version, idt type) {
    bdd_predef::liste_mise_a_jour_bdd(version,type);
    if(type == mps::b2d::bdd_version::Note_Type) {
        switch (version) {
        case mps::b2d::bdd_version::Initiale: {
            creer_table<annee>();
            creer_table<type_etablissement>();
            creer_table<etablissement>();
            creer_table<etablissement_type>();
            creer_table<niveau>();
            creer_table<classe>();
            creer_table<eleve>();
            creer_table<classe_eleve>();
            creer_table<etablissement_niveau>();
            creer_table<filiation_niveau>();
            creer_table<niveau_type_etablissement>();
            creer_table<groupe>();
            creer_table<eleve_groupe>();
            creer_table<type_controle>();
            creer_table<controle>();
            creer_table<exercice>();
            m_manager->save_version(mps::b2d::bdd_version::Creation,mps::b2d::bdd_version::Note_Type);
        }
        }
        //creer_table<bareme>();
        //creer_table<enonce>();
        //creer_table<epreuve>();
        //creer_table<controle_epreuve>();
        //creer_table<Point>();
        //creer_table<enonce_point>();
        //creer_table<Note>();
        //creer_table<_valide>();
    }
}
