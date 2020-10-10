/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef ENTITYNOTE_H
#define ENTITYNOTE_H

#include "EntityPredef.h"

/*! \defgroup groupeManagerNote Entités et base de données note
 * \brief Ensemble des entités, managers et base de données de l'application note.
 */

namespace infoEntityNote {

    //! Liste des identifiants des entités
    enum entityId{AnneeId = infoEntity::NbrEntityPredefinie,
                  BaremeId,
                  ClasseId,
                  ClasseEleveId,
                  ClasseGroupeId,
                  ControleId,
                  ControleEpreuveId,
                  EleveId,
                  EleveGroupeId,
                  EnonceId,
                  EnoncePointId,
                  EpreuveId,
                  EtablissementId,
                  EtablissementNiveauId,
                  EtablissementTypeId,
                  FiliationNiveauId,
                  GroupeId,
                  NiveauId,
                  NiveauTypeEtablissementId,
                  NoteId,
                  PointId,
                  TypeControleId,
                  TypeEtablissementId,
                  ValideId,
                  NbrEntityId};
}

/*! \ingroup groupeManagerNote
 * \brief Espace de noms de l'application note.
 */
namespace noteMPS {
using namespace typeMPS;
namespace emps = entityMPS;

// Entités prédéfinies
using namespace emps::ensembleCommentaire;
using namespace emps::ensembleDonnee;
using namespace emps::ensembleEvenement;
using namespace emps::ensembleMotCle;
using namespace emps::ensembleRestriction;
using namespace emps::ensembleTexte;
using namespace emps::ensembleType;
using namespace emps::ensembleUtilisation;

// Entités de type prédéfinies
RELATION_ENTITY(EleveGroupe,RelationNum,infoEntityNote::EleveGroupeId,Eleve,Groupe)
RELATION_ENTITY(EtablissementNiveau,Relation,infoEntityNote::EtablissementNiveauId,Etab,Niveau)
RELATION_ENTITY(EtablissementType,Relation,infoEntityNote::EtablissementTypeId,Etab,TpEtab)
RELATION_ENTITY(FiliationNiveau,Relation,infoEntityNote::FiliationNiveauId,Precedent,Suivant)
RELATION_ENTITY(NiveauTypeEtablissement,Relation,infoEntityNote::NiveauTypeEtablissementId,Niveau,TpEtab)
RELATION_ENTITY(Note,RelationDateTimeCurrentSaisieValeurInt,infoEntityNote::NoteId,Controle,Eleve)
RELATION_ENTITY(Valide,RelationDateTimeCurrentValeurInt,infoEntityNote::ValideId,Note,Point)
//using Annee = entityBaseMPS::NumEntity<infoEntityNote::AnneeId>;
using Bareme = entityBaseMPS::CibleSimpleTypeValeurDoubleEntity<infoEntityNote::BaremeId>;
using Enonce = entityBaseMPS::TypeVersionEntity<infoEntityNote::EnonceId>;
using Etablissement = entityBaseMPS::NcNomEntity<infoEntityNote::EtablissementId>;
using Niveau = entityBaseMPS::NcNomTypeEntity<infoEntityNote::NiveauId>;
using Point = entityBaseMPS::TypeValeurIntVersionEntity<infoEntityNote::PointId>;
using TypeEtablissement = entityBaseMPS::NcNomEntity<infoEntityNote::TypeEtablissementId>;

// Attribut
SINGLE_ATTRIBUT(EnsPointAttribut,attributMPS::AttributId,EnsPoint,ensPoint)
SINGLE_ATTRIBUT(EntreeAttribut,attributMPS::AttributDateValide,Entree,entree)
SINGLE_ATTRIBUT(FilleAttribut,attributMPS::AttributBool,Fille,fille)
SINGLE_ATTRIBUT(MinimaAttribut,attributMPS::AttributIntSup<-1>,Minima,minima)
SINGLE_ATTRIBUT(NumCEAttribut,attributMPS::AttributIntSup<-1>,Num,num)
SINGLE_ATTRIBUT(PrenomAttribut,attributMPS::AttributStringNotEmpty,Prenom,prenom)
SINGLE_ATTRIBUT(SortieAttribut,attributMPS::AttributDateValide,Sortie,sortie)

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Annee.
 */
class Annee : public emps::EntityID<infoEntityNote::AnneeId,attributMPS::NumAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,Annee>;
public:
using EAID = emps::EntityID<infoEntityNote::AnneeId,attributMPS::NumAttribut>;

    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Num = PositionEnum<NumAttribut>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EAID::EntityID;
    BASE_ENTITY(Annee)

    //! Constructeur à partir des valeurs attributs.
    Annee(int num, idt id = 0)
        : EAID(id)
        {setNum(num);}

    //! Renvoie la chaîne de caractères "Num"-"Num+1".
    QString texte() const
        {return QString::number(num()).append("-").append(QString::number(num()+1));}
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Classe.
 */
class Classe : public emps::EntityIDs<infoEntityNote::ClasseId,attributMPS::Id1Attribut,
                                                               attributMPS::Id2Attribut,
                                                               attributMPS::Id3Attribut,
                                                               attributMPS::NcAttribut,
                                                               attributMPS::NomAttribut,
                                                               attributMPS::NumAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,Classe>;
public:
using EAID = emps::EntityIDs<infoEntityNote::ClasseId,attributMPS::Id1Attribut,
                                                      attributMPS::Id2Attribut,
                                                      attributMPS::Id3Attribut,
                                                      attributMPS::NcAttribut,
                                                      attributMPS::NomAttribut,
                                                      attributMPS::NumAttribut>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Id1 = PositionEnum<Id1Attribut>::Position,
                   Id2 = PositionEnum<Id2Attribut>::Position,
                   Id3 = PositionEnum<Id3Attribut>::Position,
                   Nc = PositionEnum<NcAttribut>::Position,
                   Nom = PositionEnum<NomAttribut>::Position,
                   Num = PositionEnum<NumAttribut>::Position,
                   NbrAtt = EAID::NbrAtt,
                   IdAn = Id1,
                   IdEtab = Id2,
                   IdNiveau = Id3,
                   };

    using EAID::EntityID;
    BASE_ENTITY(Classe)
    ALIAS_CLE(An,1)
    ALIAS_CLE(Etab,2)
    ALIAS_CLE(Niveau,3)

    //! Constructeur à partir des valeurs attributs.
    Classe(idt idAn, idt idEtab, idt idNiveau, const QString & nc, const QString & nom, int num, idt id = 0)
        : EAID(id) {
        setIdAn(idAn);
        setIdEtab(idEtab);
        setIdNiveau(idNiveau);
        setNc(nc);
        setNom(nom);
        setNum(num);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité ClasseEleve.
 */
class ClasseEleve : public emps::EntityIDs<infoEntityNote::ClasseEleveId,attributMPS::RelationAttribut,
                                                                         EntreeAttribut,
                                                                         SortieAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,ClasseEleve>;
public:
    using EAID = emps::EntityIDs<infoEntityNote::ClasseEleveId,attributMPS::RelationAttribut,
                                                               EntreeAttribut,
                                                               SortieAttribut>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Id1 = PositionEnum<Id1Attribut>::Position,
                   Id2 = PositionEnum<Id2Attribut>::Position,
                   Entree = PositionEnum<EntreeAttribut>::Position,
                   Sortie = PositionEnum<SortieAttribut>::Position,
                   NbrAtt =EAID::NbrAtt,
                   IdClasse = Id1,
                   IdEleve = Id2};

    using EAID::EntityID;
    BASE_ENTITY(ClasseEleve)
    ALIAS_CLE(Classe,1)
    ALIAS_CLE(Eleve,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    ClasseEleve(idt idCl, idt idEl, idt id = 0)
        : EAID(id) {
        setIdClasse(idCl);
        setIdEleve(idEl);
    }

    //! Constructeur à partir des valeurs attributs.
    ClasseEleve(idt idCl, idt idEl, const QDate & entree, const QDate & sortie = QDate(), idt id = 0)
        : ClasseEleve(idCl, idEl, id) {
          setEntree(entree);
          setSortie(sortie);
    }

    //! Validité de l'entitée.
    bool isValid() const override {
        return EAID::isValid() && entree() <= sortie();
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Controle.
 */
class Controle : public emps::EntityIDs<infoEntityNote::ControleId,attributMPS::Id1Attribut,
                                                                   attributMPS::CodeAttribut,
                                                                   attributMPS::DateTimeValideAttribut,
                                                                   attributMPS::DecimaleAttribut,
                                                                   MinimaAttribut,
                                                                   attributMPS::NcNomAttribut,
                                                                   attributMPS::NumAttribut,
                                                                   attributMPS::TotalAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,Controle>;
public:
    using EAID = emps::EntityIDs<infoEntityNote::ControleId,attributMPS::Id1Attribut,
                                                            attributMPS::CodeAttribut,
                                                            attributMPS::DateTimeValideAttribut,
                                                            attributMPS::DecimaleAttribut,
                                                            MinimaAttribut,
                                                            attributMPS::NcNomAttribut,
                                                            attributMPS::NumAttribut,
                                                            attributMPS::TotalAttribut>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Id1 = PositionEnum<Id1Attribut>::Position,
                   Code = PositionEnum<CodeAttribut>::Position,
                   DateTime = PositionEnum<DateTimeValideAttribut>::Position,
                   Decimale = PositionEnum<DecimaleAttribut>::Position,
                   Minima = PositionEnum<MinimaAttribut>::Position,
                   Nc = PositionEnum<NcAttribut>::Position,
                   Nom = PositionEnum<NomAttribut>::Position,
                   Num = PositionEnum<NumAttribut>::Position,
                   Total = PositionEnum<TotalAttribut>::Position,
                   NbrAtt = EAID::NbrAtt,
                   IdType = Id1,
                   };

    //! Decimale pour les lettres.
    enum decmaleLettre {NoPlus = 1,
                        Plus = 3,
                        PPlus = 5,
                        PPPlus = 7,

    };

    using EAID::EntityID;
    BASE_ENTITY(Controle)
    ALIAS_CLE(Type,1)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    Controle(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    Controle(idt idTp, flag code, const QDateTime & dateTime, szt decimale, int minima, const QString & nc, const QString & nom,
             int num, int total, idt id = 0)
        : Controle(nom, id) {
        setIdType(idTp);
        setCode(code);
        setDateTime(dateTime);
        setDecimale(decimale);
        setMinima(minima);
        setNc(nc);
        setNum(num);
        setTotal(total);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Les attributs de l'entité ControleEpreuve.
 */
class ControleEpreuveAttribut : public emps::Attributs<attributMPS::RelationTroisExactOneNotNullAttribut,
                                                       attributMPS::Id4Attribut,
                                                       attributMPS::Id5Attribut,
                                                       NumCEAttribut> {
public:
    CONSTR_DEFAUT(ControleEpreuveAttribut)
    CONSTR_AFFECT_DEFAUT(ControleEpreuveAttribut)

    //! Destructeur.
    ~ControleEpreuveAttribut() override;

    //! Teste si l'entité est valide.
    bool isValid() const override
        {return (num() == -1 &&((Id1NullAttribut::isValid() && id2() == 0 && id3() == 0)
                                || (Id2NullAttribut::isValid() && id1() == 0 && id3() == 0)))
                || (num() >= 0 && Id3NullAttribut::isValid() && id1() == 0 && id2() == 0);}
};


/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité ControleEpreuve.
 */
class ControleEpreuve : public emps::EntityIDs<infoEntityNote::ControleEpreuveId,ControleEpreuveAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,ControleEpreuve>;
public:
    using EAID = emps::EntityIDs<infoEntityNote::ControleEpreuveId,ControleEpreuveAttribut>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Id1 = PositionEnum<Id1NullAttribut>::Position,
                   Id2 = PositionEnum<Id2NullAttribut>::Position,
                   Id3 = PositionEnum<Id3NullAttribut>::Position,
                   Id4 = PositionEnum<Id4Attribut>::Position,
                   Id5 = PositionEnum<Id5Attribut>::Position,
                   Num = PositionEnum<NumCEAttribut>::Position,
                   NbrAtt = EAID::NbrAtt,
                   IdClasse = Id1,
                   IdEleve = Id2,
                   IdGroupe = Id3,
                   IdControle = Id4,
                   IdEpreuve= Id5
                  };

    using EAID::EntityID;
    BASE_ENTITY(ControleEpreuve)
    ALIAS_CLE(Classe,1)
    ALIAS_CLE(Eleve,2)
    ALIAS_CLE(Groupe,3)
    ALIAS_CLE(Controle,4)
    ALIAS_CLE(Epreuve,5)

    //! Constructeur à partir des valeurs attributs.
    ControleEpreuve(idt idClasse, idt idControle, idt idEleve, idt idEpreuve, idt idGroupe, int num, idt id = 0)
        : EAID(id) {
        setIdClasse(idClasse);
        setIdControle(idControle);
        setIdEleve(idEleve);
        setIdEpreuve(idEpreuve);
        setIdGroupe(idGroupe);
        setNum(num);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Eleve.
 */
class Eleve : public emps::EntityIDs<infoEntityNote::EleveId,attributMPS::DateValideAttribut,
                                                             FilleAttribut,
                                                             attributMPS::NomAttribut,
                                                             PrenomAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,Eleve>;
public:
    using EAID = emps::EntityIDs<infoEntityNote::EleveId,attributMPS::DateValideAttribut,
                                                         FilleAttribut,
                                                         attributMPS::NomAttribut,
                                                         PrenomAttribut>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Date = PositionEnum<DateValideAttribut>::Position,
                   Fille = PositionEnum<FilleAttribut>::Position,
                   Nom = PositionEnum<NomAttribut>::Position,
                   Prenom = PositionEnum<PrenomAttribut>::Position,
                   NbrAtt = EAID::NbrAtt
                  };

    using EAID::EntityID;
    BASE_ENTITY(Eleve)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    Eleve(const QDate & date, const QString & nom, const QString & prenom,  idt id = 0)
        : EAID(id) {
        setDate(date);
        setNom(nom);
        setPrenom(prenom);
    }

    //! Constructeur à partir des valeurs attributs.
    Eleve(const QDate & date, bool fille, const QString & nom, const QString & prenom,  idt id = 0)
        : Eleve(date, nom, prenom, id)
        {setFille(fille);}

    //! Compaison (<) entre deux éléves par ordre alphabétique sur nom, prénom puis date de naissance.
    bool operator < (const Eleve & eleve) const;
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité EnoncePoint.
 */
class EnoncePoint : public emps::EntityIDs<infoEntityNote::EnoncePointId,attributMPS::RelationAttribut,
                                                                         EnsPointAttribut,
                                                                         attributMPS::NumAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,EnoncePoint>;
public:
    using EAID = emps::EntityIDs<infoEntityNote::EnoncePointId,attributMPS::RelationAttribut,
                                                               EnsPointAttribut,
                                                               attributMPS::NumAttribut>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Id1 = PositionEnum<Id1Attribut>::Position,
                   Id2 = PositionEnum<Id2Attribut>::Position,
                   EnsPoint = PositionEnum<EnsPointAttribut>::Position,
                   Num = PositionEnum<NumAttribut>::Position,
                   NbrAtt =EAID::NbrAtt,
                   IdEnonce = Id1,
                   IdPoint = Id2};

    using EAID::EntityID;
    BASE_ENTITY(EnoncePoint)
    ALIAS_CLE(Enonce,1)
    ALIAS_CLE(Point,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    EnoncePoint(idt idEn, idt ensPt, int num , idt id = 0)
        : EAID(id) {
        setIdEnonce(idEn);
        setEnsPoint(ensPt);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    EnoncePoint(idt idEn, idt idPt, idt ensPt, int num, idt id = 0)
        : EnoncePoint(idEn, ensPt, num, id)
        {setIdPoint(idPt);}
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Epreuve.
 */
class Epreuve : public emps::EntityIDs<infoEntityNote::EpreuveId,attributMPS::RelationAttribut,
                                                                 EnsPointAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,Epreuve>;
public:
    using EAID = emps::EntityIDs<infoEntityNote::EpreuveId,attributMPS::RelationAttribut,
    EnsPointAttribut>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Id1 = PositionEnum<Id1Attribut>::Position,
                   Id2 = PositionEnum<Id2Attribut>::Position,
                   EnsPoint = PositionEnum<EnsPointAttribut>::Position,
                   NbrAtt =EAID::NbrAtt,
                   IdBareme = Id1,
                   IdEnonce= Id2};

    using EAID::EntityID;
    BASE_ENTITY(Epreuve)
    ALIAS_CLE(Bareme,1)
    ALIAS_CLE(Enonce,2)

    //! Constructeur à partir des valeurs attributs.
    Epreuve(idt idBar, idt idEn, idt ensPt, idt id = 0)
        : EAID(id) {
        setIdBareme(idBar);
        setIdEnonce(idEn);
        setEnsPoint(ensPt);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Groupe.
 */
class Groupe : public emps::EntityIDs<infoEntityNote::GroupeId,attributMPS::RelationExactOneNotNullAttribut,
                                                            attributMPS::StyleNumAttribut,
                                                            attributMPS::CodeAttribut,
                                                            attributMPS::NcNomTypeAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,Groupe>;
public:
    using EAID = emps::EntityIDs<infoEntityNote::GroupeId,attributMPS::RelationExactOneNotNullAttribut,
                                                          attributMPS::StyleNumAttribut,
                                                          attributMPS::CodeAttribut,
                                                          attributMPS::NcNomTypeAttribut>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Id1 = PositionEnum<Id1NullAttribut>::Position,
                   Id2 = PositionEnum<Id2NullAttribut>::Position,
                   Code = PositionEnum<CodeAttribut>::Position,
                   Nc = PositionEnum<NcAttribut>::Position,
                   Nom = PositionEnum<NomAttribut>::Position,
                   StyleNum = PositionEnum<StyleNumAttribut>::Position,
                   Type = PositionEnum<TypeAttribut>::Position,
                   NbrAtt = EAID::NbrAtt,
                   IdAn = Id1,
                   IdClasse = Id2,
                  };

    //! Code du groupe.
    enum codeFlag : flag::flag_type {Aucun = 0x0,
                                     Exclusif = 0x1,
                                     Total = 0x2
    };

    using EAID::EntityID;
    BASE_ENTITY(Groupe)
    ALIAS_CLE(An,1)
    ALIAS_CLE(Classe,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    Groupe(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    Groupe(idt idAn, idt idClasse, uint code, const QString & nc, const QString & nom,  uint styleNum, idt type,  idt id = 0)
        : Groupe(nom,id) {
        setIdAn(idAn);
        setIdClasse(idClasse);
        setStyleNum(styleNum);
        setCode(code);
        setNc(nc);
        setNom(nom);
        setType(type);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité TypeControle.
 */
class TypeControle : public emps::EntityIDs<infoEntityNote::TypeControleId,attributMPS::CodeAttribut,
                                                                           attributMPS::DecimaleAttribut,
                                                                           MinimaAttribut,
                                                                           attributMPS::NcNomAttribut,
                                                                           attributMPS::ParentAttribut,
                                                                           attributMPS::TotalAttribut> {
protected:
    template<class T> using PositionEnum = emps::PositionEnum<T,TypeControle>;
public:
    using EAID = emps::EntityIDs<infoEntityNote::TypeControleId,attributMPS::CodeAttribut,
                                                                attributMPS::DecimaleAttribut,
                                                                MinimaAttribut,
                                                                attributMPS::NcNomAttribut,
                                                                attributMPS::ParentAttribut,
                                                                attributMPS::TotalAttribut>;
    //! Code du groupe.
    enum codeFlag : flag::flag_type {Aucun = 0x0,
                                     Categorie = 0x1,
                                     Note = 0x2,
                                     Lettre = 0x4,
                                     Competences = 0x8,
                                     Capacites = 0x10,
                                     NotationModifiable = 0x20,
                                     TotalModifiable = 0x40,
                                     DecimaleModifiable = 0x80
    };
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut>::Position,
                   Code = PositionEnum<CodeAttribut>::Position,
                   Decimale = PositionEnum<DecimaleAttribut>::Position,
                   Minima = PositionEnum<MinimaAttribut>::Position,
                   Nc = PositionEnum<NcAttribut>::Position,
                   Nom = PositionEnum<NomAttribut>::Position,
                   Parent = PositionEnum<ParentAttribut>::Position,
                   Total = PositionEnum<TotalAttribut>::Position,
                   NbrAtt = EAID::NbrAtt,
                   Ordre = Nom,
                   };

    using EAID::EntityID;
    BASE_ENTITY(TypeControle)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    TypeControle(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    TypeControle(flag code, szt decimale, int minima, const QString & nc,
                 const QString & nom, idt parent, int total, idt id = 0)
        : TypeControle(nom, id) {
        setCode(code);
        setDecimale(decimale);
        setMinima(minima);
        setNc(nc);
        setParent(parent);
        setTotal(total);
    }
};
}
#endif // ENTITYNOTE_H
