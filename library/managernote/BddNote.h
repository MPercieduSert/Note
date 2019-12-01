/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/11/2019
 */
#ifndef BDDNOTE_H
#define BDDNOTE_H

#include "ManagersNote.h"
#include "BddPredef.h"

namespace bddMPS {
    namespace bddVersion  {
        enum versionBddNote{initialeNote = NbrCreationBddPredef,
                           NbrVersionBddNote};
}}

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
    //! Mise à jour de la base de donnée.
    void listeMiseAJourBdd(int version) override;
};
}
#endif // BDDNOTE_H
