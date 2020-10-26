/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ENTITYSELECTWIDGET_H
#define ENTITYSELECTWIDGET_H

#include <QLabel>
#include "AbstractEntitySelectWidget.h"
#include "IdComboBox.h"
#include "SpinBoxAnneeScolaire.h"

namespace  noteMPS{
/*! \ingroup groupeWidgetNote
 * \brief Formulaire du choix d'une Annee scolaire.
 */
class AnneeSelectWidget : public widgetMPS::AbstractEntitySelectWidget{
    Q_OBJECT
protected:
    QLabel * m_label;     //!< Label du choix de l'annee.
    SpinBoxAnneeScolaire * m_spinBox;      //!< Choix de l'annee Scolaire.

public:
    //! Constructeur.
    AnneeSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Accesseur du numéro de l'année slectionnée.
    int num() const
        {return m_spinBox->value().num();}

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    idt id() const override
        {return m_spinBox->value().id();}

    //! Mutateur de l'identifiant de l'entité séléctionnée.
    void setId(idt id) override
        {m_spinBox->setValue(Annee(id),false);}
};

/*! \ingroup groupeWidgetNote
 * \brief Formulaire du choix d'un etablissement.
 */
class EtablissementSelectWidget : public widgetMPS::ComboBoxEntitySelectWidget{
    Q_OBJECT
public:
    //! Constructeur.
    EtablissementSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr)
        : ComboBoxEntitySelectWidget(bdd,tr("Etablissement : "),orientation,parent) {
    m_box->addText(m_bdd.getList<Etablissement>(Etablissement::Nom),
                       [](const Etablissement & etab)->QString
                             {return QString(etab.nom()).append(" (").append(etab.nc()).append(")");});
    }
};

/*! \ingroup groupeWidgetNote
 * \brief Formulaire du choix d'un niveaux.
 */
class NiveauxSelectWidget : public widgetMPS::ComboBoxEntitySelectWidget{
    Q_OBJECT
public:
    //! Constructeur.
    NiveauxSelectWidget(bddMPS::Bdd & bdd, idt idEtab = 0, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr)
        : ComboBoxEntitySelectWidget(bdd,tr("Niveaux : "),orientation,parent)
    {setIdEtab(idEtab);}

public slots:
    //! Mutateur de l'identifiant de l'établissement hébergement les niveaux.
    void setIdEtab(idt id);
};
}
#endif // ENTITYSELECTWIDGET_H
