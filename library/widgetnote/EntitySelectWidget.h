/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ENTITYSELECTWIDGET_H
#define ENTITYSELECTWIDGET_H

#include <QLabel>
#include "AbstractEntitySelectWidget.h"
#include "SpinBoxAnneeScolaire.h"

namespace  noteMPS{
/*! \ingroup groupeWidgetNote
 * \brief Formulaire de choix d'une Annees.
 */
class AnneeSelectWidget : public widgetMPS::AbstractEntitySelectWidget{
    Q_OBJECT
protected:
    QLabel * m_anneeLabel;     //!< Label du choix de l'annee.
    SpinBoxAnneeScolaire * m_anneeSpinBox;      //!< Choix de l'annee Scolaire.

public:
    //! Constructeur.
    AnneeSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Accesseur du numéro de l'année slectionnée.
    int num() const
        {return m_anneeSpinBox->value().num();}

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    idt id() const override
        {return m_anneeSpinBox->value().id();}

    //! Mutateur de l'identifiant de l'entité séléctionnée.
    void setId(idt id) override
        {m_anneeSpinBox->setValue(Annee(id),false);}
};
}
#endif // ENTITYSELECTWIDGET_H
