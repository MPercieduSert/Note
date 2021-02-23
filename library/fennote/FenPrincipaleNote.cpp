#include "FenPrincipaleNote.h"

using namespace noteMPS;

FenPrincipaleNote::FenPrincipaleNote(NoyauNote * noyau, std::unique_ptr<BddNote> bdd, ZoneCentraleNote * centralZone,
                                   const QString &bddPathXML, const QString & configPath, QWidget *parent)
    : PredefFenPrincipale (noyau,std::move(bdd),centralZone,bddPathXML,configPath,parent) {
    createMenuNewModif();
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

void FenPrincipaleNote::createMenuNewModif() {
    // Année
    auto * anMenu = m_newModifMenu->addMenu(tr("Année scolaire"));
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
    auto calMenu = m_newModifMenu->addMenu(tr("Calendrier"));
    auto eveMenu = calMenu->addMenu(tr("Événement"));
    connectActionTonew_modif_dialog<dialogue::evenement_new_modif_form>(eveMenu->addAction(tr("Créer")),true);
    connectActionTonew_modif_dialog<dialogue::evenement_new_modif_form>(eveMenu->addAction(tr("Modifier")),false);
    auto styleMenu = calMenu->addMenu(tr("Style"));
    connectActionTonew_modif_dialog<dialogue::evenement_style_new_modif_form>(styleMenu->addAction(tr("Créer")),true);
    connectActionTonew_modif_dialog<dialogue::evenement_style_new_modif_form>(styleMenu->addAction(tr("Modifier")),false);
    //Classe
    auto * classeMenu = m_newModifMenu->addMenu(tr("Classe"));
    connectActionTonew_modif_dialog<ClasseNewModifForm>(classeMenu->addAction(tr("Créer")),true);
    connectActionTonew_modif_dialog<ClasseNewModifForm>(classeMenu->addAction(tr("Modifier")),false);
    connectActionToOpenTab(classeMenu->addAction(tr("Liste des éléves")),{TabNote::ClasseEleveTabId,0});
    //Controle
    auto * ctrl= m_newModifMenu->addMenu(tr("Controle"));
    connectActionTonew_modif_dialog<ControleNewModifForm>(ctrl->addAction(tr("Créer")),true);
    connectActionTonew_modif_dialog<ControleNewModifForm>(ctrl->addAction(tr("Modifier")),false);
    // Eleve
    connectActionToOpenTab(m_newModifMenu->addAction(tr("Eleve")),{TabNote::ElevesTabId,fenMPS::AbstractTabModule::NoId});
    // Établissement
    auto * etabMenu = m_newModifMenu->addMenu(tr("Établissement"));
    connectActionTonew_modif_dialog<EtablissementNewModifForm>(etabMenu->addAction(tr("Créer")),true);
    connectActionTonew_modif_dialog<EtablissementNewModifForm>(etabMenu->addAction(tr("Modifier")),false);
    // Exercice
    auto * exerciceMenu = m_newModifMenu->addMenu(tr("Exercice"));
    connectActionToOpenTab(exerciceMenu->addAction(tr("Créer")),{TabNote::ExerciceTabId,0});
    // Groupe
    auto * grMenu = m_newModifMenu->addMenu(tr("Groupe"));
    connectActionTonew_modif_dialog<GroupeNewModifForm>(grMenu->addAction(tr("Créer")),true);
    connectActionTonew_modif_dialog<GroupeNewModifForm>(grMenu->addAction(tr("Modifier")),false);
    connectActionToOpenTab(grMenu->addAction(tr("Liste des éléves")),{TabNote::GroupeTabId,0});
    // Niveau
    auto * niveauMenu = m_newModifMenu->addMenu(tr("Niveau"));
    connectActionTonew_modif_dialog<NiveauNewModifForm>(niveauMenu->addAction((tr("Créer"))),true);
    connectActionTonew_modif_dialog<NiveauNewModifForm>(niveauMenu->addAction((tr("Modifier"))),false);
    // Type
    //auto * grType = m_newModifMenu->addMenu(tr("Type"));
    connectActionToOpenTab(m_newModifMenu->addAction(tr("Type")),{fenMPS::PredefTab::GestionTypeTabId,0});
    // Source
    auto * sourceMenu = m_newModifMenu->addMenu(tr("Source"));
    connectActionTonew_modif_dialog<dialogue::source_new_modif_form>(sourceMenu->addAction((tr("Créer"))),true);
    connectActionTonew_modif_dialog<dialogue::source_new_modif_form>(sourceMenu->addAction((tr("Modifier"))),false);
    // Type de controle
    auto * tpCtrl= m_newModifMenu->addMenu(tr("Type de controle"));
    connectActionTonew_modif_dialog<TypeControleNewModifForm>(tpCtrl->addAction(tr("Créer")),true);
    connectActionTonew_modif_dialog<TypeControleNewModifForm>(tpCtrl->addAction(tr("Modifier")),false);
    // Type d'établissement
    auto * tpEtabMenu = m_newModifMenu->addMenu(tr("Type d'étabblissement"));
    connectActionTonew_modif_dialog<TypeEtablissementNewModifForm>(tpEtabMenu->addAction(tr("Créer")),true);
    connectActionTonew_modif_dialog<TypeEtablissementNewModifForm>(tpEtabMenu->addAction(tr("Modifier")),false);
}

