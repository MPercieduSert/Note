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
#include "AbstractFindModel.h"
#include "AbstractTabTableau.h"
#include "BddNote.h"
#include "FindWidget.h"
#include "TableauNote.h"

namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Formulaire de création des Annees.
 */
class ElevesTab : public fenMPS::AbstractTabTableau {
    Q_OBJECT
protected:
    bool m_findEnabled = true;      //! Autorise la rechercha.
//    QCheckBox * m_sexeFemBox;             //!< Case à cocher de fille.
//    QCheckBox * m_sexeMasBox;             //!< Case à cocher de garçon.
    QGroupBox * m_addGroup;         //!< Groupe du choix nombre d'éléves à ajouter.
    QGroupBox * m_findGroup;        //!< Groupe de recherche.
//    QLabel * m_apresLabel;          //!< Label de la date de naissance après.
//    QLabel * m_avantLabel;          //!< Label de la date de naissance avant.
//    QLabel * m_nomLabel;            //!< Label de recherche du nom.
//    QLabel * m_prenomLabel;         //!< Label de recherche du prenom.
//    QLabel * m_sexeLabel;           //!< Label de recherche du sexe.
//    QDateEdit * m_apresEdit;        //!< Champ de recherche de la date de naissance après.
//    QDateEdit * m_avantEdit;        //!< Champ de recherche de la date de naissance avant.
//    QLineEdit * m_nomEdit;          //!< Champ de recherche du nom.
//    QLineEdit * m_prenomEdit;       //!< Champ de recherche du prenom.
    QPushButton * m_addButton;      //!< Bouton de l'ajout des éléves dans le tableau.
    QPushButton * m_delButton;      //!< Bouton de suppression d'éléves.
//    QPushButton * m_resetFindButton;        //!< Bouton de reset de recherche.
    QSpinBox * m_addSpinBox;        //!< Choix du nombre l'éléve à ajouter.
    widgetMPS::FindWidget * m_findWidget;      //!< Recherche dans la liste d'éléve.

    QVBoxLayout * m_findLayout;     //!< Claque de recherche.
    QHBoxLayout * m_addLayout;      //!< Calque de l'ajout d'éléve dans le tableau.
    QHBoxLayout * m_mainLayout;     //!< Calque principal.
    QVBoxLayout * m_secondLayout;   //!< Calque secondaire.

public:
    //! Position des colonnes.
    enum colonne {Nom, Prenom, Naissance, Sexe};

    //! Constructeur.
    ElevesTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const override
        {return static_cast<BddNote &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Listes des éléves");}

public slots:
    //! Recherche les éléves vérifiant les informations de recherche.
//    void find();

    //! Supprime les élèves séléctionnés.
    void remove();
};
/*
class Eleves2Tab : public fenMPS::AbstractTabTableau {
    Q_OBJECT
protected:
//    bool m_findEnabled = true;      //! Autorise la rechercha.
//    QCheckBox * m_sexeFemBox;             //!< Case à cocher de fille.
//    QCheckBox * m_sexeMasBox;             //!< Case à cocher de garçon.
//    QGroupBox * m_addGroup;         //!< Groupe du choix nombre d'éléves à ajouter.
//    QGroupBox * m_findGroup;        //!< Groupe de recherche.
//    QLabel * m_apresLabel;          //!< Label de la date de naissance après.
//    QLabel * m_avantLabel;          //!< Label de la date de naissance avant.
//    QLabel * m_nomLabel;            //!< Label de recherche du nom.
//    QLabel * m_prenomLabel;         //!< Label de recherche du prenom.
//    QLabel * m_sexeLabel;           //!< Label de recherche du sexe.
//    QDateEdit * m_apresEdit;        //!< Champ de recherche de la date de naissance après.
//    QDateEdit * m_avantEdit;        //!< Champ de recherche de la date de naissance avant.
//    QLineEdit * m_nomEdit;          //!< Champ de recherche du nom.
//    QLineEdit * m_prenomEdit;       //!< Champ de recherche du prenom.
//    QPushButton * m_addButton;      //!< Bouton de l'ajout des éléves dans le tableau.
//    QPushButton * m_delButton;      //!< Bouton de suppression d'éléves.
//    QPushButton * m_resetFindButton;        //!< Bouton de reset de recherche.
//    QSpinBox * m_addSpinBox;        //!< Choix du nombre l'éléve à ajouter.

//    QGridLayout * m_findLayout;     //!< Claque de recherche.
//    QHBoxLayout * m_addLayout;      //!< Calque de l'ajout d'éléve dans le tableau.
    QHBoxLayout * m_mainLayout;     //!< Calque principal.
//    QVBoxLayout * m_secondLayout;   //!< Calque secondaire.

public:
    //! Constructeur.
    Eleves2Tab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const override
        {return static_cast<BddNote &>(m_bdd);}

    //! Renvoie un pointeur sur le model.
    EleveModel * model() const override
        {return static_cast<EleveModel *>(m_model);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Listes des éléves (bêta)");}

public slots:
//    //! Recherche les éléves vérifiant les informations de recherche.
//    void find();

//    //! Supprime les élèves séléctionnés.
//    void remove();
};*/
}
#endif // ELEVESTAB_H
