#include "FenPrincipaleNote.h"

using namespace noteMPS;

FenPrincipaleNote::FenPrincipaleNote(NoyauNote * noyau, std::unique_ptr<BddNote> bdd, ZoneCentraleNote * centralZone,
                                   const QString &bddPathXML, const QString & configPath, QWidget *parent)
    : PredefFenPrincipale (noyau,std::move(bdd),centralZone,bddPathXML,configPath,parent) {
    createMenuNewModif();
}

void FenPrincipaleNote::createMenuNewModif() {
    //Année
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
    //Classe
    auto * classeMenu = m_newModifMenu->addMenu(tr("Classe"));
    connectActionToNewModifDialog<ClasseNewModifForm>(classeMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<ClasseNewModifForm>(classeMenu->addAction(tr("Modifier")),false);
//    connect(classeMenu->addAction(tr("Liste des éléves")),&QAction::triggered,this,[this](){
//        centraleZone()->openTab({TabNote::EleveClasseTabId,0});
//    });
    //Eleve
    connectActionToOpenTab(m_newModifMenu->addAction(tr("Eleve")),{TabNote::ElevesTabId,fenMPS::AbstractTabModule::NoId});
    //Établissement
    auto * etabMenu = m_newModifMenu->addMenu(tr("Établissement"));
    connectActionToNewModifDialog<EtablissementNewModifForm>(etabMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<EtablissementNewModifForm>(etabMenu->addAction(tr("Modifier")),false);
    //Niveau
    auto * niveauMenu = m_newModifMenu->addMenu(tr("Niveau"));
    connectActionToNewModifDialog<NiveauNewModifForm>(niveauMenu->addAction((tr("Créer"))),true);
    connectActionToNewModifDialog<NiveauNewModifForm>(niveauMenu->addAction((tr("Modifier"))),false);
    //Type d'établissement
    auto * tpEtabMenu = m_newModifMenu->addMenu(tr("Type d'étabblissement"));
    connectActionToNewModifDialog<TypeEtablissementNewModifForm>(tpEtabMenu->addAction(tr("Créer")),true);
    connectActionToNewModifDialog<TypeEtablissementNewModifForm>(tpEtabMenu->addAction(tr("Modifier")),false);
}

