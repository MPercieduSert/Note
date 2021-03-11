/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef ENTITY_NOTE_H
#define ENTITY_NOTE_H

#include "entity_predef.h"
#include "enum_manager_note.h"

/*! \defgroup groupe_manager_note Entités et base de données note
 * \brief Ensemble des entités, managers et base de données de l'application note.
 */

/*! \ingroup groupe_manager_note
 * \brief Espace de noms de l'application note.
 */
namespace note_mps {
// Type prédéfinies
using entidt = mps::entidt;
using enumt = mps::enumt;
using flag = mps::flag;
using idt = mps::idt;
using numt = mps::numt;
using post = mps::post;
using szt = mps::szt;

//using entities =

// Entités prédéfinies
using namespace mps::entities::ensemble_commentaire;
using namespace mps::entities::ensemble_donnee;
using namespace mps::entities::ensemble_evenement;
using namespace mps::entities::ensemble_mot_cle;
using namespace mps::entities::ensemble_restriction;
using namespace mps::entities::ensemble_texte;
using namespace mps::entities::ensemble_type;
using namespace mps::entities::ensemble_utilisation;

// Entités de type prédéfinies
RELATION_ENTITY(eleve_groupe,relation_num,info_entity_note::Eleve_Groupe,Eleve,eleve,Groupe,groupe)
RELATION_ENTITY(etablissement_niveau,relation,info_entity_note::Etablissement_Niveau,Etab,etab,Niveau,niveau)
RELATION_ENTITY(etablissement_type,relation,info_entity_note::Etablissement_Type,Etab,etab,Tp_Etab,tp_etab)
RELATION_ENTITY(filiation_niveau,relation,info_entity_note::Filiation_Niveau,Precedent,precedent,Suivant,suivant)
RELATION_ENTITY(niveau_type_etablissement,relation,info_entity_note::Niveau_Type_Etablissement,Niveau,niveau,Tp_Etab,tp_etab)
RELATION_ENTITY(note,relation_date_time_current_saisie_valeur_int,info_entity_note::Note,Controle,controle,Eleve,eleve)
RELATION_ENTITY(valide,relation_date_time_current_valeur_int,info_entity_note::Valide,Note,note,Point,point)
//using annee = mps::entities_base::NumEntity<info_entity_note::annee_Id>;
using bareme = mps::entities_base::cible_simple_type_valeur_double_entity<info_entity_note::Bareme>;
using enonce = mps::entities_base::type_version_entity<info_entity_note::Enonce>;
using etablissement = mps::entities_base::nc_nom_entity<info_entity_note::Etablissement>;
using niveau = mps::entities_base::nc_nom_type_entity<info_entity_note::Niveau>;
using point = mps::entities_base::type_valeur_int_version_entity<info_entity_note::Point>;
using type_etablissement = mps::entities_base::nc_nom_entity<info_entity_note::Type_Etablissement>;

// Attribut
SINGLE_ATTRIBUT(ens_point_attribut,mps::attribut::Attribut_id,Ens_Point,ens_point)
SINGLE_ATTRIBUT(entree_attribut,mps::attribut::attribut_date_valide,Entree,entree)
SINGLE_ATTRIBUT(fille_attribut,mps::attribut::attribut_bool,Fille,fille)
SINGLE_ATTRIBUT(minima_attribut,mps::attribut::attribut_int_sup<0>,Minima,minima)
SINGLE_ATTRIBUT(num_ce_attribut,mps::attribut::attribut_int_sup<-1>,Num,num)
SINGLE_ATTRIBUT(prenom_attribut,mps::attribut::attribut_string_not_empty,Prenom,prenom)
SINGLE_ATTRIBUT(sortie_attribut,mps::attribut::attribut_date_valide,Sortie,sortie)

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité annee.
 */
class annee : public mps::entities::entity_id<info_entity_note::Annee,mps::attribut::num_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,annee>;
public:
using eaid = mps::entities::entity_id<info_entity_note::Annee,mps::attribut::num_attribut>;

    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Num = position_enum<num_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using eaid::entity_id;
    BASE_ENTITY(annee)

    //! Constructeur à partir des valeurs attributs.
    annee(int num, idt id = 0)
        : eaid(id)
        {set_num(num);}

    //! Renvoie la chaîne de caractères "Num"-"Num+1".
    QString texte() const
        {return QString::number(num()).append("-").append(QString::number(num()+1));}
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité Classe.
 */
class classe : public mps::entities::entity_ids<info_entity_note::Classe,mps::attribut::id_1_attribut,
                                                               mps::attribut::id_2_attribut,
                                                               mps::attribut::id_3_attribut,
                                                               mps::attribut::nc_attribut,
                                                               mps::attribut::nom_attribut,
                                                               mps::attribut::num_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,classe>;
public:
using eaid = mps::entities::entity_ids<info_entity_note::Classe,mps::attribut::id_1_attribut,
                                                      mps::attribut::id_2_attribut,
                                                      mps::attribut::id_3_attribut,
                                                      mps::attribut::nc_attribut,
                                                      mps::attribut::nom_attribut,
                                                      mps::attribut::num_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_attribut>::Position,
                   Id_2 = position_enum<id_2_attribut>::Position,
                   Id_3 = position_enum<id_3_attribut>::Position,
                   Nc = position_enum<nc_attribut>::Position,
                   Nom = position_enum<nom_attribut>::Position,
                   Num = position_enum<num_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Id_An = Id_1,
                   Id_Etab = Id_2,
                   Id_Niveau = Id_3,
                   };

    using eaid::entity_id;
    BASE_ENTITY(classe)
    ALIAS_CLE(an,1)
    ALIAS_CLE(etab,2)
    ALIAS_CLE(niveau,3)

    //! Constructeur à partir des valeurs attributs.
    classe(idt id_an, idt id_etab, idt id_niveau, const QString & nc, const QString & nom, int num, idt id = 0)
        : eaid(id) {
        set_id_an(id_an);
        set_id_etab(id_etab);
        set_id_niveau(id_niveau);
        set_nc(nc);
        set_nom(nom);
        set_num(num);
    }
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité classe_eleve.
 */
class classe_eleve : public mps::entities::entity_ids<info_entity_note::Classe_Eleve,mps::attribut::relation_attribut,
                                                                         entree_attribut,
                                                                         sortie_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,classe_eleve>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Classe_Eleve,mps::attribut::relation_attribut,
                                                               entree_attribut,
                                                               sortie_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_attribut>::Position,
                   Id_2 = position_enum<id_2_attribut>::Position,
                   Entree = position_enum<entree_attribut>::Position,
                   Sortie = position_enum<sortie_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Id_Classe = Id_1,
                   Id_Eleve = Id_2};

    using eaid::entity_id;
    BASE_ENTITY(classe_eleve)
    ALIAS_CLE(classe,1)
    ALIAS_CLE(eleve,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    classe_eleve(idt id_cl, idt id_el, idt id = 0)
        : eaid(id) {
        set_id_classe(id_cl);
        set_id_eleve(id_el);
    }

    //! Constructeur à partir des valeurs attributs.
    classe_eleve(idt id_cl, idt id_el, const QDate & entr, const QDate & st = QDate(), idt id = 0)
        : classe_eleve(id_cl, id_el, id) {
          set_entree(entr);
          set_sortie(st);
    }

    //! Validité de l'entitée.
    bool is_valid() const override {
        return eaid::is_valid() && entree() <= sortie();
    }
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité controle.
 */
class controle : public mps::entities::entity_ids<info_entity_note::Controle,mps::attribut::id_1_attribut,
                                                                   mps::attribut::cibles_attribut,
                                                                   mps::attribut::code_attribut,
                                                                   mps::attribut::date_time_valide_attribut,
                                                                   mps::attribut::decimale_attribut,
                                                                   minima_attribut,
                                                                   mps::attribut::nc_nom_attribut,
                                                                   mps::attribut::num_attribut,
                                                                   mps::attribut::total_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,controle>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Controle,mps::attribut::id_1_attribut,
                                                            mps::attribut::cibles_attribut,
                                                            mps::attribut::code_attribut,
                                                            mps::attribut::date_time_valide_attribut,
                                                            mps::attribut::decimale_attribut,
                                                            minima_attribut,
                                                            mps::attribut::nc_nom_attribut,
                                                            mps::attribut::num_attribut,
                                                            mps::attribut::total_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_attribut>::Position,
                   Id_Cible = position_enum<id_cible_attribut>::Position,
                   Cible = position_enum<cible_attribut>::Position,
                   Code = position_enum<code_attribut>::Position,
                   Date_Time = position_enum<date_time_valide_attribut>::Position,
                   Decimale = position_enum<decimale_attribut>::Position,
                   Minima = position_enum<minima_attribut>::Position,
                   Nc = position_enum<nc_attribut>::Position,
                   Nom = position_enum<nom_attribut>::Position,
                   Num = position_enum<num_attribut>::Position,
                   Total = position_enum<total_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Id_Type = Id_1,
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
                                     Non_Commun = 0x10000,
                                     Horaire_groupe = Non_Commun,
                                     Horaire_eleve = Non_Commun * 0x2,
                                     Sujet_Groupe = Non_Commun * 0x4,
                                     Sujet_eleve = Non_Commun * 0x8
    };

    using eaid::entity_id;
    BASE_ENTITY(controle)
    ALIAS_CLE(type,1)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    controle(const QString & nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    controle(idt id_tp, idt id_cible, int cible, flag code, const QDateTime & dt, int decimale, int minima,
             const QString & nc, const QString & nom, int num, int total, idt id = 0)
        : controle(nom, id) {
        set_id_type(id_tp);
        set_id_cible(id_cible);
        set_cible(cible);
        set_code(code);
        set_date_time(dt);
        set_decimale(decimale);
        set_minima(minima);
        set_nc(nc);
        set_num(num);
        set_total(total);
    }
};

/*! \ingroup groupe_manager_note
 * \brief Les attributs de l'entité controle_epreuve.
 */
class controle_epreuve_attribut : public mps::entities::attributs<mps::attribut::relation_trois_exact_one_not_null_attribut,
                                                       mps::attribut::id_4_attribut,
                                                       mps::attribut::id_5_attribut,
                                                       num_ce_attribut> {
public:
    CONSTR_DEFAUT(controle_epreuve_attribut)
    CONSTR_AFFECT_DEFAUT(controle_epreuve_attribut)

    //! Destructeur.
    ~controle_epreuve_attribut() override;

    //! Teste si l'entité est valide.
    bool is_valid() const override
        {return (num() == -1 &&((id_1_null_attribut::is_valid() && id_2() == 0 && id_3() == 0)
                                || (id_2_null_attribut::is_valid() && id_1() == 0 && id_3() == 0)))
                || (num() >= 0 && id_3_null_attribut::is_valid() && id_1() == 0 && id_2() == 0);}
};


/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité controle_epreuve.
 */
class controle_epreuve : public mps::entities::entity_ids<info_entity_note::Controle_Epreuve,controle_epreuve_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,controle_epreuve>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Controle_Epreuve,controle_epreuve_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_null_attribut>::Position,
                   Id_2 = position_enum<id_2_null_attribut>::Position,
                   Id_3 = position_enum<id_3_null_attribut>::Position,
                   Id_4 = position_enum<id_4_attribut>::Position,
                   Id_5 = position_enum<id_5_attribut>::Position,
                   Num = position_enum<num_ce_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Id_Classe = Id_1,
                   Id_Eleve = Id_2,
                   Id_Groupe = Id_3,
                   Id_Controle = Id_4,
                   Id_epreuve= Id_5
                  };

    using eaid::entity_id;
    BASE_ENTITY(controle_epreuve)
    ALIAS_CLE(classe,1)
    ALIAS_CLE(eleve,2)
    ALIAS_CLE(groupe,3)
    ALIAS_CLE(controle,4)
    ALIAS_CLE(epreuve,5)

    //! Constructeur à partir des valeurs attributs.
    controle_epreuve(idt id_classe, idt id_controle, idt id_eleve, idt id_epreuve, idt id_groupe, int num, idt id = 0)
        : eaid(id) {
        set_id_classe(id_classe);
        set_id_controle(id_controle);
        set_id_eleve(id_eleve);
        set_id_epreuve(id_epreuve);
        set_id_groupe(id_groupe);
        set_num(num);
    }
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité eleve.
 */
class eleve : public mps::entities::entity_ids<info_entity_note::Eleve,mps::attribut::date_valide_attribut,
                                                             fille_attribut,
                                                             mps::attribut::nom_attribut,
                                                             prenom_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,eleve>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Eleve,mps::attribut::date_valide_attribut,
                                                         fille_attribut,
                                                         mps::attribut::nom_attribut,
                                                         prenom_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Date = position_enum<date_valide_attribut>::Position,
                   Fille = position_enum<fille_attribut>::Position,
                   Nom = position_enum<nom_attribut>::Position,
                   Prenom = position_enum<prenom_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att
                  };

    using eaid::entity_id;
    BASE_ENTITY(eleve)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    eleve(const QDate & dt, const QString & nom, const QString & prenom,  idt id = 0)
        : eaid(id) {
        set_date(dt);
        set_nom(nom);
        set_prenom(prenom);
    }

    //! Constructeur à partir des valeurs attributs.
    eleve(const QDate & dt, bool fille, const QString & nom, const QString & prenom,  idt id = 0)
        : eleve(dt, nom, prenom, id)
        {set_fille(fille);}

    //! Compaison (<) entre deux éléves par ordre alphabétique sur nom, prénom puis date de naissance.
    bool operator < (const eleve & eleve) const;
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité enonce_point.
 */
class enonce_point : public mps::entities::entity_ids<info_entity_note::Enonce_Point,mps::attribut::relation_attribut,
                                                                         ens_point_attribut,
                                                                         mps::attribut::num_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,enonce_point>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Enonce_Point,mps::attribut::relation_attribut,
                                                               ens_point_attribut,
                                                               mps::attribut::num_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_attribut>::Position,
                   Id_2 = position_enum<id_2_attribut>::Position,
                   Ens_Point = position_enum<ens_point_attribut>::Position,
                   Num = position_enum<num_attribut>::Position,
                   Nbr_Att =eaid::Nbr_Att,
                   Id_Enonce = Id_1,
                   Id_Point = Id_2};

    using eaid::entity_id;
    BASE_ENTITY(enonce_point)
    ALIAS_CLE(enonce,1)
    ALIAS_CLE(point,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    enonce_point(idt id_en, idt ens_pt, int num , idt id = 0)
        : eaid(id) {
        set_id_enonce(id_en);
        set_ens_point(ens_pt);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    enonce_point(idt id_en, idt id_pt, idt ens_pt, int num, idt id = 0)
        : enonce_point(id_en, ens_pt, num, id)
        {set_id_point(id_pt);}
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité exercice.
 */
class exercice : public mps::entities::entity_ids<info_entity_note::Exercice,mps::attribut::id_1_null_attribut,
                                                                   mps::attribut::type_attribut,
                                                                   mps::attribut::version_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,exercice>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Exercice,mps::attribut::id_1_null_attribut,
                                                            mps::attribut::type_attribut,
                                                            mps::attribut::version_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_null_attribut>::Position,
                   Type = position_enum<type_attribut>::Position,
                   Version = position_enum<version_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Id_Original = Id_1};

    using eaid::entity_id;
    BASE_ENTITY(exercice)
    ALIAS_CLE(original,1)

    //! Constructeur à partir des valeurs attributs.
    exercice(idt id_original, idt type, int version,  idt id = 0)
        : eaid(id) {
        set_id_original(id_original);
        set_type(type);
        set_version(version);
    }
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité epreuve.
 */
class epreuve : public mps::entities::entity_ids<info_entity_note::Epreuve,mps::attribut::relation_attribut,
                                                                 ens_point_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,epreuve>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Epreuve,mps::attribut::relation_attribut,
    ens_point_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_attribut>::Position,
                   Id_2 = position_enum<id_2_attribut>::Position,
                   Ens_Point = position_enum<ens_point_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Id_Bareme = Id_1,
                   Id_Enonce = Id_2};

    using eaid::entity_id;
    BASE_ENTITY(epreuve)
    ALIAS_CLE(bareme,1)
    ALIAS_CLE(enonce,2)

    //! Constructeur à partir des valeurs attributs.
    epreuve(idt id_bar, idt id_en, idt ens_pt, idt id = 0)
        : eaid(id) {
        set_id_bareme(id_bar);
        set_id_enonce(id_en);
        set_ens_point(ens_pt);
    }
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité Groupe.
 */
class groupe : public mps::entities::entity_ids<info_entity_note::Groupe,mps::attribut::relation_exact_one_not_null_attribut,
                                                            mps::attribut::style_num_attribut,
                                                            mps::attribut::code_attribut,
                                                            mps::attribut::nc_nom_type_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,groupe>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Groupe,mps::attribut::relation_exact_one_not_null_attribut,
                                                          mps::attribut::style_num_attribut,
                                                          mps::attribut::code_attribut,
                                                          mps::attribut::nc_nom_type_attribut>;
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_null_attribut>::Position,
                   Id_2 = position_enum<id_2_null_attribut>::Position,
                   Code = position_enum<code_attribut>::Position,
                   Nc = position_enum<nc_attribut>::Position,
                   Nom = position_enum<nom_attribut>::Position,
                   Style_Num = position_enum<style_num_attribut>::Position,
                   Type = position_enum<type_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Id_An = Id_1,
                   Id_Classe = Id_2,
                  };

    //! Code du groupe.
    enum code_flag : flag::flag_type {Aucun = 0x0,
                                     Exclusif = 0x1,
                                     Total = 0x2
    };

    using eaid::entity_id;
    BASE_ENTITY(groupe)
    ALIAS_CLE(an,1)
    ALIAS_CLE(classe,2)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    groupe(const QString & nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    groupe(idt id_an, idt id_classe, flag_type code, const QString & nc, const QString & nom,
           enumt styleNum, idt type,  idt id = 0)
        : groupe(nom,id) {
        set_id_an(id_an);
        set_id_classe(id_classe);
        set_style_num(styleNum);
        set_code(code);
        set_nc(nc);
        set_nom(nom);
        set_type(type);
    }
};

/*! \ingroup groupe_manager_note
 * \brief Représentation de l'entité type_controle.
 */
class type_controle : public mps::entities::entity_ids<info_entity_note::Type_Controle,mps::attribut::code_attribut,
                                                                           mps::attribut::decimale_attribut,
                                                                           minima_attribut,
                                                                           mps::attribut::nc_nom_attribut,
                                                                           mps::attribut::parent_attribut,
                                                                           mps::attribut::total_attribut> {
protected:
    template<class T> using position_enum = mps::entities::position_enum<T,type_controle>;
public:
    using eaid = mps::entities::entity_ids<info_entity_note::Type_Controle,mps::attribut::code_attribut,
                                                                mps::attribut::decimale_attribut,
                                                                minima_attribut,
                                                                mps::attribut::nc_nom_attribut,
                                                                mps::attribut::parent_attribut,
                                                                mps::attribut::total_attribut>;
    //! Code du type de controle.
    enum code_flag : flag::flag_type {Aucun = 0x0,
                                     Note = controle::Note,
                                     Lettre = controle::Lettre,
                                     Depassement = controle::Depassement,
                                     Courbe = controle::Courbe,
                                     Classement = controle::Classement,
                                     Barre = controle::Barre,
                                     Competences = controle::Competences,
                                     Capacites = controle::Capacites,
                                     Appreciation = controle::Appreciation,
                                     Commentaire = controle::Commentaire,
                                     Non_Commun = controle::Non_Commun,
                                     Categorie = Non_Commun,
                                     Note_Modifiable = Non_Commun * 0x2,
                                     Type_Note_Modifiable = Non_Commun * 0x4,
                                     Total_Modifiable = Non_Commun * 0x8,
                                     Depassement_Modifiable = Non_Commun * 0x10,
                                     Decimale_Modifiable = Non_Commun * 0x20,
                                     Courbe_Modifiable = Non_Commun * 0x40,
                                     Classement_Modifiable = Non_Commun * 0x80,
                                     Barre_Modifiable = Non_Commun * 0x100,
                                     Minima_Modifiable = Non_Commun * 0x200,
                                     Competences_Modifiable = Non_Commun * 0x400,
                                     Capacites_Modifiable = Non_Commun * 0x800,
                                     Appreciation_Modifiable = Non_Commun * 0x1000,
                                     Commentaire_Modifiable = Non_Commun * 0x2000
    };
    //! positions des attributs.
    enum position {Id = position_enum<id_attribut>::Position,
                   Code = position_enum<code_attribut>::Position,
                   Decimale = position_enum<decimale_attribut>::Position,
                   Minima = position_enum<minima_attribut>::Position,
                   Nc = position_enum<nc_attribut>::Position,
                   Nom = position_enum<nom_attribut>::Position,
                   Parent = position_enum<parent_attribut>::Position,
                   Total = position_enum<total_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Ordre = Nom,
                   };

    using eaid::entity_id;
    BASE_ENTITY(type_controle)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    type_controle(const QString & nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    type_controle(flag code, int decimale, int minima, const QString & nc,
                 const QString & nom, idt parent, int total, idt id = 0)
        : type_controle(nom, id) {
        set_code(code);
        set_decimale(decimale);
        set_minima(minima);
        set_nc(nc);
        set_parent(parent);
        set_total(total);
    }
};
}
#endif // ENTITY_NOTE_H
