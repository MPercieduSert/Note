/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/05/2020
 */
#ifndef TABLEAUNOTE_H
#define TABLEAUNOTE_H

#include "AbstractColonnesModel.h"
#include "EntityNote.h"

namespace noteMPS {
/*! \defgroup groupeTableauNote Tableau  de note
 * \brief Ensemble des tableaux de données des models de note.
 */

/*! \ingroup groupeModelNote
 * \brief Tableau d'éléves.
 */
class EleveVecTableau : public modelMPS::AbstractBddVectorEntTableau<Eleve> {
public:
    //! Identifiant des colonnes assoiées au tableau d'éléves.
    enum idColonne {Nom,
                    Prenom,
                    Naissance,
                    Sexe};

    //! Colonne des noms des éléves
    class NomColonne : public modelMPS::VectorPtrColonne<Eleve> {
    public:
        //! Constructeur.
        NomColonne(const QString & name, Qt::ItemFlags flags, conteneurMPS::VectorPtr<Eleve> & vec);

        //! Compare entre deux lignes ( ligne1 < ligne2).
        bool compare(szt ligne1, szt ligne2) const override;

    };

    //! Constructeur.
    using modelMPS::AbstractBddVectorEntTableau<Eleve>::AbstractBddVectorEntTableau;

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne1, szt ligne2) const override;


    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
        makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) override;
};
}
#endif // TABLEAUNOTE_H
