/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef TABNOTE_H
#define TABNOTE_H

#include "ClasseEleveTab.h"
#include "ElevesTab.h"
#include "ExerciceTab.h"
#include "GroupeTab.h"
#include "predef_tab.h"


namespace noteMPS {
namespace fmps = fenetre;
/*! \ingroup groupe_fenInv
 * \brief Fenêtre à onglet de l'application inventaire.
 */
class TabNote : public fmps::predef_tab {
public:
    enum NoteTabId {ClasseEleveTabId = Nbr_Predef_Tab_Id,
                    ElevesTabId,
                    ExerciceTabId,
                    GroupeTabId
                    };
    using predef_tab::predef_tab;

    //! Connect l'onglet aux signaux commun à tous les onglets.
    void connect_tab(fmps::abstract_tab_module * tab) const override
        {predef_tab::connect_tab(tab);}

    //! Créateur d'onglet.
    fmps::abstract_tab_module *create_tab(const tab_index & pair,
                                       const std::vector<QVariant> & args = std::vector<QVariant>()) const override;

    //! Accesseur de la base de donnée.
    BddNote & bdd() const noexcept
        {return static_cast<BddNote &>(m_bdd);}
};
}
#endif // TABNOTE_H
