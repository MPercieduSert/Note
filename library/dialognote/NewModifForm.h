/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/02/2020
 */
#ifndef NEWMODIFFORM_H
#define NEWMODIFFORM_H

#include <QListWidget>
#include "NewModifDialog.h"
#include "SelectInListBox.h"
#include "BddNote.h"

namespace noteMPS {
/*! \defgroup groupeDialogNote Dialogues de note
 * \brief Ensemble des dialogues de l'application note.
 */

/*! \ingroup groupeDialogNote
 * \brief Formulaire de création des niveaux.
 */
class NiveauNewModifForm : public dialogMPS::AbstractTypeNcNomNewModifForm {
    Q_OBJECT
protected:
    QLabel * m_listLabel;                 //!< Label de la liste des types d'établissement.
    QLabel * m_nivPrecLabel;                //!< Label du choix des niveaux précédents.
    QLabel * m_nivSuivLabel;                //!< Label du choix des niveaux précédents.
    QListWidget * m_nivPrecList;            //!< Liste des niveaux précédents.
    QListWidget * m_nivSuivList;            //!< Liste des niveaux suivants
    QListWidget * m_TEList;                 //!< Liste des types d'établissement.
    QHBoxLayout * m_nivLayout;              //!< Calque du choix des niveaux précédents et suivants.
    QVBoxLayout * m_nivPrecLayout;          //!< Calque du choix des niveaux précédents.
    QVBoxLayout * m_nivSuivLayout;          //!< Calque du choix des niveaux suivants.

public:
    //! Constructeur.
    NiveauNewModifForm(bddMPS::BddPredef &bdd, bool newEnt, QWidget * parent = nullptr);

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
 * \brief Formulaire de création des type d'établissement.
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
