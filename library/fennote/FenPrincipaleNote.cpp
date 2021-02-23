#include "fen_principaleNote.h"

using namespace noteMPS;

fen_principaleNote::fen_principaleNote(NoyauNote * noyau, std::unique_ptr<BddNote> bdd, ZoneCentraleNote * central_zone,
                                   const QString &bdd_path_xml, const QString & config_path, QWidget *parent)
    : predef_fen_principale (noyau,std::move(bdd),central_zone,bdd_path_xml,config_path,parent) {
    create_menuNewModif();
//    QDockWidget *dockWidget = new QDockWidget(tr("Rechecher"), this);
//    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//    auto * find = new find_widget();
//    dockWidget->setWidget(find);
//    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

//    QDockWidget *dockWidget2 = new QDockWidget(tr("Dock Widget 2"), this);
//    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
//                                      Qt::RightDockWidgetArea);
//    QLabel * label2 = new QLabel("Dock test2");
//    dockWidget2->setWidget(label2);
//    addDockWidget(Qt::RightDockWidgetArea, dockWidget2);
}

void fen_principaleNote::create_menuNewModif() {
    // Année
    auto * anMenu = m_new_modif_menu->addMenu(tr("Année scolaire"));
    connect(anMenu->addAction(tr("Crée une année")),&QAction::triggered,this,[this](){
        auto * form = new AnneeNewModifForm(noyau()->bdd(), true);
        dialogue::new_modif_dialog diag(form,this);
        if(diag.exec() && form->anneeCourante()) {
            auto an = form->annee();
            noyau()->bdd().get_unique(an);
            noyau()->setAnnee(an);
        }
    });
    connect(anMenu->addAction(tr("Choix de l'année courante")),&QAction::triggered,this,[this](){
        auto * form = new AnneeNewModifForm(noyau()->bdd(), false);
        dialogue::new_modif_dialog diag(form,this);
        if(diag.exec())
            noyau()->setAnnee(form->annee());
    });
    // Calendrier
    auto calMenu = m_new_modif_menu->addMenu(tr("Calendrier"));
    auto eveMenu = calMenu->addMenu(tr("Événement"));
    connect_action_to_new_modif_dialog<dialogue::evenement_new_modif_form>(eveMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<dialogue::evenement_new_modif_form>(eveMenu->addAction(tr("Modifier")),false);
    auto styleMenu = calMenu->addMenu(tr("Style"));
    connect_action_to_new_modif_dialog<dialogue::evenement_style_new_modif_form>(styleMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<dialogue::evenement_style_new_modif_form>(styleMenu->addAction(tr("Modifier")),false);
    //Classe
    auto * classeMenu = m_new_modif_menu->addMenu(tr("Classe"));
    connect_action_to_new_modif_dialog<ClasseNewModifForm>(classeMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<ClasseNewModifForm>(classeMenu->addAction(tr("Modifier")),false);
    connect_action_to_ppen_tab(classeMenu->addAction(tr("Liste des éléves")),{TabNote::ClasseEleveTabId,0});
    //Controle
    auto * ctrl= m_new_modif_menu->addMenu(tr("Controle"));
    connect_action_to_new_modif_dialog<ControleNewModifForm>(ctrl->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<ControleNewModifForm>(ctrl->addAction(tr("Modifier")),false);
    // Eleve
    connect_action_to_ppen_tab(m_new_modif_menu->addAction(tr("Eleve")),{TabNote::ElevesTabId,fenetre::abstract_tab_module::No_Id});
    // Établissement
    auto * etabMenu = m_new_modif_menu->addMenu(tr("Établissement"));
    connect_action_to_new_modif_dialog<EtablissementNewModifForm>(etabMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<EtablissementNewModifForm>(etabMenu->addAction(tr("Modifier")),false);
    // Exercice
    auto * exerciceMenu = m_new_modif_menu->addMenu(tr("Exercice"));
    connect_action_to_ppen_tab(exerciceMenu->addAction(tr("Créer")),{TabNote::ExerciceTabId,0});
    // Groupe
    auto * gr_menu = m_new_modif_menu->addMenu(tr("Groupe"));
    connect_action_to_new_modif_dialog<GroupeNewModifForm>(gr_menu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<GroupeNewModifForm>(gr_menu->addAction(tr("Modifier")),false);
    connect_action_to_ppen_tab(gr_menu->addAction(tr("Liste des éléves")),{TabNote::GroupeTabId,0});
    // Niveau
    auto * niveauMenu = m_new_modif_menu->addMenu(tr("Niveau"));
    connect_action_to_new_modif_dialog<NiveauNewModifForm>(niveauMenu->addAction((tr("Créer"))),true);
    connect_action_to_new_modif_dialog<NiveauNewModifForm>(niveauMenu->addAction((tr("Modifier"))),false);
    // Type
    //auto * grType = m_new_modif_menu->addMenu(tr("Type"));
    connect_action_to_ppen_tab(m_new_modif_menu->addAction(tr("Type")),{fenetre::predef_tab::Gestion_Type_Tab_Id,0});
    // Source
    auto * sourceMenu = m_new_modif_menu->addMenu(tr("Source"));
    connect_action_to_new_modif_dialog<dialogue::source_new_modif_form>(sourceMenu->addAction((tr("Créer"))),true);
    connect_action_to_new_modif_dialog<dialogue::source_new_modif_form>(sourceMenu->addAction((tr("Modifier"))),false);
    // Type de controle
    auto * tpCtrl= m_new_modif_menu->addMenu(tr("Type de controle"));
    connect_action_to_new_modif_dialog<TypeControleNewModifForm>(tpCtrl->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<TypeControleNewModifForm>(tpCtrl->addAction(tr("Modifier")),false);
    // Type d'établissement
    auto * tpEtabMenu = m_new_modif_menu->addMenu(tr("Type d'étabblissement"));
    connect_action_to_new_modif_dialog<TypeEtablissementNewModifForm>(tpEtabMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<TypeEtablissementNewModifForm>(tpEtabMenu->addAction(tr("Modifier")),false);
}

