/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/05/2020
 */
#ifndef CLASSEELEVETAB_H
#define CLASSEELEVETAB_H

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>
#include "abstract_tab_tableau.h"
#include "BddNote.h"
#include "EntitySelectWidget.h"
#include "find_widget.h"
#include "id_combo_box.h"
#include "ModelNote.h"
#include "spin_box_anneeScolaire.h"
#include "TableauNote.h"

namespace noteMPS {
/*! \ingroup groupeDialogNote
 * \brief Formulaire de création des Annees.
 */
class ClasseEleveTab: public fenetre::abstract_tab_tableau {
    Q_OBJECT
protected:
    // Model
    model_base::table_model * m_eleveModel;            //!< Model des élèves à sélectionner pour la classe.

    // Widget
    ClasseSelectWidget * m_classeSelect;            //!< Choix de la classe.
    QPushButton * m_addButton;                      //!< Bouton d'ajout des élèves séléctionnés.
    QPushButton * m_delButton;                      //!< Bouton de retrait des élèves séléctionnés.
    QTableView * m_eleveView;                       //!< Vue de sélection des élèves.
    widget::find_widget * m_eleveFind;            //!< Recherche des des élèves à sélectionner pour la classe.

    // Calque
    QHBoxLayout * m_bouton_layout;                   //!< Calque des boutons.
    QHBoxLayout * m_eleve_layout;                    //!< Calque de séléction des éléves.
    QVBoxLayout * m_main_layout;                     //!< Calque Principal.

public:
    //! position des colonnes du model de sélection de éléves.
    enum colonneEleve {NomEl, PrenomEl, NaissanceEl, SexeEl};

    //! Constructeur.
    ClasseEleveTab(BddNote & bdd, tab_index pair, QWidget * parent = nullptr);

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
