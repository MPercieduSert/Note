/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef ENTITYNOTE_H
#define ENTITYNOTE_H

#include "entity_predef.h"

/*! \defgroup groupeManagerNote Entités et base de données note
 * \brief Ensemble des entités, managers et base de données de l'application note.
 */

namespace info_entityNote {

    //! Liste des identifiants des entités
    enum entityId{AnneeId = info_entity::NbrEntityPredefinie,
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
                  ExerciceId,
                  FiliationNiveauId,
                  GroupeId,
                  NiveauId,
                  NiveauTypeEtablissementId,
                  NoteId,
                  PointId,
                  TypeControleId,
                  TypeEtablissementId,
                  _valideId,
                  NbrEntityId};
}

/*! \ingroup groupeManagerNote
 * \brief Espace de noms de l'application note.
 */
namespace noteMPS {
using namespace type_mps;
namespace emps = entityMPS;

// Entités prédéfinies
using namespace emps::ensemble_commentaire;
using namespace emps::ensemble_donnee;
using namespace emps::ensemble_evenement;
using namespace emps::ensemble_mot_cle;
using namespace emps::ensemble_restriction;
using namespace emps::ensemble_texte;
using namespace emps::ensemble_type;
using namespace emps::ensemble_utilisation;

// Entités de type prédéfinies
RELATION_ENTITY(EleveGroupe,relation_num,info_entityNote::EleveGroupeId,Eleve,Groupe)
RELATION_ENTITY(EtablissementNiveau,Relation,info_entityNote::EtablissementNiveauId,Etab,Niveau)
RELATION_ENTITY(EtablissementType,Relation,info_entityNote::EtablissementTypeId,Etab,TpEtab)
RELATION_ENTITY(FiliationNiveau,Relation,info_entityNote::FiliationNiveauId,Precedent,Suivant)
RELATION_ENTITY(NiveauTypeEtablissement,Relation,info_entityNote::NiveauTypeEtablissementId,Niveau,TpEtab)
RELATION_ENTITY(Note,RelationDateTime_current_saisie_valeurInt,info_entityNote::NoteId,Controle,Eleve)
RELATION_ENTITY(_valide,RelationDateTime_current_valeurInt,info_entityNote::_valideId,Note,Point)
//using Annee = entityBaseMPS::NumEntity<info_entityNote::AnneeId>;
using Bareme = entityBaseMPS::cible_type_valeurDoubleEntity<info_entityNote::BaremeId>;
using Enonce = entityBaseMPS::TypeVersionEntity<info_entityNote::EnonceId>;
using Etablissement = entityBaseMPS::NcNomEntity<info_entityNote::EtablissementId>;
using Niveau = entityBaseMPS::Ncnom_typeEntity<info_entityNote::NiveauId>;
using Point = entityBaseMPS::type_valeurIntVersionEntity<info_entityNote::PointId>;
using TypeEtablissement = entityBaseMPS::NcNomEntity<info_entityNote::TypeEtablissementId>;

// Attribut
SINGLE_ATTRIBUT(EnsPointAttribut,attributMPS::Attribut_id,EnsPoint,ensPoint)
SINGLE_ATTRIBUT(EntreeAttribut,attributMPS::attribut_date_valide,Entree,entree)
SINGLE_ATTRIBUT(FilleAttribut,attributMPS::attribut_bool,Fille,fille)
SINGLE_ATTRIBUT(MinimaAttribut,attributMPS::attribut_int_sup<0>,Minima,minima)
SINGLE_ATTRIBUT(NumCEAttribut,attributMPS::attribut_int_sup<-1>,Num,num)
SINGLE_ATTRIBUT(PrenomAttribut,attributMPS::attribut_string_not_empty,Prenom,prenom)
SINGLE_ATTRIBUT(SortieAttribut,attributMPS::attribut_date_valide,Sortie,sortie)

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Annee.
 */
class Annee : public emps::EntityID<info_entityNote::AnneeId,attributMPS::num_attribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,Annee>;
public:
using EAID = emps::EntityID<info_entityNote::AnneeId,attributMPS::num_attribut>;

    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Num = positionEnum<num_attribut>::position,
                   Nbr_Att = EAID::Nbr_Att};

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
class Classe : public emps::EntityIDs<info_entityNote::ClasseId,attributMPS::id_1_attribut,
                                                               attributMPS::id_2_attribut,
                                                               attributMPS::id_3_attribut,
                                                               attributMPS::nc_attribut,
                                                               attributMPS::nom_attribut,
                                                               attributMPS::num_attribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,Classe>;
public:
using EAID = emps::EntityIDs<info_entityNote::ClasseId,attributMPS::id_1_attribut,
                                                      attributMPS::id_2_attribut,
                                                      attributMPS::id_3_attribut,
                                                      attributMPS::nc_attribut,
                                                      attributMPS::nom_attribut,
                                                      attributMPS::num_attribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Id1 = positionEnum<id_1_attribut>::position,
                   Id2 = positionEnum<id_2_attribut>::position,
                   id_3 = positionEnum<id_3_attribut>::position,
                   Nc = positionEnum<nc_attribut>::position,
                   Nom = positionEnum<nom_attribut>::position,
                   Num = positionEnum<num_attribut>::position,
                   Nbr_Att = EAID::Nbr_Att,
                   IdAn = Id1,
                   IdEtab = Id2,
                   IdNiveau = id_3,
                   };

    using EAID::EntityID;
    BASE_ENTITY(Classe)
    ALIAS_CLE(An,1)
    ALIAS_CLE(Etab,2)
    ALIAS_CLE(Niveau,3)

    //! Constructeur à partir des valeurs attributs.
    Classe(idt idAn, idt idEtab, idt idNiveau, const QString & nc, const QString & nom, int num, idt id = 0)
        : EAID(id) {
        set_idAn(idAn);
        set_idEtab(idEtab);
        set_idNiveau(idNiveau);
        setNc(nc);
        setNom(nom);
        setNum(num);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité ClasseEleve.
 */
class ClasseEleve : public emps::EntityIDs<info_entityNote::ClasseEleveId,attributMPS::relation_attribut,
                                                                         EntreeAttribut,
                                                                         SortieAttribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,ClasseEleve>;
public:
    using EAID = emps::EntityIDs<info_entityNote::ClasseEleveId,attributMPS::relation_attribut,
                                                               EntreeAttribut,
                                                               SortieAttribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Id1 = positionEnum<id_1_attribut>::position,
                   Id2 = positionEnum<id_2_attribut>::position,
                   Entree = positionEnum<EntreeAttribut>::position,
                   Sortie = positionEnum<SortieAttribut>::position,
                   Nbr_Att =EAID::Nbr_Att,
                   IdClasse = Id1,
                   IdEleve = Id2};

    using EAID::EntityID;
    BASE_ENTITY(ClasseEleve)
    ALIAS_CLE(Classe,1)
    ALIAS_CLE(Eleve,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    ClasseEleve(idt idCl, idt idEl, idt id = 0)
        : EAID(id) {
        set_idClasse(idCl);
        set_idEleve(idEl);
    }

    //! Constructeur à partir des valeurs attributs.
    ClasseEleve(idt idCl, idt idEl, const QDate & entree, const QDate & sortie = QDate(), idt id = 0)
        : ClasseEleve(idCl, idEl, id) {
          setEntree(entree);
          setSortie(sortie);
    }

    //! Validité de l'entitée.
    bool is_valid() const override {
        return EAID::is_valid() && entree() <= sortie();
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Controle.
 */
class Controle : public emps::EntityIDs<info_entityNote::ControleId,attributMPS::id_1_attribut,
                                                                   attributMPS::cibles_attribut,
                                                                   attributMPS::code_attribut,
                                                                   attributMPS::date_time_valide_attribut,
                                                                   attributMPS::decimale_attribut,
                                                                   MinimaAttribut,
                                                                   attributMPS::nc_nom_attribut,
                                                                   attributMPS::num_attribut,
                                                                   attributMPS::total_attribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,Controle>;
public:
    using EAID = emps::EntityIDs<info_entityNote::ControleId,attributMPS::id_1_attribut,
                                                            attributMPS::cibles_attribut,
                                                            attributMPS::code_attribut,
                                                            attributMPS::date_time_valide_attribut,
                                                            attributMPS::decimale_attribut,
                                                            MinimaAttribut,
                                                            attributMPS::nc_nom_attribut,
                                                            attributMPS::num_attribut,
                                                            attributMPS::total_attribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Id1 = positionEnum<id_1_attribut>::position,
                   Id_Cible = positionEnum<id_cible_attribut>::position,
                   Cible = positionEnum<cible_attribut>::position,
                   Code = positionEnum<code_attribut>::position,
                   DateTime = positionEnum<date_time_valide_attribut>::position,
                   Decimale = positionEnum<decimale_attribut>::position,
                   Minima = positionEnum<MinimaAttribut>::position,
                   Nc = positionEnum<nc_attribut>::position,
                   Nom = positionEnum<nom_attribut>::position,
                   Num = positionEnum<num_attribut>::position,
                   Total = positionEnum<total_attribut>::position,
                   Nbr_Att = EAID::Nbr_Att,
                   id_type = Id1,
                   };

    //! Decimale pour les lettres.
    enum decmaleLettre {NoPlus = 1,
                        Plus = 3,
                        PPlus = 5,
                        PPPlus = 7
    };

    //! Code du type de controle.
    enum code_flag : flag::flag_type {Aucun = 0x0,
                                     Note = 0x1,
                                     Lettre = 0x2,
                                     Depassement = 0x4,
                                     Courbe = 0x8,
                                     Classement = 0x10,
                                     Barre = 0x20,
                                     Competences = 0x40,
                                     Capacites = 0x80,
                                     Appreciation = 0x100,
                                     Commentaire = 0x200,
                                     NonCommun = 0x10000,
                                     HoraireGroupe = NonCommun,
                                     HoraireEleve = NonCommun * 0x2,
                                     SujetGroupe = NonCommun * 0x4,
                                     SujetEleve = NonCommun * 0x8
    };

    using EAID::EntityID;
    BASE_ENTITY(Controle)
    ALIAS_CLE(Type,1)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    Controle(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    Controle(idt idTp, idt id_cible, int cible, flag code, const QDateTime & dateTime, int decimale, int minima,
             const QString & nc, const QString & nom, int num, int total, idt id = 0)
        : Controle(nom, id) {
        set_id_type(idTp);
        set_id_cible(id_cible);
        set_cible(cible);
        set_code(code);
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
class ControleEpreuveAttribut : public emps::Attributs<attributMPS::relation_trois_exact_one_not_null_attribut,
                                                       attributMPS::id_4_attribut,
                                                       attributMPS::id_5_attribut,
                                                       NumCEAttribut> {
public:
    CONSTR_DEFAUT(ControleEpreuveAttribut)
    CONSTR_AFFECT_DEFAUT(ControleEpreuveAttribut)

    //! Destructeur.
    ~ControleEpreuveAttribut() override;

    //! Teste si l'entité est valide.
    bool is_valid() const override
        {return (num() == -1 &&((id_1_null_attribut::is_valid() && id2() == 0 && id_3() == 0)
                                || (id_2_null_attribut::is_valid() && id1() == 0 && id_3() == 0)))
                || (num() >= 0 && id_3_null_attribut::is_valid() && id1() == 0 && id2() == 0);}
};


/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité ControleEpreuve.
 */
class ControleEpreuve : public emps::EntityIDs<info_entityNote::ControleEpreuveId,ControleEpreuveAttribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,ControleEpreuve>;
public:
    using EAID = emps::EntityIDs<info_entityNote::ControleEpreuveId,ControleEpreuveAttribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Id1 = positionEnum<id_1_null_attribut>::position,
                   Id2 = positionEnum<id_2_null_attribut>::position,
                   id_3 = positionEnum<id_3_null_attribut>::position,
                   Id4 = positionEnum<id_4_attribut>::position,
                   Id5 = positionEnum<id_5_attribut>::position,
                   Num = positionEnum<NumCEAttribut>::position,
                   Nbr_Att = EAID::Nbr_Att,
                   IdClasse = Id1,
                   IdEleve = Id2,
                   IdGroupe = id_3,
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
        set_idClasse(idClasse);
        set_idControle(idControle);
        set_idEleve(idEleve);
        set_idEpreuve(idEpreuve);
        set_idGroupe(idGroupe);
        setNum(num);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Eleve.
 */
class Eleve : public emps::EntityIDs<info_entityNote::EleveId,attributMPS::date_valide_attribut,
                                                             FilleAttribut,
                                                             attributMPS::nom_attribut,
                                                             PrenomAttribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,Eleve>;
public:
    using EAID = emps::EntityIDs<info_entityNote::EleveId,attributMPS::date_valide_attribut,
                                                         FilleAttribut,
                                                         attributMPS::nom_attribut,
                                                         PrenomAttribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Date = positionEnum<date_valide_attribut>::position,
                   Fille = positionEnum<FilleAttribut>::position,
                   Nom = positionEnum<nom_attribut>::position,
                   Prenom = positionEnum<PrenomAttribut>::position,
                   Nbr_Att = EAID::Nbr_Att
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
class EnoncePoint : public emps::EntityIDs<info_entityNote::EnoncePointId,attributMPS::relation_attribut,
                                                                         EnsPointAttribut,
                                                                         attributMPS::num_attribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,EnoncePoint>;
public:
    using EAID = emps::EntityIDs<info_entityNote::EnoncePointId,attributMPS::relation_attribut,
                                                               EnsPointAttribut,
                                                               attributMPS::num_attribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Id1 = positionEnum<id_1_attribut>::position,
                   Id2 = positionEnum<id_2_attribut>::position,
                   EnsPoint = positionEnum<EnsPointAttribut>::position,
                   Num = positionEnum<num_attribut>::position,
                   Nbr_Att =EAID::Nbr_Att,
                   IdEnonce = Id1,
                   IdPoint = Id2};

    using EAID::EntityID;
    BASE_ENTITY(EnoncePoint)
    ALIAS_CLE(Enonce,1)
    ALIAS_CLE(Point,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    EnoncePoint(idt idEn, idt ensPt, int num , idt id = 0)
        : EAID(id) {
        set_idEnonce(idEn);
        setEnsPoint(ensPt);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    EnoncePoint(idt idEn, idt idPt, idt ensPt, int num, idt id = 0)
        : EnoncePoint(idEn, ensPt, num, id)
        {set_idPoint(idPt);}
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Exercice.
 */
class Exercice : public emps::EntityIDs<info_entityNote::ExerciceId,attributMPS::id_1_null_attribut,
                                                                   attributMPS::type_attribut,
                                                                   attributMPS::version_attribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,Exercice>;
public:
    using EAID = emps::EntityIDs<info_entityNote::ExerciceId,attributMPS::id_1_null_attribut,
                                                            attributMPS::type_attribut,
                                                            attributMPS::version_attribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Id1 = positionEnum<id_1_null_attribut>::position,
                   Type = positionEnum<type_attribut>::position,
                   Version = positionEnum<version_attribut>::position,
                   Nbr_Att = EAID::Nbr_Att,
                   IdOriginal = Id1};

    using EAID::EntityID;
    BASE_ENTITY(Exercice)
    ALIAS_CLE(Original,1)

    //! Constructeur à partir des valeurs attributs.
    Exercice(idt idOriginal, idt type, int version,  idt id = 0)
        : EAID(id) {
        set_idOriginal(idOriginal);
        setType(type);
        setVersion(version);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Epreuve.
 */
class Epreuve : public emps::EntityIDs<info_entityNote::EpreuveId,attributMPS::relation_attribut,
                                                                 EnsPointAttribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,Epreuve>;
public:
    using EAID = emps::EntityIDs<info_entityNote::EpreuveId,attributMPS::relation_attribut,
    EnsPointAttribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Id1 = positionEnum<id_1_attribut>::position,
                   Id2 = positionEnum<id_2_attribut>::position,
                   EnsPoint = positionEnum<EnsPointAttribut>::position,
                   Nbr_Att =EAID::Nbr_Att,
                   IdBareme = Id1,
                   IdEnonce= Id2};

    using EAID::EntityID;
    BASE_ENTITY(Epreuve)
    ALIAS_CLE(Bareme,1)
    ALIAS_CLE(Enonce,2)

    //! Constructeur à partir des valeurs attributs.
    Epreuve(idt idBar, idt idEn, idt ensPt, idt id = 0)
        : EAID(id) {
        set_idBareme(idBar);
        set_idEnonce(idEn);
        setEnsPoint(ensPt);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité Groupe.
 */
class Groupe : public emps::EntityIDs<info_entityNote::GroupeId,attributMPS::relation_exact_one_not_null_attribut,
                                                            attributMPS::style_num_attribut,
                                                            attributMPS::code_attribut,
                                                            attributMPS::nc_nom_type_attribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,Groupe>;
public:
    using EAID = emps::EntityIDs<info_entityNote::GroupeId,attributMPS::relation_exact_one_not_null_attribut,
                                                          attributMPS::style_num_attribut,
                                                          attributMPS::code_attribut,
                                                          attributMPS::nc_nom_type_attribut>;
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Id1 = positionEnum<id_1_null_attribut>::position,
                   Id2 = positionEnum<id_2_null_attribut>::position,
                   Code = positionEnum<code_attribut>::position,
                   Nc = positionEnum<nc_attribut>::position,
                   Nom = positionEnum<nom_attribut>::position,
                   StyleNum = positionEnum<style_num_attribut>::position,
                   Type = positionEnum<type_attribut>::position,
                   Nbr_Att = EAID::Nbr_Att,
                   IdAn = Id1,
                   IdClasse = Id2,
                  };

    //! Code du groupe.
    enum code_flag : flag::flag_type {Aucun = 0x0,
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
    Groupe(idt idAn, idt idClasse, flag_type code, const QString & nc, const QString & nom,  enumt styleNum, idt type,  idt id = 0)
        : Groupe(nom,id) {
        set_idAn(idAn);
        set_idClasse(idClasse);
        set_styleNum(styleNum);
        set_code(code);
        setNc(nc);
        setNom(nom);
        setType(type);
    }
};

/*! \ingroup groupeManagerNote
 * \brief Représentation de l'entité TypeControle.
 */
class TypeControle : public emps::EntityIDs<info_entityNote::TypeControleId,attributMPS::code_attribut,
                                                                           attributMPS::decimale_attribut,
                                                                           MinimaAttribut,
                                                                           attributMPS::nc_nom_attribut,
                                                                           attributMPS::parent_attribut,
                                                                           attributMPS::total_attribut> {
protected:
    template<class T> using positionEnum = emps::positionEnum<T,TypeControle>;
public:
    using EAID = emps::EntityIDs<info_entityNote::TypeControleId,attributMPS::code_attribut,
                                                                attributMPS::decimale_attribut,
                                                                MinimaAttribut,
                                                                attributMPS::nc_nom_attribut,
                                                                attributMPS::parent_attribut,
                                                                attributMPS::total_attribut>;
    //! Code du type de controle.
    enum code_flag : flag::flag_type {Aucun = 0x0,
                                     Note = Controle::Note,
                                     Lettre = Controle::Lettre,
                                     Depassement = Controle::Depassement,
                                     Courbe = Controle::Courbe,
                                     Classement = Controle::Classement,
                                     Barre = Controle::Barre,
                                     Competences = Controle::Competences,
                                     Capacites = Controle::Capacites,
                                     Appreciation = Controle::Appreciation,
                                     Commentaire = Controle::Commentaire,
                                     NonCommun = Controle::NonCommun,
                                     Categorie = NonCommun,
                                     NoteModifiable = NonCommun * 0x2,
                                     TypeNoteModifiable = NonCommun * 0x4,
                                     TotalModifiable = NonCommun * 0x8,
                                     DepassementModifiable = NonCommun * 0x10,
                                     DecimaleModifiable = NonCommun * 0x20,
                                     CourbeModifiable = NonCommun * 0x40,
                                     ClassementModifiable = NonCommun * 0x80,
                                     BarreModifiable = NonCommun * 0x100,
                                     MinimaModifiable = NonCommun * 0x200,
                                     CompetencesModifiable = NonCommun * 0x400,
                                     CapacitesModifiable = NonCommun * 0x800,
                                     AppreciationModifiable = NonCommun * 0x1000,
                                     CommentaireModifiable = NonCommun * 0x2000
    };
    //! positions des attributs.
    enum position {Id = positionEnum<id_attribut>::position,
                   Code = positionEnum<code_attribut>::position,
                   Decimale = positionEnum<decimale_attribut>::position,
                   Minima = positionEnum<MinimaAttribut>::position,
                   Nc = positionEnum<nc_attribut>::position,
                   Nom = positionEnum<nom_attribut>::position,
                   Parent = positionEnum<parent_attribut>::position,
                   Total = positionEnum<total_attribut>::position,
                   Nbr_Att = EAID::Nbr_Att,
                   Ordre = Nom,
                   };

    using EAID::EntityID;
    BASE_ENTITY(TypeControle)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    TypeControle(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    TypeControle(flag code, int decimale, int minima, const QString & nc,
                 const QString & nom, idt parent, int total, idt id = 0)
        : TypeControle(nom, id) {
        set_code(code);
        setDecimale(decimale);
        setMinima(minima);
        setNc(nc);
        set_parent(parent);
        setTotal(total);
    }
};
}
#endif // ENTITYNOTE_H
