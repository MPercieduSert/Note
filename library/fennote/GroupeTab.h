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
#include "AbstractTabTableau.h"
#include "BddNote.h"
#include "FindWidget.h"
#include "IdComboBox.h"
#include "ModelNote.h"
#include "SpinBoxAnneeScolaire.h"
#include "TableauNote.h"


namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Formulaire de création des Annees.
 */
class GroupeTab : public fenMPS::AbstractTabTableau {
    Q_OBJECT
protected:
    // Model et vue
    CandidatGroupeModel * m_candidatModel = nullptr;      //!< Modèle de séléction des élèves pour les groupes.
    QTableView * m_candidatView;                //!< Vue de séléction des élèves pour les groupes.

    // Widget
    QLabel * m_anLabel;                         //!< Label du choix de l'année.
    QLabel * m_catLabel;                        //!< Label du choix de la catégorie du groupe.
    QLabel * m_classeLabel;                     //!< Label du choix de la classe.
    QLabel * m_etabLabel;                       //!< Label du choix de l'établissement.
    QLabel * m_groupeLabel;                     //!< Label du choix de groupe.
    QLabel * m_propGrLabel;                     //!< Label des propriétés du groupe.
    QLabel * m_typeLabel;                       //!< Label de sélection du type.
    QGroupBox * m_findGroup;                    //!< Groupe de recherche.
    QGroupBox * m_groupeGroup;                  //!< Groupe de sélection du groupe.
    QComboBox * m_catCB;                        //!< Sélection de la catégorie du groupe.
    QCheckBox * m_exclusifCheck;                //!< Etat exclusif du groupe.
    QCheckBox * m_totalCheck;                   //!< Etat total du groupe.
    QPushButton * m_addGroupeButton;            //!< Bouton d'ajout d'un groupe.
    QPushButton * m_delEleveButton;             //!< Bouton d'ajout des éléves sélectionnés au groupe sélectionné.
    widgetMPS::FindWidget * m_findWidget;       //!< Recherche dans la liste d'éléve.
    widgetMPS::IdComboBox * m_classeCB;         //!< Sélection de la classe du groupe.
    widgetMPS::IdComboBox * m_etabCB;           //!< Sélection de l'établissement du groupe.
    widgetMPS::IdComboBox * m_groupeCB;         //!< Sélection du groupe.
    widgetMPS::IdComboBox * m_typeCB;           //!< Sélection du type.
    SpinBoxAnneeScolaire * m_anSB;              //!< Sélection de l'année du groupe.

    // Calque
    QHBoxLayout * m_buttonLayout;               //!< Calque des boutons.
    QHBoxLayout * m_eleveLayout;                //!< Claque de sélection des éléves.
    QVBoxLayout * m_findLayout;                 //!< Claque de recherche.
    QHBoxLayout * m_groupeLayout;               //!< Claque de sélection du groupe.
    QVBoxLayout * m_mainLayout;                 //!< Calque principal.

public:
    //! Position des colonnes.
    enum colonne {Nom, Prenom, Naissance, Sexe};

    //! Constructeur.
    GroupeTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const override
        {return static_cast<BddNote &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Listes des éléves");}
    
public slots:
    //! Affecte les éléves séléctionnés dans la vue de candidature au groupe choisi.
    void affecte(int column);

    //! Action à effectuer lorsque l'onglet devient actif.
    void becomeCurrent() override;

    //! Mise à jour après changement de catégorie.
    void catChange();

    //! Retire les élèves sectionnés dans view des groupes.
    void remove();

    //! Slot coorepondant à l'action sauver.
    void sauver() override
        {m_candidatModel->save();}

    //! Mise à jour des classes.
    void updateClasse();

    //! Mise à jour des éléves.
    void updateEleve();
    
    //! Mise à jour des groupes.
    void updateGroupe();

    //! Mise à jour des types.
    void updateType();
};
}
#endif // GROUPETAB_H
