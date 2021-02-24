/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ENTITYSELECTWIDGET_H
#define ENTITYSELECTWIDGET_H

#include <QLabel>
#include "abstract_entity_select_widget.h"
#include "bdd_predef.h"
#include "id_combo_box.h"
#include "spin_box_annee_scolaire.h"

namespace  note_mps{
/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'une annee scolaire.
 */
class annee_select_widget : public mps::widget::abstract_entity_select_widget{
    Q_OBJECT
protected:
    QLabel *m_label;     //!< Label du choix de l'annee.
    spin_box_annee_scolaire *m_spin_box;      //!< Choix de l'annee Scolaire.

public:
    //! Constructeur.
    annee_select_widget(mps::b2d::bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget *parent = nullptr);

    //! Accesseur du numéro de l'année slectionnée.
    int num() const
        {return m_spin_box->value().num();}

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    idt id() const override
        {return m_spin_box->value().id();}
public slots:
    //! Mutateur de l'identifiant de l'entité séléctionnée.
    void set_id(idt id) override
        {m_spin_box->set_value(annee(id),false);}
};

/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'un etablissement.
 */
class etablissement_select_widget : public mps::widget::combo_box_entity_select_widget{
    Q_OBJECT
public:
    //! Constructeur.
    etablissement_select_widget(mps::b2d::bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget *parent = nullptr)
        : combo_box_entity_select_widget(bdd,tr("etablissement : "),orientation,parent) {
        m_box->add_text(m_bdd.get_list<etablissement>(etablissement::Nom),
                       [](const etablissement & etab)->QString
                             {return QString(etab.nom()).append(" (").append(etab.nc()).append(")");});
    }
};

/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'une classe.
 */
class classe_select_widget : public mps::widget::combo_box_entity_select_widget{
    Q_OBJECT
protected:
    annee_select_widget *m_annee_select;          //!< Choix de l'année de la classe.
    etablissement_select_widget *m_etab_select;   //!< Choix de l'établissement de la classe.
    QHBoxLayout *m_classe_layout;               //!< Calque du choix de la classe.
public:
    //! Constructeur.
    classe_select_widget(mps::b2d::bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget *parent = nullptr);

    //! Accesseur de l'identifiant de l'année.
    idt id_an() const
        {return m_annee_select->id();}
    //! Accesseur de l'identifiant de l'établissement.
    idt id_etab() const
        {return m_etab_select->id();}

public slots:
    //! Mutateur de l'etat de permission du l'établissement et de la classe.
    void set_enabled_etab_classe(bool bb);

    //! Mutateur de l'identifiant de la classe.
    void set_id(idt id) override;

    //! Mutateur de l'identifiant de l'année de la classe.
    void set_id_an(idt id)
        {m_annee_select->set_id(id);}

    //! Mutateur de l'identifiant de l'établissement de la classe.
    void set_id_etab(idt id)
        {m_etab_select->set_id(id);}

    //! Mise à jour de la liste des classes.
    void update_classe();

signals:
    //! Signale le changement d'identifiant de l'année.
    void id_an_changed(idt id_an);
};

/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'un élève.
 */
class eleve_select_widget : public mps::widget::combo_box_entity_select_widget{
    Q_OBJECT
protected:
    classe_select_widget *m_classe_select;        //!< Choix de la classe.
    QHBoxLayout *m_eleve_layout;               //!< Calque du choix de la classe.
public:
    //! Constructeur.
    eleve_select_widget(mps::b2d::bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget *parent = nullptr);

    //! Accesseur de l'identifiant de l'année.
    idt id_an() const
        {return m_classe_select->id_an();}

    //! Accesseur de l'identifiant de la classe.
    idt id_classe() const
        {return m_classe_select->id();}

    //! Accesseur de l'identifiant de l'établissement.
    idt id_etab() const
        {return m_classe_select->id_etab();}

public slots:
    //! Mutateur de l'identifiant de l'élève.
    void set_id(idt id) override;

    //! Mutateur de l'identifiant de l'année de la classe.
    void set_id_an(idt id)
        {m_classe_select->set_id_an(id);}

    //! Mutateur de l'identifiant de la classe.
    void set_id_classe(idt id)
        {m_classe_select->set_id(id);}

    //! Mutateur de l'identifiant de l'établissement de la classe.
    void set_id_etab(idt id)
        {m_classe_select->set_id_etab(id);}

    //! Mise à jour de la liste des éléves.
    void update_eleve();
};
/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'un Groupe.
 */
class groupe_select_widget : public mps::widget::combo_box_entity_select_widget{
    Q_OBJECT
protected:
    QLabel *m_cat_label;                        //!< Label du choix de la catégorie du groupe.
    QLabel *m_type_label;                       //!< Label de sélection du type.
    QComboBox *m_catCB;                        //!< Sélection de la catégorie du groupe.
    mps::widget::id_combo_box *m_type_cb;           //!< Sélection du type.
    classe_select_widget *m_classe_select;        //!< Choix de la classe ou de l'année.
    QHBoxLayout *m_cat_layout;                  //!< Calque du choix de la catégorie du groupe.
    QHBoxLayout *m_groupe_layout;               //!< Calque du choix du type groupe.
    QHBoxLayout *m_type_layout;                 //!< Calque du choix du type de groupe.
public:
    //! Constructeur.
    groupe_select_widget(mps::b2d::bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget *parent = nullptr);

    //! Accesseur de la base de donnée.
    mps::b2d::bdd_predef & bdd() const
        {return static_cast<mps::b2d::bdd_predef &>(m_bdd);}

public slots:
    //! Mise à jour après changement de catégorie.
    void cat_change();

    //! Mutateur de l'identifiant du groupe.
    void set_id(idt id) override;

    //! Mise à jour des groupes.
    void update_groupe();

    //! Mise à jour des types.
    void update_type();
};


/*! \ingroup groupe_WidgetNote
 * \brief Formulaire du choix d'un niveaux.
 */
class niveaux_select_widget : public mps::widget::combo_box_entity_select_widget{
    Q_OBJECT
public:
    //! Constructeur.
    niveaux_select_widget(mps::b2d::bdd & bdd, idt id_etab = 0,
                          Qt::Orientations orientation = Qt::Horizontal, QWidget *parent = nullptr)
        : combo_box_entity_select_widget(bdd,tr("Niveaux : "),orientation,parent)
    {set_id_etab(id_etab);}

public slots:
    //! Mutateur de l'identifiant de l'établissement hébergement les niveaux.
    void set_id_etab(idt id);
};
}
#endif // ENTITYSELECTWIDGET_H
