/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/05/2020
 */
#ifndef CLASSEELEVETAB_H
#define CLASSEELEVETAB_H

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include "AbstractTabTableau.h"
#include "BddNote.h"
#include "FindWidget.h"
#include "IdComboBox.h"
#include "SpinBoxAnneeScolaire.h"
#include "TableauNote.h"

namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Formulaire de création des Annees.
 */
class ClasseEleveTab: public fenMPS::AbstractTabTableau {
    Q_OBJECT
protected:
    // Widget
    QLabel * m_anLabel;                             //!< Label du choix de l'année de la classe.
    QLabel * m_classeLabel;                         //!< Label du choix de la classe.
    widgetMPS::IdComboBox * m_classeComboBox;       //!< Selection de la classe.
    SpinBoxAnneeScolaire * m_anSpinBox;             //!< Selection de l'année de la classe.

    // Calque
    QHBoxLayout *  m_classeLayout;                  //!< Calque du choix de la classe.
    QHBoxLayout * m_eleveLayout;                    //!< Calque de séléction des éléves.
    QVBoxLayout * m_mainLayout;                     //!< Calque Principal.

public:
    //! Position des colonnes.
    enum colonne {Nom, Prenom, Naissance, Sexe};

    //! Constructeur.
    ClasseEleveTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const override
        {return static_cast<BddNote &>(m_bdd);}

    //! Accesseur du titre.
    QString title() const override
        {return tr("Classe: Eleves");}

public slots:
    //! Supprime les élèves séléctionnés.
    void remove() {}

    //! Met-à-jour la liste des classes vis-à-vis de l'année.
    void updateClasseListe();
};
}
#endif // CLASSEELEVETAB_H
