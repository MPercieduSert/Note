/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef EXERCICE_TAB_H
#define EXERCICE_TAB_H

#include "abstract_tab_module.h"
#include "enum_fen_note.h"
#include "exercice_model.h"
#include "find_model.h"
#include "id_combo_box.h"
#include "standard_node_widget.h"

namespace note_mps {
/*! \ingroup groupe_dialog_note
 * \brief Onglet d'édition d'exercice.
 */
class edit_exercice_tab : public mps::fenetre::abstract_tab_module_with_bdd {
    Q_OBJECT
protected:
    model_exo::edit_exercice_model *m_model;                   //!< Model.
    mps::widget::node_view *m_view;                 //!< Vue.
    mps::widget::sub_node_widget * m_type_exo;      //! choix du type d'exercice.
    QPushButton *m_save_bouton;                     //!< Bouton de sauvegarde.
    QHBoxLayout *m_header_layout;                   //!< Calque d'entête.
    QVBoxLayout *m_main_layout;                     //!< Calque principal.
public:
    //! Constructeur.
    edit_exercice_tab(bdd_note & bdd, tab_index pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const override
        {return static_cast<bdd_note &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Édition d'exercice");}

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    void become_current() override;

    //! Slot coorepondant à l'action sauver.
    void sauver() override
        {m_model->save();}

    //! Met à jour la liste des types possible de l'exercice.
    void update_type();
};
/*! \ingroup groupe_dialog_note
 * \brief Onglet de recherche d'exercice.
 */
class find_exercice_tab : public mps::fenetre::abstract_tab_module_with_bdd {
    Q_OBJECT
protected:
    model_exo::read_exercice_model *m_model;               //!< Model.
    //mps::model_base::find_model
    mps::widget::node_view *m_view;             //!< Vue.
    QPushButton *m_select_bouton;               //!< Bouton de sélection.
    //QVBoxLayout *m_exo_layout;                  //!< Calque des exercices.
    QVBoxLayout *m_find_layout;                 //!< Calque de recherche.
    QHBoxLayout *m_main_layout;                 //!< Calque principal.
public:
    //! Constructeur.
    find_exercice_tab(bdd_note & bd, tab_index pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const override
        {return static_cast<bdd_note &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Recherche d'exercice");}

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    void become_current() override;
};
}
#endif // EXERCICE_TAB_H
