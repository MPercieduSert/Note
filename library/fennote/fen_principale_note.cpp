#include "fen_principale_note.h"

using namespace note_mps;

fen_principale_note::fen_principale_note(noyau_note * noyau, std::unique_ptr<bdd_note> bdd, zone_centrale_note * central_zone,
                                   const QString &bdd_path_xml, const QString & config_path, QWidget *parent)
    : predef_fen_principale (noyau,std::move(bdd),central_zone,bdd_path_xml,config_path,parent) {
    create_menu_new_modif();
//    QDockWidget *dockWidget = new QDockWidget(tr("Rechecher"), this);
//    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//    auto * find = new find_widget();
//    dockWidget->setWidget(find);
//    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

//    QDockWidget *dockWidget2 = new QDockWidget(tr("Dock Widget 2"), this);
//    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
//                                      Qt::RightDockWidgetArea);
//    QLabel * label_2 = new QLabel("Dock test2");
//    dockWidget2->setWidget(label_2);
//    addDockWidget(Qt::RightDockWidgetArea, dockWidget2);
}

void fen_principale_note::create_menu_new_modif() {
    // Année
    auto *anMenu = m_new_modif_menu->addMenu(tr("Année scolaire"));
    connect(anMenu->addAction(tr("Crée une année")),&QAction::triggered,this,[this](){
        auto *form = new annee_new_modif_form(noyau()->bdd(), true);
        mps::dialogue::new_modif_dialog diag(form,this);
        if(diag.exec() && form->annee_courante()) {
            auto an = form->get_annee();
            noyau()->bdd().get_unique(an);
            noyau()->set_annee(an);
        }
    });
    connect(anMenu->addAction(tr("Choix de l'année courante")),&QAction::triggered,this,[this](){
        auto *form = new annee_new_modif_form(noyau()->bdd(), false);
        mps::dialogue::new_modif_dialog diag(form,this);
        if(diag.exec())
            noyau()->set_annee(form->get_annee());
    });
    // Calendrier
    auto calMenu = m_new_modif_menu->addMenu(tr("Calendrier"));
    auto eveMenu = calMenu->addMenu(tr("Événement"));
    connect_action_to_new_modif_dialog<mps::dialogue::evenement_new_modif_form>(eveMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<mps::dialogue::evenement_new_modif_form>(eveMenu->addAction(tr("Modifier")),false);
    auto styleMenu = calMenu->addMenu(tr("Style"));
    connect_action_to_new_modif_dialog<mps::dialogue::evenement_style_new_modif_form>(styleMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<mps::dialogue::evenement_style_new_modif_form>(styleMenu->addAction(tr("Modifier")),false);
    //Classe
    auto * classeMenu = m_new_modif_menu->addMenu(tr("Classe"));
    connect_action_to_new_modif_dialog<classe_new_modif_form>(classeMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<classe_new_modif_form>(classeMenu->addAction(tr("Modifier")),false);
    connect_action_to_ppen_tab(classeMenu->addAction(tr("Liste des éléves")),{tab_note::Classe_Eleve_Tab_Id,0});
    //controle
    auto * ctrl= m_new_modif_menu->addMenu(tr("Controle"));
    connect_action_to_new_modif_dialog<controle_new_modif_form>(ctrl->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<controle_new_modif_form>(ctrl->addAction(tr("Modifier")),false);
    // eleve
    connect_action_to_ppen_tab(m_new_modif_menu->addAction(tr("Eleve")),
        {tab_note::Eleves_Tab_Id,mps::fenetre::abstract_tab_module::No_Id});
    // Établissement
    auto * etabMenu = m_new_modif_menu->addMenu(tr("Établissement"));
    connect_action_to_new_modif_dialog<etablissement_new_modif_form>(etabMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<etablissement_new_modif_form>(etabMenu->addAction(tr("Modifier")),false);
    // exercice
    auto * exerciceMenu = m_new_modif_menu->addMenu(tr("Exercice"));
    connect_action_to_ppen_tab(exerciceMenu->addAction(tr("Créer")),{tab_note::Edit_Exercice_Tab_Id,0});
    connect_action_to_ppen_tab(exerciceMenu->addAction(tr("Chercher")),{tab_note::Find_Exercice_Tab_Id,0});
    // Groupe
    auto * gr_menu = m_new_modif_menu->addMenu(tr("Groupe"));
    connect_action_to_new_modif_dialog<groupe_new_modif_form>(gr_menu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<groupe_new_modif_form>(gr_menu->addAction(tr("Modifier")),false);
    connect_action_to_ppen_tab(gr_menu->addAction(tr("Liste des éléves")),{tab_note::Groupe_Tab_Id,0});
    // Niveau
    auto * niveauMenu = m_new_modif_menu->addMenu(tr("Niveau"));
    connect_action_to_new_modif_dialog<niveau_new_modif_form>(niveauMenu->addAction((tr("Créer"))),true);
    connect_action_to_new_modif_dialog<niveau_new_modif_form>(niveauMenu->addAction((tr("Modifier"))),false);
    // Type
    //auto * grType = m_new_modif_menu->addMenu(tr("Type"));
    connect_action_to_ppen_tab(m_new_modif_menu->addAction(tr("Type")),{mps::fenetre::predef_tab::Gestion_Type_Tab_Id,0});
    // Source
    auto * sourceMenu = m_new_modif_menu->addMenu(tr("Source"));
    connect_action_to_new_modif_dialog<mps::dialogue::source_new_modif_form>(sourceMenu->addAction((tr("Créer"))),true);
    connect_action_to_new_modif_dialog<mps::dialogue::source_new_modif_form>(sourceMenu->addAction((tr("Modifier"))),false);
    // Type de controle
    auto * tpCtrl= m_new_modif_menu->addMenu(tr("Type de controle"));
    connect_action_to_new_modif_dialog<type_controle_new_modif_form>(tpCtrl->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<type_controle_new_modif_form>(tpCtrl->addAction(tr("Modifier")),false);
    // Type d'établissement
    auto * tpEtabMenu = m_new_modif_menu->addMenu(tr("Type d'étabblissement"));
    connect_action_to_new_modif_dialog<type_etablissement_new_modif_form>(tpEtabMenu->addAction(tr("Créer")),true);
    connect_action_to_new_modif_dialog<type_etablissement_new_modif_form>(tpEtabMenu->addAction(tr("Modifier")),false);
}

