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
 * \brief Tableau des candidats pour les groupes.
 */
class CandidatGroupeTableau : public modelMPS::CompositionTableau {
protected:
    BddNote & m_bdd;    //! Lien avec la base de données.
    Groupe m_groupe;     //! Identifiant du groupe.
public:
    //! Position des tableaux.
    enum positionTableau {EleveTableau, EleveGroupeTableau, DonneeTableau, NbrTableau};
    enum {NonAffecter = -1, NumRole = Qt::UserRole + 1};
    //! Constructeur.
    CandidatGroupeTableau(BddNote & bdd, szt idGroupe);

    //! Hydrate la donnée d'une ligne du tableau.
    void hydrate(szt ligne) override {
        m_tableaux.front()->hydrate(ligne);
        hydrateEleveGroupe(ligne);
    }

    //! Hydrate la donnée d'une ligne du tableau.
    void hydrateEleveGroupe(szt ligne);

    //! Acceseur de groupe.
    const Groupe  & groupe() const
        {return m_groupe;}
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
        bool compare(szt ligne1, szt ligne2) const override
            {return m_vec[ligne1] < m_vec[ligne2];}
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
 * \brief Tableau des éléves dans les groupes.
 */
class EleveGroupeTableau : public modelMPS::HeterogeneTailleTableau<Eleve> {
protected:
    BddNote & m_bdd;        //! Lien avec la base de données.
    Groupe m_groupe;        //! Identifiant du groupe.
public:
    //! Constructeur.
    EleveGroupeTableau(BddNote & bdd)
        : m_bdd(bdd) {}

    //! Ajoute un éléve à un groupe.
    void addEleve(const std::list<szt> & listEl, szt num);

    //! Supprime les éléves d'un groupe.
    void delEleve(const std::map<szt,std::forward_list<szt>> & mapDel);

    //! Acceseur de l'identifiant du groupe.
    const Groupe & groupe() const
        {return m_groupe;}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
        makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) override;

    //! Mutateur de l'identifiant du groupe.
    void setIdGroupe(szt id);
};

/*! \ingroup groupeModelNote
 * \brief Groupe pour les tableau de groupe.
 */
class GroupeForTableau  {
protected:
    Groupe m_groupe;            //! groupe associé à la colonne.
public:
    //! Accesseur du groupe.
    const Groupe & groupe() const
        {return m_groupe;}

    //! Mutateur du groupe.
    void setGroupe(const Groupe & groupe)
        {m_groupe = groupe;}
};

/*! \ingroup groupeModelNote
 * \brief Tableau d'un vecteur de listes eleve-groupe.
 */
class EleveGroupeListTableau : public GroupeForTableau, public modelMPS::AbstractBddVectorListTableau<EleveGroupe> {
public:
    //! Constructeur.
    EleveGroupeListTableau(BddNote & bdd, szt count)
     : modelMPS::AbstractBddVectorListTableau<EleveGroupe>(bdd,count) {}

    //! Comparaison d'égalité entre deux éléments d'une liste.
    bool egalElement(const EleveGroupe & element1, const EleveGroupe & element2) const override
        {return element1.num() == element2.num();}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
        makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) override;
};

/*! \ingroup groupeModelNote
 * \brief Tableau d'un vecteur de eleve-groupe.
 */
class EleveGroupeVecTableau : public GroupeForTableau, public modelMPS::AbstractBddVectorEntTableau<EleveGroupe> {
public:
    //! Constructeur.
    EleveGroupeVecTableau(BddNote & bdd, szt count)
     : modelMPS::AbstractBddVectorEntTableau<EleveGroupe>(bdd,count) {}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
        makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) override;
};
}
#endif // TABLEAUNOTE_H
