#include "ManagersNote.h"

using namespace noteMPS;
using namespace managerMPS;

ManagersNote::ManagersNote()
    : ManagersPredef (infoEntityNote::NbrEntityId,bmps::cibleId::NbrCible,"NoteVersion")
{
    enableRestriction("RestModif");
    enableType("Type","PermType",
               std::make_unique<GestionRestrictionCibleCode<Type,Restriction>>(bmps::cibleId::Type,
                                                                                get<Restriction>()),
               std::make_unique<GestionRestrictionCibleCode<TypePermission,Restriction>>(bmps::cibleId::TypePermission,
                                                                                          get<Restriction>()));
    enableCommentaire("Commentaire","CbCommentaire");
    enableDonnee("Donnee","ArbDonnee","CbDonnee","CardDonnee",
                 std::make_unique<GestionRestrictionCibleCode<Donnee,Restriction>>(bmps::cibleId::Donnee,
                                                                                    get<Restriction>()),
                 std::make_unique<GestionRestrictionCibleCode<DonneeCible,Restriction>>(bmps::cibleId::DonneeCible,
                                                                                         get<Restriction>()),
                 std::make_unique<GestionRestrictionCibleCode<DonneeCard,Restriction>>(bmps::cibleId::DonneeCard,
                                                                                        get<Restriction>()));
    enableEvenement("Evenement","EvenementCible","EvenementStyle");
    enableMotCle("MotCle","ArbMotCle","CbMotCle","CbMotProg","PermMotCle","PermMotProg",
                 std::make_unique<GestionRestrictionCibleCode<MotCle,Restriction>>(bmps::cibleId::MotCle,
                                                                                    get<Restriction>()),
                 std::make_unique<GestionRestrictionCibleCode<MotClePermission,Restriction>>(bmps::cibleId::MotClePermission,
                                                                                              get<Restriction>()),
                 std::make_unique<GestionRestrictionCibleCode<MotProgPermission,Restriction>>(bmps::cibleId::MotProgPermission,
                                                                                               get<Restriction>()));
    enableTexte("Texte","CbTexte","Source","TexteSource");
    enableUtilisation("Utilisation","Usage","ArbUsage",
                      std::make_unique<GestionRestrictionCibleCode<Usage,Restriction>>(bmps::cibleId::Usage,
                                                                                        get<Restriction>()));
    
    //Annee
    using UniqueAn = NumUniqueSql<Annee>;
    InfoBdd infoAn("Annee","Annee",Annee::NbrAtt,{UniqueAn::NbrUnique});
    infoAn.setAttribut(Annee::Num,"num");
    infoAn.setUnique(Annee::Num,UniqueAn::NumUnique);
    setManager<Annee>(std::make_unique<ManagerModifControle<Annee>>(infoAn,
                std::make_unique<GestionRestrictionCibleCode<Annee,Restriction>>(bmps::cibleId::Annee,
                                                                                              get<Restriction>()),
                std::make_unique<UniqueAn>()));
    setCible<Annee>(bmps::cibleId::Annee);
    
    //Bareme
    auto infoArbBareme = infoBddArbre("ArbBareme");
    InfoBdd infoBareme("Bareme","Bareme",Bareme::NbrAtt);
    infoBareme.setAttribut(Bareme::IdCible,"idCb");
    infoBareme.setAttribut(Bareme::Cible,"cb");
    infoBareme.setAttribut(Bareme::Type,"tp");
    infoBareme.setAttribut(Bareme::Valeur,"vl",bmps::typeAttributBdd::Double);
    infoBareme.setForeignKey(Bareme::Id,infoArbBareme);
    setTypeForeignKey<Bareme>(infoBareme);
    setManager<Bareme>(std::make_unique<ManagerArbre<Bareme>>(infoBareme,infoArbBareme));
    setCible<Bareme>(bmps::cibleId::Bareme);

    //TypeEtablissement
    using UniqueTpEtab = NomUniqueSql<TypeEtablissement>;
    InfoBdd infoTpEtab("TypeEtablissement","TpEtab",TypeEtablissement::NbrAtt,{UniqueTpEtab::NbrUnique});
    infoTpEtab.setAttribut(TypeEtablissement::Nc,"nc",bmps::typeAttributBdd::Text);
    infoTpEtab.setAttribut(TypeEtablissement::Nom,"nm",bmps::typeAttributBdd::Text);
    infoTpEtab.setUnique(TypeEtablissement::Nom,UniqueTpEtab::NomUnique);
    setManager<TypeEtablissement>(std::make_unique<ManagerModifControle<TypeEtablissement>>(infoTpEtab,
               std::make_unique<GestionRestrictionCibleCode<TypeEtablissement,Restriction>>(bmps::cibleId::TypeEtablissement,
                                                                                             get<Restriction>()),
               std::make_unique<UniqueTpEtab>()));
    setCible<TypeEtablissement>(bmps::cibleId::TypeEtablissement);

    //Etablissement
    using UniqueEtab = NomUniqueSql<Etablissement>;
    InfoBdd infoEtab("Etablissement","Etab",Etablissement::NbrAtt,{UniqueEtab::NbrUnique});
    infoEtab.setAttribut(Etablissement::Nc,"nc",bmps::typeAttributBdd::Text);
    infoEtab.setAttribut(Etablissement::Nom,"nm",bmps::typeAttributBdd::Text);
    infoEtab.setUnique(Etablissement::Nom,UniqueEtab::NomUnique);
    setManager<Etablissement>(std::make_unique<ManagerModifControle<Etablissement>>(infoEtab,
                std::make_unique<GestionRestrictionCibleCode<Etablissement,Restriction>>(bmps::cibleId::Etablissement,
                                                                                              get<Restriction>()),
                std::make_unique<UniqueEtab>()));
    setCible<Etablissement>(bmps::cibleId::Etablissement);

    //EtablissementType
    using UniqueEtabTp = RelationUniqueSql<EtablissementType>;
    InfoBdd infoEtabTp("EtablissementType","EtabTp",EtablissementType::NbrAtt,{UniqueEtabTp::NbrUnique});
    infoEtabTp.setAttribut(EtablissementType::IdEtab,"idEtab");
    infoEtabTp.setAttribut(EtablissementType::IdTpEtab,"idTpEtab");
    infoEtabTp.setUnique(EtablissementType::IdEtab,UniqueEtabTp::Id1Unique);
    infoEtabTp.setUnique(EtablissementType::IdTpEtab,UniqueEtabTp::Id2Unique);
    infoEtabTp.setForeignKey(EtablissementType::IdEtab,infoEtab);
    infoEtabTp.setForeignKey(EtablissementType::IdTpEtab,infoTpEtab);
    setManager<EtablissementType>(std::make_unique<ManagerSql<EtablissementType>>(infoEtabTp, std::make_unique<UniqueEtabTp>()));
    setCible<EtablissementType>(bmps::cibleId::EtablissementType);

    //Niveau
    using UniqueNiveau = NomUniqueSql<Niveau>;
    InfoBdd infoNiveau("Niveau","Niveau",Niveau::NbrAtt,{UniqueNiveau::NbrUnique});
    infoNiveau.setAttribut(Niveau::Nc,"nc",bmps::typeAttributBdd::Text);
    infoNiveau.setAttribut(Niveau::Nom,"nm",bmps::typeAttributBdd::Text);
    infoNiveau.setAttribut(Niveau::Type,"tp");
    infoNiveau.setUnique(Niveau::Nom,UniqueNiveau::NomUnique);
    setTypeForeignKey<Niveau>(infoNiveau);
    setManager<Niveau>(std::make_unique<ManagerModifControle<Niveau>>(infoNiveau,
               std::make_unique<GestionRestrictionCibleCode<Niveau,Restriction>>(bmps::cibleId::Niveau,
                                                                                  get<Restriction>()),
               std::make_unique<UniqueNiveau>()));
    setCible<Niveau>(bmps::cibleId::Niveau);
    
    //Classe
    using UniqueClasse = RelationTroisNumUniqueSql<Classe>;
    InfoBdd infoClasse(Classe::Name(),"Classe",Classe::NbrAtt,{UniqueClasse::NbrUnique});
    infoClasse.setAttribut(Classe::IdAn,"idAn");
    infoClasse.setAttribut(Classe::IdEtab,"idEt");
    infoClasse.setAttribut(Classe::IdNiveau,"idNiv");
    infoClasse.setAttribut(Classe::Nc,"nc",bmps::typeAttributBdd::Text);
    infoClasse.setAttribut(Classe::Nom,"nm",bmps::typeAttributBdd::Text);
    infoClasse.setAttribut(Classe::Num,"num");
    infoClasse.setUnique(Classe::IdAn,UniqueClasse::Id1Unique);
    infoClasse.setUnique(Classe::IdEtab,UniqueClasse::Id2Unique);
    infoClasse.setUnique(Classe::IdNiveau,UniqueClasse::Id3Unique);
    infoClasse.setUnique(Classe::Num,UniqueClasse::NumUnique);
    infoClasse.setForeignKey(Classe::IdAn,infoAn);
    infoClasse.setForeignKey(Classe::IdEtab,infoEtab);
    infoClasse.setForeignKey(Classe::IdNiveau,infoNiveau);
    setManager<Classe>(std::make_unique<ManagerModifControle<Classe>>(infoClasse,
                std::make_unique<GestionRestrictionCibleCode<Classe,Restriction>>(bmps::cibleId::Classe,
                                                                                    get<Restriction>()),
                std::make_unique<UniqueClasse>()));
    setCible<Classe>(bmps::cibleId::Classe);

    //Eleve
    InfoBdd infoEleve(Eleve::Name(),"Eleve",Eleve::NbrAtt,{UniqueEleve::NbrUnique});
    infoEleve.setAttribut(Eleve::Date,"dt",bmps::typeAttributBdd::Date);
    infoEleve.setAttribut(Eleve::Fille,"fl",bmps::typeAttributBdd::Bool);
    infoEleve.setAttribut(Eleve::Nom,"nm",bmps::typeAttributBdd::Text);
    infoEleve.setAttribut(Eleve::Prenom,"pnm",bmps::typeAttributBdd::Text);
    infoEleve.setUnique(Eleve::Nom,UniqueEleve::NomUnique);
    infoEleve.setUnique(Eleve::Prenom,UniqueEleve::PrenomUnique);
    infoEleve.setUnique(Eleve::Date,UniqueEleve::DateUnique);
    setManager<Eleve>(std::make_unique<ManagerModifControle<Eleve>>(infoEleve,
                std::make_unique<GestionRestrictionCibleCode<Eleve,Restriction>>(bmps::cibleId::Eleve,
                                                                                        get<Restriction>()),
                std::make_unique<UniqueEleve>()));
    setCible<Eleve>(bmps::cibleId::Eleve);
    
    //ClasseEleve
    using UniqueClEl = RelationUniqueSql<ClasseEleve>;
    InfoBdd infoClEl(ClasseEleve::Name(),"ClasseEleve",ClasseEleve::NbrAtt,{UniqueClEl::NbrUnique});
    infoClEl.setAttribut(ClasseEleve::IdClasse,"IdCl");
    infoClEl.setAttribut(ClasseEleve::IdEleve,"IdEl");
    infoClEl.setAttribut(ClasseEleve::Entree,"ent",bmps::typeAttributBdd::Date);
    infoClEl.setAttribut(ClasseEleve::Sortie,"sort",bmps::typeAttributBdd::Date);
    infoClEl.setUnique(ClasseEleve::IdClasse,UniqueClEl::Id1Unique);
    infoClEl.setUnique(ClasseEleve::IdEleve,UniqueClEl::Id2Unique);
    infoClEl.setForeignKey(ClasseEleve::IdClasse,infoClasse);
    infoClEl.setForeignKey(ClasseEleve::IdEleve,infoEleve);
    setManager<ClasseEleve>(std::make_unique<ManagerSql<ClasseEleve>>(infoClEl,std::make_unique<UniqueClEl>()));
    setCible<ClasseEleve>(bmps::cibleId::ClasseEleve);

    //Groupe
    using UniqueGroupe = NomUniqueSql<Groupe>;
    InfoBdd infoGroupe(Groupe::Name(),"Groupe",Groupe::NbrAtt,{UniqueGroupe::NbrUnique});
    infoGroupe.setAttribut(Groupe::IdAn,"idAn",bmps::typeAttributBdd::Integer,false);
    infoGroupe.setAttribut(Groupe::IdClasse,"idCl",bmps::typeAttributBdd::Integer,false);
    infoGroupe.setAttribut(Groupe::StyleNum,"al");
    infoGroupe.setAttribut(Groupe::Code,"cd");
    infoGroupe.setAttribut(Groupe::Nc,"nc",bmps::typeAttributBdd::Text);
    infoGroupe.setAttribut(Groupe::Nom,"nm",bmps::typeAttributBdd::Text);
    infoGroupe.setAttribut(Groupe::Type,"tp");
    infoGroupe.setUnique(Groupe::Nom,UniqueGroupe::NomUnique);
    infoGroupe.setForeignKey(Groupe::IdAn,infoAn);
    infoGroupe.setForeignKey(Groupe::IdClasse,infoClasse);
    setTypeForeignKey<Groupe>(infoGroupe);
    setManager<Groupe>(std::make_unique<ManagerModifControle<Groupe>>(infoGroupe,
              std::make_unique<GestionRestrictionCibleCode<Groupe,Restriction>>(bmps::cibleId::Groupe,
                                                                                      get<Restriction>()),
              std::make_unique<UniqueGroupe>()));
    setCible<Groupe>(bmps::cibleId::Groupe);

    //TypeControle
    using UniqueTpCtr = NomUniqueSql<TypeControle>;
    InfoBdd infoTpCtr(TypeControle::Name(),"TpControle",TypeControle::NbrAtt,{UniqueTpCtr::NbrUnique});
    infoTpCtr.setAttribut(TypeControle::Code,"cd");
    infoTpCtr.setAttribut(TypeControle::Decimale,"dcm");
    infoTpCtr.setAttribut(TypeControle::Minima,"min");
    infoTpCtr.setAttribut(TypeControle::Nc,"nc",bmps::typeAttributBdd::Text);
    infoTpCtr.setAttribut(TypeControle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoTpCtr.setAttribut(TypeControle::Parent,"pt",bmps::typeAttributBdd::Integer,false);
    infoTpCtr.setAttribut(TypeControle::Total,"tt");
    infoTpCtr.setUnique(TypeControle::Nom,UniqueTpCtr::NomUnique);
    infoTpCtr.setForeignKey(TypeControle::Parent,infoTpCtr);
    setManager<TypeControle>(std::make_unique<ManagerArbreSimpleModifControle<TypeControle>>(infoTpCtr,
                    std::make_unique<GestionRestrictionCibleCode<TypeControle,Restriction>>(bmps::cibleId::TypeControle,
                                                                                             get<Restriction>()),
                    std::make_unique<UniqueTpCtr>()));
    setCible<TypeControle>(bmps::cibleId::TypeControle);

    //Controle
    using UniqueCtr = NomCibleUniqueSql<Controle>;
    InfoBdd infoCtr(Controle::Name(),"Controle",Controle::NbrAtt,{UniqueCtr::NbrUnique_1,UniqueCtr::NbrUnique_2});
    infoCtr.setAttribut(Controle::IdType,"idTp");
    infoCtr.setAttribut(Controle::IdCible,"idCb");
    infoCtr.setAttribut(Controle::Cible,"cb");
    infoCtr.setAttribut(Controle::Code,"cd");
    infoCtr.setAttribut(Controle::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoCtr.setAttribut(Controle::Decimale,"dcm");
    infoCtr.setAttribut(Controle::Minima,"min");
    infoCtr.setAttribut(Controle::Nc,"nc",bmps::typeAttributBdd::Text);
    infoCtr.setAttribut(Controle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoCtr.setAttribut(Controle::Num,"num");
    infoCtr.setAttribut(Controle::Total,"tt");
    infoCtr.setUnique(Controle::Nom,UniqueCtr::NomUnique,UniqueCtr::NomUniqueSet);
    infoCtr.setUnique(Controle::IdType,UniqueCtr::Id1Unique,UniqueCtr::Id1UniqueSet);
    infoCtr.setUnique(Controle::IdCible,UniqueCtr::IdCibleUnique,UniqueCtr::IdCibleUniqueSet);
    infoCtr.setUnique(Controle::Cible,UniqueCtr::CibleUnique,UniqueCtr::CibleUniqueSet);
    infoCtr.setUnique(Controle::Num,UniqueCtr::NumUnique,UniqueCtr::NumUniqueSet);
    infoCtr.setForeignKey(Controle::IdType,infoTpCtr);
    setManager<Controle>(std::make_unique<ManagerSql<Controle>>(infoCtr,std::make_unique<UniqueCtr>()));
    setCible<Controle>(bmps::cibleId::Controle);

    //Enonce
    auto infoArbEnonce = infoBddArbre("ArbEnonce");
    InfoBdd infoEnonce("Enonce","Enonce",Enonce::NbrAtt);
    infoEnonce.setAttribut(Enonce::Type,"tp");
    infoEnonce.setAttribut(Enonce::Version,"ver");
    infoEnonce.setForeignKey(Enonce::Id,infoArbEnonce);
    setTypeForeignKey<Enonce>(infoEnonce);
    setManager<Enonce>(std::make_unique<ManagerArbre<Enonce>>(infoEnonce,infoArbEnonce));
    setCible<Enonce>(bmps::cibleId::Enonce);

    //Epreuve
    InfoBdd infoEpreuve("Epreuve","Epreuve",Epreuve::NbrAtt,{UniqueEpreuve::NbrUnique});
    infoEpreuve.setAttribut(Epreuve::IdBareme,"idBr");
    infoEpreuve.setAttribut(Epreuve::IdEnonce,"idEn");
    infoEpreuve.setAttribut(Epreuve::EnsPoint,"ensPt");
    infoEpreuve.setUnique(Epreuve::IdBareme,UniqueEpreuve::Id1Unique);
    infoEpreuve.setUnique(Epreuve::IdEnonce,UniqueEpreuve::Id2Unique);
    infoEpreuve.setUnique(Epreuve::EnsPoint,UniqueEpreuve::EnsPointUnique);
    infoEpreuve.setForeignKey(Epreuve::IdBareme,infoBareme);
    infoEpreuve.setForeignKey(Epreuve::IdEnonce,infoEnonce);
    setManager<Epreuve>(std::make_unique<ManagerSql<Epreuve>>(infoEpreuve, std::make_unique<UniqueEpreuve>()));
    setCible<Epreuve>(bmps::cibleId::Epreuve);

    //ControleEpreuve
    InfoBdd infoCtrEp(ControleEpreuve::Name(),"ControleEpreuve",ControleEpreuve::NbrAtt,
        {UniqueControleEpreuve::NbrUnique_1,UniqueControleEpreuve::NbrUnique_2,UniqueControleEpreuve::NbrUnique_3});
    infoCtrEp.setAttribut(ControleEpreuve::IdClasse,"idCl",bmps::typeAttributBdd::Integer,false);
    infoCtrEp.setAttribut(ControleEpreuve::IdControle,"idCtr");
    infoCtrEp.setAttribut(ControleEpreuve::IdEleve,"idEl",bmps::typeAttributBdd::Integer,false);
    infoCtrEp.setAttribut(ControleEpreuve::IdEpreuve,"idEp");
    infoCtrEp.setAttribut(ControleEpreuve::IdGroupe,"idGr",bmps::typeAttributBdd::Integer,false);
    infoCtrEp.setAttribut(ControleEpreuve::Num,"num");
    infoCtrEp.setUnique(ControleEpreuve::IdClasse,UniqueControleEpreuve::Id1Unique,UniqueControleEpreuve::Id1UniqueSet);
    infoCtrEp.setUnique(ControleEpreuve::IdControle,UniqueControleEpreuve::Id4Unique_1,UniqueControleEpreuve::Id4Unique_1Set);
    infoCtrEp.setUnique(ControleEpreuve::IdGroupe,UniqueControleEpreuve::Id2Unique,UniqueControleEpreuve::Id2UniqueSet);
    infoCtrEp.setUnique(ControleEpreuve::IdControle,UniqueControleEpreuve::Id4Unique_2,UniqueControleEpreuve::Id4Unique_2Set);
    infoCtrEp.setUnique(ControleEpreuve::Num,UniqueControleEpreuve::NumUnique,UniqueControleEpreuve::NumUniqueSet);
    infoCtrEp.setUnique(ControleEpreuve::IdEleve,UniqueControleEpreuve::Id3Unique,UniqueControleEpreuve::Id3UniqueSet);
    infoCtrEp.setUnique(ControleEpreuve::
                        IdControle,UniqueControleEpreuve::Id4Unique_3,UniqueControleEpreuve::Id4Unique_3Set);
    infoCtrEp.setForeignKey(ControleEpreuve::IdClasse,infoClasse);
    infoCtrEp.setForeignKey(ControleEpreuve::IdControle,infoCtr);
    infoCtrEp.setForeignKey(ControleEpreuve::IdEleve,infoEleve);
    infoCtrEp.setForeignKey(ControleEpreuve::IdEpreuve,infoEpreuve);
    infoCtrEp.setForeignKey(ControleEpreuve::IdGroupe,infoGroupe);
    setManager<ControleEpreuve>(std::make_unique<ManagerSql<ControleEpreuve>>(infoCtrEp, std::make_unique<UniqueControleEpreuve>()));
    setCible<ControleEpreuve>(bmps::cibleId::ControleEpreuve);

    //EleveGroupe
    using UniqueElGr = RelationNumUniqueSql<EleveGroupe>;
    InfoBdd infoElGr(EleveGroupe::Name(),"EleveGroupe",EleveGroupe::NbrAtt,{UniqueElGr::NbrUnique});
    infoElGr.setAttribut(EleveGroupe::IdEleve,"idEl");
    infoElGr.setAttribut(EleveGroupe::IdGroupe,"idGr");
    infoElGr.setAttribut(EleveGroupe::Num,"num");
    infoElGr.setUnique(EleveGroupe::IdEleve,UniqueElGr::Id1Unique);
    infoElGr.setUnique(EleveGroupe::IdGroupe,UniqueElGr::Id2Unique);
    infoElGr.setUnique(EleveGroupe::Num,UniqueElGr::NumUnique);
    infoElGr.setForeignKey(EleveGroupe::IdEleve,infoEleve);
    infoElGr.setForeignKey(EleveGroupe::IdGroupe,infoGroupe);
    setManager<EleveGroupe>(std::make_unique<ManagerSql<EleveGroupe>>(infoElGr, std::make_unique<UniqueElGr>()));
    setCible<EleveGroupe>(bmps::cibleId::EleveGroupe);

    //Point
    InfoBdd infoPoint("Point","Point",Point::NbrAtt);
    infoPoint.setAttribut(Point::Type,"tp");
    infoPoint.setAttribut(Point::Valeur,"vl");
    infoPoint.setAttribut(Point::Version,"ver");
    setTypeForeignKey<Point>(infoPoint);
    setManager<Point>(std::make_unique<ManagerSql<Point>>(infoPoint));
    setCible<Point>(bmps::cibleId::Point);

    //EnoncePoint
    InfoBdd infoEnPt(EnoncePoint::Name(),"EnoncePoint",EnoncePoint::NbrAtt,{UniqueEnoncePoint::NbrUnique});
    infoEnPt.setAttribut(EnoncePoint::IdEnonce,"idEn");
    infoEnPt.setAttribut(EnoncePoint::IdPoint,"idPt");
    infoEnPt.setAttribut(EnoncePoint::EnsPoint,"ensPt");
    infoEnPt.setAttribut(EnoncePoint::Num,"num");
    infoEnPt.setUnique(EnoncePoint::IdEnonce,UniqueEnoncePoint::Id1Unique);
    infoEnPt.setUnique(EnoncePoint::EnsPoint,UniqueEnoncePoint::EnsPointUnique);
    infoEnPt.setUnique(EnoncePoint::Num,UniqueEnoncePoint::NumUnique);
    infoEnPt.setForeignKey(EnoncePoint::IdEnonce,infoEnonce);
    infoEnPt.setForeignKey(EnoncePoint::IdPoint,infoPoint);
    setManager<EnoncePoint>(std::make_unique<ManagerSql<EnoncePoint>>(infoEnPt,std::make_unique<UniqueEnoncePoint>()));
    setCible<EnoncePoint>(bmps::cibleId::EnoncePoint);

    //EtablissementNiveau
    using UniqueEtabNiv = RelationUniqueSql<EtablissementNiveau>;
    InfoBdd infoEtabNiv(EtablissementNiveau::Name(),"EtabNiveau",EtablissementNiveau::NbrAtt,{UniqueEtabNiv::NbrUnique});
    infoEtabNiv.setAttribut(EtablissementNiveau::IdEtab,"idEt");
    infoEtabNiv.setAttribut(EtablissementNiveau::IdNiveau,"idNiv");
    infoEtabNiv.setForeignKey(EtablissementNiveau::IdEtab,infoEtab);
    infoEtabNiv.setForeignKey(EtablissementNiveau::IdNiveau,infoNiveau);
    infoEtabNiv.setUnique(EtablissementNiveau::IdEtab,UniqueEtabNiv::Id1Unique);
    infoEtabNiv.setUnique(EtablissementNiveau::IdNiveau,UniqueEtabNiv::Id2Unique);
    setManager<EtablissementNiveau>(std::make_unique<ManagerSql<EtablissementNiveau>>(infoEtabNiv, std::make_unique<UniqueEtabNiv>()));
    setCible<EtablissementNiveau>(bmps::cibleId::EtablissementNiveau);

    //FiliationNiveau
    using UniqueFilNiv = RelationUniqueSql<FiliationNiveau>;
    InfoBdd infoFilNiv(FiliationNiveau::Name(),"FiliationNiveau",FiliationNiveau::NbrAtt,{UniqueFilNiv::NbrUnique});
    infoFilNiv.setAttribut(FiliationNiveau::IdPrecedent,"idP");
    infoFilNiv.setAttribut(FiliationNiveau::IdSuivant,"idS");
    infoFilNiv.setForeignKey(FiliationNiveau::IdPrecedent,infoNiveau);
    infoFilNiv.setForeignKey(FiliationNiveau::IdSuivant,infoNiveau);
    infoFilNiv.setUnique(FiliationNiveau::IdPrecedent,UniqueFilNiv::Id1Unique);
    infoFilNiv.setUnique(FiliationNiveau::IdSuivant,UniqueFilNiv::Id2Unique);
    setManager<FiliationNiveau>(std::make_unique<ManagerModifControle<FiliationNiveau>>(infoFilNiv,
                   std::make_unique<GestionRestrictionCibleCode<FiliationNiveau,Restriction>>(bmps::cibleId::FiliationNiveau,
                                                                                               get<Restriction>()),
                   std::make_unique<UniqueFilNiv>()));
    setCible<FiliationNiveau>(bmps::cibleId::FiliationNiveau);

    //NiveauTypeEtablissement
    using UniqueNivTpEtab = RelationUniqueSql<NiveauTypeEtablissement>;
    InfoBdd infoNivTpEtab(NiveauTypeEtablissement::Name(),"NiveauTpEtab",NiveauTypeEtablissement::NbrAtt,{UniqueNivTpEtab::NbrUnique});
    infoNivTpEtab.setAttribut(NiveauTypeEtablissement::IdNiveau,"idNiv");
    infoNivTpEtab.setAttribut(NiveauTypeEtablissement::IdTpEtab,"idTE");
    infoNivTpEtab.setForeignKey(NiveauTypeEtablissement::IdNiveau,infoNiveau);
    infoNivTpEtab.setForeignKey(NiveauTypeEtablissement::IdTpEtab,infoTpEtab);
    infoNivTpEtab.setUnique(NiveauTypeEtablissement::IdNiveau,UniqueNivTpEtab::Id1Unique);
    infoNivTpEtab.setUnique(NiveauTypeEtablissement::IdTpEtab,UniqueNivTpEtab::Id2Unique);
    setManager<NiveauTypeEtablissement>(std::make_unique<ManagerModifControle<NiveauTypeEtablissement>>(infoNivTpEtab,
               std::make_unique<GestionRestrictionCibleCode<NiveauTypeEtablissement,Restriction>>
                                                                 (bmps::cibleId::NiveauTypeEtablissement,get<Restriction>()),
               std::make_unique<UniqueNivTpEtab>()));
    setCible<NiveauTypeEtablissement>(bmps::cibleId::NiveauTypeEtablissement);

    //Note
    using UniqueNote = RelationUniqueSql<Note>;
    InfoBdd infoNote(Note::Name(),"Note",Note::NbrAtt,{UniqueNote::NbrUnique});
    infoNote.setAttribut(Note::IdControle,"idCtr");
    infoNote.setAttribut(Note::IdEleve,"idEl");
    infoNote.setAttribut(Note::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoNote.setAttribut(Note::Saisie,"ss");
    infoNote.setAttribut(Note::Valeur,"vl");
    infoNote.setUnique(Note::IdControle,UniqueNote::Id1Unique);
    infoNote.setUnique(Note::IdEleve,UniqueNote::Id2Unique);
    infoNote.setForeignKey(Note::IdControle,infoCtr);
    infoNote.setForeignKey(Note::IdEleve,infoEleve);
    setManager<Note>(std::make_unique<ManagerSql<Note>>(infoNote,std::make_unique<UniqueNote>()));
    setCible<Note>(bmps::cibleId::Note);

    //Valide
    using UniqueValide = RelationUniqueSql<Valide>;
    InfoBdd infoValide(Valide::Name(),"Valide",Valide::NbrAtt,{UniqueValide::NbrUnique});
    infoValide.setAttribut(Valide::IdNote,"idNt");
    infoValide.setAttribut(Valide::IdPoint,"idPt");
    infoValide.setAttribut(Valide::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoValide.setAttribut(Valide::Valeur,"vl");
    infoValide.setUnique(Valide::IdNote,UniqueValide::Id1Unique);
    infoValide.setUnique(Valide::IdPoint,UniqueValide::Id2Unique);
    infoValide.setForeignKey(Valide::IdNote,infoNote);
    infoValide.setForeignKey(Valide::IdPoint,infoPoint);
    setManager<Valide>(std::make_unique<ManagerSql<Valide>>(infoValide, std::make_unique<UniqueValide>()));
    setCible<Valide>(bmps::cibleId::Valide);
}
