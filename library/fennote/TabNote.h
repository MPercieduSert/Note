/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef TABNOTE_H
#define TABNOTE_H

#include "PredefTab.h"

namespace noteMPS {
namespace fmps = fenMPS;
/*! \ingroup groupeFenInv
 * \brief Fenêtre à onglet de l'application inventaire.
 */
class TabNote : public fmps::PredefTab {
public:
    using PredefTab::PredefTab;

    //! Connect l'onglet aux signaux commun à tous les onglets.
    void connectTab(fmps::AbstractTabModule * /*tab*/) const override
        {}

    //! Créateur d'onglet.
    fmps::AbstractTabModule *createTab(std::pair<int,int> pair) const override;
};
}
#endif // TABNOTE_H
