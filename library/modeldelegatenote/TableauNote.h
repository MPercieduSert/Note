/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/05/2020
 */
#ifndef TABLEAUNOTE_H
#define TABLEAUNOTE_H

#include "AbstractColonnesModel.h"
#include "BddNote.h"
#include "EntityNote.h"


/*! \defgroup groupeTableauNote Tableau  de note
 * \brief Ensemble des tableaux de données des models de note.
 */

namespace noteMPS {
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
    class NomColonne : public modelMPS::IdVectorPtrColonne<Eleve> {
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

/*! \ingroup groupeModelNote
 * \brief Tableau d'un vecteur de classe-eleve.
 */
class ClasseEleveVecTableau : public modelMPS::AbstractBddVectorEntTableau<ClasseEleve> {
public:
    //! Identifiant des colonnes assoiées au tableau d'éléves.
    enum idColonne {Entree,
                    Sortie};

    //! Constructeur.
    using modelMPS::AbstractBddVectorEntTableau<ClasseEleve>::AbstractBddVectorEntTableau;

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
        makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) override;
};

/*! \ingroup groupeModelNote
 * \brief Tableau d'un vecteur de classe-eleve.
 */
class ClasseEleveCompositionTableau : public modelMPS::CompositionTableau {
protected:
    BddNote & m_bdd;    //! Lien avec la base de données.
    szt m_idClasse;     //! Identifiant de la classe à la quelle les élèves appartiennent.
public:
    //! Position des tableaux.
    enum positionTableau {EleveTableau, ClasseEleveTableau, NbrTableau};

    //! Constructeur.
    ClasseEleveCompositionTableau(BddNote & bdd, szt idClasse);

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override {
        m_tableaux.front()->hydrate(ligne);
        hydrateClasseEleve(ligne);
    }

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrateClasseEleve(szt ligne);

    //! Acceseur de l'identifiant de la classe.
    szt idClasse() const
        {return m_idClasse;}

    //! Mutateur de l'identifiant de la classe.
    void setIdClasse(szt id);
};
}
#endif // TABLEAUNOTE_H
