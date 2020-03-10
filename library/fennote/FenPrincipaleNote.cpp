#include "FenPrincipaleNote.h"

using namespace noteMPS;

FenPrincipaleNote::FenPrincipaleNote(NoyauNote * noyau, std::unique_ptr<BddNote> bdd, ZoneCentraleNote * centralZone,
                                   const QString &bddPathXML, const QString & configPath, QWidget *parent)
    : PredefFenPrincipale (noyau,std::move(bdd),centralZone,bddPathXML,configPath,parent)
{
    createMenuNew();
    createMenuModif();
}

void FenPrincipaleNote::createMenuModif() {
    m_menuModif = new QMenu(tr("Modification"));
    m_menuFichier->insertMenu(m_actionSave,m_menuModif);
    connect(m_menuModif->addAction(tr("Année scolaire courante")),&QAction::triggered,this,[this](){
        auto * form = new AnneeNewModifForm(noyau()->bdd(), false);
        dialogMPS::NewModifDialog diag(form,this);
        if(diag.exec())
            noyau()->setAnnee(form->annee());
    });
    connectActionToNewModifDialog<EtablissementNewModifForm>(m_menuModif->addAction(tr("Établissment")),false);
    connectActionToNewModifDialog<NiveauNewModifForm>(m_menuModif->addAction((tr("Niveau"))),false);
    connectActionToNewModifDialog<TypeEtablissementNewModifForm>(m_menuModif->addAction(tr("Type d'étabblissement")),false);
}

void FenPrincipaleNote::createMenuNew() {
    m_menuNew = new QMenu(tr("Nouveau"));
    m_menuFichier->insertMenu(m_actionSave,m_menuNew);
    connect(m_menuNew->addAction(tr("Année scolaire")),&QAction::triggered,this,[this](){
        auto * form = new AnneeNewModifForm(noyau()->bdd(), true);
        dialogMPS::NewModifDialog diag(form,this);
        if(diag.exec() && form->anneeCourante()) {
            auto an = form->annee();
            noyau()->bdd().getUnique(an);
            noyau()->setAnnee(an);
        }
    });
    connectActionToNewModifDialog<EtablissementNewModifForm>(m_menuNew->addAction(tr("Établissment")),true);
    connectActionToNewModifDialog<NiveauNewModifForm>(m_menuNew->addAction((tr("Niveau"))),true);
    connectActionToNewModifDialog<TypeEtablissementNewModifForm>(m_menuNew->addAction(tr("Type d'étabblissement")),true);
}

