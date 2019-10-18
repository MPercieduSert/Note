/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef ENTITYNOTE_H
#define ENTITYNOTE_H

#include "EntityCible.h"
#include "EntityDivers.h"
#include "EntityRelation.h"

/*! \defgroup groupeManagerNote Entités et base de données note
 * \brief Ensemble des entités, managers et base de données de l'application note.
 */

namespace InfoEntity {

    //! Liste des identifiants des entités
    enum entityId{AcquisitionId = NbrEntityPredefinie,
                  AnneeId,
                  BaremeId,
                  ClasseId,
                  ClasseEleveId,
                  ClasseGroupeId,
                  ControleId,
                  ControleEnonceId,
                  EleveId,
                  EleveGroupeId,
                  EnonceId,
                  EtablissementId,
                  EtablissementNiveauId,
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
// Entités de type prédéfinies
ID1_ENTITY(Bareme,IdTypeValeurDouble,InfoEntity::BaremeId,Point)
ID1_ENTITY(Etablissement,IdNcNom,InfoEntity::EtablissementId,TpEtab)
ID1_ENTITY(Point,IdNumTypeVersionValeurInt,InfoEntity::PointId,Enonce)
RELATION_ENTITY(EleveGroupe,Num,InfoEntity::EleveGroupeId,Eleve,Groupe)
RELATION_ENTITY(EtablissementNiveau,,InfoEntity::EtablissementNiveauId,Etab,Niveau)
RELATION_ENTITY(FiliationNiveau,,InfoEntity::FiliationNiveauId,Precedent,Suivant)
RELATION_ENTITY(NiveauTypeEtablissement,,InfoEntity::NiveauTypeEtablissementId,Niveau,TpEtab)
RELATION_ENTITY(Note,CodeDateTimeCurrentNumValeurInt,InfoEntity::NoteId,Controle,Eleve)
RELATION_ENTITY(Valide,DateTimeCurrentValeurInt,InfoEntity::ValideId,Note,Point)
using Annee = NumEntity<InfoEntity::AnneeId>;
using Enonce = IdNullTypeVersionEntity<InfoEntity::BaremeId>;
using TypeEtablissement = NcNomEntity<InfoEntity::EtablissementId>;
using Niveau = NcNomTypeEntity<InfoEntity::NiveauId>;



// Attribut
SINGLE_ATTRIBUT(AppreciationAttribut,AttributCode,Appreciation,appreciation,int)
SINGLE_ATTRIBUT(EntreeAttribut,AttributDateValide,Entree,entree,QDate)
SINGLE_ATTRIBUT(FilleAttribut,AttributBool,Fille,fille,bool)
SINGLE_ATTRIBUT(MinimaAttribut,AttributNumNull,Minima,minima,int)
SINGLE_ATTRIBUT(PrenomAttribut,AttributStringNotEmpty,Prenom,prenom,QString)
SINGLE_ATTRIBUT(SortieAttribut,AttributDateNull,Sortie,sortie,QDate)

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Classe.
 */
class Classe : public EntityAttributsID<Attributs<Id1Attribut,
                                                   Id2Attribut,
                                                   Id3Attribut,
                                                   NumAttribut>,
                                            InfoEntity::ClasseId>
{
public:
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,Classe>::Position,
                   Id1 = PositionEnum<Id1Attribut,Classe>::Position,
                   Id2 = PositionEnum<Id2Attribut,Classe>::Position,
                   Id3 = PositionEnum<Id3Attribut,Classe>::Position,
                   Num = PositionEnum<NumAttribut,Classe>::Position,
                   NbrAtt,
                   IdAn = Id1,
                   IdEtab = Id2,
                   IdNiveau = Id3,
                   };

    using EAID = EntityAttributsID<Attributs<Id1Attribut,Id2Attribut,Id3Attribut,NumAttribut>,InfoEntity::ClasseId>;
    using EAID::EntityAttributsID;
    BASE_ENTITY(Classe)
    ALIAS_CLE(An,1)
    ALIAS_CLE(Etab,2)
    ALIAS_CLE(Niveau,3)

    //! Constructeur à partir des valeurs attributs.
    Classe(int idAn, int idEtab, int idNiveau, int num, int id = 0)
        : EAID(id)
    {
        setIdAn(idAn);
        setIdEtab(idEtab);
        setIdNiveau(idNiveau);
        setNum(num);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité ClasseEleve.
 */
class ClasseEleve : public EntityAttributsID<Attributs<RelationAttribut,EntreeAttribut,SortieAttribut>,InfoEntity::ClasseEleveId>
{
public:
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,ClasseEleve>::Position,
                   Id1 = PositionEnum<Id1Attribut,ClasseEleve>::Position,
                   Id2 = PositionEnum<Id2Attribut,ClasseEleve>::Position,
                   Entree = PositionEnum<EntreeAttribut,ClasseEleve>::Position,
                   Sortie = PositionEnum<SortieAttribut,ClasseEleve>::Position,
                   NbrAtt,
                   IdClasse = Id1,
                   IdEleve = Id2};

    using EAID = EntityAttributsID<Attributs<RelationAttribut,EntreeAttribut,SortieAttribut>,InfoEntity::ClasseEleveId>;
    using EAID::EntityAttributsID;
    BASE_ENTITY(ClasseEleve)
    ALIAS_CLE(Classe,1)
    ALIAS_CLE(Eleve,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    ClasseEleve(int idCl, int idEl, int id = 0)
        : EAID(id)
    {
        setIdClasse(idCl);
        setIdEleve(idEl);
    }

    //! Constructeur à partir des valeurs attributs.
    ClasseEleve(int idCl, int idEl, const QDate & entree, const QDate & sortie = QDate(), int id = 0)
        : ClasseEleve(idCl, idEl, id)
    {
          setEntree(entree);
          setSortie(sortie);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité ClasseGroupe.
 */
class ClasseGroupe : public EntityAttributsID<Attributs<RelationExactOneNotNullAttribut,Id3Attribut>,InfoEntity::ClasseGroupeId>
{
public:
    using EAID = EntityAttributsID<Attributs<RelationExactOneNotNullAttribut,Id3Attribut>,InfoEntity::ClasseGroupeId>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,ClasseGroupe>::Position,
                   Id1 = PositionEnum<Id1NullAttribut,ClasseGroupe>::Position,
                   Id2 = PositionEnum<Id2NullAttribut,ClasseGroupe>::Position,
                   Id3 = PositionEnum<Id3Attribut,ClasseGroupe>::Position,
                   NbrAtt = EAID::NbrAtt,
                   IdAn = Id1,
                   IdClasse = Id2,
                   IdGroupe = Id3
                  };

    using EAID::EntityAttributsID;
    BASE_ENTITY(ClasseGroupe)
    ALIAS_CLE(An,1)
    ALIAS_CLE(Classe,2)
    ALIAS_CLE(Groupe,3)

    //! Constructeur à partir des valeurs attributs.
    ClasseGroupe(int idAn, int idClasse, int idGroupe, int id = 0)
        : EAID(id)
    {
        setIdAn(idAn);
        setIdClasse(idClasse);
        setIdGroupe(idGroupe);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Controle.
 */
class Controle : public EntityAttributsID<Attributs<Id1Attribut,
                                                   CodeAttribut,
                                                   DateTimeValideAttribut,
                                                   DecimaleAttribut,
                                                   MinimaAttribut,
                                                   NcNomAttribut,
                                                   NumAttribut,
                                                   TotalAttribut>,
                                            InfoEntity::ControleId>
{
public:
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,Controle>::Position,
                   Id1 = PositionEnum<Id1Attribut,Controle>::Position,
                   Code = PositionEnum<CodeAttribut,Controle>::Position,
                   DateTime = PositionEnum<DateTimeValideAttribut,Controle>::Position,
                   Decimale = PositionEnum<DecimaleAttribut,Controle>::Position,
                   Minima = PositionEnum<MinimaAttribut,Controle>::Position,
                   Nc = PositionEnum<NcAttribut,Controle>::Position,
                   Nom = PositionEnum<NomAttribut,Controle>::Position,
                   Num = PositionEnum<NumAttribut,Controle>::Position,
                   Total = PositionEnum<TotalAttribut,Controle>::Position,
                   NbrAtt,
                   IdType = Id1,
                   };

    using EAID = EntityAttributsID<Attributs<Id1Attribut,CodeAttribut,DateTimeValideAttribut,DecimaleAttribut,MinimaAttribut,
                                                NcNomAttribut,NumAttribut,TotalAttribut>,InfoEntity::ControleId>;
    using EAID::EntityAttributsID;
    BASE_ENTITY(Controle)
    ALIAS_CLE(Type,1)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    Controle(const QString & nom, int id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    Controle(int idTp, int code, const QDateTime & dateTime, int decimale, int minima, const QString & nc, const QString & nom, int num,
                int total, int id = 0)
        : Controle(nom, id)
    {
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
 * \brief Les attributs de l'entité ControleEnonce.
 */
class ControleEnonceAttribut : public Attributs<RelationTroisExactOneNotNullAttribut,Id4Attribut,Id5Attribut,NumNullAttribut>
{
public:
    CONSTR_DEFAUT(ControleEnonceAttribut)
    CONSTR_AFFECT_DEFAUT(ControleEnonceAttribut)

    //! Destructeur.
    ~ControleEnonceAttribut() override;

    //! Teste si l'entité est valide.
    bool isValid() const override
        {return (num() == -1 &&((Id1NullAttribut::isValid() && id2() == 0 && id3() == 0)
                                || (Id2NullAttribut::isValid() && id1() == 0 && id3() == 0)))
                || (num() >= 0 && Id3NullAttribut::isValid() && id1() == 0 && id2() == 0);}
};


/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité ControleEnonce.
 */
class ControleEnonce : public EntityAttributsID<ControleEnonceAttribut,InfoEntity::ControleEnonceId>
{
public:
    using EAID = EntityAttributsID<ControleEnonceAttribut,InfoEntity::ControleEnonceId>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,ControleEnonce>::Position,
                   Id1 = PositionEnum<Id1NullAttribut,ControleEnonce>::Position,
                   Id2 = PositionEnum<Id2NullAttribut,ControleEnonce>::Position,
                   Id3 = PositionEnum<Id3NullAttribut,ControleEnonce>::Position,
                   Id4 = PositionEnum<Id4Attribut,ControleEnonce>::Position,
                   Id5 = PositionEnum<Id5Attribut,ControleEnonce>::Position,
                   Num = PositionEnum<NumAttribut,ControleEnonce>::Position,
                   NbrAtt = EAID::NbrAtt,
                   IdClasse = Id1,
                   IdEleve = Id2,
                   IdGroupe = Id3,
                   IdControle = Id4,
                   IdEnonce = Id5
                  };

    using EAID::EntityAttributsID;
    BASE_ENTITY(ControleEnonce)
    ALIAS_CLE(Classe,1)
    ALIAS_CLE(Eleve,2)
    ALIAS_CLE(Groupe,3)
    ALIAS_CLE(Controle,4)
    ALIAS_CLE(Enonce,5)

    //! Constructeur à partir des valeurs attributs.
    ControleEnonce(int idClasse, int idControle, int idEleve, int idEnonce, int idGroupe, int num, int id = 0)
        : EAID(id)
    {
        setIdClasse(idClasse);
        setIdControle(idControle);
        setIdEleve(idEleve);
        setIdEnonce(idEnonce);
        setIdGroupe(idGroupe);
        setNum(num);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Eleve.
 */
class Eleve : public EntityAttributsID<Attributs<DateValideAttribut,FilleAttribut,NomAttribut,PrenomAttribut>,InfoEntity::EleveId>
{
public:
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,Eleve>::Position,
                   Date = PositionEnum<DateValideAttribut,Eleve>::Position,
                   Fille = PositionEnum<FilleAttribut,Eleve>::Position,
                   Nom = PositionEnum<NomAttribut,Eleve>::Position,
                   Prenom = PositionEnum<PrenomAttribut,Eleve>::Position,
                   NbrAtt};

    using EAID = EntityAttributsID<Attributs<DateValideAttribut,FilleAttribut,NomAttribut,PrenomAttribut>,InfoEntity::EleveId>;
    using EAID::EntityAttributsID;
    BASE_ENTITY(Eleve)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    Eleve(const QDate & date, const QString & nom, const QString & prenom,  int id = 0)
        : EAID(id)
    {
        setDate(date);
        setNom(nom);
        setPrenom(prenom);
    }

    //! Constructeur à partir des valeurs attributs.
    Eleve(const QDate & date, bool fille, const QString & nom, const QString & prenom,  int id = 0)
        : Eleve(date, nom, prenom, id)
        {setFille(fille);}
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Groupe.
 */
class Groupe : public EntityAttributsID<Attributs<AlphaAttribut,NcNomTypeAttribut>,InfoEntity::GroupeId>
{
public:
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,Groupe>::Position,
                   Alpha = PositionEnum<AlphaAttribut,Groupe>::Position,
                   Nc = PositionEnum<NcAttribut,Groupe>::Position,
                   Nom = PositionEnum<NomAttribut,Groupe>::Position,
                   Type = PositionEnum<TypeAttribut,Groupe>::Position,
                   NbrAtt};

    using EAID = EntityAttributsID<Attributs<AlphaAttribut,NcNomTypeAttribut>,InfoEntity::GroupeId>;
    using EAID::EntityAttributsID;
    BASE_ENTITY(Groupe)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    Groupe(const QString & nom, int id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    Groupe(int alpha, const QString & nc, const QString & nom, int type,  int id = 0)
        : Groupe(nom,id)
    {
        setAlpha(alpha);
        setNc(nc);
        setNom(nom);
        setType(type);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité TypeControle.
 */
class TypeControle : public EntityAttributsID<Attributs<Id1Attribut,
                                                   AppreciationAttribut,
                                                   CodeAttribut,
                                                   DecimaleAttribut,
                                                   MinimaAttribut,
                                                   ModifAttribut,
                                                   NcNomAttribut,
                                                   ArbreSimpleAttribut,
                                                   TotalAttribut>,
                                            InfoEntity::TypeControleId>
{
public:
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,TypeControle>::Position,
                   Appreciation = PositionEnum<AppreciationAttribut,TypeControle>::Position,
                   Code = PositionEnum<CodeAttribut,TypeControle>::Position,
                   Decimale = PositionEnum<DecimaleAttribut,TypeControle>::Position,
                   Minima = PositionEnum<MinimaAttribut,TypeControle>::Position,
                   Modif = PositionEnum<ModifAttribut,TypeControle>::Position,
                   Nc = PositionEnum<NcAttribut,TypeControle>::Position,
                   Nom = PositionEnum<NomAttribut,TypeControle>::Position,
                   Parent = PositionEnum<ParentAttribut,TypeControle>::Position,
                   Total = PositionEnum<TotalAttribut,TypeControle>::Position,
                   NbrAtt,
                   Ordre = Nom,
                   };

    using EAID = EntityAttributsID<Attributs<Id1Attribut,AppreciationAttribut,CodeAttribut,DecimaleAttribut,MinimaAttribut,ModifAttribut,
                                                NcNomAttribut,ArbreSimpleAttribut,TotalAttribut>,InfoEntity::TypeControleId>;
    using EAID::EntityAttributsID;
    BASE_ENTITY(TypeControle)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    TypeControle(const QString & nom, int id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    TypeControle(int appreciation, int code, int decimale, int minima, int modif, const QString & nc, const QString & nom, int parent,
                int total, int id = 0)
        : TypeControle(nom, id)
    {
        setAppreciation(appreciation);
        setCode(code);
        setDecimale(decimale);
        setMinima(minima);
        setModif(modif);
        setNc(nc);
        setParent(parent);
        setTotal(total);
    }
};
#endif // ENTITYNOTE_H
