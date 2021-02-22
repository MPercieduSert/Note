#include "ManagersNote.h"

using namespace noteMPS;
using namespace manager;

managers_note::managers_note()
    : managers_predef (info_entityNote::NbrEntityId,b2d::cible_id::NbrCible,"NoteVersion")
{
    enable_restriction("RestModif");
    enable_type("Type","PermType",
               std::make_unique<gestion_restriction_cible_code<Type,Restriction>>(b2d::cible_id::Type,
                                                                                get<Restriction>()),
               std::make_unique<gestion_restriction_cible_code<type_permission,Restriction>>(b2d::cible_id::type_permission,
                                                                                          get<Restriction>()));
    enable_commentaire("Commentaire","CbCommentaire");
    enable_donnee("donnee","Arbdonnee","Cbdonnee","Carddonnee",
                 std::make_unique<gestion_restriction_cible_code<donnee,Restriction>>(b2d::cible_id::donnee,
                                                                                    get<Restriction>()),
                 std::make_unique<gestion_restriction_cible_code<donnee_cible,Restriction>>(b2d::cible_id::donnee_cible,
                                                                                         get<Restriction>()),
                 std::make_unique<gestion_restriction_cible_code<donnee_card,Restriction>>(b2d::cible_id::donnee_card,
                                                                                        get<Restriction>()));
    enable_evenement("Evenement","evenement_cible","evenement_style");
    enable_mot_cle("mot_cle","Arbmot_cle","Cbmot_cle","CbMotProg","Permmot_cle","PermMotProg",
                 std::make_unique<gestion_restriction_cible_code<mot_cle,Restriction>>(b2d::cible_id::mot_cle,
                                                                                    get<Restriction>()),
                 std::make_unique<gestion_restriction_cible_code<mot_cle_permission,Restriction>>(b2d::cible_id::mot_cle_permission,
                                                                                              get<Restriction>()),
                 std::make_unique<gestion_restriction_cible_code<mot_prog_permission,Restriction>>(b2d::cible_id::mot_prog_permission,
                                                                                               get<Restriction>()));
    enable_texte("Texte","CbTexte","Source","TexteSource");
    enable_utilisation("Utilisation","Usage","ArbUsage",
                      std::make_unique<gestion_restriction_cible_code<Usage,Restriction>>(b2d::cible_id::Usage,
                                                                                        get<Restriction>()));
    
    //Annee
    using UniqueAn = num_unique_sql<Annee>;
    info_bdd infoAn("Annee","Annee",Annee::Nbr_Att,{UniqueAn::Nbr_Unique});
    infoAn.set_attribut(Annee::Num,"num");
    infoAn.set_unique(Annee::Num,UniqueAn::Num_Unique);
    set_manager<Annee>(std::make_unique<manager_modif_controle<Annee>>(infoAn,
                std::make_unique<gestion_restriction_cible_code<Annee,Restriction>>(b2d::cible_id::Annee,
                                                                                              get<Restriction>()),
                std::make_unique<UniqueAn>()));
    set_cible<Annee>(b2d::cible_id::Annee);
    
    //Bareme
    auto infoArbBareme = infoBddArbre("ArbBareme");
    info_bdd infoBareme("Bareme","Bareme",Bareme::Nbr_Att);
    infoBareme.set_attribut(Bareme::Id_Cible,"idCb");
    infoBareme.set_attribut(Bareme::Cible,"cb");
    infoBareme.set_attribut(Bareme::Type,"tp");
    infoBareme.set_attribut(Bareme::Valeur,"vl",b2d::type_attribut_bdd::Double);
    infoBareme.set_foreign_key(Bareme::Id,infoArbBareme);
    setTypeForeignKey<Bareme>(infoBareme);
    set_manager<Bareme>(std::make_unique<ma_arbre<Bareme>>(infoBareme,infoArbBareme));
    set_cible<Bareme>(b2d::cible_id::Bareme);

    //TypeEtablissement
    using UniqueTpEtab = nom_unique_sql<TypeEtablissement>;
    info_bdd infoTpEtab("TypeEtablissement","TpEtab",TypeEtablissement::Nbr_Att,{UniqueTpEtab::Nbr_Unique});
    infoTpEtab.set_attribut(TypeEtablissement::Nc,"nc",b2d::type_attribut_bdd::Text);
    infoTpEtab.set_attribut(TypeEtablissement::Nom,"nm",b2d::type_attribut_bdd::Text);
    infoTpEtab.set_unique(TypeEtablissement::Nom,UniqueTpEtab::Nom_Unique);
    set_manager<TypeEtablissement>(std::make_unique<manager_modif_controle<TypeEtablissement>>(infoTpEtab,
               std::make_unique<gestion_restriction_cible_code<TypeEtablissement,Restriction>>(b2d::cible_id::TypeEtablissement,
                                                                                             get<Restriction>()),
               std::make_unique<UniqueTpEtab>()));
    set_cible<TypeEtablissement>(b2d::cible_id::TypeEtablissement);

    //Etablissement
    using UniqueEtab = nom_unique_sql<Etablissement>;
    info_bdd infoEtab("Etablissement","Etab",Etablissement::Nbr_Att,{UniqueEtab::Nbr_Unique});
    infoEtab.set_attribut(Etablissement::Nc,"nc",b2d::type_attribut_bdd::Text);
    infoEtab.set_attribut(Etablissement::Nom,"nm",b2d::type_attribut_bdd::Text);
    infoEtab.set_unique(Etablissement::Nom,UniqueEtab::Nom_Unique);
    set_manager<Etablissement>(std::make_unique<manager_modif_controle<Etablissement>>(infoEtab,
                std::make_unique<gestion_restriction_cible_code<Etablissement,Restriction>>(b2d::cible_id::Etablissement,
                                                                                              get<Restriction>()),
                std::make_unique<UniqueEtab>()));
    set_cible<Etablissement>(b2d::cible_id::Etablissement);

    //EtablissementType
    using UniqueEtabTp = relation_unique_sql<EtablissementType>;
    info_bdd infoEtabTp("EtablissementType","EtabTp",EtablissementType::Nbr_Att,{UniqueEtabTp::Nbr_Unique});
    infoEtabTp.set_attribut(EtablissementType::IdEtab,"idEtab");
    infoEtabTp.set_attribut(EtablissementType::IdTpEtab,"idTpEtab");
    infoEtabTp.set_unique(EtablissementType::IdEtab,UniqueEtabTp::Id1Unique);
    infoEtabTp.set_unique(EtablissementType::IdTpEtab,UniqueEtabTp::Id2Unique);
    infoEtabTp.set_foreign_key(EtablissementType::IdEtab,infoEtab);
    infoEtabTp.set_foreign_key(EtablissementType::IdTpEtab,infoTpEtab);
    set_manager<EtablissementType>(std::make_unique<manager_sql<EtablissementType>>(infoEtabTp, std::make_unique<UniqueEtabTp>()));
    set_cible<EtablissementType>(b2d::cible_id::EtablissementType);

    //Niveau
    using UniqueNiveau = nom_unique_sql<Niveau>;
    info_bdd infoNiveau("Niveau","Niveau",Niveau::Nbr_Att,{UniqueNiveau::Nbr_Unique});
    infoNiveau.set_attribut(Niveau::Nc,"nc",b2d::type_attribut_bdd::Text);
    infoNiveau.set_attribut(Niveau::Nom,"nm",b2d::type_attribut_bdd::Text);
    infoNiveau.set_attribut(Niveau::Type,"tp");
    infoNiveau.set_unique(Niveau::Nom,UniqueNiveau::Nom_Unique);
    setTypeForeignKey<Niveau>(infoNiveau);
    set_manager<Niveau>(std::make_unique<manager_modif_controle<Niveau>>(infoNiveau,
               std::make_unique<gestion_restriction_cible_code<Niveau,Restriction>>(b2d::cible_id::Niveau,
                                                                                  get<Restriction>()),
               std::make_unique<UniqueNiveau>()));
    set_cible<Niveau>(b2d::cible_id::Niveau);
    
    //Classe
    using UniqueClasse = relation_trois_num_unique_sql<Classe>;
    info_bdd infoClasse(Classe::Name(),"Classe",Classe::Nbr_Att,{UniqueClasse::Nbr_Unique});
    infoClasse.set_attribut(Classe::IdAn,"idAn");
    infoClasse.set_attribut(Classe::IdEtab,"idEt");
    infoClasse.set_attribut(Classe::IdNiveau,"idNiv");
    infoClasse.set_attribut(Classe::Nc,"nc",b2d::type_attribut_bdd::Text);
    infoClasse.set_attribut(Classe::Nom,"nm",b2d::type_attribut_bdd::Text);
    infoClasse.set_attribut(Classe::Num,"num");
    infoClasse.set_unique(Classe::IdAn,UniqueClasse::Id1Unique);
    infoClasse.set_unique(Classe::IdEtab,UniqueClasse::Id2Unique);
    infoClasse.set_unique(Classe::IdNiveau,UniqueClasse::id_3_Unique);
    infoClasse.set_unique(Classe::Num,UniqueClasse::Num_Unique);
    infoClasse.set_foreign_key(Classe::IdAn,infoAn);
    infoClasse.set_foreign_key(Classe::IdEtab,infoEtab);
    infoClasse.set_foreign_key(Classe::IdNiveau,infoNiveau);
    set_manager<Classe>(std::make_unique<manager_modif_controle<Classe>>(infoClasse,
                std::make_unique<gestion_restriction_cible_code<Classe,Restriction>>(b2d::cible_id::Classe,
                                                                                    get<Restriction>()),
                std::make_unique<UniqueClasse>()));
    set_cible<Classe>(b2d::cible_id::Classe);

    //Eleve
    info_bdd infoEleve(Eleve::Name(),"Eleve",Eleve::Nbr_Att,{UniqueEleve::Nbr_Unique});
    infoEleve.set_attribut(Eleve::Date,"dt",b2d::type_attribut_bdd::Date);
    infoEleve.set_attribut(Eleve::Fille,"fl",b2d::type_attribut_bdd::Bool);
    infoEleve.set_attribut(Eleve::Nom,"nm",b2d::type_attribut_bdd::Text);
    infoEleve.set_attribut(Eleve::Prenom,"pnm",b2d::type_attribut_bdd::Text);
    infoEleve.set_unique(Eleve::Nom,UniqueEleve::Nom_Unique);
    infoEleve.set_unique(Eleve::Prenom,UniqueEleve::PrenomUnique);
    infoEleve.set_unique(Eleve::Date,UniqueEleve::DateUnique);
    set_manager<Eleve>(std::make_unique<manager_modif_controle<Eleve>>(infoEleve,
                std::make_unique<gestion_restriction_cible_code<Eleve,Restriction>>(b2d::cible_id::Eleve,
                                                                                        get<Restriction>()),
                std::make_unique<UniqueEleve>()));
    set_cible<Eleve>(b2d::cible_id::Eleve);
    
    //ClasseEleve
    using UniqueClEl = relation_unique_sql<ClasseEleve>;
    info_bdd infoClEl(ClasseEleve::Name(),"ClasseEleve",ClasseEleve::Nbr_Att,{UniqueClEl::Nbr_Unique});
    infoClEl.set_attribut(ClasseEleve::IdClasse,"IdCl");
    infoClEl.set_attribut(ClasseEleve::IdEleve,"IdEl");
    infoClEl.set_attribut(ClasseEleve::Entree,"ent",b2d::type_attribut_bdd::Date);
    infoClEl.set_attribut(ClasseEleve::Sortie,"sort",b2d::type_attribut_bdd::Date);
    infoClEl.set_unique(ClasseEleve::IdClasse,UniqueClEl::Id1Unique);
    infoClEl.set_unique(ClasseEleve::IdEleve,UniqueClEl::Id2Unique);
    infoClEl.set_foreign_key(ClasseEleve::IdClasse,infoClasse);
    infoClEl.set_foreign_key(ClasseEleve::IdEleve,infoEleve);
    set_manager<ClasseEleve>(std::make_unique<manager_sql<ClasseEleve>>(infoClEl,std::make_unique<UniqueClEl>()));
    set_cible<ClasseEleve>(b2d::cible_id::ClasseEleve);

    //Groupe
    using UniqueGroupe = nom_unique_sql<Groupe>;
    info_bdd infoGroupe(Groupe::Name(),"Groupe",Groupe::Nbr_Att,{UniqueGroupe::Nbr_Unique});
    infoGroupe.set_attribut(Groupe::IdAn,"idAn",b2d::type_attribut_bdd::Integer,false);
    infoGroupe.set_attribut(Groupe::IdClasse,"idCl",b2d::type_attribut_bdd::Integer,false);
    infoGroupe.set_attribut(Groupe::StyleNum,"al");
    infoGroupe.set_attribut(Groupe::Code,"cd");
    infoGroupe.set_attribut(Groupe::Nc,"nc",b2d::type_attribut_bdd::Text);
    infoGroupe.set_attribut(Groupe::Nom,"nm",b2d::type_attribut_bdd::Text);
    infoGroupe.set_attribut(Groupe::Type,"tp");
    infoGroupe.set_unique(Groupe::Nom,UniqueGroupe::Nom_Unique);
    infoGroupe.set_foreign_key(Groupe::IdAn,infoAn);
    infoGroupe.set_foreign_key(Groupe::IdClasse,infoClasse);
    setTypeForeignKey<Groupe>(infoGroupe);
    set_manager<Groupe>(std::make_unique<manager_modif_controle<Groupe>>(infoGroupe,
              std::make_unique<gestion_restriction_cible_code<Groupe,Restriction>>(b2d::cible_id::Groupe,
                                                                                      get<Restriction>()),
              std::make_unique<UniqueGroupe>()));
    set_cible<Groupe>(b2d::cible_id::Groupe);

    //TypeControle
    using UniqueTpCtr = nom_unique_sql<TypeControle>;
    info_bdd infoTpCtr(TypeControle::Name(),"TpControle",TypeControle::Nbr_Att,{UniqueTpCtr::Nbr_Unique});
    infoTpCtr.set_attribut(TypeControle::Code,"cd");
    infoTpCtr.set_attribut(TypeControle::Decimale,"dcm");
    infoTpCtr.set_attribut(TypeControle::Minima,"min");
    infoTpCtr.set_attribut(TypeControle::Nc,"nc",b2d::type_attribut_bdd::Text);
    infoTpCtr.set_attribut(TypeControle::Nom,"nm",b2d::type_attribut_bdd::Text);
    infoTpCtr.set_attribut(TypeControle::Parent,"pt",b2d::type_attribut_bdd::Integer,false);
    infoTpCtr.set_attribut(TypeControle::Total,"tt");
    infoTpCtr.set_unique(TypeControle::Nom,UniqueTpCtr::Nom_Unique);
    infoTpCtr.set_foreign_key(TypeControle::Parent,infoTpCtr);
    set_manager<TypeControle>(std::make_unique<ma_arbreSimpleModifControle<TypeControle>>(infoTpCtr,
                    std::make_unique<gestion_restriction_cible_code<TypeControle,Restriction>>(b2d::cible_id::TypeControle,
                                                                                             get<Restriction>()),
                    std::make_unique<UniqueTpCtr>()));
    set_cible<TypeControle>(b2d::cible_id::TypeControle);

    //Controle
    using UniqueCtr = nom_cible_unique_sql<Controle>;
    info_bdd infoCtr(Controle::Name(),"Controle",Controle::Nbr_Att,{UniqueCtr::Nbr_Unique_1,UniqueCtr::Nbr_Unique_2});
    infoCtr.set_attribut(Controle::id_type,"idTp");
    infoCtr.set_attribut(Controle::Id_Cible,"idCb");
    infoCtr.set_attribut(Controle::Cible,"cb");
    infoCtr.set_attribut(Controle::Code,"cd");
    infoCtr.set_attribut(Controle::DateTime,"dt",b2d::type_attribut_bdd::DateTime);
    infoCtr.set_attribut(Controle::Decimale,"dcm");
    infoCtr.set_attribut(Controle::Minima,"min");
    infoCtr.set_attribut(Controle::Nc,"nc",b2d::type_attribut_bdd::Text);
    infoCtr.set_attribut(Controle::Nom,"nm",b2d::type_attribut_bdd::Text);
    infoCtr.set_attribut(Controle::Num,"num");
    infoCtr.set_attribut(Controle::Total,"tt");
    infoCtr.set_unique(Controle::Nom,UniqueCtr::Nom_Unique,UniqueCtr::Nom_Unique_Set);
    infoCtr.set_unique(Controle::id_type,UniqueCtr::Id1Unique,UniqueCtr::Id1Unique_Set);
    infoCtr.set_unique(Controle::Id_Cible,UniqueCtr::Id_Cible_Unique,UniqueCtr::Id_Cible_Unique_Set);
    infoCtr.set_unique(Controle::Cible,UniqueCtr::Cible_Unique,UniqueCtr::Cible_Unique_Set);
    infoCtr.set_unique(Controle::Num,UniqueCtr::Num_Unique,UniqueCtr::Num_Unique_Set);
    infoCtr.set_foreign_key(Controle::id_type,infoTpCtr);
    set_manager<Controle>(std::make_unique<manager_sql<Controle>>(infoCtr,std::make_unique<UniqueCtr>()));
    set_cible<Controle>(b2d::cible_id::Controle);

    //Enonce
    auto infoArbEnonce = infoBddArbre("ArbEnonce");
    info_bdd infoEnonce("Enonce","Enonce",Enonce::Nbr_Att);
    infoEnonce.set_attribut(Enonce::Type,"tp");
    infoEnonce.set_attribut(Enonce::Version,"ver");
    infoEnonce.set_foreign_key(Enonce::Id,infoArbEnonce);
    setTypeForeignKey<Enonce>(infoEnonce);
    set_manager<Enonce>(std::make_unique<ma_arbre<Enonce>>(infoEnonce,infoArbEnonce));
    set_cible<Enonce>(b2d::cible_id::Enonce);

    //Epreuve
    info_bdd infoEpreuve("Epreuve","Epreuve",Epreuve::Nbr_Att,{UniqueEpreuve::Nbr_Unique});
    infoEpreuve.set_attribut(Epreuve::IdBareme,"idBr");
    infoEpreuve.set_attribut(Epreuve::IdEnonce,"idEn");
    infoEpreuve.set_attribut(Epreuve::EnsPoint,"ensPt");
    infoEpreuve.set_unique(Epreuve::IdBareme,UniqueEpreuve::Id1Unique);
    infoEpreuve.set_unique(Epreuve::IdEnonce,UniqueEpreuve::Id2Unique);
    infoEpreuve.set_unique(Epreuve::EnsPoint,UniqueEpreuve::EnsPointUnique);
    infoEpreuve.set_foreign_key(Epreuve::IdBareme,infoBareme);
    infoEpreuve.set_foreign_key(Epreuve::IdEnonce,infoEnonce);
    set_manager<Epreuve>(std::make_unique<manager_sql<Epreuve>>(infoEpreuve, std::make_unique<UniqueEpreuve>()));
    set_cible<Epreuve>(b2d::cible_id::Epreuve);

    //ControleEpreuve
    info_bdd infoCtrEp(ControleEpreuve::Name(),"ControleEpreuve",ControleEpreuve::Nbr_Att,
        {UniqueControleEpreuve::Nbr_Unique_1,UniqueControleEpreuve::Nbr_Unique_2,UniqueControleEpreuve::Nbr_Unique_3});
    infoCtrEp.set_attribut(ControleEpreuve::IdClasse,"idCl",b2d::type_attribut_bdd::Integer,false);
    infoCtrEp.set_attribut(ControleEpreuve::IdControle,"idCtr");
    infoCtrEp.set_attribut(ControleEpreuve::IdEleve,"idEl",b2d::type_attribut_bdd::Integer,false);
    infoCtrEp.set_attribut(ControleEpreuve::IdEpreuve,"idEp");
    infoCtrEp.set_attribut(ControleEpreuve::IdGroupe,"idGr",b2d::type_attribut_bdd::Integer,false);
    infoCtrEp.set_attribut(ControleEpreuve::Num,"num");
    infoCtrEp.set_unique(ControleEpreuve::IdClasse,UniqueControleEpreuve::Id1Unique,UniqueControleEpreuve::Id1Unique_Set);
    infoCtrEp.set_unique(ControleEpreuve::IdControle,UniqueControleEpreuve::Id4Unique_1,UniqueControleEpreuve::Id4Unique_1Set);
    infoCtrEp.set_unique(ControleEpreuve::IdGroupe,UniqueControleEpreuve::Id2Unique,UniqueControleEpreuve::Id2Unique_Set);
    infoCtrEp.set_unique(ControleEpreuve::IdControle,UniqueControleEpreuve::Id4Unique_2,UniqueControleEpreuve::Id4Unique_2Set);
    infoCtrEp.set_unique(ControleEpreuve::Num,UniqueControleEpreuve::Num_Unique,UniqueControleEpreuve::Num_Unique_Set);
    infoCtrEp.set_unique(ControleEpreuve::IdEleve,UniqueControleEpreuve::id_3_Unique,UniqueControleEpreuve::Id_3_Unique_Set);
    infoCtrEp.set_unique(ControleEpreuve::
                        IdControle,UniqueControleEpreuve::Id4Unique_3,UniqueControleEpreuve::Id4Unique_3Set);
    infoCtrEp.set_foreign_key(ControleEpreuve::IdClasse,infoClasse);
    infoCtrEp.set_foreign_key(ControleEpreuve::IdControle,infoCtr);
    infoCtrEp.set_foreign_key(ControleEpreuve::IdEleve,infoEleve);
    infoCtrEp.set_foreign_key(ControleEpreuve::IdEpreuve,infoEpreuve);
    infoCtrEp.set_foreign_key(ControleEpreuve::IdGroupe,infoGroupe);
    set_manager<ControleEpreuve>(std::make_unique<manager_sql<ControleEpreuve>>(infoCtrEp, std::make_unique<UniqueControleEpreuve>()));
    set_cible<ControleEpreuve>(b2d::cible_id::ControleEpreuve);

    //EleveGroupe
    using UniqueElGr = relation_num_unique_sql<EleveGroupe>;
    info_bdd infoElGr(EleveGroupe::Name(),"EleveGroupe",EleveGroupe::Nbr_Att,{UniqueElGr::Nbr_Unique});
    infoElGr.set_attribut(EleveGroupe::IdEleve,"idEl");
    infoElGr.set_attribut(EleveGroupe::IdGroupe,"idGr");
    infoElGr.set_attribut(EleveGroupe::Num,"num");
    infoElGr.set_unique(EleveGroupe::IdEleve,UniqueElGr::Id1Unique);
    infoElGr.set_unique(EleveGroupe::IdGroupe,UniqueElGr::Id2Unique);
    infoElGr.set_unique(EleveGroupe::Num,UniqueElGr::Num_Unique);
    infoElGr.set_foreign_key(EleveGroupe::IdEleve,infoEleve);
    infoElGr.set_foreign_key(EleveGroupe::IdGroupe,infoGroupe);
    set_manager<EleveGroupe>(std::make_unique<manager_sql<EleveGroupe>>(infoElGr, std::make_unique<UniqueElGr>()));
    set_cible<EleveGroupe>(b2d::cible_id::EleveGroupe);

    //Point
    info_bdd infoPoint("Point","Point",Point::Nbr_Att);
    infoPoint.set_attribut(Point::Type,"tp");
    infoPoint.set_attribut(Point::Valeur,"vl");
    infoPoint.set_attribut(Point::Version,"ver");
    setTypeForeignKey<Point>(infoPoint);
    set_manager<Point>(std::make_unique<manager_sql<Point>>(infoPoint));
    set_cible<Point>(b2d::cible_id::Point);

    //EnoncePoint
    info_bdd infoEnPt(EnoncePoint::Name(),"EnoncePoint",EnoncePoint::Nbr_Att,{UniqueEnoncePoint::Nbr_Unique});
    infoEnPt.set_attribut(EnoncePoint::IdEnonce,"idEn");
    infoEnPt.set_attribut(EnoncePoint::IdPoint,"idPt");
    infoEnPt.set_attribut(EnoncePoint::EnsPoint,"ensPt");
    infoEnPt.set_attribut(EnoncePoint::Num,"num");
    infoEnPt.set_unique(EnoncePoint::IdEnonce,UniqueEnoncePoint::Id1Unique);
    infoEnPt.set_unique(EnoncePoint::EnsPoint,UniqueEnoncePoint::EnsPointUnique);
    infoEnPt.set_unique(EnoncePoint::Num,UniqueEnoncePoint::Num_Unique);
    infoEnPt.set_foreign_key(EnoncePoint::IdEnonce,infoEnonce);
    infoEnPt.set_foreign_key(EnoncePoint::IdPoint,infoPoint);
    set_manager<EnoncePoint>(std::make_unique<manager_sql<EnoncePoint>>(infoEnPt,std::make_unique<UniqueEnoncePoint>()));
    set_cible<EnoncePoint>(b2d::cible_id::EnoncePoint);

    //EtablissementNiveau
    using UniqueEtabNiv = relation_unique_sql<EtablissementNiveau>;
    info_bdd infoEtabNiv(EtablissementNiveau::Name(),"EtabNiveau",EtablissementNiveau::Nbr_Att,{UniqueEtabNiv::Nbr_Unique});
    infoEtabNiv.set_attribut(EtablissementNiveau::IdEtab,"idEt");
    infoEtabNiv.set_attribut(EtablissementNiveau::IdNiveau,"idNiv");
    infoEtabNiv.set_foreign_key(EtablissementNiveau::IdEtab,infoEtab);
    infoEtabNiv.set_foreign_key(EtablissementNiveau::IdNiveau,infoNiveau);
    infoEtabNiv.set_unique(EtablissementNiveau::IdEtab,UniqueEtabNiv::Id1Unique);
    infoEtabNiv.set_unique(EtablissementNiveau::IdNiveau,UniqueEtabNiv::Id2Unique);
    set_manager<EtablissementNiveau>(std::make_unique<manager_sql<EtablissementNiveau>>(infoEtabNiv, std::make_unique<UniqueEtabNiv>()));
    set_cible<EtablissementNiveau>(b2d::cible_id::EtablissementNiveau);

    // Exercice
    auto infoArbExo = infoBddArbre("ArbExercice");
    info_bdd infoExo(Exercice::Name(),"Exercice",Exercice::Nbr_Att);
    infoExo.set_attribut(Exercice::IdOriginal,"idOrig",b2d::type_attribut_bdd::Integer,false);
    infoExo.set_attribut(Exercice::Type,"tp");
    infoExo.set_attribut(Exercice::Version,"ver");
    infoExo.set_foreign_key(Exercice::Id,infoArbExo);
    infoExo.set_foreign_key(Exercice::IdOriginal,infoExo);
    setTypeForeignKey<Exercice>(infoExo);
    set_manager<Exercice>(std::make_unique<ma_arbre<Exercice>>(infoExo,infoArbExo));
    set_cible<Exercice>(b2d::cible_id::Exercice);

    //FiliationNiveau
    using UniqueFilNiv = relation_unique_sql<FiliationNiveau>;
    info_bdd infoFilNiv(FiliationNiveau::Name(),"FiliationNiveau",FiliationNiveau::Nbr_Att,{UniqueFilNiv::Nbr_Unique});
    infoFilNiv.set_attribut(FiliationNiveau::IdPrecedent,"idP");
    infoFilNiv.set_attribut(FiliationNiveau::IdSuivant,"idS");
    infoFilNiv.set_foreign_key(FiliationNiveau::IdPrecedent,infoNiveau);
    infoFilNiv.set_foreign_key(FiliationNiveau::IdSuivant,infoNiveau);
    infoFilNiv.set_unique(FiliationNiveau::IdPrecedent,UniqueFilNiv::Id1Unique);
    infoFilNiv.set_unique(FiliationNiveau::IdSuivant,UniqueFilNiv::Id2Unique);
    set_manager<FiliationNiveau>(std::make_unique<manager_modif_controle<FiliationNiveau>>(infoFilNiv,
                   std::make_unique<gestion_restriction_cible_code<FiliationNiveau,Restriction>>(b2d::cible_id::FiliationNiveau,
                                                                                               get<Restriction>()),
                   std::make_unique<UniqueFilNiv>()));
    set_cible<FiliationNiveau>(b2d::cible_id::FiliationNiveau);

    //NiveauTypeEtablissement
    using UniqueNivTpEtab = relation_unique_sql<NiveauTypeEtablissement>;
    info_bdd infoNivTpEtab(NiveauTypeEtablissement::Name(),"NiveauTpEtab",NiveauTypeEtablissement::Nbr_Att,{UniqueNivTpEtab::Nbr_Unique});
    infoNivTpEtab.set_attribut(NiveauTypeEtablissement::IdNiveau,"idNiv");
    infoNivTpEtab.set_attribut(NiveauTypeEtablissement::IdTpEtab,"idTE");
    infoNivTpEtab.set_foreign_key(NiveauTypeEtablissement::IdNiveau,infoNiveau);
    infoNivTpEtab.set_foreign_key(NiveauTypeEtablissement::IdTpEtab,infoTpEtab);
    infoNivTpEtab.set_unique(NiveauTypeEtablissement::IdNiveau,UniqueNivTpEtab::Id1Unique);
    infoNivTpEtab.set_unique(NiveauTypeEtablissement::IdTpEtab,UniqueNivTpEtab::Id2Unique);
    set_manager<NiveauTypeEtablissement>(std::make_unique<manager_modif_controle<NiveauTypeEtablissement>>(infoNivTpEtab,
               std::make_unique<gestion_restriction_cible_code<NiveauTypeEtablissement,Restriction>>
                                                                 (b2d::cible_id::NiveauTypeEtablissement,get<Restriction>()),
               std::make_unique<UniqueNivTpEtab>()));
    set_cible<NiveauTypeEtablissement>(b2d::cible_id::NiveauTypeEtablissement);

    //Note
    using UniqueNote = relation_unique_sql<Note>;
    info_bdd infoNote(Note::Name(),"Note",Note::Nbr_Att,{UniqueNote::Nbr_Unique});
    infoNote.set_attribut(Note::IdControle,"idCtr");
    infoNote.set_attribut(Note::IdEleve,"idEl");
    infoNote.set_attribut(Note::DateTime,"dt",b2d::type_attribut_bdd::DateTime);
    infoNote.set_attribut(Note::Saisie,"ss");
    infoNote.set_attribut(Note::Valeur,"vl");
    infoNote.set_unique(Note::IdControle,UniqueNote::Id1Unique);
    infoNote.set_unique(Note::IdEleve,UniqueNote::Id2Unique);
    infoNote.set_foreign_key(Note::IdControle,infoCtr);
    infoNote.set_foreign_key(Note::IdEleve,infoEleve);
    set_manager<Note>(std::make_unique<manager_sql<Note>>(infoNote,std::make_unique<UniqueNote>()));
    set_cible<Note>(b2d::cible_id::Note);

    //_valide
    using Unique_valide = relation_unique_sql<_valide>;
    info_bdd info_valide(_valide::Name(),"_valide",_valide::Nbr_Att,{Unique_valide::Nbr_Unique});
    info_valide.set_attribut(_valide::IdNote,"idNt");
    info_valide.set_attribut(_valide::IdPoint,"idPt");
    info_valide.set_attribut(_valide::DateTime,"dt",b2d::type_attribut_bdd::DateTime);
    info_valide.set_attribut(_valide::Valeur,"vl");
    info_valide.set_unique(_valide::IdNote,Unique_valide::Id1Unique);
    info_valide.set_unique(_valide::IdPoint,Unique_valide::Id2Unique);
    info_valide.set_foreign_key(_valide::IdNote,infoNote);
    info_valide.set_foreign_key(_valide::IdPoint,infoPoint);
    set_manager<_valide>(std::make_unique<manager_sql<_valide>>(info_valide, std::make_unique<Unique_valide>()));
    set_cible<_valide>(b2d::cible_id::_valide);
}
