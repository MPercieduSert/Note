/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/05/2020
 */
#ifndef TABLEAUNOTE_H
#define TABLEAUNOTE_H

#include "TableauForModel.h"
#include "BddNote.h"
#include "EntityNote.h"


/*! \defgroup groupeTableauNote Tableau  de note
 * \brief Ensemble des tableaux de données des models de note.
 */

namespace noteMPS {
/*! \ingroup groupe_modelNote
 * \brief Tableau des candidats pour les groupes.
 */
class CandidatGroupeTableau : public model_base::composition_tableau {
protected:
    BddNote & m_bdd;                    //!< Lien avec la base de données.
    Groupe m_groupe;                    //!< Identifiant du groupe.
public:
    //! position des tableaux.
    enum positionTableau {EleveTableau, Eleve_groupeTableau, donneeTableau, NbrTableau};
    enum {NonAffecter = -1, Num_Role = Qt::UserRole + 1};
    //! Constructeur.
    CandidatGroupeTableau(BddNote & bdd, idt id_groupe);

    //! Hydrate la donnée d'une ligne du tableau.
    void hydrate(szt ligne) override {
        m_tableaux.front()->hydrate(ligne);
        hydrateEleve_groupe(ligne);
    }

    //! Hydrate la donnée d'une ligne du tableau.
    void hydrateEleve_groupe(szt ligne);

    //! Acceseur de groupe.
    const Groupe  & groupe() const
        {return m_groupe;}

    //! Retire des élèves des groupes.
    void remove(std::vector<std::pair<idt,int>> && vecIdNum);

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override;
};

/*! \ingroup groupe_modelNote
 * \brief Tableau d'un vecteur de classe-eleve.
 */
class ClasseElevecomposition_tableau : public model_base::composition_tableau {
protected:
    BddNote & m_bdd;    //!< Lien avec la base de données.
    idt m_idClasse;     //!< Identifiant de la classe à la quelle les élèves appartiennent.
public:
    //! position des tableaux.
    enum positionTableau {EleveTableau, ClasseEleveTableau, NbrTableau};

    //! Constructeur.
    ClasseElevecomposition_tableau(BddNote & bdd, idt idClasse);

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override {
        m_tableaux.front()->hydrate(ligne);
        hydrateClasseEleve(ligne);
    }

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrateClasseEleve(szt ligne);

    //! Acceseur de l'identifiant de la classe.
    idt idClasse() const
        {return m_idClasse;}

    //! Mutateur de l'identifiant de la classe.
    void set_idClasse(idt id);
};

/*! \ingroup groupe_modelNote
 * \brief Tableau d'un vecteur de classe-eleve.
 */
class ClasseEleveVecTableau : public model_base::AbstractBddVectorEntTableau<ClasseEleve> {
public:
    //! Identifiant des colonnes assoiées au tableau d'éléves.
    enum idColonne {Entree,
                    Sortie};

    //! Constructeur.
    using model_base::AbstractBddVectorEntTableau<ClasseEleve>::AbstractBddVectorEntTableau;

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) override;
};

/*! \ingroup groupe_modelNote
 * \brief Tableau d'éléves.
 */
class EleveVecTableau : public model_base::AbstractBddVectorEntTableau<Eleve> {
public:
    //! Identifiant des colonnes assoiées au tableau d'éléves.
    enum idColonne {Nom,
                    Prenom,
                    Naissance,
                    Sexe};

    //! Colonne des noms des éléves
    class NomColonne : public model_base::vector_ptr_id_colonne<Eleve> {
    public:
        //! Constructeur.
        NomColonne(const QString & name, Qt::ItemFlags flags, vector_ptr<Eleve> & vec);

        //! Compare entre deux lignes ( ligne_1 < ligne_2).
        bool compare(szt ligne_1, szt ligne_2) const override
            {return m_vec[ligne_1] < m_vec[ligne_2];}
    };

    //! Constructeur.
    using model_base::AbstractBddVectorEntTableau<Eleve>::AbstractBddVectorEntTableau;

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne_1, szt ligne_2) const override;

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) override;
};

/*! \ingroup groupe_modelNote
 * \brief Tableau des éléves dans les groupes.
 */
class Eleve_groupeTableau : public model_base::heterogene_taille_tableau<Eleve> {
protected:
    BddNote & m_bdd;        //!< Lien avec la base de données.
    Groupe m_groupe;        //!< Identifiant du groupe.
    divers::num_to_string m_styleNum;   //!< Convertiseur des numéros de groupe.
public:
    //! Constructeur.
    Eleve_groupeTableau(BddNote & bdd)
        : m_bdd(bdd) {}

    //! Ajoute un éléve à un groupe.
    void addEleve(const std::list<idt> &listEl, szt num);

    //! Supprime les éléves des groupes (paire (id_groupe,idEleve)).
    void delEleve(const std::map<szt,std::forward_list<idt>> & mapDel);

    //! Acceseur de l'identifiant du groupe.
    const Groupe & groupe() const
        {return m_groupe;}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) override;

    //! Supprime les éléves des groupes (les listes de map doivent être trier dans l'ordre croissant).
    void remove(const std::map<szt,std::list<idt>> & map);

    //! Mutateur de l'identifiant du groupe.
    void set_id_groupe(idt id);

    //! Convertissuer de numero en texte..
    QString numToTexte(int num) const
        {return m_styleNum.texte(num);}
};

/*! \ingroup groupe_modelNote
 * \brief Groupe pour les tableau de groupe.
 */
class GroupeForTableau  {
protected:
    Groupe m_groupe;            //!< groupe associé à la colonne.
    divers::num_to_string m_styleNum;   //!< Convertiseur des numéros de groupe.
public:
    //! Accesseur du groupe.
    const Groupe & groupe() const
        {return m_groupe;}

    //! Mutateur du groupe.
    void setGroupe(const Groupe & groupe) {
        m_groupe = groupe;
        m_styleNum.set_style(m_groupe.styleNum());
    }

    //! Convertissuer de numero en texte..
    QString numToTexte(int num) const
        {return m_styleNum.texte(num);}
};

/*! \ingroup groupe_modelNote
 * \brief Tableau d'un vecteur de listes eleve-groupe.
 */
class Eleve_groupeListTableau : public GroupeForTableau, public model_base::AbstractBddvector_list_Tableau<Eleve_groupe> {
public:
    //! Constructeur.
    Eleve_groupeListTableau(BddNote & bdd, szt count)
     : model_base::AbstractBddvector_list_Tableau<Eleve_groupe>(bdd,count) {}

    //! Comparaison d'égalité entre deux éléments d'une liste.
    bool egal_element(const Eleve_groupe & element1, const Eleve_groupe & element2) const override
        {return element1.num() == element2.num();}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) override;
};

/*! \ingroup groupe_modelNote
 * \brief Tableau d'un vecteur de eleve-groupe.
 */
class Eleve_groupeVecTableau : public GroupeForTableau, public model_base::AbstractBddVectorEntTableau<Eleve_groupe> {
public:
    //! Constructeur.
    Eleve_groupeVecTableau(BddNote & bdd, szt count)
     : model_base::AbstractBddVectorEntTableau<Eleve_groupe>(bdd,count) {}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) override;
};
}
#endif // TABLEAUNOTE_H
