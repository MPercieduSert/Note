/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef TABNOTE_H
#define TABNOTE_H

#include "PredefTab.h"
#include "ElevesTab.h"

namespace noteMPS {
namespace fmps = fenMPS;
/*! \ingroup groupeFenInv
 * \brief Fenêtre à onglet de l'application inventaire.
 */
class TabNote : public fmps::PredefTab {
public:
    enum NoteTabId {ElevesTabId = NbrPredefTabId,
                   Eleves2TabId};
    using PredefTab::PredefTab;

    //! Connect l'onglet aux signaux commun à tous les onglets.
    void connectTab(fmps::AbstractTabModule * tab) const override
        {PredefTab::connectTab(tab);}

    //! Créateur d'onglet.
    fmps::AbstractTabModule *createTab(const std::pair<int,int> & pair,
                                       const std::vector<QVariant> & args = std::vector<QVariant>()) const override;

    //! Accesseur de la base de donnée.
    BddNote & bdd() const noexcept
        {return static_cast<BddNote &>(m_bdd);}
};
}
#endif // TABNOTE_H
