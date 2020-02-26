#include "ManagersNote.h"

using namespace noteMPS;
using namespace managerMPS;

ManagersNote::ManagersNote()
    : ManagersPredef (infoEntityNote::NbrEntityId,bmps::cibleIdNote::NbrCible,"NoteVersion")
{
    enableRestriction("RestModif");
    enableType("Type","PermType",
               std::make_unique<GestionAutorisationCibleCode<Type,Restriction>>(bmps::cibleId::Type,
                                                                                get<Restriction>()),
               std::make_unique<GestionAutorisationCibleCode<TypePermission,Restriction>>(bmps::cibleId::TypePermission,
                                                                                          get<Restriction>()));
    enableCommentaire("Commentaire","CbCommentaire");
    enableDonnee("Donnee","ArbDonnee","CbDonnee","CardDonnee",
                 std::make_unique<GestionAutorisationCibleCode<Donnee,Restriction>>(bmps::cibleId::Donnee,
                                                                                    get<Restriction>()),
                 std::make_unique<GestionAutorisationCibleCode<DonneeCible,Restriction>>(bmps::cibleId::DonneeCible,
                                                                                         get<Restriction>()),
                 std::make_unique<GestionAutorisationCibleCode<DonneeCard,Restriction>>(bmps::cibleId::DonneeCard,
                                                                                        get<Restriction>()));
    enableMotCle("MotCle","ArbMotCle","CbMotCle","CbMotProg","PermMotCle","PermMotProg",
                 std::make_unique<GestionAutorisationCibleCode<MotCle,Restriction>>(bmps::cibleId::MotCle,
                                                                                    get<Restriction>()),
                 std::make_unique<GestionAutorisationCibleCode<MotClePermission,Restriction>>(bmps::cibleId::MotClePermission,
                                                                                              get<Restriction>()),
                 std::make_unique<GestionAutorisationCibleCode<MotProgPermission,Restriction>>(bmps::cibleId::MotProgPermission,
                                                                                               get<Restriction>()));
    enableTexte("Texte","CbTexte","Source","TexteSource");
    enableUtilisation("Utilisation","Usage","ArbUsage",
                      std::make_unique<GestionAutorisationCibleCode<Usage,Restriction>>(bmps::cibleId::Usage,
                                                                                        get<Restriction>()));
    
    //Annee
    using UniqueAn = NumUniqueSql<Annee>;
    InfoBdd infoAn("Annee","Annee",Annee::NbrAtt,{UniqueAn::NbrUnique});
    infoAn.setAttribut(Annee::Num,"num");
    infoAn.setUnique(Annee::Num,UniqueAn::NumUnique);
    setManager<Annee>(std::make_unique<ManagerSql<Annee>>(infoAn, std::make_unique<UniqueAn>()));
    setCible<Annee>(bmps::cibleIdNote::Annee);
    
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
    setCible<Bareme>(bmps::cibleIdNote::Bareme);

    //TypeEtablissement
    using UniqueTpEtab = NomUniqueSql<TypeEtablissement>;
    InfoBdd infoTpEtab("TypeEtablissement","TpEtab",TypeEtablissement::NbrAtt,{UniqueTpEtab::NbrUnique});
    infoTpEtab.setAttribut(TypeEtablissement::Nc,"nc",bmps::typeAttributBdd::Text);
    infoTpEtab.setAttribut(TypeEtablissement::Nom,"nm",bmps::typeAttributBdd::Text);
    infoTpEtab.setUnique(TypeEtablissement::Nom,UniqueTpEtab::NomUnique);
    setManager<TypeEtablissement>(std::make_unique<ManagerModifControle<TypeEtablissement>>(infoTpEtab,
               std::make_unique<GestionAutorisationCibleCode<TypeEtablissement,Restriction>>(bmps::cibleIdNote::TypeEtablissement,
                                                                                             get<Restriction>()),
               std::make_unique<UniqueTpEtab>()));
    setCible<TypeEtablissement>(bmps::cibleIdNote::TypeEtablissement);

    //Etablissement
    using UniqueEtab = NomUniqueSql<Etablissement>;
    InfoBdd infoEtab("Etablissement","Etab",Etablissement::NbrAtt,{UniqueEtab::NbrUnique});
    infoEtab.setAttribut(Etablissement::Nc,"nc",bmps::typeAttributBdd::Text);
    infoEtab.setAttribut(Etablissement::Nom,"nm",bmps::typeAttributBdd::Text);
    infoEtab.setUnique(Etablissement::Nom,UniqueEtab::NomUnique);
    setManager<Etablissement>(std::make_unique<ManagerSql<Etablissement>>(infoEtab, std::make_unique<UniqueEtab>()));
    setCible<Etablissement>(bmps::cibleIdNote::Etablissement);

    //EtablissementType
    using UniqueEtabTp = RelationUniqueSql<EtablissementType>;
    InfoBdd infoEtabTp("EtablissementType","EtabTp",EtablissementType::NbrAtt,{UniqueEtabTp::NbrUnique});
    infoEtabTp.setAttribut(EtablissementType::IdEtab,"idEtab");
    infoEtabTp.setAttribut(EtablissementType::IdTpEtab,"idTpEtab");
    infoEtabTp.setUnique(EtablissementType::IdEtab,UniqueEtabTp::Id1Unique);
    infoEtabTp.setUnique(EtablissementType::IdTpEtab,UniqueEtabTp::Id2Unique);
    setManager<EtablissementType>(std::make_unique<ManagerSql<EtablissementType>>(infoEtabTp, std::make_unique<UniqueEtabTp>()));
    setCible<EtablissementType>(bmps::cibleIdNote::EtablissementType);

    //Niveau
    using UniqueNiveau = NomUniqueSql<Niveau>;
    InfoBdd infoNiveau("Niveau","Niveau",Niveau::NbrAtt,{UniqueNiveau::NbrUnique});
    infoNiveau.setAttribut(Niveau::Nc,"nc",bmps::typeAttributBdd::Text);
    infoNiveau.setAttribut(Niveau::Nom,"nm",bmps::typeAttributBdd::Text);
    infoNiveau.setAttribut(Niveau::Type,"tp");
    infoNiveau.setUnique(Niveau::Nom,UniqueNiveau::NomUnique);
    setTypeForeignKey<Niveau>(infoNiveau);
    setManager<Niveau>(std::make_unique<ManagerModifControle<Niveau>>(infoNiveau,
               std::make_unique<GestionAutorisationCibleCode<Niveau,Restriction>>(bmps::cibleIdNote::Niveau,
                                                                                  get<Restriction>()),
               std::make_unique<UniqueNiveau>()));
    setCible<Niveau>(bmps::cibleIdNote::Niveau);
    
    //Classe
    using UniqueClasse = RelationTroisNumUniqueSql<Classe>;
    InfoBdd infoClasse(Classe::Name(),"Classe",Classe::NbrAtt,{UniqueClasse::NbrUnique});
    infoClasse.setAttribut(Classe::IdAn,"idAn");
    infoClasse.setAttribut(Classe::IdEtab,"idEt");
    infoClasse.setAttribut(Classe::IdNiveau,"idNiv");
    infoClasse.setAttribut(Classe::Num,"num");
    infoClasse.setUnique(Classe::IdAn,UniqueClasse::Id1Unique);
    infoClasse.setUnique(Classe::IdEtab,UniqueClasse::Id2Unique);
    infoClasse.setUnique(Classe::IdNiveau,UniqueClasse::Id3Unique);
    infoClasse.setUnique(Classe::Num,UniqueClasse::NumUnique);
    infoClasse.setForeignKey(Classe::IdAn,infoAn);
    infoClasse.setForeignKey(Classe::IdEtab,infoEtab);
    infoClasse.setForeignKey(Classe::IdNiveau,infoNiveau);
    setManager<Classe>(std::make_unique<ManagerSql<Classe>>(infoClasse,std::make_unique<UniqueClasse>()));
    setCible<Classe>(bmps::cibleIdNote::Classe);

    //Eleve
    InfoBdd infoEleve(Eleve::Name(),"Eleve",Eleve::NbrAtt,{UniqueEleve::NbrUnique});
    infoEleve.setAttribut(Eleve::Date,"dt",bmps::typeAttributBdd::Date);
    infoEleve.setAttribut(Eleve::Fille,"fl",bmps::typeAttributBdd::Bool);
    infoEleve.setAttribut(Eleve::Nom,"nm",bmps::typeAttributBdd::Text);
    infoEleve.setAttribut(Eleve::Prenom,"pnm",bmps::typeAttributBdd::Text);
    infoEleve.setUnique(Eleve::Nom,UniqueEleve::NomUnique);
    infoEleve.setUnique(Eleve::Prenom,UniqueEleve::PrenomUnique);
    infoEleve.setUnique(Eleve::Date,UniqueEleve::DateUnique);
    setManager<Eleve>(std::make_unique<ManagerSql<Eleve>>(infoEleve, std::make_unique<UniqueEleve>()));
    setCible<Eleve>(bmps::cibleIdNote::Eleve);
    
    //ClasseEleve
    using UniqueClEl = RelationUniqueSql<ClasseEleve>;
    InfoBdd infoClEl(ClasseEleve::Name(),"ClasseEleve",ClasseEleve::NbrAtt,{UniqueClEl::NbrUnique});
    infoClEl.setAttribut(ClasseEleve::IdClasse,"IdCl");
    infoClEl.setAttribut(ClasseEleve::IdEleve,"IdEl");
    infoClEl.setAttribut(ClasseEleve::Entree,"ent",bmps::typeAttributBdd::Date);
    infoClEl.setAttribut(ClasseEleve::Sortie,"sort",bmps::typeAttributBdd::Date,false);
    infoClEl.setUnique(ClasseEleve::IdClasse,UniqueClEl::Id1Unique);
    infoClEl.setUnique(ClasseEleve::IdEleve,UniqueClEl::Id2Unique);
    infoClEl.setForeignKey(ClasseEleve::IdClasse,infoClasse);
    infoClEl.setForeignKey(ClasseEleve::IdEleve,infoEleve);
    setManager<ClasseEleve>(std::make_unique<ManagerSql<ClasseEleve>>(infoClEl,std::make_unique<UniqueClEl>()));
    setCible<ClasseEleve>(bmps::cibleIdNote::ClasseEleve);

    //Groupe
    using UniqueGroupe = NomUniqueSql<Groupe>;
    InfoBdd infoGroupe(Groupe::Name(),"Groupe",Groupe::NbrAtt,{UniqueGroupe::NbrUnique});
    infoGroupe.setAttribut(Groupe::Alpha,"al");
    infoGroupe.setAttribut(Groupe::Nc,"nc",bmps::typeAttributBdd::Text);
    infoGroupe.setAttribut(Groupe::Nom,"nm",bmps::typeAttributBdd::Text);
    infoGroupe.setAttribut(Groupe::Type,"tp");
    infoGroupe.setUnique(Groupe::Nom,UniqueGroupe::NomUnique);
    setTypeForeignKey<Groupe>(infoGroupe);
    setManager<Groupe>(std::make_unique<ManagerSql<Groupe>>(infoGroupe, std::make_unique<UniqueGroupe>()));
    setCible<Groupe>(bmps::cibleIdNote::Groupe);

    //ClasseGroupe
    using UniqueClGr = IdRelationExactOneNotNullUniqueSql<ClasseGroupe>;
    InfoBdd infoClGr(ClasseGroupe::Name(),"ClasseGroupe",ClasseGroupe::NbrAtt,{UniqueClGr::NbrUnique_1,UniqueClGr::NbrUnique_2});
    infoClGr.setAttribut(ClasseGroupe::IdAn,"idAn",bmps::typeAttributBdd::Integer,false);
    infoClGr.setAttribut(ClasseGroupe::IdClasse,"idCl",bmps::typeAttributBdd::Integer,false);
    infoClGr.setAttribut(ClasseGroupe::IdGroupe,"idGr");
    infoClGr.setUnique(ClasseGroupe::IdAn,UniqueClGr::Id1Unique,UniqueClGr::Id1UniqueSet);
    infoClGr.setUnique(ClasseGroupe::IdGroupe,UniqueClGr::Id3Unique,UniqueClGr::UniqueSet1);
    infoClGr.setUnique(ClasseGroupe::IdClasse,UniqueClGr::Id2Unique,UniqueClGr::Id2UniqueSet);
    infoClGr.setUnique(ClasseGroupe::IdGroupe,UniqueClGr::Id3Unique,UniqueClGr::UniqueSet2);
    infoClGr.setForeignKey(ClasseGroupe::IdAn,infoAn);
    infoClGr.setForeignKey(ClasseGroupe::IdClasse,infoClasse);
    infoClGr.setForeignKey(ClasseGroupe::IdGroupe,infoGroupe);
    setManager<ClasseGroupe>(std::make_unique<ManagerSql<ClasseGroupe>>(infoClGr, std::make_unique<UniqueClGr>()));
    setCible<ClasseGroupe>(bmps::cibleIdNote::ClasseGroupe);

    //TypeControle
    using UniqueTpCtr = NomUniqueSql<TypeControle>;
    InfoBdd infoTpCtr(TypeControle::Name(),"TpControle",TypeControle::NbrAtt,{UniqueTpCtr::NbrUnique});
    infoTpCtr.setAttribut(TypeControle::Appreciation,"app");
    infoTpCtr.setAttribut(TypeControle::Code,"cd");
    infoTpCtr.setAttribut(TypeControle::Decimale,"dcm");
    infoTpCtr.setAttribut(TypeControle::Minima,"min");
    infoTpCtr.setAttribut(TypeControle::Modif,"md");
    infoTpCtr.setAttribut(TypeControle::Nc,"nc",bmps::typeAttributBdd::Text);
    infoTpCtr.setAttribut(TypeControle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoTpCtr.setAttribut(TypeControle::Parent,"pt",bmps::typeAttributBdd::Integer,false);
    infoTpCtr.setAttribut(TypeControle::Total,"tt");
    infoTpCtr.setUnique(TypeControle::Nom,UniqueTpCtr::NomUnique);
    setManager<TypeControle>(std::make_unique<ManagerArbreSimpleModifControle<TypeControle>>(infoTpCtr,
                    std::make_unique<GestionAutorisationCibleCode<TypeControle,Restriction>>(bmps::cibleIdNote::TypeControle,
                                                                                             get<Restriction>()),
                    std::make_unique<UniqueTpCtr>()));
    setCible<TypeControle>(bmps::cibleIdNote::TypeControle);

    //Controle
    using UniqueCtr = NomUniqueSql<Controle>;
    InfoBdd infoCtr(Controle::Name(),"Controle",Controle::NbrAtt,{UniqueCtr::NbrUnique});
    infoCtr.setAttribut(Controle::IdType,"idTp");
    infoCtr.setAttribut(Controle::Code,"cd");
    infoCtr.setAttribut(Controle::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoCtr.setAttribut(Controle::Decimale,"dcm");
    infoCtr.setAttribut(Controle::Minima,"min");
    infoCtr.setAttribut(Controle::Nc,"nc",bmps::typeAttributBdd::Text);
    infoCtr.setAttribut(Controle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoCtr.setAttribut(Controle::Num,"num");
    infoCtr.setAttribut(Controle::Total,"tt");
    infoTpCtr.setUnique(TypeControle::Nom,UniqueTpCtr::NomUnique);
    infoCtr.setForeignKey(Controle::IdType,infoTpCtr);
    setManager<Controle>(std::make_unique<ManagerSql<Controle>>(infoCtr,std::make_unique<UniqueCtr>()));
    setCible<Controle>(bmps::cibleIdNote::Controle);

    //Enonce
    auto infoArbEnonce = infoBddArbre("ArbEnonce");
    InfoBdd infoEnonce("Enonce","Enonce",Enonce::NbrAtt);
    infoEnonce.setAttribut(Enonce::Type,"tp");
    infoEnonce.setAttribut(Enonce::Version,"ver");
    infoEnonce.setForeignKey(Enonce::Id,infoArbEnonce);
    setTypeForeignKey<Enonce>(infoEnonce);
    setManager<Enonce>(std::make_unique<ManagerArbre<Enonce>>(infoEnonce,infoArbEnonce));
    setCible<Enonce>(bmps::cibleIdNote::Enonce);

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
    setCible<Epreuve>(bmps::cibleIdNote::Epreuve);

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
    infoCtrEp.setUnique(ControleEpreuve::IdControle,UniqueControleEpreuve::Id4Unique_3,UniqueControleEpreuve::Id4Unique_3Set);
    infoCtrEp.setForeignKey(ControleEpreuve::IdClasse,infoClasse);
    infoCtrEp.setForeignKey(ControleEpreuve::IdControle,infoCtr);
    infoCtrEp.setForeignKey(ControleEpreuve::IdEleve,infoEleve);
    infoCtrEp.setForeignKey(ControleEpreuve::IdEpreuve,infoEpreuve);
    infoCtrEp.setForeignKey(ControleEpreuve::IdGroupe,infoGroupe);
    setManager<ControleEpreuve>(std::make_unique<ManagerSql<ControleEpreuve>>(infoCtrEp, std::make_unique<UniqueControleEpreuve>()));
    setCible<ControleEpreuve>(bmps::cibleIdNote::ControleEpreuve);

    //EleveGroupe
    using UniqueElGr = RelationUniqueSql<EleveGroupe>;
    InfoBdd infoElGr(EleveGroupe::Name(),"EleveGroupe",EleveGroupe::NbrAtt,{UniqueElGr::NbrUnique});
    infoElGr.setAttribut(EleveGroupe::IdEleve,"idEl");
    infoElGr.setAttribut(EleveGroupe::IdGroupe,"idGr");
    infoElGr.setAttribut(EleveGroupe::Num,"num");
    infoElGr.setUnique(EleveGroupe::IdEleve,UniqueElGr::Id1Unique);
    infoElGr.setUnique(EleveGroupe::IdGroupe,UniqueElGr::Id2Unique);
    infoElGr.setForeignKey(EleveGroupe::IdEleve,infoEleve);
    infoElGr.setForeignKey(EleveGroupe::IdGroupe,infoGroupe);
    setManager<EleveGroupe>(std::make_unique<ManagerSql<EleveGroupe>>(infoElGr, std::make_unique<UniqueElGr>()));
    setCible<EleveGroupe>(bmps::cibleIdNote::EleveGroupe);

    //Point
    InfoBdd infoPoint("Point","Point",Point::NbrAtt);
    infoPoint.setAttribut(Point::Type,"tp");
    infoPoint.setAttribut(Point::Valeur,"vl");
    infoPoint.setAttribut(Point::Version,"ver");
    setTypeForeignKey<Point>(infoPoint);
    setManager<Point>(std::make_unique<ManagerSql<Point>>(infoPoint));
    setCible<Point>(bmps::cibleIdNote::Point);

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
    setCible<EnoncePoint>(bmps::cibleIdNote::EnoncePoint);

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
    setCible<EtablissementNiveau>(bmps::cibleIdNote::EtablissementNiveau);

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
                   std::make_unique<GestionAutorisationCibleCode<FiliationNiveau,Restriction>>(bmps::cibleIdNote::FiliationNiveau,
                                                                                               get<Restriction>()),
                   std::make_unique<UniqueFilNiv>()));
    setCible<FiliationNiveau>(bmps::cibleIdNote::FiliationNiveau);

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
               std::make_unique<GestionAutorisationCibleCode<NiveauTypeEtablissement,Restriction>>
                                                                 (bmps::cibleIdNote::NiveauTypeEtablissement,get<Restriction>()),
               std::make_unique<UniqueNivTpEtab>()));
    setCible<NiveauTypeEtablissement>(bmps::cibleIdNote::NiveauTypeEtablissement);

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
    setCible<Note>(bmps::cibleIdNote::Note);

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
    setCible<Valide>(bmps::cibleIdNote::Valide);
}
