/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/05/2020
 */
#ifndef CLASSEELEVETAB_H
#define CLASSEELEVETAB_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include "abstract_tab_tableau.h"
#include "bdd_note.h"
#include "entity_select_widget.h"
#include "find_widget.h"
#include "id_combo_box.h"
#include "model_note.h"
#include "spin_box_annee_scolaire.h"
#include "tableau_note.h"

namespace note_mps {
/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création des annees.
 */
class eleves_tab : public mps::fenetre::abstract_tab_tableau {
    Q_OBJECT
protected:
    bool m_find_enabled = true;      //! Autorise la rechercha.
    QGroupBox * m_add_group;         //!< Groupe du choix nombre d'éléves à ajouter.
    QGroupBox * m_find_group;        //!< Groupe de recherche.
    QPushButton * m_add_bouton;      //!< Bouton de l'ajout des éléves dans le tableau.
    QPushButton * m_del_bouton;      //!< Bouton de suppression d'éléves.
    QSpinBox * m_add_spin_box;        //!< Choix du nombre l'éléve à ajouter.
    mps::widget::find_widget * m_find_widget;      //!< Recherche dans la liste d'éléve.

    QVBoxLayout * m_find_layout;     //!< Claque de recherche.
    QHBoxLayout * m_addLayout;      //!< Calque de l'ajout d'éléve dans le tableau.
    QHBoxLayout * m_main_layout;     //!< Calque principal.
    QVBoxLayout * m_second_layout;   //!< Calque secondaire.

public:
    //! position des colonnes.
    enum colonne {Nom, Prenom, Naissance, Sexe};

    //! Constructeur.
    eleves_tab(bdd_note & bdd, tab_index pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const override
        {return static_cast<bdd_note &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Listes des éléves");}
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création des annees.
 */
class classe_eleve_tab: public mps::fenetre::abstract_tab_tableau {
    Q_OBJECT
protected:
    // Model
    mps::model_base::table_model * m_eleveModel;            //!< Model des élèves à sélectionner pour la classe.

    // Widget
    classe_select_widget * m_classeSelect;            //!< Choix de la classe.
    QPushButton * m_add_bouton;                      //!< Bouton d'ajout des élèves séléctionnés.
    QPushButton * m_del_bouton;                      //!< Bouton de retrait des élèves séléctionnés.
    QTableView * m_eleveView;                       //!< Vue de sélection des élèves.
    mps::widget::find_widget * m_eleveFind;            //!< Recherche des des élèves à sélectionner pour la classe.

    // Calque
    QHBoxLayout * m_bouton_layout;                   //!< Calque des boutons.
    QHBoxLayout * m_eleve_layout;                    //!< Calque de séléction des éléves.
    QVBoxLayout * m_main_layout;                     //!< Calque Principal.

public:
    //! position des colonnes du model de sélection de éléves.
    enum colonneEleve {NomEl, PrenomEl, NaissanceEl, SexeEl};

    //! Constructeur.
    classe_eleve_tab(bdd_note & bdd, tab_index pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const override
        {return static_cast<bdd_note &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Classe: Eleves");}

public slots:
    //! Supprime les élèves séléctionnés.
    void remove() {}
};
}
#endif // CLASSEELEVETAB_H
