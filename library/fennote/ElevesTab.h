/*Auteur: PERCIE DU SERT Maxime
 *Date: 12/04/2020
 */
#ifndef ELEVESTAB_H
#define ELEVESTAB_H

#include <QCalendarWidget>
#include <QCheckBox>
#include <QDateEdit>
#include <QGroupBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QSpinBox>
#include "AbstractTabTableau.h"
#include "BddNote.h"
#include "find_widget.h"
#include "TableauNote.h"

namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Formulaire de création des Annees.
 */
class ElevesTab : public fenMPS::AbstractTabTableau {
    Q_OBJECT
protected:
    bool m_findEnabled = true;      //! Autorise la rechercha.
    QGroupBox * m_addGroup;         //!< Groupe du choix nombre d'éléves à ajouter.
    QGroupBox * m_findGroup;        //!< Groupe de recherche.
    QPushButton * m_addButton;      //!< Bouton de l'ajout des éléves dans le tableau.
    QPushButton * m_delButton;      //!< Bouton de suppression d'éléves.
    QSpinBox * m_addSpinBox;        //!< Choix du nombre l'éléve à ajouter.
    widget::find_widget * m_findWidget;      //!< Recherche dans la liste d'éléve.

    QVBoxLayout * m_findLayout;     //!< Claque de recherche.
    QHBoxLayout * m_addLayout;      //!< Calque de l'ajout d'éléve dans le tableau.
    QHBoxLayout * m_main_layout;     //!< Calque principal.
    QVBoxLayout * m_secondLayout;   //!< Calque secondaire.

public:
    //! position des colonnes.
    enum colonne {Nom, Prenom, Naissance, Sexe};

    //! Constructeur.
    ElevesTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const override
        {return static_cast<BddNote &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Listes des éléves");}
};
}
#endif // ELEVESTAB_H
