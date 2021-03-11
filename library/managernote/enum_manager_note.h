/*Auteur: PERCIE DU SERT Maxime
 *Date: 10/03/2021
 */
#ifndef ENUM_MANAGER_NOTE_H
#define ENUM_MANAGER_NOTE_H

#include "bdd_predef.h"

namespace mps {
namespace b2d {
    namespace bdd_version {
        //! Version de la base de données.
        enum version_bdd_note{Nbr_Version_Bdd_Note = Nbr_Version};
        //! Type de mise à jour.
        enum type_bdd_note{Note_Type = Nbr_Type, Nbr_Type_Note};
    }

    namespace Id_Prog {
        //! Identifiant de programmation de la cible configuration de la table cibleDonnéee.
        enum Id_Prog_Configuration {Annee_Courante = 1};
    }

    namespace cible_id {
        //! Numéro de cible des entités de Note non prédéfinies.
        enum EntityNote {Annee = cible_id::Nbr_Cible_Predef,
                         Bareme,
                         Classe,
                         Classe_Eleve,
                         Classe_Groupe,
                         Controle,
                         Controle_Epreuve,
                         Eleve,
                         Eleve_Groupe,
                         Enonce,
                         Enonce_Point,
                         Epreuve,
                         Etablissement,
                         Etablissement_Niveau,
                         Etablissement_Type,
                         Exercice,
                         Filiation_Niveau,
                         Groupe,
                         Niveau,
                         Niveau_Type_Etablissement,
                         Note,
                         Point,
                         Type_Controle,
                         Type_Etablissement,
                         Valide,
                         Nbr_Cible};
}
}}

namespace note_mps {
    namespace donnee_mps {
        //! Identitifant de programmation de la table donnée.
        enum id_prog_donnee {
            Configuration_Id,
            Default_Value_Id,
            Default_Date_Id
        };

        //! Numero d'une donnée
        enum num_donnee {
            Entree_Num,
            Sortie_Num,
            Nbr_Default_Date_Classe
        };
    }
    namespace info_entity_note {
        //! Liste des identifiants des entités
        enum entity_id{Annee = mps::info_entity::Nbr_Entity_Predef,
                      Bareme,
                      Classe,
                      Classe_Eleve,
                      Classe_Groupe,
                      Controle,
                      Controle_Epreuve,
                      Eleve,
                      Eleve_Groupe,
                      Enonce,
                      Enonce_Point,
                      Epreuve,
                      Etablissement,
                      Etablissement_Niveau,
                      Etablissement_Type,
                      Exercice,
                      Filiation_Niveau,
                      Groupe,
                      Niveau,
                      Niveau_Type_Etablissement,
                      Note,
                      Point,
                      Type_Controle,
                      Type_Etablissement,
                      Valide,
                      Nbr_Entity_Note};
    }
}
#endif // ENUM_MANAGER_NOTE_H
