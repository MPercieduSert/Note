/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/11/2019
 */
#ifndef BDD_NOTE_H
#define BDD_NOTE_H

#include "managers_note.h"
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

/*! \ingroup groupe_managerInv
 * \brief Base de donnée de l'application inventaire.
 */
class bdd_note : public mps::b2d::bdd_predef {
public:
    //! Constructeurs hérités.
    using bdd_predef::bdd_predef;

    //Constructeur.
    bdd_note()
        :bdd_predef("QSQLITE",{mps::b2d::bdd_version::Nbr_Version_Predef -1, mps::b2d::bdd_version::Nbr_Version_Bdd_Note - 1},
                   std::make_unique<managers_note>()) {}

    //! Destructeur. Referme la base de donnée.
    ~bdd_note() override = default;

    //! Renvoie l'enumeration associé à str pour une entitée de type id_ent.
    enumt str_id_to_enum(const QString & str, idt id_entity, QString &controle) const noexcept override;
protected:
    //! Supprime l'entité d'identifiant id de type d'identifiant id_entity de la base de données.
    bool del_v(idt id, entidt id_entity) override;

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    bool test_autorisation_v(idt id, entidt id_entity, flag autoris) override;

    //! Mise à jour de la base de donnée.
    void liste_mise_a_jour_bdd(int version, idt type) override;
};
}
#endif // BDD_NOTE_H
