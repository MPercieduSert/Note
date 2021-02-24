/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef MANAGERS_NOTE_H
#define MANAGERS_NOTE_H

#include "entity_note.h"
#include "gestion_restriction_cible.h"
#include "managers_predef.h"

namespace mps {
namespace b2d {
    namespace cible_id {
        //! Numéro de cible des entités de Note non prédéfinies.
        enum EntityNote {Annee = cible_id::Nbr_Cible_Predef,
                         Bareme,
                         Classe,
                         Classe_Eleve,
                         Classe_Groupe,
                         Controle,
                         Controle_Epreuve,
                         Eleve,
                         Eleve_Groupe,
                         Enonce,
                         Enonce_Point,
                         Epreuve,
                         Etablissement,
                         Etablissement_Niveau,
                         Etablissement_Type,
                         Exercice,
                         Filiation_Niveau,
                         Groupe,
                         Niveau,
                         Niveau_Type_Etablissement,
                         Note,
                         Point,
                         Type_Controle,
                         Type_Etablissement,
                         Valide,
                         Nbr_Cible};
}}}

namespace note_mps {
/*! \ingroup groupe_manager_note
 * \brief Managers de l'application note.
 */
class managers_note : public mps::manager::managers_predef {

public:
    //! Constructeur.
    managers_note();

    //! Destructeur.
    ~managers_note() = default;
};

/*! \ingroup groupe_manager_note
 * \brief Classe condition d'unicité pour les entités eleve, unicité du triple (nom, prenom, date).
 */
class unique_eleve : public mps::manager::nom_unique_sql<eleve> {
protected:
    using mps::manager::nom_unique_sql<eleve>::bind_value;

public:
    enum {Date_Unique = nom_unique_sql<eleve>::Nbr_Unique, Prenom_Unique, Nbr_Unique};
    CONSTR_DEFAUT(unique_eleve)

    //! Destructeur.
    ~unique_eleve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const eleve &ent) override {
        nom_unique_sql<eleve>::bind_values_unique(ent);
        bind_value(Date_Unique,ent.date());
        bind_value(Prenom_Unique,ent.prenom());
    }
};

/*! \ingroup groupe_manager_note
 * \brief Classe condition d'unicité pour les entités epreuve, unicité du triple (id_1,id_2,Ens_Point).
 */
class unique_epreuve : public mps::manager::relation_unique_sql<epreuve> {
protected:
    using mps::manager::relation_unique_sql<epreuve>::bind_value;

public:
    enum {Ens_Point_Unique = relation_unique_sql<epreuve>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(unique_epreuve)

    //! Destructeur.
    ~unique_epreuve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const epreuve &ent) override {
        relation_unique_sql<epreuve>::bind_values_unique(ent);
        bind_value(Ens_Point_Unique,ent.ens_point());
    }
};

/*! \ingroup groupe_manager_note
 * \brief Classe condition d'unicité pour les entités controle_epreuve sur les trois conditions
 * (id_1,id_4), (id_2,id_4,num) et (id_3,id_4).
 */
class unique_controle_epreuve : public mps::manager::triple_exact_one_not_null_unique_sql<controle_epreuve> {
protected:
    using te_unique = mps::manager::triple_exact_one_not_null_unique_sql<controle_epreuve>;
    using te_unique::bind_value;

public:
    enum {Id_4_Unique_1 = te_unique::Nbr_Unique_1, Nbr_Unique_1,
         Id_4_Unique_2 = te_unique::Nbr_Unique_2, Num_Unique, Nbr_Unique_2,
         Id_4_Unique_3 = te_unique::Nbr_Unique_3, Nbr_Unique_3,
         Id_4_Unique_Set_1 = te_unique::Unique_Set_1,
         Id_4_Unique_Set_2 = te_unique::Unique_Set_2,
         Num_Unique_Set = te_unique::Unique_Set_2,
         Id_4_Unique_Set_3 = te_unique::Unique_Set_3};
    CONSTR_DEFAUT(unique_controle_epreuve)

    //! Destructeur.
    ~unique_controle_epreuve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique1.
    void bind_values_unique(const controle_epreuve &ent) override {
        te_unique::bind_values_unique(ent);
        bind_value(Id_4_Unique_1,ent.id_4());
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique2.
    void bind_values_unique_2(const controle_epreuve &ent) override {
        te_unique::bind_values_unique_2(ent);
        bind_value(Id_4_Unique_2,ent.id_4());
        bind_value(Num_Unique,ent.id_4());
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique3.
    void bind_values_unique_3(const controle_epreuve &ent) override {
        te_unique::bind_values_unique_3(ent);
        bind_value(Id_4_Unique_3,ent.id_4());
    }
};

/*! \ingroup groupe_manager_note
 * \brief Classe condition d'unicité pour les entités enonce_point, unicité du triple (id_1, Ens_Point, num).
 */
class unique_enonce_point : public mps::manager::id_num_unique_sql<enonce_point> {
protected:
    using mps::manager::id_num_unique_sql<enonce_point>::bind_value;

public:
    enum {Ens_Point_Unique = id_num_unique_sql<enonce_point>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(unique_enonce_point)

    //! Destructeur.
    ~unique_enonce_point() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const enonce_point &ent) override {
        id_num_unique_sql<enonce_point>::bind_values_unique(ent);
        bind_value(Ens_Point_Unique,ent.ens_point());
    }
};
}
#endif // MANAGERS_NOTE_H
