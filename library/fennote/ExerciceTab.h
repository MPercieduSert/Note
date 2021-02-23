/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef EXERCICETAB_H
#define EXERCICETAB_H

#include "abstract_tab_module_with_bdd.h"
#include "ExerciceModel.h"
#include "Standardnode_widget.h"

namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Onglet d'édition d'exercice.
 */
class ExerciceTab : public fenetre::abstract_tab_module_with_bdd {
    Q_OBJECT
protected:
    ExerciceEditModel * m_model;            //!< Model.
    widget::node_view * m_view;           //!< Vue.
    QPushButton * m_save_bouton;                 //!< Bouton de sauvegarde.
    QVBoxLayout * m_main_layout;                 //!< Calque principal.
public:
    //! Constructeur.
    ExerciceTab(BddNote & bdd, tab_index pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const override
        {return static_cast<BddNote &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Exercice");}

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    void becomeCurrent() override;

    //! Slot coorepondant à l'action sauver.
    void sauver() override
        {}
};
}
#endif // EXERCICETAB_H
