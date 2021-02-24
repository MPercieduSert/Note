/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef EXERCICE_TAB_H
#define EXERCICE_TAB_H

#include "abstract_tab_module.h"
#include "exercice_model.h"
#include "standard_node_widget.h"

namespace note_mps {
/*! \ingroup groupe_dialog_note
 * \brief Onglet d'édition d'exercice.
 */
class exercice_tab : public mps::fenetre::abstract_tab_module_with_bdd {
    Q_OBJECT
protected:
    exercice_edit_model * m_model;            //!< Model.
    mps::widget::node_view * m_view;           //!< Vue.
    QPushButton * m_save_bouton;                 //!< Bouton de sauvegarde.
    QVBoxLayout * m_main_layout;                 //!< Calque principal.
public:
    //! Constructeur.
    exercice_tab(bdd_note & bdd, tab_index pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const override
        {return static_cast<bdd_note &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("exercice");}

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    void become_current() override;

    //! Slot coorepondant à l'action sauver.
    void sauver() override
        {}
};
}
#endif // EXERCICE_TAB_H
