/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/05/2020
 */
#ifndef CLASSEELEVETAB_H
#define CLASSEELEVETAB_H

#include "AbstractTabTableau.h"
#include "BddNote.h"
#include "FindWidget.h"
#include "TableauNote.h"

namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Formulaire de création des Annees.
 */
class ClasseEleveTab: public fenMPS::AbstractTabTableau {
    Q_OBJECT
protected:

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
};
}
#endif // CLASSEELEVETAB_H
