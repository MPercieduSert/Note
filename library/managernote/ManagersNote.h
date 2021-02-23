/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef MANAGERSNOTE_H
#define MANAGERSNOTE_H

#include "EntityNote.h"
#include "GestionRestrictionCible.h"
#include "ManagersPredef.h"

namespace b2d {
    namespace cible_id {
        //! Numéro de cible des entités de Note non prédéfinies.
        enum EntityNote {Annee = cible_id::Nbr_Cible_Predef,
                        Bareme,
                        Classe,
                        ClasseEleve,
                        Classe_groupe,
                        Controle,
                        ControleEpreuve,
                        Eleve,
                        Eleve_groupe,
                        Enonce,
                        EnoncePoint,
                        Epreuve,
                        Etablissement,
                        EtablissementNiveau,
                        EtablissementType,
                        FiliationNiveau,
                        Groupe,
                        Niveau,
                        NiveauTypeEtablissement,
                        Note,
                        Point,
                        TypeControle,
                        TypeEtablissement,
                        _valide,
                        Exercice,
                       NbrCible};
}}

namespace noteMPS {
namespace b2d = b2d;
/*! \ingroup groupe_managerNote
 * \brief Managers de l'application note.
 */
class managers_note : public manager::managers_predef {

public:
    //! Constructeur.
    managers_note();

    //! Destructeur.
    ~managers_note() = default;
};

/*! \ingroup groupe_managerNote
 * \brief Classe condition d'unicité pour les entités Eleve, unicité du triple (nom, prenom, date).
 */
class UniqueEleve : public manager::nom_unique_sql<Eleve> {
protected:
    using nom_unique_sql<Eleve>::bindValue;

public:
    enum {DateUnique = nom_unique_sql<Eleve>::Nbr_Unique, PrenomUnique, Nbr_Unique};
    CONSTR_DEFAUT(UniqueEleve)

    //! Destructeur.
    ~UniqueEleve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Eleve &entity) override {
        nom_unique_sql<Eleve>::bind_values_unique(ent);
        bindValue(DateUnique,ent.date());
        bindValue(PrenomUnique,ent.prenom());
    }
};

/*! \ingroup groupe_managerNote
 * \brief Classe condition d'unicité pour les entités Epreuve, unicité du triple (id1,id2,EnsPoint).
 */
class UniqueEpreuve : public manager::relation_unique_sql<Epreuve> {
protected:
    using relation_unique_sql<Epreuve>::bindValue;

public:
    enum {EnsPointUnique = relation_unique_sql<Epreuve>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(UniqueEpreuve)

    //! Destructeur.
    ~UniqueEpreuve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Epreuve &entity) override {
        relation_unique_sql<Epreuve>::bind_values_unique(ent);
        bindValue(EnsPointUnique,ent.ensPoint());
    }
};

/*! \ingroup groupe_managerNote
 * \brief Classe condition d'unicité pour les entités ControleEpreuve sur les trois conditions
 * (id1,id4), (id2,id4,num) et (id_3,id4).
 */
class UniqueControleEpreuve : public manager::triple_exact_one_not_null_unique_sql<ControleEpreuve> {
protected:
    using TEUnique = triple_exact_one_not_null_unique_sql<ControleEpreuve>;
    using TEUnique::bindValue;

public:
    enum {Id4Unique_1 = TEUnique::Nbr_Unique_1, Nbr_Unique_1,
         Id4Unique_2 = TEUnique::Nbr_Unique_2, Num_Unique, Nbr_Unique_2,
         Id4Unique_3 = TEUnique::Nbr_Unique_3, Nbr_Unique_3,
         Id4Unique_1Set = TEUnique::Unique_Set_1,
         Id4Unique_2Set = TEUnique::Unique_Set_2, Num_Unique_Set = TEUnique::Unique_Set_2,
         Id4Unique_3Set = TEUnique::Unique_Set_3};
    CONSTR_DEFAUT(UniqueControleEpreuve)

    //! Destructeur.
    ~UniqueControleEpreuve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique1.
    void bind_values_unique(const ControleEpreuve &entity) override {
        TEUnique::bind_values_unique(ent);
        bindValue(Id4Unique_1,ent.id4());
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique2.
    void bind_values_unique_2(const ControleEpreuve &entity) override {
        TEUnique::bind_values_unique_2(ent);
        bindValue(Id4Unique_2,ent.id4());
        bindValue(Num_Unique,ent.id4());
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique3.
    void bind_values_unique_3(const ControleEpreuve &entity) override {
        TEUnique::bind_values_unique_3(ent);
        bindValue(Id4Unique_3,ent.id4());
    }
};

/*! \ingroup groupe_managerNote
 * \brief Classe condition d'unicité pour les entités EnoncePoint, unicité du triple (id1, EnsPoint, num).
 */
class UniqueEnoncePoint : public manager::id_num_unique_sql<EnoncePoint> {
protected:
    using id_num_unique_sql<EnoncePoint>::bindValue;

public:
    enum {EnsPointUnique = id_num_unique_sql<EnoncePoint>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(UniqueEnoncePoint)

    //! Destructeur.
    ~UniqueEnoncePoint() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const EnoncePoint &entity) override {
        id_num_unique_sql<EnoncePoint>::bind_values_unique(ent);
        bindValue(EnsPointUnique,ent.ensPoint());
    }
};
}
#endif // MANAGERSNOTE_H
