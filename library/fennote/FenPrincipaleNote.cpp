#include "FenPrincipaleNote.h"

using namespace noteMPS;

FenPrincipaleNote::FenPrincipaleNote(NoyauNote * noyau, std::unique_ptr<BddNote> bdd, ZoneCentraleNote * centralZone,
                                   const QString &bddPathXML, const QString & configPath, QWidget *parent)
    : PredefFenPrincipale (noyau,std::move(bdd),centralZone,bddPathXML,configPath,parent) {
    createMenuNewModif();
//    QDockWidget *dockWidget = new QDockWidget(tr("Rechecher"), this);
//    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
//    auto * find = new FindWidget();
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
        dialogMPS::NewModifDialog diag(form,this);
        if(diag.exec() && form->anneeCourante()) {
            auto an = form->annee();
            noyau()->bdd().getUnique(an);
            noyau()->setAnnee(an);
        }
    });
    connect(anMenu->addAction(tr("Choix de l'année courante")),&QAction::triggered,this,[this](){
        auto * form = new AnneeNewModifForm(noyau()->bdd(), false);
        dialogMPS::NewModifDialog diag(form,this);
        if(diag.exec())
            noyau()->setAnnee(form->annee());
    });
    // Calendrier
    auto calMenu = m_newModifMenu->addMenu(tr("Calendrier"));
    auto eveMenu = calMenu->addMenu(tr("Événement"));
    connectActionToNewModifDialog<dialogMPS::EvenementNewModifForm>(eveMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<dialogMPS::EvenementNewModifForm>(eveMenu->addAction(tr("Modifier")),false);
    auto styleMenu = calMenu->addMenu(tr("Style"));
    connectActionToNewModifDialog<dialogMPS::EvenementStyleNewModifForm>(styleMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<dialogMPS::EvenementStyleNewModifForm>(styleMenu->addAction(tr("Modifier")),false);
    //Classe
    auto * classeMenu = m_newModifMenu->addMenu(tr("Classe"));
    connectActionToNewModifDialog<ClasseNewModifForm>(classeMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<ClasseNewModifForm>(classeMenu->addAction(tr("Modifier")),false);
    connect(classeMenu->addAction(tr("Liste des éléves")),&QAction::triggered,this,[this](){
        centraleZone()->openTab({TabNote::ClasseEleveTabId,0});
        });
    // Eleve
    connectActionToOpenTab(m_newModifMenu->addAction(tr("Eleve")),{TabNote::ElevesTabId,fenMPS::AbstractTabModule::NoId});
    // Établissement
    auto * etabMenu = m_newModifMenu->addMenu(tr("Établissement"));
    connectActionToNewModifDialog<EtablissementNewModifForm>(etabMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<EtablissementNewModifForm>(etabMenu->addAction(tr("Modifier")),false);
    // Groupe
    auto * grMenu = m_newModifMenu->addMenu(tr("Groupe"));
    connectActionToNewModifDialog<GroupeNewModifForm>(grMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<GroupeNewModifForm>(grMenu->addAction(tr("Modifier")),false);
    connect(grMenu->addAction(tr("Liste des éléves")),&QAction::triggered,this,[this](){
        centraleZone()->openTab({TabNote::GroupeTabId,0});
        });
    // Niveau
    auto * niveauMenu = m_newModifMenu->addMenu(tr("Niveau"));
    connectActionToNewModifDialog<NiveauNewModifForm>(niveauMenu->addAction((tr("Créer"))),true);
    connectActionToNewModifDialog<NiveauNewModifForm>(niveauMenu->addAction((tr("Modifier"))),false);
    // Type d'établissement
    auto * tpEtabMenu = m_newModifMenu->addMenu(tr("Type d'étabblissement"));
    connectActionToNewModifDialog<TypeEtablissementNewModifForm>(tpEtabMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<TypeEtablissementNewModifForm>(tpEtabMenu->addAction(tr("Modifier")),false);
}

