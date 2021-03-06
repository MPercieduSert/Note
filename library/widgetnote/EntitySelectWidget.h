/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ENTITYSELECTWIDGET_H
#define ENTITYSELECTWIDGET_H

#include <QLabel>
#include "AbstractEntitySelectWidget.h"
#include "BddPredef.h"
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
 * \brief Formulaire du choix d'une classe.
 */
class ClasseSelectWidget : public widgetMPS::ComboBoxEntitySelectWidget{
    Q_OBJECT
protected:
    AnneeSelectWidget * m_anneeSelect;          //!< Choix de l'année de la classe.
    EtablissementSelectWidget * m_etabSelect;   //!< Choix de l'établissement de la classe.
    QHBoxLayout * m_classeLayout;               //!< Calque du choix de la classe.
public:
    //! Constructeur.
    ClasseSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Accesseur de l'identifiant de l'année.
    idt idAn() const
        {return m_anneeSelect->id();}
    //! Accesseur de l'identifiant de l'établissement.
    idt idEtab() const
        {return m_etabSelect->id();}

public slots:
    //! Mutateur de l'etat de permission du l'établissement et de la classe.
    void setEnabledEtabClasse(bool bb);

    //! Mutateur de l'identifiant de l'année de la classe.
    void setIdAn(idt id)
        {m_anneeSelect->setId(id);}

    //! Mutateur de l'identifiant de l'année de la classe.
    void setIdEtab(idt id)
        {m_etabSelect->setId(id);}

    //! Mise à jour de la liste des classes.
    void updateClasse();

signals:
    //! Signale le changement d'identifiant de l'année.
    void idAnChanged(idt idAn);
};

/*! \ingroup groupeWidgetNote
 * \brief Formulaire du choix d'un Groupe.
 */
class GroupeSelectWidget : public widgetMPS::ComboBoxEntitySelectWidget{
    Q_OBJECT
protected:
    QLabel * m_catLabel;                        //!< Label du choix de la catégorie du groupe.
    QLabel * m_typeLabel;                       //!< Label de sélection du type.
    QComboBox * m_catCB;                        //!< Sélection de la catégorie du groupe.
    widgetMPS::IdComboBox * m_typeCB;           //!< Sélection du type.
    ClasseSelectWidget * m_classeSelect;        //!< Choix de la classe ou de l'année.
    QHBoxLayout * m_catLayout;                  //!< Calque du choix de la catégorie du groupe.
    QHBoxLayout * m_groupeLayout;               //!< Calque du choix du type groupe.
    QHBoxLayout * m_typeLayout;                 //!< Calque du choix du type de groupe.
public:
    //! Constructeur.
    GroupeSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    bddMPS::BddPredef & bdd() const
        {return static_cast<bddMPS::BddPredef &>(m_bdd);}

public slots:
    //! Mise à jour après changement de catégorie.
    void catChange();

    //! Mise à jour des groupes.
    void updateGroupe();

    //! Mise à jour des types.
    void updateType();
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
