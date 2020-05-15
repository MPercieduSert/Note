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
        enum versionBddNote{InitialeNote = NbrCreationBddPredef,
                            IdProg001Note,
                            NbrVersionBddNote};
    }
    namespace idProg {
        //! Identifiant de programmation de la cible configuration de la table cibleDonnéee.
        enum idProgConfiguration {AnneeCourante = 1};

        //! Identifiant de programmation de la table type.
        enum idProgType {
            Null = 0,
            EtudeType = 1
        };

    }
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
        :BddPredef("QSQLITE",bmps::bddVersion::NbrVersionBddNote - 1, std::make_unique<ManagersNote>())
    {}

    //! Destructeur. Referme la base de donnée.
    ~BddNote() override = default;

protected:
    //! Supprime l'entité d'identifiant id de type d'identifiant idEntity de la base de données.
    bool delP(idt id, szt idEntity) override;

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    bool getAutorisationP(idt id, szt idEntity, bmps::autorisation autoris) override;

    //! Mise à jour de la base de donnée.
    void listeMiseAJourBdd(int version) override;
};
}
#endif // BDDNOTE_H
