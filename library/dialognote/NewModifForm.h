/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/02/2020
 */
#ifndef NEWMODIFFORM_H
#define NEWMODIFFORM_H

#include <QButtonGroup>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include "BddNote.h"
#include "Checklist.h"
#include "EntitySelectWidget.h"
#include "NewModifDialog.h"
#include "SelectInListBox.h"
#include "SpinBoxAnneeScolaire.h"
#include "SpinBox.h"

namespace noteMPS {
/*! \defgroup groupeDialogNote Dialogues de note
 * \brief Ensemble des dialogues de l'application note.
 */

/*! \ingroup groupeDialogNote
 * \brief Classe mère des formulaire de création et modification des controles et de leurs types..
 */
class AbstractControleNewModifForm : public dialogMPS::AbstractParentNcNomNewModifForm {
    Q_OBJECT
protected:
    // Widget
    QLabel * m_decimalLabel;                        //!< Label du choix des décimale.
    QLabel * m_totalLabel;                          //!< Label du choix du total.
    QLabel * m_typeLabel;                           //!< Label du choix d'une notation chiffré ou avec lettre.
    QCheckBox * m_appreciationCheck;                //!< Option d'appreciation.
    QCheckBox * m_barreCheck;                       //!< Option de présence de barre de classement.
    QCheckBox * m_capaciteCheck;                    //!< Option de capacités.
    QCheckBox * m_classementCheck;                  //!< Option de classement.
    QCheckBox * m_commentaireCheck;                 //!< Option de commentaire.
    QCheckBox * m_competenceCheck;                  //!< Option de competences.
    QCheckBox * m_courbeCheck;                      //!< Option de courbe d'ajustement.
    QCheckBox * m_depassementCheck;                 //!< Option de depassement.
    QCheckBox * m_noteCheck;                        //!< Option de controle noté.
    QComboBox * m_decimaleCB;                       //!< Choix des décimales.
    QRadioButton * m_chiffreRadio;                  //!< Note chiffrée.
    QRadioButton * m_lettreRadio;                   //!< Note avec lettre.
    widgetMPS::SpinBoxLettre * m_totalSpinBox;      //!< Choix du total.
    widgetMPS::SpinBoxDecimale * m_minimaSpinBox;   //!< Choix de la barre de classement.
    QGroupBox * m_noteGr;                           //!< Groupe d'option de notation.
    QGroupBox * m_optGr;                            //!< Groupe des options.

    // Calque
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
    AbstractControleNewModifForm(bddMPS::Bdd &bdd, const QString & labelParent,
                                 const QString &labelNc, const QString &labelNom,
                                 bool newEnt, QWidget * parent = nullptr);

    //!Destructeur.
    ~AbstractControleNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

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

    //! Active et désactive les options de notations.
    void noteEnable();

    //! Met à jour la décimale de minima.
    void totalChange()
        {m_minimaSpinBox->setMaximumDouble(m_totalSpinBox->value());}

    //! Passe d'un type de notation à l'autre.
    void typeNoteChange();

protected:
    //! Crée une entitée avec les données du formualire.
    template<class Ent> Ent entityNoteOption(bool noteOption);

    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void updateNoteOption(Ent & entity);
};

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
    idt m_idClasse = 0;                         //!< Identifiant de la classe (après enregistrement pour la nouvelle).

    //Widget
    QLabel * m_debutLabel;                      //!< Label du choix de la date du dédut des cours.
    QLabel * m_finLabel;                        //!< Label du choix de la date de la fin des cours.
    QLabel * m_numLabel;                        //!< Label du numéro de la classe.
    QLabel * m_listeEleveLabel;                 //!< Label de la liste des éléves.
    AnneeSelectWidget * m_anneeSelect;          //!< Choix de l'annee Scolaire.
    EtablissementSelectWidget * m_etabSelect;   //!< Choix de l'établissement.
    NiveauxSelectWidget * m_nivSelect;          //!< Choix du niveau.
    QCalendarWidget * m_debutCalendar;          //<! Choix de la date du debut des cours.
    QCalendarWidget * m_finCalendar;            //!< Choix de la date de la fin des cours.
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
};

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création et modification des types de controle.
 */
class ControleNewModifForm : public AbstractControleNewModifForm {
    Q_OBJECT
protected:
    // Widget
    int m_cibleEntitySelect;                                       //!< Identifiant du type d'entité sélectionné.
    QLabel * m_dateLabel;                                       //!< Date et heure du controle.
    QLabel * m_numLabel;                                        //!< Numéro du controle.
    QButtonGroup * m_pourBG;                                    //!< Groupe des boutons de destination.
    QDateTimeEdit * m_dateTimeEdit;                             //!< Choix de la date du controle.
    widgetMPS::AbstractEntitySelectWidget * m_entitySelect;     //!< Choix de la destination.
    QRadioButton * m_classeRadio;                                //!< Destination pour classe.
    QRadioButton * m_eleveRadio;                                 //!< Destination pour eleve.
    QRadioButton * m_groupeRadio;                                //!< Destination pour groupe.
    widgetMPS::SpinBoxNumExclu * m_numSpinBox;                                    //!< Choix du numéro du controle.
    QGroupBox * m_pourGr;                                       //!< Choix de la destination du controle.

    // Calque
    QHBoxLayout * m_pourButtonLayout;       //!< Calque de positionnement des boutons de destination.
    QVBoxLayout * m_pourLayout;             //!< Calque du groupe du choix de la destination du controle.

public:
    //! Constructeur.
    ControleNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent = nullptr);

    //!Destructeur.
    ~ControleNewModifForm() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau  controle :") :
                        tr("Modification d'un controle :");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(Controle(id()));}

    //! Sauve le type de controle.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override;

    //! Met à jour la liste des numéro exclu.
    void updateNum();

    //! Met à jour le type de séléction d'entité.
    void updateSelectEntity(int ID);
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
    QCheckBox * m_exclusifCheck;            //!< Option de groupe exclusif.
    QCheckBox * m_totalCheck;               //!< Option de groupe total.
    QComboBox * m_alphaCB;                  //!< Choix de l'identifiant des groupes.
    ClasseSelectWidget * m_classeSelect;    //!< Choix de la classe.
    QRadioButton * m_anRadio;               //!< Groupe de type Année.
    QRadioButton * m_clRadio;               //!< Groupe de type classe.
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
class TypeControleNewModifForm : public AbstractControleNewModifForm {
    Q_OBJECT
protected:
    // Widget
    QCheckBox * m_appreciationModifCheck;           //!< Option d'appreciation modifiable.
    QCheckBox * m_barreModifCheck;                  //!< Option de présence barre de classement modifiable.
    QCheckBox * m_capaciteModifCheck;               //!< Option de capacités modifiable.
    QCheckBox * m_categorieCheck;                   //!< Option de catégorie.
    QCheckBox * m_classementModifCheck;             //!< Option de classement modifiable.
    QCheckBox * m_commentaireModifCheck;            //!< Option de commentaire modifiable.
    QCheckBox * m_competenceModifCheck;             //!< Option de competences modifiable.
    QCheckBox * m_courbeModifCheck;                 //!< Option de courbe d'ajustement modifiable.
    QCheckBox * m_decimaleModifCheck;               //!< Option de decimale modifiable.
    QCheckBox * m_depassementModifCheck;            //!< Option de depassement modifiable.
    QCheckBox * m_minimaModifCheck;                 //!< Option de barre modifiable.
    QCheckBox * m_noteModifCheck;                   //!< Option de controle noté modifiable.
    QCheckBox * m_typeNoteModifCheck;               //!< Option de type de note modifiable.
    QCheckBox * m_totalModifCheck;                  //!< Option de total modifiable.
    QGroupBox * m_modifGr;                          //!< Groupe des options de modifications.

    // Calque
    QGridLayout * m_modifLayout;                    //!< Calque des options de modifiactions.

    //! Position des widget dans le calques des notes.
    enum position { NoteModifColonne = ColonneUne,
                    NombreModifColonne = ColonneDeux,
                    OptionModifColonne = ColonneZero
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
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(TypeControle(id()));}

    //! Active et désactive les options modification de note.
    void noteModifEnable();

    //! Active et désactive toutes les options du type de controle.
    void optionEnable();

    //! Sauve le type de controle.
    void save() override;

    //! Met à jour le formulaire.
    void updateData() override;
};

///////////////////////////////////////////////// Définition //////////////////////////////////////////////////
template<class Ent> Ent AbstractControleNewModifForm::entityNoteOption(bool noteOption) {
    Ent entity;
    if(!m_new)
        entity.setId(id());
    entity.setNc(nc());
    entity.setNom(nom());
    if(noteOption) {
        //Note
        if(m_noteCheck->isChecked()){
            entity.add(TypeControle::Note);
            entity.setTotal(m_totalSpinBox->value());
            entity.setDecimale(m_decimaleCB->currentData().toInt());
            if(m_chiffreRadio->isChecked()) {
                if(m_depassementCheck->isChecked())
                    entity.add(TypeControle::Depassement);
                if(m_courbeCheck->isChecked())
                    entity.add(TypeControle::Courbe);
                if(m_classementCheck->isChecked()) {
                    entity.add(TypeControle::Classement);
                    if(m_barreCheck->isChecked()){
                        entity.add(TypeControle::Barre);
                        entity.setMinima(m_minimaSpinBox->value());
                    }
                }
            }
            else
                entity.add(TypeControle::Lettre);
        }
        else
            entity.setTotal(1);
        //Option
        if(m_appreciationCheck->isChecked())
            entity.add(TypeControle::Appreciation);
        if(m_capaciteCheck->isChecked())
            entity.add(TypeControle::Capacites);
        if(m_commentaireCheck->isChecked())
            entity.add(TypeControle::Commentaire);
        if(m_competenceCheck->isChecked())
            entity.add(TypeControle::Competences);
    }
    return entity;
}

template<class Ent> void AbstractControleNewModifForm::updateNoteOption(Ent & entity) {
    //Note
    m_noteCheck->setChecked(entity.code().test(TypeControle::Note));
    if(m_noteCheck->isChecked()){
        m_lettreRadio->setChecked(entity.code().test(TypeControle::Lettre));
        m_totalSpinBox->setValue(entity.total());
        auto index = m_decimaleCB->findData(entity.decimale());
        if(index != -1)
            m_decimaleCB->setCurrentIndex(index);
        if(m_chiffreRadio->isChecked()){
            m_depassementCheck->setChecked(entity.code().test(TypeControle::Depassement));
            m_courbeCheck->setChecked(entity.code().test(TypeControle::Courbe));
            m_classementCheck->setChecked(entity.code().test(TypeControle::Classement));
            if(m_classementCheck->isChecked()){
                m_barreCheck->setChecked(entity.code().test(TypeControle::Barre));
                if(m_barreCheck->isChecked())
                    m_minimaSpinBox->setValue(entity.minima());
            }
        }
    }
    //Option
    m_appreciationCheck->setChecked(entity.code().test(TypeControle::Appreciation));
    m_capaciteCheck->setChecked(entity.code().test(TypeControle::Capacites));
    m_commentaireCheck->setChecked(entity.code().test(TypeControle::Commentaire));
    m_competenceCheck->setChecked(entity.code().test(TypeControle::Competences));
}
}
#endif // NEWMODIFFORM_H
