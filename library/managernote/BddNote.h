/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/11/2019
 */
#ifndef BDDNOTE_H
#define BDDNOTE_H

#include "ManagersNote.h"
#include "bdd_predef.h"

namespace b2d {
    namespace bdd_version {
        //! Version de la base de données.
        enum versionBddNote{Nbr_VersionBddNote = Nbr_Version};
        //! Type de mise à jour.
        enum type_bddNote{NoteType = Nbr_Type, Nbr_TypeNote};
    }
    namespace Id_Prog {
        //! Identifiant de programmation de la cible configuration de la table cibleDonnéee.
        enum Id_ProgConfiguration {AnneeCourante = 1};
    }
}

namespace donnee {
    //! Identitifant de programmation de la table donnée.
    enum Id_Progdonnee {
        ConfigurationId,
        Default_ValueId,
        DefaultDateId
    };

    //! Numero d'une donnée
    enum numdonnee {
        EntreeNum,
        SortieNum,
        NbrDefaultDateClasse
    };
}

namespace noteMPS {
/*! \ingroup groupe_managerInv
 * \brief Base de donnée de l'application inventaire.
 */
class BddNote : public b2d::bdd_predef {
public:
    //! Constructeurs hérités.
    using bdd_predef::bdd_predef;

    //Constructeur.
    BddNote()
        :bdd_predef("QSQLITE",{b2d::bdd_version::Nbr_Version_Predef -1, b2d::bdd_version::Nbr_VersionBddNote - 1},
                   std::make_unique<managers_note>()) {}

    //! Destructeur. Referme la base de donnée.
    ~BddNote() override = default;

    //! Renvoie l'enumeration associé à str pour une entitée de type id_ent.
    enumt str_id_to_enum(const QString & str, idt id_entity, QString &controle) const noexcept override;
protected:
    //! Supprime l'entité d'identifiant id de type d'identifiant id_entity de la base de données.
    bool del_v(idt id, entidt id_entity) override;

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    bool test_autorisation_v(idt id, entidt id_entity, flag autoris) override;

    //! Mise à jour de la base de donnée.
    void listeMiseAJourBdd(int version, idt type) override;
};
}
#endif // BDDNOTE_H
