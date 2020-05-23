/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/05/2020
 */
#ifndef MODELNOTE_H
#define MODELNOTE_H

#include "BddNote.h"
#include "TableModel.h"
#include "TableauNote.h"

namespace noteMPS {
/*! \ingroup groupeModelNote
 * \brief Model des élèves d'une classe.
 */
class ClasseEleveModel : public modelMPS::TableModel {
    Q_OBJECT
public:
    //! Position des colonnes du model des éléves affectés à la classe.
    enum colonneClasseEleve {Nom, Prenom, Naissance, Sexe, Entree, Sortie};

    //! Constructeur.
    ClasseEleveModel(BddNote & bdd, szt idClasse, QObject * parent = nullptr);

    //! Ajoute un élève à la classe à partir de son identifiant.
    void add(szt idEleve);
public slots:
    //! Mutateur de l'identifiant de la classe.
    void setIdClasse(szt idClasse){
        beginResetModel();
            static_cast<ClasseEleveCompositionTableau&>(*m_data).setIdClasse(idClasse);
            resetRowToLigne();
        endResetModel();
    }
};
}
#endif // MODELNOTE_H
