/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/02/2020
 */
#ifndef NEWMODIFFORM_H
#define NEWMODIFFORM_H

#include <QCalendarWidget>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include "BddNote.h"
#include "Checklist.h"
#include "NewModifDialog.h"
#include "SelectInListBox.h"
#include "SpinBoxAnneeScolaire.h"
#include "SpinBox.h"

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

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création et modification des types de controle.
 */
class TypeControleNewModifForm : public dialogMPS::AbstractParentNcNomNewModifForm {
    Q_OBJECT
protected:
    // Widget
    QLabel * m_decimalLabel;                        //!< Label du choix des décimale.
    QLabel * m_totalLabel;                          //!< Label du choix du total.
    QLabel * m_typeLabel;                           //!< Label du choix d'une notation chiffré ou avec lettre.
    QCheckBox * m_appreciationCheck;                //!< Option d'appreciation.
    QCheckBox * m_appreciationModifCheck;           //!< Option d'appreciation modifiable.
    QCheckBox * m_barreCheck;                       //!< Option de présence de barre de classement.
    QCheckBox * m_barreModifCheck;                  //!< Option de présence barre de classement modifiable.
    QCheckBox * m_capaciteCheck;                    //!< Option de capacités.
    QCheckBox * m_capaciteModifCheck;               //!< Option de capacités modifiable.
    QCheckBox * m_categorieCheck;                   //!< Option de catégorie.
    QCheckBox * m_classementCheck;                  //!< Option de classement.
    QCheckBox * m_classementModifCheck;             //!< Option de classement modifiable.
    QCheckBox * m_commentaireCheck;                 //!< Option de commentaire.
    QCheckBox * m_commentaireModifCheck;            //!< Option de commentaire modifiable.
    QCheckBox * m_competenceCheck;                  //!< Option de competences.
    QCheckBox * m_competenceModifCheck;             //!< Option de competences modifiable.
    QCheckBox * m_courbeCheck;                      //!< Option de courbe d'ajustement.
    QCheckBox * m_courbeModifCheck;                 //!< Option de courbe d'ajustement modifiable.
    QCheckBox * m_decimaleModifCheck;               //!< Option de decimale modifiable.
    QCheckBox * m_depassementCheck;                 //!< Option de depassement.
    QCheckBox * m_depassementModifCheck;            //!< Option de depassement modifiable.
    QCheckBox * m_minimaModifCheck;                 //!< Option de barre modifiable.
    QCheckBox * m_noteCheck;                        //!< Option de controle noté.
    QCheckBox * m_noteModifCheck;                   //!< Option de controle noté modifiable.
    QCheckBox * m_typeNoteModifCheck;               //!< Option de type de note modifiable.
    QCheckBox * m_totalModifCheck;                  //!< Option de total modifiable.
    QComboBox * m_decimaleCB;                       //!< Choix des décimales.
    QRadioButton * m_chiffreRadio;                  //!< Note chiffrée.
    QRadioButton * m_lettreRadio;                   //!< Note avec lettre.
    widgetMPS::SpinBoxLettre * m_totalSpinBox;      //!< Choix du total.
    widgetMPS::SpinBoxDecimale * m_minimaSpinBox;   //!< Choix de la barre de classement.
    QGroupBox * m_modifGr;                          //!< Groupe des options de modifications.
    QGroupBox * m_noteGr;                           //!< Groupe d'option de notation.
    QGroupBox * m_optGr;                            //!< Groupe des options.

    // Calque
    QGridLayout * m_modifLayout;                    //!< Calque des options de modifiactions.
    QGridLayout * m_noteLayout;                     //!< Calque des options de notation.
    QGridLayout * m_optLayout;                      //!< Calque des options.

    //! Position des widget dans le calques des notes.
    enum position { ColonneZero = 0,
                    ColonneUne = 1,
                    ColonneDeux = 2,
                    LigneZero = 0,
                    LigneUne = 1,
                    LigneDeux = 2,
                    LigneTrois = 3,
                    LigneQuatre = 4,
                    LigneCinq = 5,
                    LabelColonne = ColonneZero,
                    NoteModifColonne = ColonneUne,
                    NombreModifColonne = ColonneDeux,
                    OptionModifColonne = ColonneZero,
                    CheckNoteLigne = LigneZero,
                    TypeLigne = LigneUne,
                    TotalLigne = LigneDeux,
                    DecimaleLigne = LigneTrois,
                    ClassementLigne = LigneQuatre,
                    BarreLigne = LigneCinq,
                    AppreciationCommentaireLigne = LigneZero,
                    CapaciteCompetenceLigne = LigneUne
    };

public:
    //! Constructeur.
    TypeControleNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //!Destructeur.
    ~TypeControleNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau type de controle :") :
                        tr("Modification d'un type de controle :");}

public slots:
    //! Active et désactive les options de barre de classement.
    void barreEnable();

    //! Active et désactive les options de classement.
    void classementEnable();

    //! Met à jour la décimale de minima.
    void decimaleChange() {
        m_minimaSpinBox->setPrecision(attributMPS::AttributDecimale::precisionDecimale(m_decimaleCB->currentData().toInt()));
        m_minimaSpinBox->setDecimale(m_decimaleCB->currentData().toInt());
    }

    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(TypeControle(id()));}

    //! Active et désactive les options de notations.
    void noteEnable();

    //! Active et désactive les options modification de note.
    void noteModifEnable();

    //! Active et désactive toutes les options du type de controle.
    void optionEnable();

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour la décimale de minima.
    void totalChange()
        {m_minimaSpinBox->setMaximumDouble(m_totalSpinBox->value());}

    //! Passe d'un type de notation à l'autre.
    void typeNoteChange();

    //! Met à jour le formulaire.
    void updateData() override;
};
}
#endif // NEWMODIFFORM_H
