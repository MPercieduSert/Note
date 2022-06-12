/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef TAB_NOTE_H
#define TAB_NOTE_H

#include "eleve_tab.h"
#include "enum_fen_note.h"
#include "exercice_tab.h"
#include "groupe_tab.h"
#include "predef_tab.h"

namespace note_mps {
/*! \ingroup groupe_fen_note
 * \brief Fenêtre à onglet de l'application note.
 */
class tab_note : public mps::fenetre::predef_tab {
public:
    using predef_tab::predef_tab;

//    //! Connect l'onglet aux signaux commun à tous les onglets.
//    void connect_tab(mps::fenetre::abstract_tab_module * tab) override
//        {predef_tab::connect_tab(tab);}

    //! Créateur d'onglet.
    mps::fenetre::abstract_tab_module *create_tab(const tab_index & pair,
                                       const std::vector<QVariant> & args = std::vector<QVariant>()) const override;

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const noexcept
        {return static_cast<bdd_note &>(m_bdd);}
};
}
#endif // TAB_NOTE_H
