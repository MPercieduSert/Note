/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef EXERCICETAB_H
#define EXERCICETAB_H

#include "AbstractTabModuleWithBdd.h"
#include "ExerciceModel.h"
#include "StandardNodeWidget.h"

namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Onglet d'édition d'exercice.
 */
class ExerciceTab : public fenMPS::AbstractTabModuleWithBdd {
    Q_OBJECT
protected:
    ExerciceEditModel * m_model;            //!< Model.
    widgetMPS::NodeView * m_view;           //!< Vue.
public:
    //! Constructeur.
    ExerciceTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent = nullptr);

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
