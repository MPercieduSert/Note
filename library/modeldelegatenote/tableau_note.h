/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/05/2020
 */
#ifndef TABLEAU_NOTE_H
#define TABLEAU_NOTE_H

#include "tableau_for_model.h"
#include "bdd_note.h"
#include "entity_note.h"

/*! \defgroup groupe_tableau_note Tableau  de note
 * \brief Ensemble des tableaux de données des models de note.
 */

namespace note_mps {
/*! \ingroup groupe_tableau_note
 * \brief Tableau des candidats pour les groupes.
 */
class candidat_groupe_tableau : public mps::model_base::composition_tableau {
protected:
    bdd_note & m_bdd;                    //!< Lien avec la base de données.
    groupe m_groupe;                    //!< Identifiant du groupe.
public:
    //! position des tableaux.
    enum position_tableau {Eleve_Tableau, Eleve_Groupe_Tableau, Donnee_Tableau, Nbr_Tableau};
    enum {Non_Affecter = -1, Num_Role = Qt::UserRole + 1};
    //! Constructeur.
    candidat_groupe_tableau(bdd_note & bdd, idt id_groupe);

    //! Hydrate la donnée d'une ligne du tableau.
    void hydrate(szt ligne) override {
        m_tableaux.front()->hydrate(ligne);
        hydrate_eleve_groupe(ligne);
    }

    //! Hydrate la donnée d'une ligne du tableau.
    void hydrate_eleve_groupe(szt ligne);

    //! Acceseur de groupe.
    const groupe  & get_groupe() const
        {return m_groupe;}

    //! Retire des élèves des groupes.
    void remove(std::vector<std::pair<idt,int>> && vec_id_num);

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override;
};

/*! \ingroup groupe_model_note
 * \brief Tableau d'un vecteur de classe-eleve.
 */
class classe_eleve_composition_tableau : public mps::model_base::composition_tableau {
protected:
    bdd_note & m_bdd;    //!< Lien avec la base de données.
    idt m_id_classe;     //!< Identifiant de la classe à la quelle les élèves appartiennent.
public:
    //! position des tableaux.
    enum position_tableau {Eleve_Tableau, Classe_Eleve_Tableau, Nbr_Tableau};

    //! Constructeur.
    classe_eleve_composition_tableau(bdd_note & bdd, idt id_classe);

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override {
        m_tableaux.front()->hydrate(ligne);
        hydrate_classe_eleve(ligne);
    }

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate_classe_eleve(szt ligne);

    //! Acceseur de l'identifiant de la classe.
    idt id_classe() const
        {return m_id_classe;}

    //! Mutateur de l'identifiant de la classe.
    void set_id_classe(idt id);
};

/*! \ingroup groupe_model_note
 * \brief Tableau d'un vecteur de classe-eleve.
 */
class classe_eleve_vec_tableau : public mps::model_base::abstract_bdd_vector_ent_tableau<classe_eleve> {
public:
    //! Identifiant des colonnes assoiées au tableau d'éléves.
    enum id_colonne {Entree,
                    Sortie};

    //! Constructeur.
    using mps::model_base::abstract_bdd_vector_ent_tableau<classe_eleve>::abstract_bdd_vector_ent_tableau;

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) override;
};

/*! \ingroup groupe_model_note
 * \brief Tableau d'éléves.
 */
class eleve_vec_tableau : public mps::model_base::abstract_bdd_vector_ent_tableau<eleve> {
public:
    //! Identifiant des colonnes assoiées au tableau d'éléves.
    enum id_colonne {Nom,
                    Prenom,
                    Naissance,
                    Sexe};

    //! Colonne des noms des éléves
    class nom_colonne : public mps::model_base::vector_ptr_id_colonne<eleve> {
    public:
        //! Constructeur.
        nom_colonne(const QString & name, Qt::ItemFlags flags, mps::vector_ptr<eleve> & vec);

        //! Compare entre deux lignes ( ligne_1 < ligne_2).
        bool compare(szt ligne_1, szt ligne_2) const override
            {return m_vec[ligne_1] < m_vec[ligne_2];}
    };

    //! Constructeur.
    using mps::model_base::abstract_bdd_vector_ent_tableau<eleve>::abstract_bdd_vector_ent_tableau;

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne_1, szt ligne_2) const override;

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) override;
};

/*! \ingroup groupe_model_note
 * \brief Tableau des éléves dans les groupes.
 */
class eleve_groupe_tableau : public mps::model_base::heterogene_taille_tableau<eleve> {
protected:
    bdd_note & m_bdd;        //!< Lien avec la base de données.
    groupe m_groupe;        //!< Identifiant du groupe.
    mps::divers::num_to_string m_style_num;   //!< Convertiseur des numéros de groupe.
public:
    //! Constructeur.
    eleve_groupe_tableau(bdd_note & bdd)
        : m_bdd(bdd) {}

    //! Ajoute un éléve à un groupe.
    void add_eleve(const std::list<idt> &list_el, szt num);

    //! Supprime les éléves des groupes (paire (id_groupe,id_eleve)).
    void del_eleve(const std::map<szt,std::forward_list<idt>> & map_del);

    //! Acceseur de l'identifiant du groupe.
    const groupe & get_groupe() const
        {return m_groupe;}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) override;

    //! Supprime les éléves des groupes (les listes de map doivent être trier dans l'ordre croissant).
    void remove(const std::map<szt,std::list<idt>> & map);

    //! Mutateur de l'identifiant du groupe.
    void set_id_groupe(idt id);

    //! Convertissuer de numero en texte..
    QString num_to_texte(int num) const
        {return m_style_num.texte(num);}
};

/*! \ingroup groupe_model_note
 * \brief Groupe pour les tableau de groupe.
 */
class groupe_for_tableau  {
protected:
    groupe m_groupe;            //!< groupe associé à la colonne.
    mps::divers::num_to_string m_style_num;   //!< Convertiseur des numéros de groupe.
public:
    //! Accesseur du groupe.
    const groupe & get_groupe() const
        {return m_groupe;}

    //! Mutateur du groupe.
    void set_groupe(const groupe & gr) {
        m_groupe = gr;
        m_style_num.set_style(m_groupe.style_num());
    }

    //! Convertissuer de numero en texte..
    QString num_to_texte(int num) const
        {return m_style_num.texte(num);}
};

/*! \ingroup groupe_model_note
 * \brief Tableau d'un vecteur de listes eleve-groupe.
 */
class eleve_groupe_list_tableau : public groupe_for_tableau,
                                  public mps::model_base::abstract_bdd_vector_list_tableau<eleve_groupe> {
public:
    //! Constructeur.
    eleve_groupe_list_tableau(bdd_note & bdd, szt count)
     : mps::model_base::abstract_bdd_vector_list_tableau<eleve_groupe>(bdd,count) {}

    //! Comparaison d'égalité entre deux éléments d'une liste.
    bool egal_element(const eleve_groupe & element1, const eleve_groupe & element2) const override
        {return element1.num() == element2.num();}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) override;
};

/*! \ingroup groupe_model_note
 * \brief Tableau d'un vecteur de eleve-groupe.
 */
class eleve_groupe_vec_tableau : public groupe_for_tableau, public mps::model_base::abstract_bdd_vector_ent_tableau<eleve_groupe> {
public:
    //! Constructeur.
    eleve_groupe_vec_tableau(bdd_note & bdd, szt count)
     : mps::model_base::abstract_bdd_vector_ent_tableau<eleve_groupe>(bdd,count) {}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
        make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) override;
};
}
#endif // TABLEAU_NOTE_H
