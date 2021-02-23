/*Auteur: PERCIE DU SERT Maxime
 *Date: 12/04/2020
 */
#ifndef GROUPETAB_H
#define GROUPETAB_H

#include <QCheckBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QRadioButton>
#include "abstract_tab_tableau.h"
#include "BddNote.h"
#include "EntitySelectWidget.h"
#include "find_widget.h"
#include "id_combo_box.h"
#include "ModelNote.h"
#include "spin_box_anneeScolaire.h"
#include "TableauNote.h"


namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Onglet de gestion des élèves dans les groupes.
 */
class GroupeTab : public fenetre::abstract_tab_tableau {
    Q_OBJECT
protected:
    // Model et vue
    CandidatGroupeModel * m_candidatModel = nullptr;      //!< Modèle de séléction des élèves pour les groupes.
    QTableView * m_candidatView;                //!< Vue de séléction des élèves pour les groupes.

    // Widget
    GroupeSelectWidget * m_groupeSelect;        //!< Choix du groupe.
    QLabel * m_propGrLabel;                     //!< Label des propriétés du groupe.
    QGroupBox * m_findGroup;                    //!< Groupe de recherche.
    QCheckBox * m_exclusifCheck;                //!< Etat exclusif du groupe.
    QCheckBox * m_totalCheck;                   //!< Etat total du groupe.
    QPushButton * m_add_groupeButton;            //!< Bouton d'ajout d'un groupe.
    QPushButton * m_delEleveButton;             //!< Bouton d'ajout des éléves sélectionnés au groupe sélectionné.
    widget::find_widget * m_findWidget;       //!< Recherche dans la liste d'éléve.

    // Calque
    QHBoxLayout * m_bouton_layout;               //!< Calque des boutons.
    QHBoxLayout * m_eleve_layout;                //!< Claque de sélection des éléves.
    QVBoxLayout * m_findLayout;                 //!< Claque de recherche.
    QVBoxLayout * m_main_layout;                 //!< Calque principal.

public:
    //! position des colonnes.
    enum colonne {Nom, Prenom, Naissance, Sexe};

    //! Constructeur.
    GroupeTab(BddNote & bdd, tab_index pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const override
        {return static_cast<BddNote &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Listes des éléves");}
    
public slots:
    //! Ajout un groupe contenant les élèves sélectionnés.
    void add_groupe() {
        static_cast<Eleve_groupeModel&>(*m_model).push_back();
        affecte(static_cast<Eleve_groupeModel&>(*m_model).columnCount() - 1);
    }

    //! Affecte les éléves séléctionnés dans la vue de candidature au groupe choisi.
    void affecte(int column);

    //! Action à effectuer lorsque l'onglet devient actif.
    void becomeCurrent() override;

    //! Retire les élèves sectionnés dans view des groupes.
    void remove();

    //! Slot coorepondant à l'action sauver.
    void sauver() override
        {m_candidatModel->save();}

    //! Mise à jour des éléves.
    void updateEleve();
};
}
#endif // GROUPETAB_H
