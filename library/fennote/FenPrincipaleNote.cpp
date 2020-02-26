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
    auto * etab = m_menuModif->addAction(tr("Établissment"));
    auto * niveau = m_menuModif->addAction((tr("Niveau")));

    connect(niveau,&QAction::triggered,this, [this](){
                            auto form = new NiveauNewModifForm(noyau()->bdd(),false);                
                            dialogMPS::NewModifDialog diag(form,this);
                            diag.exec();});
    auto * typeEtab = m_menuModif->addAction(tr("Type d'étabblissement"));
    connect(typeEtab,&QAction::triggered,this, [this](){
                            auto form = new TypeEtablissementNewModifForm(noyau()->bdd(),false);
                            dialogMPS::NewModifDialog diag(form,this);
                            diag.exec();});
}

void FenPrincipaleNote::createMenuNew() {
    m_menuNew = new QMenu(tr("Nouveau"));
    m_menuFichier->insertMenu(m_actionSave,m_menuNew);
    auto * etab = m_menuNew->addAction(tr("Établissment"));
    auto * niveau = m_menuNew->addAction((tr("Niveau")));
    connect(niveau,&QAction::triggered,this, [this](){
                            auto form = new NiveauNewModifForm(noyau()->bdd(),true);
                            dialogMPS::NewModifDialog diag(form,this);
                            diag.exec();});
    auto * typeEtab = m_menuNew->addAction(tr("Type d'étabblissement"));
    connect(typeEtab,&QAction::triggered,this,[this](){
                            auto form = new TypeEtablissementNewModifForm(noyau()->bdd(),true);
                            dialogMPS::NewModifDialog diag(form,this);
                            diag.exec();});
}

