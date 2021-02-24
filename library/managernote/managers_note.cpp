#include "managers_note.h"

using namespace note_mps;
using namespace mps::b2d;
using namespace mps::manager;

managers_note::managers_note()
    : managers_predef (info_entity_note::Nbr_Entity_Note,cible_id::Nbr_Cible,"note_version")
{
    enable_restriction("rest_modif");
    enable_type("type","perm_type",
               std::make_unique<gestion_restriction_cible_code<type,restriction>>(cible_id::Type,
                                                                                get<restriction>()),
               std::make_unique<gestion_restriction_cible_code<type_permission,restriction>>(cible_id::Type_Permission,
                                                                                          get<restriction>()));
    enable_commentaire("commentaire","cb_commentaire");
    enable_donnee("donnee","arb_donnee","cb_donnee","card_donnee",
                 std::make_unique<gestion_restriction_cible_code<donnee,restriction>>(cible_id::Donnee,
                                                                                    get<restriction>()),
                 std::make_unique<gestion_restriction_cible_code<donnee_cible,restriction>>(cible_id::Donnee_Cible,
                                                                                         get<restriction>()),
                 std::make_unique<gestion_restriction_cible_code<donnee_card,restriction>>(cible_id::Donnee_Card,
                                                                                        get<restriction>()));
    enable_evenement("evenement","evenement_cible","evenement_style");
    enable_mot_cle("mot_cle","arb_mot_cle","cb_mot_cle","cb_mot_prog","perm_mot_cle","perm_mot_prog",
                 std::make_unique<gestion_restriction_cible_code<mot_cle,restriction>>(cible_id::Mot_Cle,
                                                                                    get<restriction>()),
                 std::make_unique<gestion_restriction_cible_code<mot_cle_permission,restriction>>(cible_id::Mot_Cle_Permission,
                                                                                              get<restriction>()),
                 std::make_unique<gestion_restriction_cible_code<mot_prog_permission,restriction>>(cible_id::Mot_Prog_Permission,
                                                                                               get<restriction>()));
    enable_texte("texte","cb_texte","source","texte_source");
    enable_utilisation("utilisation","usage","arb_usage",
                      std::make_unique<gestion_restriction_cible_code<usage,restriction>>(cible_id::Usage,
                                                                                        get<restriction>()));
    
    //annee
    using unique_an = num_unique_sql<annee>;
    info_bdd info_an("annee","annee",annee::Nbr_Att,{unique_an::Nbr_Unique});
    info_an.set_attribut(annee::Num,"num");
    info_an.set_unique(annee::Num,unique_an::Num_Unique);
    set_manager<annee>(std::make_unique<manager_modif_controle<annee>>(info_an,
                std::make_unique<gestion_restriction_cible_code<annee,restriction>>(cible_id::Annee,
                                                                                              get<restriction>()),
                std::make_unique<unique_an>()));
    set_cible<annee>(cible_id::Annee);
    
    //bareme
    auto info_arb_bareme = info_bdd_arbre("arb_bareme");
    info_bdd info_bareme("bareme","bareme",bareme::Nbr_Att);
    info_bareme.set_attribut(bareme::Id_Cible,"id_cb");
    info_bareme.set_attribut(bareme::Cible,"cb");
    info_bareme.set_attribut(bareme::Type,"tp");
    info_bareme.set_attribut(bareme::Valeur,"vl",type_attribut_bdd::Double);
    info_bareme.set_foreign_key(bareme::Id,info_arb_bareme);
    set_type_foreign_key<bareme>(info_bareme);
    set_manager<bareme>(std::make_unique<manager_arbre<bareme>>(info_bareme,info_arb_bareme));
    set_cible<bareme>(cible_id::Bareme);

    //type_etablissement
    using unique_tp_etab = nom_unique_sql<type_etablissement>;
    info_bdd info_tp_etab("type_etablissement","tp_etab",type_etablissement::Nbr_Att,{unique_tp_etab::Nbr_Unique});
    info_tp_etab.set_attribut(type_etablissement::Nc,"nc",type_attribut_bdd::Text);
    info_tp_etab.set_attribut(type_etablissement::Nom,"nm",type_attribut_bdd::Text);
    info_tp_etab.set_unique(type_etablissement::Nom,unique_tp_etab::Nom_Unique);
    set_manager<type_etablissement>(std::make_unique<manager_modif_controle<type_etablissement>>(info_tp_etab,
               std::make_unique<gestion_restriction_cible_code<type_etablissement,restriction>>(cible_id::Type_Etablissement,
                                                                                             get<restriction>()),
               std::make_unique<unique_tp_etab>()));
    set_cible<type_etablissement>(cible_id::Type_Etablissement);

    //etablissement
    using unique_etab = nom_unique_sql<etablissement>;
    info_bdd info_etab("etablissement","etab",etablissement::Nbr_Att,{unique_etab::Nbr_Unique});
    info_etab.set_attribut(etablissement::Nc,"nc",type_attribut_bdd::Text);
    info_etab.set_attribut(etablissement::Nom,"nm",type_attribut_bdd::Text);
    info_etab.set_unique(etablissement::Nom,unique_etab::Nom_Unique);
    set_manager<etablissement>(std::make_unique<manager_modif_controle<etablissement>>(info_etab,
                std::make_unique<gestion_restriction_cible_code<etablissement,restriction>>(cible_id::Etablissement,
                                                                                              get<restriction>()),
                std::make_unique<unique_etab>()));
    set_cible<etablissement>(cible_id::Etablissement);

    //etablissementType
    using unique_etab_tp = relation_unique_sql<etablissement_type>;
    info_bdd info_etab_tp("etablissement_type","EtabTp",etablissement_type::Nbr_Att,{unique_etab_tp::Nbr_Unique});
    info_etab_tp.set_attribut(etablissement_type::Id_Etab,"id_etab");
    info_etab_tp.set_attribut(etablissement_type::Id_Tp_Etab,"id_tpEtab");
    info_etab_tp.set_unique(etablissement_type::Id_Etab,unique_etab_tp::Id_1_Unique);
    info_etab_tp.set_unique(etablissement_type::Id_Tp_Etab,unique_etab_tp::Id_2_Unique);
    info_etab_tp.set_foreign_key(etablissement_type::Id_Etab,info_etab);
    info_etab_tp.set_foreign_key(etablissement_type::Id_Tp_Etab,info_tp_etab);
    set_manager<etablissement_type>(std::make_unique<manager_sql<etablissement_type>>(info_etab_tp,
                                                                                      std::make_unique<unique_etab_tp>()));
    set_cible<etablissement_type>(cible_id::Etablissement_Type);

    //niveau
    using unique_niveau = nom_unique_sql<niveau>;
    info_bdd info_niveau("niveau","niveau",niveau::Nbr_Att,{unique_niveau::Nbr_Unique});
    info_niveau.set_attribut(niveau::Nc,"nc",type_attribut_bdd::Text);
    info_niveau.set_attribut(niveau::Nom,"nm",type_attribut_bdd::Text);
    info_niveau.set_attribut(niveau::Type,"tp");
    info_niveau.set_unique(niveau::Nom,unique_niveau::Nom_Unique);
    set_type_foreign_key<niveau>(info_niveau);
    set_manager<niveau>(std::make_unique<manager_modif_controle<niveau>>(info_niveau,
               std::make_unique<gestion_restriction_cible_code<niveau,restriction>>(cible_id::Niveau,
                                                                                  get<restriction>()),
               std::make_unique<unique_niveau>()));
    set_cible<niveau>(cible_id::Niveau);
    
    //classe
    using unique_classe = relation_trois_num_unique_sql<classe>;
    info_bdd info_classe(classe::Name(),"classe",classe::Nbr_Att,{unique_classe::Nbr_Unique});
    info_classe.set_attribut(classe::Id_An,"id_an");
    info_classe.set_attribut(classe::Id_Etab,"id_et");
    info_classe.set_attribut(classe::Id_Niveau,"id_niv");
    info_classe.set_attribut(classe::Nc,"nc",type_attribut_bdd::Text);
    info_classe.set_attribut(classe::Nom,"nm",type_attribut_bdd::Text);
    info_classe.set_attribut(classe::Num,"num");
    info_classe.set_unique(classe::Id_An,unique_classe::Id_1_Unique);
    info_classe.set_unique(classe::Id_Etab,unique_classe::Id_2_Unique);
    info_classe.set_unique(classe::Id_Niveau,unique_classe::Id_3_Unique);
    info_classe.set_unique(classe::Num,unique_classe::Num_Unique);
    info_classe.set_foreign_key(classe::Id_An,info_an);
    info_classe.set_foreign_key(classe::Id_Etab,info_etab);
    info_classe.set_foreign_key(classe::Id_Niveau,info_niveau);
    set_manager<classe>(std::make_unique<manager_modif_controle<classe>>(info_classe,
                std::make_unique<gestion_restriction_cible_code<classe,restriction>>(cible_id::Classe,
                                                                                    get<restriction>()),
                std::make_unique<unique_classe>()));
    set_cible<classe>(cible_id::Classe);

    //eleve
    info_bdd info_eleve(eleve::Name(),"eleve",eleve::Nbr_Att,{unique_eleve::Nbr_Unique});
    info_eleve.set_attribut(eleve::Date,"dt",type_attribut_bdd::Date);
    info_eleve.set_attribut(eleve::Fille,"fl",type_attribut_bdd::Bool);
    info_eleve.set_attribut(eleve::Nom,"nm",type_attribut_bdd::Text);
    info_eleve.set_attribut(eleve::Prenom,"pnm",type_attribut_bdd::Text);
    info_eleve.set_unique(eleve::Nom,unique_eleve::Nom_Unique);
    info_eleve.set_unique(eleve::Prenom,unique_eleve::Prenom_Unique);
    info_eleve.set_unique(eleve::Date,unique_eleve::Date_Unique);
    set_manager<eleve>(std::make_unique<manager_modif_controle<eleve>>(info_eleve,
                std::make_unique<gestion_restriction_cible_code<eleve,restriction>>(cible_id::Eleve,get<restriction>()),
                std::make_unique<unique_eleve>()));
    set_cible<eleve>(cible_id::Eleve);
    
    //classe_eleve
    using unique_cl_el = relation_unique_sql<classe_eleve>;
    info_bdd info_cl_el(classe_eleve::Name(),"classe_eleve",classe_eleve::Nbr_Att,{unique_cl_el::Nbr_Unique});
    info_cl_el.set_attribut(classe_eleve::Id_Classe,"id_cl");
    info_cl_el.set_attribut(classe_eleve::Id_Eleve,"id_el");
    info_cl_el.set_attribut(classe_eleve::Entree,"ent",type_attribut_bdd::Date);
    info_cl_el.set_attribut(classe_eleve::Sortie,"sort",type_attribut_bdd::Date);
    info_cl_el.set_unique(classe_eleve::Id_Classe,unique_cl_el::Id_1_Unique);
    info_cl_el.set_unique(classe_eleve::Id_Eleve,unique_cl_el::Id_2_Unique);
    info_cl_el.set_foreign_key(classe_eleve::Id_Classe,info_classe);
    info_cl_el.set_foreign_key(classe_eleve::Id_Eleve,info_eleve);
    set_manager<classe_eleve>(std::make_unique<manager_sql<classe_eleve>>(info_cl_el,std::make_unique<unique_cl_el>()));
    set_cible<classe_eleve>(cible_id::Classe_Eleve);

    //groupe
    using unique_groupe = nom_unique_sql<groupe>;
    info_bdd info_groupe(groupe::Name(),"groupe",groupe::Nbr_Att,{unique_groupe::Nbr_Unique});
    info_groupe.set_attribut(groupe::Id_An,"id_an",type_attribut_bdd::Integer,false);
    info_groupe.set_attribut(groupe::Id_Classe,"id_cl",type_attribut_bdd::Integer,false);
    info_groupe.set_attribut(groupe::Style_Num,"al");
    info_groupe.set_attribut(groupe::Code,"cd");
    info_groupe.set_attribut(groupe::Nc,"nc",type_attribut_bdd::Text);
    info_groupe.set_attribut(groupe::Nom,"nm",type_attribut_bdd::Text);
    info_groupe.set_attribut(groupe::Type,"tp");
    info_groupe.set_unique(groupe::Nom,unique_groupe::Nom_Unique);
    info_groupe.set_foreign_key(groupe::Id_An,info_an);
    info_groupe.set_foreign_key(groupe::Id_Classe,info_classe);
    set_type_foreign_key<groupe>(info_groupe);
    set_manager<groupe>(std::make_unique<manager_modif_controle<groupe>>(info_groupe,
              std::make_unique<gestion_restriction_cible_code<groupe,restriction>>(cible_id::Groupe,
                                                                                      get<restriction>()),
              std::make_unique<unique_groupe>()));
    set_cible<groupe>(cible_id::Groupe);

    //type_controle
    using unique_tp_ctr = nom_unique_sql<type_controle>;
    info_bdd info_tp_ctr(type_controle::Name(),"tp_controle",type_controle::Nbr_Att,{unique_tp_ctr::Nbr_Unique});
    info_tp_ctr.set_attribut(type_controle::Code,"cd");
    info_tp_ctr.set_attribut(type_controle::Decimale,"dcm");
    info_tp_ctr.set_attribut(type_controle::Minima,"min");
    info_tp_ctr.set_attribut(type_controle::Nc,"nc",type_attribut_bdd::Text);
    info_tp_ctr.set_attribut(type_controle::Nom,"nm",type_attribut_bdd::Text);
    info_tp_ctr.set_attribut(type_controle::Parent,"pt",type_attribut_bdd::Integer,false);
    info_tp_ctr.set_attribut(type_controle::Total,"tt");
    info_tp_ctr.set_unique(type_controle::Nom,unique_tp_ctr::Nom_Unique);
    info_tp_ctr.set_foreign_key(type_controle::Parent,info_tp_ctr);
    set_manager<type_controle>(std::make_unique<manager_arbre_simple_modif_controle<type_controle>>(info_tp_ctr,
                    std::make_unique<gestion_restriction_cible_code<type_controle,restriction>>(cible_id::Type_Controle,
                                                                                             get<restriction>()),
                    std::make_unique<unique_tp_ctr>()));
    set_cible<type_controle>(cible_id::Type_Controle);

    //controle
    using unique_ctr = nom_cible_unique_sql<controle>;
    info_bdd info_ctr(controle::Name(),"controle",controle::Nbr_Att,{unique_ctr::Nbr_Unique_1,unique_ctr::Nbr_Unique_2});
    info_ctr.set_attribut(controle::Id_Type,"id_tp");
    info_ctr.set_attribut(controle::Id_Cible,"id_cb");
    info_ctr.set_attribut(controle::Cible,"cb");
    info_ctr.set_attribut(controle::Code,"cd");
    info_ctr.set_attribut(controle::Date_Time,"dt",type_attribut_bdd::Date_Time);
    info_ctr.set_attribut(controle::Decimale,"dcm");
    info_ctr.set_attribut(controle::Minima,"min");
    info_ctr.set_attribut(controle::Nc,"nc",type_attribut_bdd::Text);
    info_ctr.set_attribut(controle::Nom,"nm",type_attribut_bdd::Text);
    info_ctr.set_attribut(controle::Num,"num");
    info_ctr.set_attribut(controle::Total,"tt");
    info_ctr.set_unique(controle::Nom,unique_ctr::Nom_Unique,unique_ctr::Nom_Unique_Set);
    info_ctr.set_unique(controle::Id_Type,unique_ctr::Id_1_Unique,unique_ctr::Id_1_Unique_Set);
    info_ctr.set_unique(controle::Id_Cible,unique_ctr::Id_Cible_Unique,unique_ctr::Id_Cible_Unique_Set);
    info_ctr.set_unique(controle::Cible,unique_ctr::Cible_Unique,unique_ctr::Cible_Unique_Set);
    info_ctr.set_unique(controle::Num,unique_ctr::Num_Unique,unique_ctr::Num_Unique_Set);
    info_ctr.set_foreign_key(controle::Id_Type,info_tp_ctr);
    set_manager<controle>(std::make_unique<manager_sql<controle>>(info_ctr,std::make_unique<unique_ctr>()));
    set_cible<controle>(cible_id::Controle);

    //enonce
    auto info_arb_enonce = info_bdd_arbre("arb_enonce");
    info_bdd info_enonce("enonce","enonce",enonce::Nbr_Att);
    info_enonce.set_attribut(enonce::Type,"tp");
    info_enonce.set_attribut(enonce::Version,"ver");
    info_enonce.set_foreign_key(enonce::Id,info_arb_enonce);
    set_type_foreign_key<enonce>(info_enonce);
    set_manager<enonce>(std::make_unique<manager_arbre<enonce>>(info_enonce,info_arb_enonce));
    set_cible<enonce>(cible_id::Enonce);

    //epreuve
    info_bdd info_epreuve("epreuve","epreuve",epreuve::Nbr_Att,{unique_epreuve::Nbr_Unique});
    info_epreuve.set_attribut(epreuve::Id_Bareme,"idBr");
    info_epreuve.set_attribut(epreuve::Id_Enonce,"id_en");
    info_epreuve.set_attribut(epreuve::Ens_Point,"ens_pt");
    info_epreuve.set_unique(epreuve::Id_Bareme,unique_epreuve::Id_1_Unique);
    info_epreuve.set_unique(epreuve::Id_Enonce,unique_epreuve::Id_2_Unique);
    info_epreuve.set_unique(epreuve::Ens_Point,unique_epreuve::Ens_Point_Unique);
    info_epreuve.set_foreign_key(epreuve::Id_Bareme,info_bareme);
    info_epreuve.set_foreign_key(epreuve::Id_Enonce,info_enonce);
    set_manager<epreuve>(std::make_unique<manager_sql<epreuve>>(info_epreuve, std::make_unique<unique_epreuve>()));
    set_cible<epreuve>(cible_id::Epreuve);

    //controle_epreuve
    info_bdd info_ctr_ep(controle_epreuve::Name(),"controle_epreuve",controle_epreuve::Nbr_Att,
        {unique_controle_epreuve::Nbr_Unique_1,unique_controle_epreuve::Nbr_Unique_2,unique_controle_epreuve::Nbr_Unique_3});
    info_ctr_ep.set_attribut(controle_epreuve::Id_Classe,"id_cl",type_attribut_bdd::Integer,false);
    info_ctr_ep.set_attribut(controle_epreuve::Id_Controle,"id_ctr");
    info_ctr_ep.set_attribut(controle_epreuve::Id_Eleve,"id_el",type_attribut_bdd::Integer,false);
    info_ctr_ep.set_attribut(controle_epreuve::Id_epreuve,"id_ep");
    info_ctr_ep.set_attribut(controle_epreuve::Id_Groupe,"id_gr",type_attribut_bdd::Integer,false);
    info_ctr_ep.set_attribut(controle_epreuve::Num,"num");
    info_ctr_ep.set_unique(controle_epreuve::Id_Classe,unique_controle_epreuve::Id_1_Unique,
                           unique_controle_epreuve::Id_1_Unique_Set);
    info_ctr_ep.set_unique(controle_epreuve::Id_Controle,unique_controle_epreuve::Id_4_Unique_1,
                           unique_controle_epreuve::Id_4_Unique_Set_1);
    info_ctr_ep.set_unique(controle_epreuve::Id_Groupe,unique_controle_epreuve::Id_2_Unique,
                           unique_controle_epreuve::Id_2_Unique_Set);
    info_ctr_ep.set_unique(controle_epreuve::Id_Controle,unique_controle_epreuve::Id_4_Unique_2,
                           unique_controle_epreuve::Id_4_Unique_Set_2);
    info_ctr_ep.set_unique(controle_epreuve::Num,unique_controle_epreuve::Num_Unique,
                           unique_controle_epreuve::Num_Unique_Set);
    info_ctr_ep.set_unique(controle_epreuve::Id_Eleve,unique_controle_epreuve::Id_3_Unique,
                           unique_controle_epreuve::Id_3_Unique_Set);
    info_ctr_ep.set_unique(controle_epreuve::Id_Controle,unique_controle_epreuve::Id_4_Unique_3,
                           unique_controle_epreuve::Id_4_Unique_Set_3);
    info_ctr_ep.set_foreign_key(controle_epreuve::Id_Classe,info_classe);
    info_ctr_ep.set_foreign_key(controle_epreuve::Id_Controle,info_ctr);
    info_ctr_ep.set_foreign_key(controle_epreuve::Id_Eleve,info_eleve);
    info_ctr_ep.set_foreign_key(controle_epreuve::Id_epreuve,info_epreuve);
    info_ctr_ep.set_foreign_key(controle_epreuve::Id_Groupe,info_groupe);
    set_manager<controle_epreuve>(std::make_unique<manager_sql<controle_epreuve>>(info_ctr_ep,
                                                                                  std::make_unique<unique_controle_epreuve>()));
    set_cible<controle_epreuve>(cible_id::Controle_Epreuve);

    //eleve_groupe
    using unique_el_gr = relation_num_unique_sql<eleve_groupe>;
    info_bdd info_el_gr(eleve_groupe::Name(),"eleve_groupe",eleve_groupe::Nbr_Att,{unique_el_gr::Nbr_Unique});
    info_el_gr.set_attribut(eleve_groupe::Id_Eleve,"id_el");
    info_el_gr.set_attribut(eleve_groupe::Id_Groupe,"id_gr");
    info_el_gr.set_attribut(eleve_groupe::Num,"num");
    info_el_gr.set_unique(eleve_groupe::Id_Eleve,unique_el_gr::Id_1_Unique);
    info_el_gr.set_unique(eleve_groupe::Id_Groupe,unique_el_gr::Id_2_Unique);
    info_el_gr.set_unique(eleve_groupe::Num,unique_el_gr::Num_Unique);
    info_el_gr.set_foreign_key(eleve_groupe::Id_Eleve,info_eleve);
    info_el_gr.set_foreign_key(eleve_groupe::Id_Groupe,info_groupe);
    set_manager<eleve_groupe>(std::make_unique<manager_sql<eleve_groupe>>(info_el_gr, std::make_unique<unique_el_gr>()));
    set_cible<eleve_groupe>(cible_id::Eleve_Groupe);

    //point
    info_bdd info_point("point","point",point::Nbr_Att);
    info_point.set_attribut(point::Type,"tp");
    info_point.set_attribut(point::Valeur,"vl");
    info_point.set_attribut(point::Version,"ver");
    set_type_foreign_key<point>(info_point);
    set_manager<point>(std::make_unique<manager_sql<point>>(info_point));
    set_cible<point>(cible_id::Point);

    //enonce_point
    info_bdd info_en_pt(enonce_point::Name(),"enonce_point",enonce_point::Nbr_Att,{unique_enonce_point::Nbr_Unique});
    info_en_pt.set_attribut(enonce_point::Id_Enonce,"id_en");
    info_en_pt.set_attribut(enonce_point::Id_Point,"id_pt");
    info_en_pt.set_attribut(enonce_point::Ens_Point,"ens_pt");
    info_en_pt.set_attribut(enonce_point::Num,"num");
    info_en_pt.set_unique(enonce_point::Id_Enonce,unique_enonce_point::Id_1_Unique);
    info_en_pt.set_unique(enonce_point::Ens_Point,unique_enonce_point::Ens_Point_Unique);
    info_en_pt.set_unique(enonce_point::Num,unique_enonce_point::Num_Unique);
    info_en_pt.set_foreign_key(enonce_point::Id_Enonce,info_enonce);
    info_en_pt.set_foreign_key(enonce_point::Id_Point,info_point);
    set_manager<enonce_point>(std::make_unique<manager_sql<enonce_point>>(info_en_pt,std::make_unique<unique_enonce_point>()));
    set_cible<enonce_point>(cible_id::Enonce_Point);

    //etablissement_niveau
    using unique_etab_niv = relation_unique_sql<etablissement_niveau>;
    info_bdd info_etab_niv(etablissement_niveau::Name(),"etab_niveau",etablissement_niveau::Nbr_Att,{unique_etab_niv::Nbr_Unique});
    info_etab_niv.set_attribut(etablissement_niveau::Id_Etab,"id_et");
    info_etab_niv.set_attribut(etablissement_niveau::Id_Niveau,"id_niv");
    info_etab_niv.set_foreign_key(etablissement_niveau::Id_Etab,info_etab);
    info_etab_niv.set_foreign_key(etablissement_niveau::Id_Niveau,info_niveau);
    info_etab_niv.set_unique(etablissement_niveau::Id_Etab,unique_etab_niv::Id_1_Unique);
    info_etab_niv.set_unique(etablissement_niveau::Id_Niveau,unique_etab_niv::Id_2_Unique);
    set_manager<etablissement_niveau>(std::make_unique<manager_sql<etablissement_niveau>>(info_etab_niv,
                                                                                          std::make_unique<unique_etab_niv>()));
    set_cible<etablissement_niveau>(cible_id::Etablissement_Niveau);

    // exercice
    auto info_arb_exo = info_bdd_arbre("arb_exercice");
    info_bdd info_exo(exercice::Name(),"exercice",exercice::Nbr_Att);
    info_exo.set_attribut(exercice::Id_Original,"id_orig",type_attribut_bdd::Integer,false);
    info_exo.set_attribut(exercice::Type,"tp");
    info_exo.set_attribut(exercice::Version,"ver");
    info_exo.set_foreign_key(exercice::Id,info_arb_exo);
    info_exo.set_foreign_key(exercice::Id_Original,info_exo);
    set_type_foreign_key<exercice>(info_exo);
    set_manager<exercice>(std::make_unique<manager_arbre<exercice>>(info_exo,info_arb_exo));
    set_cible<exercice>(cible_id::Exercice);

    //filiation_niveau
    using unique_fil_niv = relation_unique_sql<filiation_niveau>;
    info_bdd info_fil_niv(filiation_niveau::Name(),"filiation_niveau",filiation_niveau::Nbr_Att,{unique_fil_niv::Nbr_Unique});
    info_fil_niv.set_attribut(filiation_niveau::Id_Precedent,"idP");
    info_fil_niv.set_attribut(filiation_niveau::Id_Suivant,"idS");
    info_fil_niv.set_foreign_key(filiation_niveau::Id_Precedent,info_niveau);
    info_fil_niv.set_foreign_key(filiation_niveau::Id_Suivant,info_niveau);
    info_fil_niv.set_unique(filiation_niveau::Id_Precedent,unique_fil_niv::Id_1_Unique);
    info_fil_niv.set_unique(filiation_niveau::Id_Suivant,unique_fil_niv::Id_2_Unique);
    set_manager<filiation_niveau>(std::make_unique<manager_modif_controle<filiation_niveau>>(info_fil_niv,
                   std::make_unique<gestion_restriction_cible_code<filiation_niveau,restriction>>(cible_id::Filiation_Niveau,
                                                                                               get<restriction>()),
                   std::make_unique<unique_fil_niv>()));
    set_cible<filiation_niveau>(cible_id::Filiation_Niveau);

    //niveau_type_etablissement
    using unique_niv_tp_etab = relation_unique_sql<niveau_type_etablissement>;
    info_bdd info_niv_tp_etab(niveau_type_etablissement::Name(),"niveau_tp_etab",niveau_type_etablissement::Nbr_Att,{unique_niv_tp_etab::Nbr_Unique});
    info_niv_tp_etab.set_attribut(niveau_type_etablissement::Id_Niveau,"id_niv");
    info_niv_tp_etab.set_attribut(niveau_type_etablissement::Id_Tp_Etab,"id_te");
    info_niv_tp_etab.set_foreign_key(niveau_type_etablissement::Id_Niveau,info_niveau);
    info_niv_tp_etab.set_foreign_key(niveau_type_etablissement::Id_Tp_Etab,info_tp_etab);
    info_niv_tp_etab.set_unique(niveau_type_etablissement::Id_Niveau,unique_niv_tp_etab::Id_1_Unique);
    info_niv_tp_etab.set_unique(niveau_type_etablissement::Id_Tp_Etab,unique_niv_tp_etab::Id_2_Unique);
    set_manager<niveau_type_etablissement>(std::make_unique<manager_modif_controle<niveau_type_etablissement>>(info_niv_tp_etab,
               std::make_unique<gestion_restriction_cible_code<niveau_type_etablissement,restriction>>
                                                                 (cible_id::Niveau_Type_Etablissement,get<restriction>()),
               std::make_unique<unique_niv_tp_etab>()));
    set_cible<niveau_type_etablissement>(cible_id::Niveau_Type_Etablissement);

    //note
    using unique_note = relation_unique_sql<note>;
    info_bdd info_note(note::Name(),"note",note::Nbr_Att,{unique_note::Nbr_Unique});
    info_note.set_attribut(note::Id_Controle,"id_ctr");
    info_note.set_attribut(note::Id_Eleve,"id_el");
    info_note.set_attribut(note::Date_Time,"dt",type_attribut_bdd::Date_Time);
    info_note.set_attribut(note::Saisie,"ss");
    info_note.set_attribut(note::Valeur,"vl");
    info_note.set_unique(note::Id_Controle,unique_note::Id_1_Unique);
    info_note.set_unique(note::Id_Eleve,unique_note::Id_2_Unique);
    info_note.set_foreign_key(note::Id_Controle,info_ctr);
    info_note.set_foreign_key(note::Id_Eleve,info_eleve);
    set_manager<note>(std::make_unique<manager_sql<note>>(info_note,std::make_unique<unique_note>()));
    set_cible<note>(cible_id::Note);

    //valide
    using unique_valide = relation_unique_sql<note_mps::valide>;
    info_bdd info_valide(valide::Name(),"valide",valide::Nbr_Att,{unique_valide::Nbr_Unique});
    info_valide.set_attribut(valide::Id_Note,"idNt");
    info_valide.set_attribut(valide::Id_Point,"id_pt");
    info_valide.set_attribut(valide::Date_Time,"dt",type_attribut_bdd::Date_Time);
    info_valide.set_attribut(valide::Valeur,"vl");
    info_valide.set_unique(valide::Id_Note,unique_valide::Id_1_Unique);
    info_valide.set_unique(valide::Id_Point,unique_valide::Id_2_Unique);
    info_valide.set_foreign_key(valide::Id_Note,info_note);
    info_valide.set_foreign_key(valide::Id_Point,info_point);
    set_manager<note_mps::valide>(std::make_unique<manager_sql<note_mps::valide>>(info_valide, std::make_unique<unique_valide>()));
    set_cible<note_mps::valide>(cible_id::Valide);
}
