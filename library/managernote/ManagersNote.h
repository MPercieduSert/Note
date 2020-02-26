/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef MANAGERSNOTE_H
#define MANAGERSNOTE_H

#include "EntityNote.h"
#include "GestionAutorisationCible.h"
#include "ManagersPredef.h"

namespace bddMPS {
    namespace cibleIdNote {
        //! Numéro de cible des entités de l'inventaire non prédéfinies.
        enum EntityInv {Annee = cibleId::NbrCibleEntPredef,
                        Bareme,
                        Classe,
                        ClasseEleve,
                        ClasseGroupe,
                        Controle,
                        ControleEpreuve,
                        Eleve,
                        EleveGroupe,
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
                        Valide,
                       NbrCible};
}}

namespace noteMPS {
namespace bmps = bddMPS;
/*! \ingroup groupeManagerNote
 * \brief Managers de l'application note.
 */
class ManagersNote : public managerMPS::ManagersPredef {

public:
    //! Constructeur.
    ManagersNote();

    //! Destructeur.
    ~ManagersNote() = default;
};

/*! \ingroup groupeManagerNote
 * \brief Classe condition d'unicité pour les entités Eleve, unicité du triple (nom, prenom, date).
 */
class UniqueEleve : public managerMPS::NomUniqueSql<Eleve> {
protected:
    using NomUniqueSql<Eleve>::bindValue;

public:
    enum {DateUnique = NomUniqueSql<Eleve>::NbrUnique, PrenomUnique, NbrUnique};
    CONSTR_DEFAUT(UniqueEleve)

    //! Destructeur.
    ~UniqueEleve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Eleve &entity) override {
        NomUniqueSql<Eleve>::bindValuesUnique(entity);
        bindValue(DateUnique,entity.date());
        bindValue(PrenomUnique,entity.prenom());
    }
};

/*! \ingroup groupeManagerNote
 * \brief Classe condition d'unicité pour les entités Epreuve, unicité du triple (id1,id2,EnsPoint).
 */
class UniqueEpreuve : public managerMPS::RelationUniqueSql<Epreuve> {
protected:
    using RelationUniqueSql<Epreuve>::bindValue;

public:
    enum {EnsPointUnique = RelationUniqueSql<Epreuve>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(UniqueEpreuve)

    //! Destructeur.
    ~UniqueEpreuve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Epreuve &entity) override {
        RelationUniqueSql<Epreuve>::bindValuesUnique(entity);
        bindValue(EnsPointUnique,entity.ensPoint());
    }
};

/*! \ingroup groupeManagerNote
 * \brief Classe condition d'unicité pour les entités ControleEpreuve sur les trois conditions
 * (id1,id4), (id2,id4,num) et (id3,id4).
 */
class UniqueControleEpreuve : public managerMPS::TripleExactOneNotNullUniqueSql<ControleEpreuve> {
protected:
    using TEUnique = TripleExactOneNotNullUniqueSql<ControleEpreuve>;
    using TEUnique::bindValue;

public:
    enum {Id4Unique_1 = TEUnique::NbrUnique_1, NbrUnique_1,
         Id4Unique_2 = TEUnique::NbrUnique_2, NumUnique, NbrUnique_2,
         Id4Unique_3 = TEUnique::NbrUnique_3, NbrUnique_3,
         Id4Unique_1Set = TEUnique::UniqueSet1,
         Id4Unique_2Set = TEUnique::UniqueSet2, NumUniqueSet = TEUnique::UniqueSet2,
         Id4Unique_3Set = TEUnique::UniqueSet3};
    CONSTR_DEFAUT(UniqueControleEpreuve)

    //! Destructeur.
    ~UniqueControleEpreuve() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique1.
    void bindValuesUnique(const ControleEpreuve &entity) override {
        TEUnique::bindValuesUnique(entity);
        bindValue(Id4Unique_1,entity.id4());
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique2.
    void bindValuesUnique_2(const ControleEpreuve &entity) override {
        TEUnique::bindValuesUnique_2(entity);
        bindValue(Id4Unique_2,entity.id4());
        bindValue(NumUnique,entity.id4());
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée unique3.
    void bindValuesUnique_3(const ControleEpreuve &entity) override {
        TEUnique::bindValuesUnique_3(entity);
        bindValue(Id4Unique_3,entity.id4());
    }
};

/*! \ingroup groupeManagerNote
 * \brief Classe condition d'unicité pour les entités EnoncePoint, unicité du triple (id1, EnsPoint, num).
 */
class UniqueEnoncePoint : public managerMPS::IdNumUniqueSql<EnoncePoint> {
protected:
    using IdNumUniqueSql<EnoncePoint>::bindValue;

public:
    enum {EnsPointUnique = IdNumUniqueSql<EnoncePoint>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(UniqueEnoncePoint)

    //! Destructeur.
    ~UniqueEnoncePoint() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const EnoncePoint &entity) override {
        IdNumUniqueSql<EnoncePoint>::bindValuesUnique(entity);
        bindValue(EnsPointUnique,entity.ensPoint());
    }
};
}
#endif // MANAGERSNOTE_H
