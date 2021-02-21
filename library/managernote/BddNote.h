/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/11/2019
 */
#ifndef BDDNOTE_H
#define BDDNOTE_H

#include "ManagersNote.h"
#include "BddPredef.h"

namespace bddMPS {
    namespace bddVersion {
        //! Version de la base de données.
        enum versionBddNote{NbrVersionBddNote = NbrVersion};
        //! Type de mise à jour.
        enum typeBddNote{NoteType = NbrType, NbrTypeNote};
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
/*! \ingroup groupeManagerInv
 * \brief Base de donnée de l'application inventaire.
 */
class BddNote : public bmps::BddPredef {
public:
    //! Constructeurs hérités.
    using BddPredef::BddPredef;

    //Constructeur.
    BddNote()
        :BddPredef("QSQLITE",{bmps::bddVersion::NbrVersionPredef -1, bmps::bddVersion::NbrVersionBddNote - 1},
                   std::make_unique<ManagersNote>()) {}

    //! Destructeur. Referme la base de donnée.
    ~BddNote() override = default;

    //! Renvoie l'enumeration associé à str pour une entitée de type id_entity.
    enumt strIdToEnum(const QString & str, idt id_entity, QString &controle) const noexcept override;
protected:
    //! Supprime l'entité d'identifiant id de type d'identifiant id_entity de la base de données.
    bool delP(idt id, entidt id_entity) override;

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    bool testAutorisationP(idt id, entidt id_entity, flag autoris) override;

    //! Mise à jour de la base de donnée.
    void listeMiseAJourBdd(int version, idt type) override;
};
}
#endif // BDDNOTE_H
