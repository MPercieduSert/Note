/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/02/2020
 */
#ifndef NEW_MODIF_FORM_H
#define NEW_MODIF_FORM_H

#include <QButtonGroup>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include "bdd_note.h"
#include "check_list.h"
#include "entity_select_widget.h"
#include "new_modif_dialog.h"
#include "select_in_list_box.h"
#include "spin_box_annee_scolaire.h"
#include "spin_box.h"

namespace note_mps {
/*! \defgroup groupe_dialog_note Dialogues de note
 * \brief Ensemble des dialogues de l'application note.
 */

/*! \ingroup groupe_dialog_note
 * \brief Classe mère des formulaire de création et modification des controles et de leurs types..
 */
class abstract_controle_new_modif_form : public mps::dialogue::abstract_parent_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    // Widget
    QLabel *m_decimal_label;                        //!< Label du choix des décimale.
    QLabel *m_total_label;                          //!< Label du choix du total.
    QLabel *m_type_label;                           //!< Label du choix d'une notation chiffré ou avec lettre.
    QCheckBox *m_appreciation_check;                //!< Option d'appreciation.
    QCheckBox *m_barre_check;                       //!< Option de présence de barre de classement.
    QCheckBox *m_capacite_check;                    //!< Option de capacités.
    QCheckBox *m_classement_check;                  //!< Option de classement.
    QCheckBox *m_commentaire_check;                 //!< Option de commentaire.
    QCheckBox *m_competence_check;                  //!< Option de competences.
    QCheckBox *m_courbe_check;                      //!< Option de courbe d'ajustement.
    QCheckBox *m_depassement_check;                 //!< Option de depassement.
    QCheckBox *m_note_check;                        //!< Option de controle noté.
    QComboBox *m_decimale_cb;                       //!< Choix des décimales.
    QRadioButton *m_chiffre_radio;                  //!< Note chiffrée.
    QRadioButton *m_lettre_radio;                   //!< Note avec lettre.
    mps::widget::spin_box_lettre *m_total_spin_box;      //!< Choix du total.
    mps::widget::spin_box_decimale *m_minima_spin_box;   //!< Choix de la barre de classement.
    QGroupBox *m_note_gr;                           //!< Groupe d'option de notation.
    QGroupBox *m_opt_gr;                            //!< Groupe des options.

    // Calque
    QGridLayout *m_note_layout;                     //!< Calque des options de notation.
    QGridLayout *m_opt_layout;                      //!< Calque des options.

    //! position des widget dans le calques des notes.
    enum position { Colonne_Zero = 0,
                    Colonne_Une = 1,
                    Colonne_Deux = 2,
                    Ligne_Zero = 0,
                    Ligne_Une = 1,
                    Ligne_Deux = 2,
                    Ligne_Trois = 3,
                    Ligne_Quatre = 4,
                    Ligne_Cinq = 5,
                    Label_Colonne = Colonne_Zero,
                    Check_Note_Ligne = Ligne_Zero,
                    Type_Ligne = Ligne_Une,
                    Total_Ligne = Ligne_Deux,
                    Decimale_Ligne = Ligne_Trois,
                    Classement_Ligne = Ligne_Quatre,
                    Barre_Ligne = Ligne_Cinq,
                    Appreciation_Commentaire_Ligne = Ligne_Zero,
                    Capacite_Competence_Ligne = Ligne_Une
    };

public:
    //! Constructeur.
    abstract_controle_new_modif_form(mps::b2d::bdd &bdd, const QString &label_parent,
                                 const QString &label_nc, const QString &label_nom,
                                 bool new_ent, QWidget *parent = nullptr);

    //!Destructeur.
    ~abstract_controle_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

public slots:
    //! Active et désactive les options de barre de classement.
    void barre_enable();

    //! Active et désactive les options de classement.
    void classement_enable();

    //! Met à jour la décimale de minima.
    void decimale_change() {
        m_minima_spin_box->set_precision(
                    mps::attribut::attribut_decimale::Precision_decimale(m_decimale_cb->currentData().toInt()));
        m_minima_spin_box->set_decimale(m_decimale_cb->currentData().toInt());
    }

    //! Active et désactive les options de notations.
    void note_enable();

    //! Met à jour la décimale de minima.
    void total_change()
        {m_minima_spin_box->set_maximum_double(m_total_spin_box->value());}

    //! Passe d'un type de notation à l'autre.
    void type_note_change();

protected:
    //! Crée une entitée avec les données du formualire.
    template<class Ent> Ent entity_note_option(bool note_option);

    //! Récupère l'entité en base de donné et met à jours les données du formulaire
    template<class Ent> void update_note_option(Ent & ent);
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création des annees.
 */
class annee_new_modif_form : public mps::dialogue::abstract_new_modif_form {
    Q_OBJECT
protected:
    QLabel *m_annee_label;                  //!< Label du choix de l'année.
    QCheckBox *m_annee_courante;            //!< CheckBox demandant si l'année crée devient courante.
    spin_box_annee_scolaire *m_spin_box;       //!< Choix de l'année.
    QVBoxLayout *m_main_layout;             //!< Calque Principale.

public:
    //! Constructeur.
    annee_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //! Destructeur.
    ~annee_new_modif_form() override = default;

    //! Accesseur de l'annee courante de spinBox.
    const annee &get_annee() const
        {return m_spin_box->value();}

    //! Accesseur de l'état de la case à cocher.
    bool annee_courante() const
        {return !m_new || m_annee_courante->isChecked();}

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Évite le bouton supprimer.
    bool del_disable() const noexcept override
        {return true;}

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'une nouvelle année scolaire") :
                        tr("Choix de l'année scolaire courante");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(m_spin_box->value());}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override {}
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création et modification des classes.
 */
class classe_new_modif_form : public mps::dialogue::abstract_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    idt m_id_classe = 0;                         //!< Identifiant de la classe (après enregistrement pour la nouvelle).

    //Widget
    QLabel *m_debut_label;                      //!< Label du choix de la date du dédut des cours.
    QLabel *m_fin_label;                        //!< Label du choix de la date de la fin des cours.
    QLabel *m_num_label;                        //!< Label du numéro de la classe.
    QLabel *m_liste_eleve_label;                 //!< Label de la liste des éléves.
    annee_select_widget *m_annee_select;          //!< Choix de l'annee Scolaire.
    etablissement_select_widget *m_etab_select;   //!< Choix de l'établissement.
    niveaux_select_widget *m_niv_select;          //!< Choix du niveau.
    QCalendarWidget *m_debut_calendar;          //<! Choix de la date du debut des cours.
    QCalendarWidget *m_fin_calendar;            //!< Choix de la date de la fin des cours.
    QSpinBox *m_num_spin_box;                    //!< Choix du numéro de la classe.

    //Calque
    QHBoxLayout *m_calendar_layout;             //!< Calque du choix des dates de début et fin des cours.
    QVBoxLayout *m_debut_layout;                //!< Calque du choix du début des cours.
    QVBoxLayout *m_fin_layout;                  //!< Calque du choix du fin des cours.

public:
    //! Constructeur.
    classe_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //!Destructeur.
    ~classe_new_modif_form() override = default;

    //! Accesseur de la base de donnée.
    bdd_note & bdd()
        {return static_cast<bdd_note &>(m_bdd);}

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
        {return !m_new && m_bdd.del(classe(id()));}

    //! Sauve la classe et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour l'intervalle des dates.
    void update_calendar();

    //! Met à jour le formulaire.
    void update_data() override;
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création et modification des types de controle.
 */
class controle_new_modif_form : public abstract_controle_new_modif_form {
    Q_OBJECT
protected:
    // Widget
    int m_cible_entity_select;                                       //!< Identifiant du type d'entité sélectionné.
    QLabel *m_date_label;                                       //!< Date et heure du controle.
    QLabel *m_num_label;                                        //!< Numéro du controle.
    QButtonGroup *m_pour_bg;                                    //!< Groupe des boutons de destination.
    QDateTimeEdit *m_date_time_edit;                             //!< Choix de la date du controle.
    mps::widget::abstract_entity_select_widget *m_entity_select;     //!< Choix de la destination.
    QRadioButton *m_classe_radio;                                //!< Destination pour classe.
    QRadioButton *m_eleve_radio;                                 //!< Destination pour eleve.
    QRadioButton *m_groupe_radio;                                //!< Destination pour groupe.
    mps::widget::spin_box_num_exclu *m_num_spin_box;                                    //!< Choix du numéro du controle.
    QGroupBox *m_pour_gr;                                       //!< Choix de la destination du controle.

    // Calque
    QHBoxLayout *m_pour_bouton_layout;       //!< Calque de positionnement des boutons de destination.
    QVBoxLayout *m_pour_layout;             //!< Calque du groupe du choix de la destination du controle.

public:
    //! Constructeur.
    controle_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //!Destructeur.
    ~controle_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau  controle :") :
                        tr("Modification d'un controle :");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(controle(id()));}

    //! Sauve le type de controle.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;

    //! Met à jour la liste des numéro exclu.
    void update_num();

    //! Met à jour le type de séléction d'entité.
    void update_select_entity(int id);
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création et modification des établissements.
 */
class etablissement_new_modif_form : public mps::dialogue::abstract_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    QLabel *m_niv_label;                    //!< Label de la liste des niveaux.
    QLabel *m_te_label;                     //!< Label de la liste des types d'établissement.
    mps::widget::check_list *m_niv_list;    //!< Liste des niveaux.
    mps::widget::check_list *m_te_list;     //!< Liste des types d'établissement.

public:
    //! Constructeur.
    etablissement_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //!Destructeur.
    ~etablissement_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouvel établissement") :
                        tr("Modification d'un établissement existant");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(etablissement(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;

    //! Met à jour les niveaux disponible.
    void update_niveau();
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création et modification des groupe.
 */
class groupe_new_modif_form : public mps::dialogue::abstract_type_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    // Widget
    QLabel *m_alpha_label;                  //!< Label de l'identifiant des groupes.
    QCheckBox *m_exclusif_check;            //!< Option de groupe exclusif.
    QCheckBox *m_total_check;               //!< Option de groupe total.
    QComboBox *m_alpha_cb;                  //!< Choix de l'identifiant des groupes.
    classe_select_widget *m_classe_select;    //!< Choix de la classe.
    QRadioButton *m_an_radio;               //!< Groupe de type Année.
    QRadioButton *m_cl_radio;               //!< Groupe de type classe.
    QGroupBox *m_an_cl_gr;                   //!< Groupe du choix de groupe année - classe.
    QGroupBox *m_opt_gr;                    //!< Groupe d'option du groupe.

    // Calque
    QHBoxLayout *m_an_cl_layout;             //!< Calque du choix de groupe année - classe.
    QHBoxLayout *m_opt_layout;              //!< Calque des option du groupe.


public:
    //! Constructeur.
    groupe_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //!Destructeur.
    ~groupe_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau groupe") :
                        tr("Modification d'un groupe existant");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(groupe(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création et modification des niveaux.
 */
class niveau_new_modif_form : public mps::dialogue::abstract_type_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    QLabel *m_list_label;                 //!< Label de la liste des types d'établissement.
    QLabel *m_niv_prec_label;                //!< Label du choix des niveaux précédents.
    QLabel *m_niv_suiv_label;                //!< Label du choix des niveaux précédents.
    mps::widget::check_list *m_niv_prec_list;            //!< Liste des niveaux précédents.
    mps::widget::check_list *m_niv_suiv_list;            //!< Liste des niveaux suivants
    mps::widget::check_list *m_te_list;                 //!< Liste des types d'établissement.
    QHBoxLayout *m_niv_layout;              //!< Calque du choix des niveaux précédents et suivants.
    QVBoxLayout *m_niv_prec_layout;          //!< Calque du choix des niveaux précédents.
    QVBoxLayout *m_niv_suiv_layout;          //!< Calque du choix des niveaux suivants.

public:
    //! Constructeur.
    niveau_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //!Destructeur.
    ~niveau_new_modif_form() override = default;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau niveau") :
                        tr("Modification d'un niveau existant");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(niveau(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création et modification des types d'établissement.
 */
class type_etablissement_new_modif_form : public mps::dialogue::abstract_nc_nom_new_modif_form {
    Q_OBJECT
protected:
    mps::widget::select_in_list_box *m_niv_select;                 //!< Sélection des niveaux.

public:
    //! Constructeur.
    type_etablissement_new_modif_form(mps::b2d::bdd & bdd, bool new_ent, QWidget *parent = nullptr);

    //!Destructeur.
    ~type_etablissement_new_modif_form() override = default;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau type d'établissment") :
                        tr("Modification d'un type d'établissement existant");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(type_etablissement(id()));}

    //! Sauve le type d'établissement et les réponces du formulairs dans la bases de donnée.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;
};

/*! \ingroup groupe_dialog_note
 * \brief Formulaire de création et modification des types de controle.
 */
class type_controle_new_modif_form : public abstract_controle_new_modif_form {
    Q_OBJECT
protected:
    // Widget
    QCheckBox *m_appreciation_modif_check;           //!< Option d'appreciation modifiable.
    QCheckBox *m_barre_modif_check;                  //!< Option de présence barre de classement modifiable.
    QCheckBox *m_capacite_modif_check;               //!< Option de capacités modifiable.
    QCheckBox *m_categorie_check;                   //!< Option de catégorie.
    QCheckBox *m_classement_modif_check;             //!< Option de classement modifiable.
    QCheckBox *m_commentaire_modif_check;            //!< Option de commentaire modifiable.
    QCheckBox *m_competence_modif_check;             //!< Option de competences modifiable.
    QCheckBox *m_courbe_modif_check;                 //!< Option de courbe d'ajustement modifiable.
    QCheckBox *m_decimale_modif_check;               //!< Option de decimale modifiable.
    QCheckBox *m_depassement_modif_check;            //!< Option de depassement modifiable.
    QCheckBox *m_minima_modif_check;                 //!< Option de barre modifiable.
    QCheckBox *m_note_modif_check;                   //!< Option de controle noté modifiable.
    QCheckBox *m_type_note_modif_check;               //!< Option de type de note modifiable.
    QCheckBox *m_total_modif_check;                  //!< Option de total modifiable.
    QGroupBox *m_modif_gr;                          //!< Groupe des options de modifications.

    // Calque
    QGridLayout *m_modif_layout;                    //!< Calque des options de modifiactions.

    //! position des widget dans le calques des notes.
    enum position { Note_Modif_Colonne = Colonne_Une,
                    Nombre_Modif_Colonne = Colonne_Deux,
                    Option_Modif_Colonne = Colonne_Zero
    };

public:
    //! Constructeur.
    type_controle_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent = nullptr);

    //!Destructeur.
    ~type_controle_new_modif_form() override = default;

    //! Connecte les signals et slots du formulaire.
    void connexion() override;

    //! Titre de la fenêtre de dialogue.
    QString title() const override
        {return m_new ? tr("Création d'un nouveau type de controle :") :
                        tr("Modification d'un type de controle :");}

public slots:
    //! Supprime le type d'établissement dans la bases de donnée.
    bool del() override
        {return !m_new && m_bdd.del(type_controle(id()));}

    //! Active et désactive les options modification de note.
    void note_modif_enable();

    //! Active et désactive toutes les options du type de controle.
    void option_enable();

    //! Sauve le type de controle.
    void save() override;

    //! Met à jour le formulaire.
    void update_data() override;
};

///////////////////////////////////////////////// Définition //////////////////////////////////////////////////
template<class Ent> Ent abstract_controle_new_modif_form::entity_note_option(bool note_option) {
    Ent ent;
    if(!m_new)
        ent.set_id(id());
    ent.set_nc(nc());
    ent.set_nom(nom());
    if(note_option) {
        //Note
        if(m_note_check->isChecked()){
            ent.add(type_controle::Note);
            ent.set_total(m_total_spin_box->value());
            ent.set_decimale(m_decimale_cb->currentData().toInt());
            if(m_chiffre_radio->isChecked()) {
                if(m_depassement_check->isChecked())
                    ent.add(type_controle::Depassement);
                if(m_courbe_check->isChecked())
                    ent.add(type_controle::Courbe);
                if(m_classement_check->isChecked()) {
                    ent.add(type_controle::Classement);
                    if(m_barre_check->isChecked()){
                        ent.add(type_controle::Barre);
                        ent.set_minima(m_minima_spin_box->value());
                    }
                }
            }
            else
                ent.add(type_controle::Lettre);
        }
        else
            ent.set_total(1);
        //Option
        if(m_appreciation_check->isChecked())
            ent.add(type_controle::Appreciation);
        if(m_capacite_check->isChecked())
            ent.add(type_controle::Capacites);
        if(m_commentaire_check->isChecked())
            ent.add(type_controle::Commentaire);
        if(m_competence_check->isChecked())
            ent.add(type_controle::Competences);
    }
    return ent;
}

template<class Ent> void abstract_controle_new_modif_form::update_note_option(Ent & ent) {
    //Note
    m_note_check->setChecked(ent.code().test(type_controle::Note));
    if(m_note_check->isChecked()){
        m_lettre_radio->setChecked(ent.code().test(type_controle::Lettre));
        m_total_spin_box->setValue(ent.total());
        auto index = m_decimale_cb->findData(ent.decimale());
        if(index != -1)
            m_decimale_cb->setCurrentIndex(index);
        if(m_chiffre_radio->isChecked()){
            m_depassement_check->setChecked(ent.code().test(type_controle::Depassement));
            m_courbe_check->setChecked(ent.code().test(type_controle::Courbe));
            m_classement_check->setChecked(ent.code().test(type_controle::Classement));
            if(m_classement_check->isChecked()){
                m_barre_check->setChecked(ent.code().test(type_controle::Barre));
                if(m_barre_check->isChecked())
                    m_minima_spin_box->setValue(ent.minima());
            }
        }
    }
    //Option
    m_appreciation_check->setChecked(ent.code().test(type_controle::Appreciation));
    m_capacite_check->setChecked(ent.code().test(type_controle::Capacites));
    m_commentaire_check->setChecked(ent.code().test(type_controle::Commentaire));
    m_competence_check->setChecked(ent.code().test(type_controle::Competences));
}
}
#endif // NEW_MODIF_FORM_H
