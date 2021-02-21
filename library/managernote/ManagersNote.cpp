#include "ManagersNote.h"

using namespace noteMPS;
using namespace managerMPS;

ManagersNote::ManagersNote()
    : ManagersPredef (info_entityNote::NbrEntityId,bmps::cibleId::NbrCible,"NoteVersion")
{
    enableRestriction("RestModif");
    enableType("Type","PermType",
               std::make_unique<GestionRestrictioncible_code<Type,Restriction>>(bmps::cibleId::Type,
                                                                                get<Restriction>()),
               std::make_unique<GestionRestrictioncible_code<type_permission,Restriction>>(bmps::cibleId::type_permission,
                                                                                          get<Restriction>()));
    enableCommentaire("Commentaire","CbCommentaire");
    enabledonnee("donnee","Arbdonnee","Cbdonnee","Carddonnee",
                 std::make_unique<GestionRestrictioncible_code<donnee,Restriction>>(bmps::cibleId::donnee,
                                                                                    get<Restriction>()),
                 std::make_unique<GestionRestrictioncible_code<donnee_cible,Restriction>>(bmps::cibleId::donnee_cible,
                                                                                         get<Restriction>()),
                 std::make_unique<GestionRestrictioncible_code<donnee_card,Restriction>>(bmps::cibleId::donnee_card,
                                                                                        get<Restriction>()));
    enableEvenement("Evenement","evenement_cible","evenement_style");
    enablemot_cle("mot_cle","Arbmot_cle","Cbmot_cle","CbMotProg","Permmot_cle","PermMotProg",
                 std::make_unique<GestionRestrictioncible_code<mot_cle,Restriction>>(bmps::cibleId::mot_cle,
                                                                                    get<Restriction>()),
                 std::make_unique<GestionRestrictioncible_code<mot_cle_permission,Restriction>>(bmps::cibleId::mot_cle_permission,
                                                                                              get<Restriction>()),
                 std::make_unique<GestionRestrictioncible_code<mot_prog_permission,Restriction>>(bmps::cibleId::mot_prog_permission,
                                                                                               get<Restriction>()));
    enableTexte("Texte","CbTexte","Source","TexteSource");
    enableUtilisation("Utilisation","Usage","ArbUsage",
                      std::make_unique<GestionRestrictioncible_code<Usage,Restriction>>(bmps::cibleId::Usage,
                                                                                        get<Restriction>()));
    
    //Annee
    using UniqueAn = NumUniqueSql<Annee>;
    InfoBdd infoAn("Annee","Annee",Annee::Nbr_Att,{UniqueAn::NbrUnique});
    infoAn.set_attribut(Annee::Num,"num");
    infoAn.setUnique(Annee::Num,UniqueAn::NumUnique);
    setManager<Annee>(std::make_unique<ManagerModifControle<Annee>>(infoAn,
                std::make_unique<GestionRestrictioncible_code<Annee,Restriction>>(bmps::cibleId::Annee,
                                                                                              get<Restriction>()),
                std::make_unique<UniqueAn>()));
    set_cible<Annee>(bmps::cibleId::Annee);
    
    //Bareme
    auto infoArbBareme = infoBddArbre("ArbBareme");
    InfoBdd infoBareme("Bareme","Bareme",Bareme::Nbr_Att);
    infoBareme.set_attribut(Bareme::Id_Cible,"idCb");
    infoBareme.set_attribut(Bareme::Cible,"cb");
    infoBareme.set_attribut(Bareme::Type,"tp");
    infoBareme.set_attribut(Bareme::Valeur,"vl",bmps::typeAttributBdd::Double);
    infoBareme.setForeignKey(Bareme::Id,infoArbBareme);
    setTypeForeignKey<Bareme>(infoBareme);
    setManager<Bareme>(std::make_unique<ManagerArbre<Bareme>>(infoBareme,infoArbBareme));
    set_cible<Bareme>(bmps::cibleId::Bareme);

    //TypeEtablissement
    using UniqueTpEtab = NomUniqueSql<TypeEtablissement>;
    InfoBdd infoTpEtab("TypeEtablissement","TpEtab",TypeEtablissement::Nbr_Att,{UniqueTpEtab::NbrUnique});
    infoTpEtab.set_attribut(TypeEtablissement::Nc,"nc",bmps::typeAttributBdd::Text);
    infoTpEtab.set_attribut(TypeEtablissement::Nom,"nm",bmps::typeAttributBdd::Text);
    infoTpEtab.setUnique(TypeEtablissement::Nom,UniqueTpEtab::NomUnique);
    setManager<TypeEtablissement>(std::make_unique<ManagerModifControle<TypeEtablissement>>(infoTpEtab,
               std::make_unique<GestionRestrictioncible_code<TypeEtablissement,Restriction>>(bmps::cibleId::TypeEtablissement,
                                                                                             get<Restriction>()),
               std::make_unique<UniqueTpEtab>()));
    set_cible<TypeEtablissement>(bmps::cibleId::TypeEtablissement);

    //Etablissement
    using UniqueEtab = NomUniqueSql<Etablissement>;
    InfoBdd infoEtab("Etablissement","Etab",Etablissement::Nbr_Att,{UniqueEtab::NbrUnique});
    infoEtab.set_attribut(Etablissement::Nc,"nc",bmps::typeAttributBdd::Text);
    infoEtab.set_attribut(Etablissement::Nom,"nm",bmps::typeAttributBdd::Text);
    infoEtab.setUnique(Etablissement::Nom,UniqueEtab::NomUnique);
    setManager<Etablissement>(std::make_unique<ManagerModifControle<Etablissement>>(infoEtab,
                std::make_unique<GestionRestrictioncible_code<Etablissement,Restriction>>(bmps::cibleId::Etablissement,
                                                                                              get<Restriction>()),
                std::make_unique<UniqueEtab>()));
    set_cible<Etablissement>(bmps::cibleId::Etablissement);

    //EtablissementType
    using UniqueEtabTp = RelationUniqueSql<EtablissementType>;
    InfoBdd infoEtabTp("EtablissementType","EtabTp",EtablissementType::Nbr_Att,{UniqueEtabTp::NbrUnique});
    infoEtabTp.set_attribut(EtablissementType::IdEtab,"idEtab");
    infoEtabTp.set_attribut(EtablissementType::IdTpEtab,"idTpEtab");
    infoEtabTp.setUnique(EtablissementType::IdEtab,UniqueEtabTp::Id1Unique);
    infoEtabTp.setUnique(EtablissementType::IdTpEtab,UniqueEtabTp::Id2Unique);
    infoEtabTp.setForeignKey(EtablissementType::IdEtab,infoEtab);
    infoEtabTp.setForeignKey(EtablissementType::IdTpEtab,infoTpEtab);
    setManager<EtablissementType>(std::make_unique<ManagerSql<EtablissementType>>(infoEtabTp, std::make_unique<UniqueEtabTp>()));
    set_cible<EtablissementType>(bmps::cibleId::EtablissementType);

    //Niveau
    using UniqueNiveau = NomUniqueSql<Niveau>;
    InfoBdd infoNiveau("Niveau","Niveau",Niveau::Nbr_Att,{UniqueNiveau::NbrUnique});
    infoNiveau.set_attribut(Niveau::Nc,"nc",bmps::typeAttributBdd::Text);
    infoNiveau.set_attribut(Niveau::Nom,"nm",bmps::typeAttributBdd::Text);
    infoNiveau.set_attribut(Niveau::Type,"tp");
    infoNiveau.setUnique(Niveau::Nom,UniqueNiveau::NomUnique);
    setTypeForeignKey<Niveau>(infoNiveau);
    setManager<Niveau>(std::make_unique<ManagerModifControle<Niveau>>(infoNiveau,
               std::make_unique<GestionRestrictioncible_code<Niveau,Restriction>>(bmps::cibleId::Niveau,
                                                                                  get<Restriction>()),
               std::make_unique<UniqueNiveau>()));
    set_cible<Niveau>(bmps::cibleId::Niveau);
    
    //Classe
    using UniqueClasse = RelationTroisNumUniqueSql<Classe>;
    InfoBdd infoClasse(Classe::Name(),"Classe",Classe::Nbr_Att,{UniqueClasse::NbrUnique});
    infoClasse.set_attribut(Classe::IdAn,"idAn");
    infoClasse.set_attribut(Classe::IdEtab,"idEt");
    infoClasse.set_attribut(Classe::IdNiveau,"idNiv");
    infoClasse.set_attribut(Classe::Nc,"nc",bmps::typeAttributBdd::Text);
    infoClasse.set_attribut(Classe::Nom,"nm",bmps::typeAttributBdd::Text);
    infoClasse.set_attribut(Classe::Num,"num");
    infoClasse.setUnique(Classe::IdAn,UniqueClasse::Id1Unique);
    infoClasse.setUnique(Classe::IdEtab,UniqueClasse::Id2Unique);
    infoClasse.setUnique(Classe::IdNiveau,UniqueClasse::id_3Unique);
    infoClasse.setUnique(Classe::Num,UniqueClasse::NumUnique);
    infoClasse.setForeignKey(Classe::IdAn,infoAn);
    infoClasse.setForeignKey(Classe::IdEtab,infoEtab);
    infoClasse.setForeignKey(Classe::IdNiveau,infoNiveau);
    setManager<Classe>(std::make_unique<ManagerModifControle<Classe>>(infoClasse,
                std::make_unique<GestionRestrictioncible_code<Classe,Restriction>>(bmps::cibleId::Classe,
                                                                                    get<Restriction>()),
                std::make_unique<UniqueClasse>()));
    set_cible<Classe>(bmps::cibleId::Classe);

    //Eleve
    InfoBdd infoEleve(Eleve::Name(),"Eleve",Eleve::Nbr_Att,{UniqueEleve::NbrUnique});
    infoEleve.set_attribut(Eleve::Date,"dt",bmps::typeAttributBdd::Date);
    infoEleve.set_attribut(Eleve::Fille,"fl",bmps::typeAttributBdd::Bool);
    infoEleve.set_attribut(Eleve::Nom,"nm",bmps::typeAttributBdd::Text);
    infoEleve.set_attribut(Eleve::Prenom,"pnm",bmps::typeAttributBdd::Text);
    infoEleve.setUnique(Eleve::Nom,UniqueEleve::NomUnique);
    infoEleve.setUnique(Eleve::Prenom,UniqueEleve::PrenomUnique);
    infoEleve.setUnique(Eleve::Date,UniqueEleve::DateUnique);
    setManager<Eleve>(std::make_unique<ManagerModifControle<Eleve>>(infoEleve,
                std::make_unique<GestionRestrictioncible_code<Eleve,Restriction>>(bmps::cibleId::Eleve,
                                                                                        get<Restriction>()),
                std::make_unique<UniqueEleve>()));
    set_cible<Eleve>(bmps::cibleId::Eleve);
    
    //ClasseEleve
    using UniqueClEl = RelationUniqueSql<ClasseEleve>;
    InfoBdd infoClEl(ClasseEleve::Name(),"ClasseEleve",ClasseEleve::Nbr_Att,{UniqueClEl::NbrUnique});
    infoClEl.set_attribut(ClasseEleve::IdClasse,"IdCl");
    infoClEl.set_attribut(ClasseEleve::IdEleve,"IdEl");
    infoClEl.set_attribut(ClasseEleve::Entree,"ent",bmps::typeAttributBdd::Date);
    infoClEl.set_attribut(ClasseEleve::Sortie,"sort",bmps::typeAttributBdd::Date);
    infoClEl.setUnique(ClasseEleve::IdClasse,UniqueClEl::Id1Unique);
    infoClEl.setUnique(ClasseEleve::IdEleve,UniqueClEl::Id2Unique);
    infoClEl.setForeignKey(ClasseEleve::IdClasse,infoClasse);
    infoClEl.setForeignKey(ClasseEleve::IdEleve,infoEleve);
    setManager<ClasseEleve>(std::make_unique<ManagerSql<ClasseEleve>>(infoClEl,std::make_unique<UniqueClEl>()));
    set_cible<ClasseEleve>(bmps::cibleId::ClasseEleve);

    //Groupe
    using UniqueGroupe = NomUniqueSql<Groupe>;
    InfoBdd infoGroupe(Groupe::Name(),"Groupe",Groupe::Nbr_Att,{UniqueGroupe::NbrUnique});
    infoGroupe.set_attribut(Groupe::IdAn,"idAn",bmps::typeAttributBdd::Integer,false);
    infoGroupe.set_attribut(Groupe::IdClasse,"idCl",bmps::typeAttributBdd::Integer,false);
    infoGroupe.set_attribut(Groupe::StyleNum,"al");
    infoGroupe.set_attribut(Groupe::Code,"cd");
    infoGroupe.set_attribut(Groupe::Nc,"nc",bmps::typeAttributBdd::Text);
    infoGroupe.set_attribut(Groupe::Nom,"nm",bmps::typeAttributBdd::Text);
    infoGroupe.set_attribut(Groupe::Type,"tp");
    infoGroupe.setUnique(Groupe::Nom,UniqueGroupe::NomUnique);
    infoGroupe.setForeignKey(Groupe::IdAn,infoAn);
    infoGroupe.setForeignKey(Groupe::IdClasse,infoClasse);
    setTypeForeignKey<Groupe>(infoGroupe);
    setManager<Groupe>(std::make_unique<ManagerModifControle<Groupe>>(infoGroupe,
              std::make_unique<GestionRestrictioncible_code<Groupe,Restriction>>(bmps::cibleId::Groupe,
                                                                                      get<Restriction>()),
              std::make_unique<UniqueGroupe>()));
    set_cible<Groupe>(bmps::cibleId::Groupe);

    //TypeControle
    using UniqueTpCtr = NomUniqueSql<TypeControle>;
    InfoBdd infoTpCtr(TypeControle::Name(),"TpControle",TypeControle::Nbr_Att,{UniqueTpCtr::NbrUnique});
    infoTpCtr.set_attribut(TypeControle::Code,"cd");
    infoTpCtr.set_attribut(TypeControle::Decimale,"dcm");
    infoTpCtr.set_attribut(TypeControle::Minima,"min");
    infoTpCtr.set_attribut(TypeControle::Nc,"nc",bmps::typeAttributBdd::Text);
    infoTpCtr.set_attribut(TypeControle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoTpCtr.set_attribut(TypeControle::Parent,"pt",bmps::typeAttributBdd::Integer,false);
    infoTpCtr.set_attribut(TypeControle::Total,"tt");
    infoTpCtr.setUnique(TypeControle::Nom,UniqueTpCtr::NomUnique);
    infoTpCtr.setForeignKey(TypeControle::Parent,infoTpCtr);
    setManager<TypeControle>(std::make_unique<ManagerArbreSimpleModifControle<TypeControle>>(infoTpCtr,
                    std::make_unique<GestionRestrictioncible_code<TypeControle,Restriction>>(bmps::cibleId::TypeControle,
                                                                                             get<Restriction>()),
                    std::make_unique<UniqueTpCtr>()));
    set_cible<TypeControle>(bmps::cibleId::TypeControle);

    //Controle
    using UniqueCtr = NomCibleUniqueSql<Controle>;
    InfoBdd infoCtr(Controle::Name(),"Controle",Controle::Nbr_Att,{UniqueCtr::NbrUnique_1,UniqueCtr::NbrUnique_2});
    infoCtr.set_attribut(Controle::id_type,"idTp");
    infoCtr.set_attribut(Controle::Id_Cible,"idCb");
    infoCtr.set_attribut(Controle::Cible,"cb");
    infoCtr.set_attribut(Controle::Code,"cd");
    infoCtr.set_attribut(Controle::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoCtr.set_attribut(Controle::Decimale,"dcm");
    infoCtr.set_attribut(Controle::Minima,"min");
    infoCtr.set_attribut(Controle::Nc,"nc",bmps::typeAttributBdd::Text);
    infoCtr.set_attribut(Controle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoCtr.set_attribut(Controle::Num,"num");
    infoCtr.set_attribut(Controle::Total,"tt");
    infoCtr.setUnique(Controle::Nom,UniqueCtr::NomUnique,UniqueCtr::NomUniqueSet);
    infoCtr.setUnique(Controle::id_type,UniqueCtr::Id1Unique,UniqueCtr::Id1UniqueSet);
    infoCtr.setUnique(Controle::Id_Cible,UniqueCtr::Id_CibleUnique,UniqueCtr::Id_CibleUniqueSet);
    infoCtr.setUnique(Controle::Cible,UniqueCtr::CibleUnique,UniqueCtr::CibleUniqueSet);
    infoCtr.setUnique(Controle::Num,UniqueCtr::NumUnique,UniqueCtr::NumUniqueSet);
    infoCtr.setForeignKey(Controle::id_type,infoTpCtr);
    setManager<Controle>(std::make_unique<ManagerSql<Controle>>(infoCtr,std::make_unique<UniqueCtr>()));
    set_cible<Controle>(bmps::cibleId::Controle);

    //Enonce
    auto infoArbEnonce = infoBddArbre("ArbEnonce");
    InfoBdd infoEnonce("Enonce","Enonce",Enonce::Nbr_Att);
    infoEnonce.set_attribut(Enonce::Type,"tp");
    infoEnonce.set_attribut(Enonce::Version,"ver");
    infoEnonce.setForeignKey(Enonce::Id,infoArbEnonce);
    setTypeForeignKey<Enonce>(infoEnonce);
    setManager<Enonce>(std::make_unique<ManagerArbre<Enonce>>(infoEnonce,infoArbEnonce));
    set_cible<Enonce>(bmps::cibleId::Enonce);

    //Epreuve
    InfoBdd infoEpreuve("Epreuve","Epreuve",Epreuve::Nbr_Att,{UniqueEpreuve::NbrUnique});
    infoEpreuve.set_attribut(Epreuve::IdBareme,"idBr");
    infoEpreuve.set_attribut(Epreuve::IdEnonce,"idEn");
    infoEpreuve.set_attribut(Epreuve::EnsPoint,"ensPt");
    infoEpreuve.setUnique(Epreuve::IdBareme,UniqueEpreuve::Id1Unique);
    infoEpreuve.setUnique(Epreuve::IdEnonce,UniqueEpreuve::Id2Unique);
    infoEpreuve.setUnique(Epreuve::EnsPoint,UniqueEpreuve::EnsPointUnique);
    infoEpreuve.setForeignKey(Epreuve::IdBareme,infoBareme);
    infoEpreuve.setForeignKey(Epreuve::IdEnonce,infoEnonce);
    setManager<Epreuve>(std::make_unique<ManagerSql<Epreuve>>(infoEpreuve, std::make_unique<UniqueEpreuve>()));
    set_cible<Epreuve>(bmps::cibleId::Epreuve);

    //ControleEpreuve
    InfoBdd infoCtrEp(ControleEpreuve::Name(),"ControleEpreuve",ControleEpreuve::Nbr_Att,
        {UniqueControleEpreuve::NbrUnique_1,UniqueControleEpreuve::NbrUnique_2,UniqueControleEpreuve::NbrUnique_3});
    infoCtrEp.set_attribut(ControleEpreuve::IdClasse,"idCl",bmps::typeAttributBdd::Integer,false);
    infoCtrEp.set_attribut(ControleEpreuve::IdControle,"idCtr");
    infoCtrEp.set_attribut(ControleEpreuve::IdEleve,"idEl",bmps::typeAttributBdd::Integer,false);
    infoCtrEp.set_attribut(ControleEpreuve::IdEpreuve,"idEp");
    infoCtrEp.set_attribut(ControleEpreuve::IdGroupe,"idGr",bmps::typeAttributBdd::Integer,false);
    infoCtrEp.set_attribut(ControleEpreuve::Num,"num");
    infoCtrEp.setUnique(ControleEpreuve::IdClasse,UniqueControleEpreuve::Id1Unique,UniqueControleEpreuve::Id1UniqueSet);
    infoCtrEp.setUnique(ControleEpreuve::IdControle,UniqueControleEpreuve::Id4Unique_1,UniqueControleEpreuve::Id4Unique_1Set);
    infoCtrEp.setUnique(ControleEpreuve::IdGroupe,UniqueControleEpreuve::Id2Unique,UniqueControleEpreuve::Id2UniqueSet);
    infoCtrEp.setUnique(ControleEpreuve::IdControle,UniqueControleEpreuve::Id4Unique_2,UniqueControleEpreuve::Id4Unique_2Set);
    infoCtrEp.setUnique(ControleEpreuve::Num,UniqueControleEpreuve::NumUnique,UniqueControleEpreuve::NumUniqueSet);
    infoCtrEp.setUnique(ControleEpreuve::IdEleve,UniqueControleEpreuve::id_3Unique,UniqueControleEpreuve::id_3UniqueSet);
    infoCtrEp.setUnique(ControleEpreuve::
                        IdControle,UniqueControleEpreuve::Id4Unique_3,UniqueControleEpreuve::Id4Unique_3Set);
    infoCtrEp.setForeignKey(ControleEpreuve::IdClasse,infoClasse);
    infoCtrEp.setForeignKey(ControleEpreuve::IdControle,infoCtr);
    infoCtrEp.setForeignKey(ControleEpreuve::IdEleve,infoEleve);
    infoCtrEp.setForeignKey(ControleEpreuve::IdEpreuve,infoEpreuve);
    infoCtrEp.setForeignKey(ControleEpreuve::IdGroupe,infoGroupe);
    setManager<ControleEpreuve>(std::make_unique<ManagerSql<ControleEpreuve>>(infoCtrEp, std::make_unique<UniqueControleEpreuve>()));
    set_cible<ControleEpreuve>(bmps::cibleId::ControleEpreuve);

    //EleveGroupe
    using UniqueElGr = relation_numUniqueSql<EleveGroupe>;
    InfoBdd infoElGr(EleveGroupe::Name(),"EleveGroupe",EleveGroupe::Nbr_Att,{UniqueElGr::NbrUnique});
    infoElGr.set_attribut(EleveGroupe::IdEleve,"idEl");
    infoElGr.set_attribut(EleveGroupe::IdGroupe,"idGr");
    infoElGr.set_attribut(EleveGroupe::Num,"num");
    infoElGr.setUnique(EleveGroupe::IdEleve,UniqueElGr::Id1Unique);
    infoElGr.setUnique(EleveGroupe::IdGroupe,UniqueElGr::Id2Unique);
    infoElGr.setUnique(EleveGroupe::Num,UniqueElGr::NumUnique);
    infoElGr.setForeignKey(EleveGroupe::IdEleve,infoEleve);
    infoElGr.setForeignKey(EleveGroupe::IdGroupe,infoGroupe);
    setManager<EleveGroupe>(std::make_unique<ManagerSql<EleveGroupe>>(infoElGr, std::make_unique<UniqueElGr>()));
    set_cible<EleveGroupe>(bmps::cibleId::EleveGroupe);

    //Point
    InfoBdd infoPoint("Point","Point",Point::Nbr_Att);
    infoPoint.set_attribut(Point::Type,"tp");
    infoPoint.set_attribut(Point::Valeur,"vl");
    infoPoint.set_attribut(Point::Version,"ver");
    setTypeForeignKey<Point>(infoPoint);
    setManager<Point>(std::make_unique<ManagerSql<Point>>(infoPoint));
    set_cible<Point>(bmps::cibleId::Point);

    //EnoncePoint
    InfoBdd infoEnPt(EnoncePoint::Name(),"EnoncePoint",EnoncePoint::Nbr_Att,{UniqueEnoncePoint::NbrUnique});
    infoEnPt.set_attribut(EnoncePoint::IdEnonce,"idEn");
    infoEnPt.set_attribut(EnoncePoint::IdPoint,"idPt");
    infoEnPt.set_attribut(EnoncePoint::EnsPoint,"ensPt");
    infoEnPt.set_attribut(EnoncePoint::Num,"num");
    infoEnPt.setUnique(EnoncePoint::IdEnonce,UniqueEnoncePoint::Id1Unique);
    infoEnPt.setUnique(EnoncePoint::EnsPoint,UniqueEnoncePoint::EnsPointUnique);
    infoEnPt.setUnique(EnoncePoint::Num,UniqueEnoncePoint::NumUnique);
    infoEnPt.setForeignKey(EnoncePoint::IdEnonce,infoEnonce);
    infoEnPt.setForeignKey(EnoncePoint::IdPoint,infoPoint);
    setManager<EnoncePoint>(std::make_unique<ManagerSql<EnoncePoint>>(infoEnPt,std::make_unique<UniqueEnoncePoint>()));
    set_cible<EnoncePoint>(bmps::cibleId::EnoncePoint);

    //EtablissementNiveau
    using UniqueEtabNiv = RelationUniqueSql<EtablissementNiveau>;
    InfoBdd infoEtabNiv(EtablissementNiveau::Name(),"EtabNiveau",EtablissementNiveau::Nbr_Att,{UniqueEtabNiv::NbrUnique});
    infoEtabNiv.set_attribut(EtablissementNiveau::IdEtab,"idEt");
    infoEtabNiv.set_attribut(EtablissementNiveau::IdNiveau,"idNiv");
    infoEtabNiv.setForeignKey(EtablissementNiveau::IdEtab,infoEtab);
    infoEtabNiv.setForeignKey(EtablissementNiveau::IdNiveau,infoNiveau);
    infoEtabNiv.setUnique(EtablissementNiveau::IdEtab,UniqueEtabNiv::Id1Unique);
    infoEtabNiv.setUnique(EtablissementNiveau::IdNiveau,UniqueEtabNiv::Id2Unique);
    setManager<EtablissementNiveau>(std::make_unique<ManagerSql<EtablissementNiveau>>(infoEtabNiv, std::make_unique<UniqueEtabNiv>()));
    set_cible<EtablissementNiveau>(bmps::cibleId::EtablissementNiveau);

    // Exercice
    auto infoArbExo = infoBddArbre("ArbExercice");
    InfoBdd infoExo(Exercice::Name(),"Exercice",Exercice::Nbr_Att);
    infoExo.set_attribut(Exercice::IdOriginal,"idOrig",bmps::typeAttributBdd::Integer,false);
    infoExo.set_attribut(Exercice::Type,"tp");
    infoExo.set_attribut(Exercice::Version,"ver");
    infoExo.setForeignKey(Exercice::Id,infoArbExo);
    infoExo.setForeignKey(Exercice::IdOriginal,infoExo);
    setTypeForeignKey<Exercice>(infoExo);
    setManager<Exercice>(std::make_unique<ManagerArbre<Exercice>>(infoExo,infoArbExo));
    set_cible<Exercice>(bmps::cibleId::Exercice);

    //FiliationNiveau
    using UniqueFilNiv = RelationUniqueSql<FiliationNiveau>;
    InfoBdd infoFilNiv(FiliationNiveau::Name(),"FiliationNiveau",FiliationNiveau::Nbr_Att,{UniqueFilNiv::NbrUnique});
    infoFilNiv.set_attribut(FiliationNiveau::IdPrecedent,"idP");
    infoFilNiv.set_attribut(FiliationNiveau::IdSuivant,"idS");
    infoFilNiv.setForeignKey(FiliationNiveau::IdPrecedent,infoNiveau);
    infoFilNiv.setForeignKey(FiliationNiveau::IdSuivant,infoNiveau);
    infoFilNiv.setUnique(FiliationNiveau::IdPrecedent,UniqueFilNiv::Id1Unique);
    infoFilNiv.setUnique(FiliationNiveau::IdSuivant,UniqueFilNiv::Id2Unique);
    setManager<FiliationNiveau>(std::make_unique<ManagerModifControle<FiliationNiveau>>(infoFilNiv,
                   std::make_unique<GestionRestrictioncible_code<FiliationNiveau,Restriction>>(bmps::cibleId::FiliationNiveau,
                                                                                               get<Restriction>()),
                   std::make_unique<UniqueFilNiv>()));
    set_cible<FiliationNiveau>(bmps::cibleId::FiliationNiveau);

    //NiveauTypeEtablissement
    using UniqueNivTpEtab = RelationUniqueSql<NiveauTypeEtablissement>;
    InfoBdd infoNivTpEtab(NiveauTypeEtablissement::Name(),"NiveauTpEtab",NiveauTypeEtablissement::Nbr_Att,{UniqueNivTpEtab::NbrUnique});
    infoNivTpEtab.set_attribut(NiveauTypeEtablissement::IdNiveau,"idNiv");
    infoNivTpEtab.set_attribut(NiveauTypeEtablissement::IdTpEtab,"idTE");
    infoNivTpEtab.setForeignKey(NiveauTypeEtablissement::IdNiveau,infoNiveau);
    infoNivTpEtab.setForeignKey(NiveauTypeEtablissement::IdTpEtab,infoTpEtab);
    infoNivTpEtab.setUnique(NiveauTypeEtablissement::IdNiveau,UniqueNivTpEtab::Id1Unique);
    infoNivTpEtab.setUnique(NiveauTypeEtablissement::IdTpEtab,UniqueNivTpEtab::Id2Unique);
    setManager<NiveauTypeEtablissement>(std::make_unique<ManagerModifControle<NiveauTypeEtablissement>>(infoNivTpEtab,
               std::make_unique<GestionRestrictioncible_code<NiveauTypeEtablissement,Restriction>>
                                                                 (bmps::cibleId::NiveauTypeEtablissement,get<Restriction>()),
               std::make_unique<UniqueNivTpEtab>()));
    set_cible<NiveauTypeEtablissement>(bmps::cibleId::NiveauTypeEtablissement);

    //Note
    using UniqueNote = RelationUniqueSql<Note>;
    InfoBdd infoNote(Note::Name(),"Note",Note::Nbr_Att,{UniqueNote::NbrUnique});
    infoNote.set_attribut(Note::IdControle,"idCtr");
    infoNote.set_attribut(Note::IdEleve,"idEl");
    infoNote.set_attribut(Note::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoNote.set_attribut(Note::Saisie,"ss");
    infoNote.set_attribut(Note::Valeur,"vl");
    infoNote.setUnique(Note::IdControle,UniqueNote::Id1Unique);
    infoNote.setUnique(Note::IdEleve,UniqueNote::Id2Unique);
    infoNote.setForeignKey(Note::IdControle,infoCtr);
    infoNote.setForeignKey(Note::IdEleve,infoEleve);
    setManager<Note>(std::make_unique<ManagerSql<Note>>(infoNote,std::make_unique<UniqueNote>()));
    set_cible<Note>(bmps::cibleId::Note);

    //_valide
    using Unique_valide = RelationUniqueSql<_valide>;
    InfoBdd info_valide(_valide::Name(),"_valide",_valide::Nbr_Att,{Unique_valide::NbrUnique});
    info_valide.set_attribut(_valide::IdNote,"idNt");
    info_valide.set_attribut(_valide::IdPoint,"idPt");
    info_valide.set_attribut(_valide::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    info_valide.set_attribut(_valide::Valeur,"vl");
    info_valide.setUnique(_valide::IdNote,Unique_valide::Id1Unique);
    info_valide.setUnique(_valide::IdPoint,Unique_valide::Id2Unique);
    info_valide.setForeignKey(_valide::IdNote,infoNote);
    info_valide.setForeignKey(_valide::IdPoint,infoPoint);
    setManager<_valide>(std::make_unique<ManagerSql<_valide>>(info_valide, std::make_unique<Unique_valide>()));
    set_cible<_valide>(bmps::cibleId::_valide);
}
