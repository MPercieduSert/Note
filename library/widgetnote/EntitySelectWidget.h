/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ENTITYSELECTWIDGET_H
#define ENTITYSELECTWIDGET_H

#include <QLabel>
#include "AbstractEntitySelectWidget.h"
#include "bdd_predef.h"
#include "id_combo_box.h"
#include "spin_box_anneeScolaire.h"

namespace  noteMPS{
/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'une Annee scolaire.
 */
class AnneeSelectWidget : public widget::AbstractEntitySelectWidget{
    Q_OBJECT
protected:
    QLabel * m_label;     //!< Label du choix de l'annee.
    spin_box_anneeScolaire * m_spinBox;      //!< Choix de l'annee Scolaire.

public:
    //! Constructeur.
    AnneeSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Accesseur du numéro de l'année slectionnée.
    int num() const
        {return m_spinBox->value().num();}

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    idt id() const override
        {return m_spinBox->value().id();}
public slots:
    //! Mutateur de l'identifiant de l'entité séléctionnée.
    void set_id(idt id) override
        {m_spinBox->set_value(Annee(id),false);}
};

/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'un etablissement.
 */
class EtablissementSelectWidget : public widget::ComboBoxEntitySelectWidget{
    Q_OBJECT
public:
    //! Constructeur.
    EtablissementSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr)
        : ComboBoxEntitySelectWidget(bdd,tr("Etablissement : "),orientation,parent) {
        m_box->add_text(m_bdd.get_list<Etablissement>(Etablissement::Nom),
                       [](const Etablissement & etab)->QString
                             {return QString(etab.nom()).append(" (").append(etab.nc()).append(")");});
    }
};

/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'une classe.
 */
class ClasseSelectWidget : public widget::ComboBoxEntitySelectWidget{
    Q_OBJECT
protected:
    AnneeSelectWidget * m_anneeSelect;          //!< Choix de l'année de la classe.
    EtablissementSelectWidget * m_etabSelect;   //!< Choix de l'établissement de la classe.
    QHBoxLayout * m_classe_layout;               //!< Calque du choix de la classe.
public:
    //! Constructeur.
    ClasseSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Accesseur de l'identifiant de l'année.
    idt idAn() const
        {return m_anneeSelect->id();}
    //! Accesseur de l'identifiant de l'établissement.
    idt idEtab() const
        {return m_etabSelect->id();}

public slots:
    //! Mutateur de l'etat de permission du l'établissement et de la classe.
    void setEnabledEtabClasse(bool bb);

    //! Mutateur de l'identifiant de la classe.
    void set_id(idt id) override;

    //! Mutateur de l'identifiant de l'année de la classe.
    void set_idAn(idt id)
        {m_anneeSelect->set_id(id);}

    //! Mutateur de l'identifiant de l'établissement de la classe.
    void set_idEtab(idt id)
        {m_etabSelect->set_id(id);}

    //! Mise à jour de la liste des classes.
    void updateClasse();

signals:
    //! Signale le changement d'identifiant de l'année.
    void idAnChanged(idt idAn);
};

/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'un élève.
 */
class EleveSelectWidget : public widget::ComboBoxEntitySelectWidget{
    Q_OBJECT
protected:
    ClasseSelectWidget * m_classeSelect;        //!< Choix de la classe.
    QHBoxLayout * m_eleve_layout;               //!< Calque du choix de la classe.
public:
    //! Constructeur.
    EleveSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Accesseur de l'identifiant de l'année.
    idt idAn() const
        {return m_classeSelect->idAn();}

    //! Accesseur de l'identifiant de la classe.
    idt idClasse() const
        {return m_classeSelect->id();}

    //! Accesseur de l'identifiant de l'établissement.
    idt idEtab() const
        {return m_classeSelect->idEtab();}

public slots:
    //! Mutateur de l'identifiant de l'élève.
    void set_id(idt id) override;

    //! Mutateur de l'identifiant de l'année de la classe.
    void set_idAn(idt id)
        {m_classeSelect->set_idAn(id);}

    //! Mutateur de l'identifiant de la classe.
    void set_idClasse(idt id)
        {m_classeSelect->set_id(id);}

    //! Mutateur de l'identifiant de l'établissement de la classe.
    void set_idEtab(idt id)
        {m_classeSelect->set_idEtab(id);}

    //! Mise à jour de la liste des éléves.
    void updateEleve();
};
/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'un Groupe.
 */
class GroupeSelectWidget : public widget::ComboBoxEntitySelectWidget{
    Q_OBJECT
protected:
    QLabel * m_catLabel;                        //!< Label du choix de la catégorie du groupe.
    QLabel * m_type_label;                       //!< Label de sélection du type.
    QComboBox * m_catCB;                        //!< Sélection de la catégorie du groupe.
    widget::id_combo_box * m_type_cb;           //!< Sélection du type.
    ClasseSelectWidget * m_classeSelect;        //!< Choix de la classe ou de l'année.
    QHBoxLayout * m_catLayout;                  //!< Calque du choix de la catégorie du groupe.
    QHBoxLayout * m_groupe_layout;               //!< Calque du choix du type groupe.
    QHBoxLayout * m_type_layout;                 //!< Calque du choix du type de groupe.
public:
    //! Constructeur.
    GroupeSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    b2d::bdd_predef & bdd() const
        {return static_cast<b2d::bdd_predef &>(m_bdd);}

public slots:
    //! Mise à jour après changement de catégorie.
    void catChange();

    //! Mutateur de l'identifiant du groupe.
    void set_id(idt id) override;

    //! Mise à jour des groupes.
    void update_groupe();

    //! Mise à jour des types.
    void updateType();
};


/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'un niveaux.
 */
class NiveauxSelectWidget : public widget::ComboBoxEntitySelectWidget{
    Q_OBJECT
public:
    //! Constructeur.
    NiveauxSelectWidget(b2d::Bdd & bdd, idt idEtab = 0, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr)
        : ComboBoxEntitySelectWidget(bdd,tr("Niveaux : "),orientation,parent)
    {set_idEtab(idEtab);}

public slots:
    //! Mutateur de l'identifiant de l'établissement hébergement les niveaux.
    void set_idEtab(idt id);
};
}
#endif // ENTITYSELECTWIDGET_H
