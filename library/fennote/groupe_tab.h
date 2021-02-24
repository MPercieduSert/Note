/*Auteur: PERCIE DU SERT Maxime
 *Date: 12/04/2020
 */
#ifndef groupe_tab_H
#define groupe_tab_H

#include <QCheckBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QRadioButton>
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
 * \brief Onglet de gestion des élèves dans les groupes.
 */
class groupe_tab : public mps::fenetre::abstract_tab_tableau {
    Q_OBJECT
protected:
    // Model et vue
    candidat_groupe_model *m_candidat_model = nullptr;      //!< Modèle de séléction des élèves pour les groupes.
    QTableView *m_candidat_view;                //!< Vue de séléction des élèves pour les groupes.

    // Widget
    groupe_select_widget *m_groupe_select;        //!< Choix du groupe.
    QLabel *m_prop_gr_label;                     //!< Label des propriétés du groupe.
    QGroupBox *m_find_group;                    //!< Groupe de recherche.
    QCheckBox *m_exclusif_check;                //!< Etat exclusif du groupe.
    QCheckBox *m_total_check;                   //!< Etat total du groupe.
    QPushButton *m_add_groupe_bouton;            //!< Bouton d'ajout d'un groupe.
    QPushButton *m_del_eleve_bouton;             //!< Bouton d'ajout des éléves sélectionnés au groupe sélectionné.
    mps::widget::find_widget *m_find_widget;       //!< Recherche dans la liste d'éléve.

    // Calque
    QHBoxLayout *m_bouton_layout;               //!< Calque des boutons.
    QHBoxLayout *m_eleve_layout;                //!< Claque de sélection des éléves.
    QVBoxLayout *m_find_layout;                 //!< Claque de recherche.
    QVBoxLayout *m_main_layout;                 //!< Calque principal.

public:
    //! position des colonnes.
    enum colonne {Nom, Prenom, Naissance, Sexe};

    //! Constructeur.
    groupe_tab(bdd_note & bdd, tab_index pair, QWidget *parent = nullptr);

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const override
        {return static_cast<bdd_note &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Listes des éléves");}
    
public slots:
    //! Ajout un groupe contenant les élèves sélectionnés.
    void add_groupe() {
        static_cast<eleve_groupe_model&>(*m_model).push_back();
        affecte(static_cast<eleve_groupe_model&>(*m_model).columnCount() - 1);
    }

    //! Affecte les éléves séléctionnés dans la vue de candidature au groupe choisi.
    void affecte(int column);

    //! Action à effectuer lorsque l'onglet devient actif.
    void become_current() override;

    //! Retire les élèves sectionnés dans view des groupes.
    void remove();

    //! Slot coorepondant à l'action sauver.
    void sauver() override
        {m_candidat_model->save();}

    //! Mise à jour des éléves.
    void update_eleve();
};
}
#endif // groupe_tab_H
