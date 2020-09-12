/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/02/2020
 */
#ifndef NEWMODIFFORM_H
#define NEWMODIFFORM_H

#include <QCalendarWidget>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include "BddNote.h"
#include "Checklist.h"
#include "NewModifDialog.h"
#include "SelectInListBox.h"
#include "SpinBoxAnneeScolaire.h"

namespace noteMPS {
/*! \defgroup groupeDialogNote Dialogues de note
 * \brief Ensemble des dialogues de l'application note.
 */

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création des Annees.
 */
class AnneeNewModifForm : public dialogMPS::AbstractNewModifForm {
    Q_OBJECT
protected:
    QLabel * m_anneeLabel;                  //!< Label du choix de l'année.
    QCheckBox * m_anneeCourante;            //!< CheckBox demandant si l'année crée devient courante.
    SpinBoxAnneeScolaire * m_spinBox;       //!< Choix de l'année.
    QVBoxLayout * m_mainLayout;             //!< Calque Principale.

public:
    //! Constructeur.
    AnneeNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //! Destructeur.
    ~AnneeNewModifForm() override = default;

    //! Accesseur de l'annee courante de spinBox.
    const Annee & annee() const
        {return m_spinBox->value();}

    //! Accesseur de l'état de la case à cocher.
    bool anneeCourante() const
        {return !m_new || m_anneeCourante->isChecked();}

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Évite le bouton supprimer.
    bool delDisable() const noexcept override
        {return true;}

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'une nouvelle année scolaire") :
                        tr("Choix de l'année scolaire courante");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(m_spinBox->value());}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override {}
};

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création et modification des classes.
 */
class ClasseNewModifForm : public dialogMPS::AbstractNcNomNewModifForm {
    Q_OBJECT
protected:
    szt m_idClasse = 0;                         //!< Identifiant de la classe (après enregistrement pour la nouvelle).

    //Widget
    QLabel * m_anneeLabel;                      //!< Label du choix de l'annee.
    QLabel * m_debutLabel;                      //!< Label du choix de la date du dédut des cours.
    QLabel * m_etabLabel;                       //!< Label du choix de l'établissement.
    QLabel * m_finLabel;
    QLabel * m_nivLabel;                        //!< Label du choix du niveau.
    QLabel * m_numLabel;                        //!< Label du numéro de la classe.
    QLabel * m_listeEleveLabel;                 //!< Label de la liste des éléves.
    SpinBoxAnneeScolaire * m_anneeSpinBox;      //!< Choix de l'annee Scolaire.
    QCalendarWidget * m_debutCalendar;          //<! Choix de la date du debut des cours.
    widgetMPS::IdComboBox * m_etabCB;                       //!< Choix de l'établissement.
    QCalendarWidget * m_finCalendar;            //!< Choix de la date de la fin des cours.
    widgetMPS::IdComboBox * m_nivCB;                        //!< Choix du niveau.
    QSpinBox * m_numSpinBox;                    //!< Choix du numéro de la classe.

    //Calque
    QHBoxLayout * m_calendarLayout;             //!< Calque du choix des dates de début et fin des cours.
    QVBoxLayout * m_debutLayout;                //!< Calque du choix du début des cours.
    QVBoxLayout * m_finLayout;                  //!< Calque du choix du fin des cours.

public:
    //! Constructeur.
    ClasseNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //!Destructeur.
    ~ClasseNewModifForm() override = default;

    //! Accesseur de la base de donnée.
    BddNote & bdd()
        {return static_cast<BddNote &>(m_bdd);}

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override {
        return m_new ? tr("Création d'une nouvelle classe") :
                        tr("Modification d'une classe existante");
    }

    //! Teste si le formulaire est valide pour l'enregistrement.
    bool valide() const override;

public slots:
    //! Supprime la classe dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(Classe(id()));}

    //! Sauve la classe et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour l'intervalle des dates.
    void updateCalendar();

    //! Met à jour le formulaire.
    void updateData() override;

    //! Met à jour les niveaux disponible.
    void updateNiveau();
};

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création et modification des établissements.
 */
class EtablissementNewModifForm : public dialogMPS::AbstractNcNomNewModifForm {
    Q_OBJECT
protected:
    QLabel * m_nivLabel;                    //!< Label de la liste des niveaux.
    QLabel * m_TELabel;                     //!< Label de la liste des types d'établissement.
    widgetMPS::Checklist * m_nivList;       //!< Liste des niveaux.
    widgetMPS::Checklist * m_TEList;        //!< Liste des types d'établissement.

public:
    //! Constructeur.
    EtablissementNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //!Destructeur.
    ~EtablissementNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouvel établissement") :
                        tr("Modification d'un établissement existant");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(Etablissement(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override;

    //! Met à jour les niveaux disponible.
    void updateNiveau();
};

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création et modification des groupe.
 */
class GroupeNewModifForm : public dialogMPS::AbstractTypeNcNomNewModifForm {
    Q_OBJECT
protected:
    // Widget
    QLabel * m_alphaLabel;                  //!< Label de l'identifiant des groupes.
    QLabel * m_anneeLabel;                  //!< Label du choix de l'année.
    QLabel * m_classeLabel;                 //!< Label du choix de la classe.
    QLabel * m_etabLabel;                   //!< Label du choix de l'établissement.
    QCheckBox * m_exclusifCheck;            //!< Option de groupe exclusif.
    QCheckBox * m_totalCheck;               //!< Option de groupe total.
    QComboBox * m_alphaCB;                  //!< Choix de l'identifiant des groupes.
    widgetMPS::IdComboBox * m_classeCB;     //!< Choix de la classe.
    widgetMPS::IdComboBox * m_etabCB;       //!< Choix de l'établissement.
    QRadioButton * m_anRadio;               //!< Groupe de type Année.
    QRadioButton * m_clRadio;               //!< Groupe de type classe.
    SpinBoxAnneeScolaire * m_anneeSpinBox;  //!< Choix de l'annee Scolaire.
    QGroupBox * m_anClGr;                   //!< Groupe du choix de groupe année - classe.
    QGroupBox * m_optGr;                    //!< Groupe d'option du groupe.

    // Calque
    QHBoxLayout * m_anClLayout;             //!< Calque du choix de groupe année - classe.
    QHBoxLayout * m_optLayout;              //!< Calque des option du groupe.


public:
    //! Constructeur.
    GroupeNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //!Destructeur.
    ~GroupeNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau groupe") :
                        tr("Modification d'un groupe existant");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(Groupe(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à Jour la liste des classes.
    void updateClasse();

    //! Met à jour le formulaire.
    void updateData() override;
};

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création et modification des niveaux.
 */
class NiveauNewModifForm : public dialogMPS::AbstractTypeNcNomNewModifForm {
    Q_OBJECT
protected:
    QLabel * m_listLabel;                 //!< Label de la liste des types d'établissement.
    QLabel * m_nivPrecLabel;                //!< Label du choix des niveaux précédents.
    QLabel * m_nivSuivLabel;                //!< Label du choix des niveaux précédents.
    widgetMPS::Checklist * m_nivPrecList;            //!< Liste des niveaux précédents.
    widgetMPS::Checklist * m_nivSuivList;            //!< Liste des niveaux suivants
    widgetMPS::Checklist * m_TEList;                 //!< Liste des types d'établissement.
    QHBoxLayout * m_nivLayout;              //!< Calque du choix des niveaux précédents et suivants.
    QVBoxLayout * m_nivPrecLayout;          //!< Calque du choix des niveaux précédents.
    QVBoxLayout * m_nivSuivLayout;          //!< Calque du choix des niveaux suivants.

public:
    //! Constructeur.
    NiveauNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //!Destructeur.
    ~NiveauNewModifForm() override = default;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau niveau") :
                        tr("Modification d'un niveau existant");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(Niveau(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override;
};

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création et modification des types d'établissement.
 */
class TypeEtablissementNewModifForm : public dialogMPS::AbstractNcNomNewModifForm {
    Q_OBJECT
protected:
    widgetMPS::SelectInListBox * m_selectNiv;                 //!< Sélection des niveaux.

public:
    //! Constructeur.
    TypeEtablissementNewModifForm(bddMPS::Bdd & bdd, bool newEnt, QWidget * parent = nullptr);

    //!Destructeur.
    ~TypeEtablissementNewModifForm() override = default;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau type d'établissment") :
                        tr("Modification d'un type d'établissement existant");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(TypeEtablissement(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override;
};
}
#endif // NEWMODIFFORM_H
