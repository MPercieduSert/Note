#include "NewModifForm.h"

using namespace noteMPS;

//! Etablissement
EtablissementNewModifForm::EtablissementNewModifForm(bddMPS::BddPredef &bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom abrégé de l'établissement :"),
                                            tr("Nom de l'établissement :"),
                                            newEnt,parent) {
    // Type d'établissment
    m_TELabel = new QLabel(tr("Type(s) de l'établissement :"));
    m_TEList = new widgetMPS::Checklist(m_bdd.getList<TypeEtablissement>());

    // Niveaux
    m_nivLabel = new QLabel(tr("Niveaux présent dans l'établissement:"));
    m_nivList = new widgetMPS::Checklist(m_bdd.getList<Niveau>());

    if(!m_new)
        setNoms(m_bdd.getList<Etablissement>());

    m_mainLayout->addWidget(m_TELabel);
    m_mainLayout->addWidget(m_TEList);
    m_mainLayout->addWidget(m_nivLabel);
    m_mainLayout->addWidget(m_nivList);
}

void EtablissementNewModifForm::connexion() {
    AbstractNcNomNewModifForm::connexion();
    connect(m_TEList,&widgetMPS::Checklist::itemChanged,this,&EtablissementNewModifForm::updateNiveau);
}

void EtablissementNewModifForm::save() {
    Etablissement etab;
    if(!m_new)
        etab.setId(id());
    etab.setNc(nc());
    etab.setNom(nom());
    m_bdd.save(etab);
    widgetMPS::Checklist::Save<EtablissementType>::save(m_bdd,m_TEList,
                                                        [&etab](EtablissementType & etabType, idt idTE){
                                                                        etabType.setIdEtab(etab.id());
                                                                        etabType.setIdTpEtab(idTE);
                                                                    });
    widgetMPS::Checklist::Save<EtablissementNiveau>::save(m_bdd,m_nivList,
                                                          [&etab](EtablissementNiveau & etabNiv, idt idNiv){
                                                                        etabNiv.setIdEtab(etab.id());
                                                                        etabNiv.setIdNiveau(idNiv);
                                                                    });
}

void EtablissementNewModifForm::updateData() {
    if(!m_new) {
        Etablissement etab;
        updateTemp<Etablissement>(etab);
        m_TEList->setChecksById<EtablissementType>(m_bdd.getList<EtablissementType>(EtablissementType::IdEtab,etab.id()),
                       [](const EtablissementType& entity)->idt{return entity.idTpEtab();});
        m_nivList->setChecksById<EtablissementNiveau>(m_bdd.getList<EtablissementNiveau>(EtablissementNiveau::IdEtab,etab.id()),
                       [](const EtablissementNiveau& entity)->idt{return entity.idNiveau();});
    }
    updateNiveau();
}

void EtablissementNewModifForm::updateNiveau() {
    std::forward_list<idt> listIdTE;
    for(auto i = 0; i != m_TEList->count(); ++i)
        if(m_TEList->item(i)->checkState() == Qt::Checked)
            listIdTE.push_front(m_TEList->id(i));
    std::list<idt> listIdNiv;
    for(auto iter = listIdTE.cbegin(); iter != listIdTE.cend(); ++iter) {
        auto vecNivTE = m_bdd.getList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,*iter,NiveauTypeEtablissement::IdNiveau);
        std::list<idt> listIdNivOther;
        for (auto iterNiv = vecNivTE.cbegin(); iterNiv != vecNivTE.cend(); ++iterNiv)
            listIdNivOther.push_back(iterNiv->idNiveau());
        listIdNiv.merge(listIdNivOther);
    }
    listIdNiv.unique();
    for (auto i = 0; i != m_nivList->count(); ++i) {
        if(listIdNiv.empty() || listIdNiv.front() != m_nivList->id(i)) {
            m_nivList->item(i)->setCheckState(Qt::Unchecked);
            m_nivList->item(i)->setHidden(true);
            }
        else {
            m_nivList->item(i)->setHidden(false);
            listIdNiv.pop_front();
        }
    }
}

//! Niveau
NiveauNewModifForm::NiveauNewModifForm(bddMPS::BddPredef & bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractTypeNcNomNewModifForm (bdd,
                                                bddMPS::idProg::EtudeType,
                                                Niveau::ID,
                                                tr("type de niveau :"),
                                                tr("Nom abrégé du niveau :"),
                                                tr("Nom du niveau"),
                                                newEnt,parent)
{
    auto niveaux = m_bdd.getList<Niveau>();

    // Type d'établissment
    m_listLabel = new QLabel(tr("Type d'établissement pouvant accueillir le niveau:"));
    m_TEList = new widgetMPS::Checklist(m_bdd.getList<TypeEtablissement>());

    // Niveaux précédents
    m_nivPrecLabel = new QLabel(tr("Niveaux précédents:"));
    m_nivPrecList = new widgetMPS::Checklist(niveaux);

    // Niveaux suivants
    m_nivSuivLabel = new QLabel(tr("Niveaux suivants:"));
    m_nivSuivList = new widgetMPS::Checklist(niveaux);

    if(!m_new)
        setNoms(niveaux);
    m_nivPrecLayout = new QVBoxLayout;
    m_nivPrecLayout->addWidget(m_nivPrecLabel);
    m_nivPrecLayout->addWidget(m_nivPrecList);

    m_nivSuivLayout = new QVBoxLayout;
    m_nivSuivLayout->addWidget(m_nivSuivLabel);
    m_nivSuivLayout->addWidget(m_nivSuivList);

    m_nivLayout = new QHBoxLayout;
    m_nivLayout->addLayout(m_nivPrecLayout);
    m_nivLayout->addLayout(m_nivSuivLayout);

    m_mainLayout->addWidget(m_listLabel);
    m_mainLayout->addWidget(m_TEList);
    m_mainLayout->addLayout(m_nivLayout);
}

void NiveauNewModifForm::save() {
    Niveau niv;
    if(!m_new)
        niv.setId(id());
    niv.setNc(nc());
    niv.setNom(nom());
    niv.setType(idType());
    m_bdd.save(niv);
    widgetMPS::Checklist::Save<NiveauTypeEtablissement>::save(m_bdd,m_TEList,[&niv](NiveauTypeEtablissement & nivTE, idt idTE){
                                                                                        nivTE.setIdNiveau(niv.id());
                                                                                        nivTE.setIdTpEtab(idTE);
                                                                                    });
    widgetMPS::Checklist::Save<FiliationNiveau>::save(m_bdd,m_nivPrecList,[&niv](FiliationNiveau & filNiv, idt idNiv){
                                                                                        filNiv.setIdPrecedent(idNiv);
                                                                                        filNiv.setIdSuivant(niv.id());
                                                                                    });
    widgetMPS::Checklist::Save<FiliationNiveau>::save(m_bdd,m_nivSuivList,[&niv](FiliationNiveau & filNiv, idt idNiv){
                                                                                        filNiv.setIdPrecedent(niv.id());
                                                                                        filNiv.setIdSuivant(idNiv);
                                                                                    });
}

void NiveauNewModifForm::updateData() {
    if(!m_new) {
        Niveau niv;
        updateTemp<Niveau>(niv);
        m_TEList->setChecksById<NiveauTypeEtablissement>(m_bdd.getList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdNiveau,niv.id()),
                       [](const NiveauTypeEtablissement& entity)->idt{return entity.idTpEtab();});
        m_nivPrecList->setChecksById<FiliationNiveau>(m_bdd.getList<FiliationNiveau>(FiliationNiveau::IdSuivant,niv.id()),
                       [](const FiliationNiveau & entity){return entity.idPrecedent();});
        m_nivSuivList->setChecksById<FiliationNiveau>(m_bdd.getList<FiliationNiveau>(FiliationNiveau::IdPrecedent,niv.id()),
                       [](const FiliationNiveau & entity){return entity.idSuivant();});
    }
}

//! TypeEtablissement
TypeEtablissementNewModifForm::TypeEtablissementNewModifForm(bddMPS::Bdd & bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom abrégé du type d'établissment :"),
                                            tr("Nom du type d'établissement :"),
                                            newEnt,parent)
{
    m_selectNiv = new widgetMPS::SelectInListBox(tr("Niveaux existents :"),tr("Niveaux présents :"));
    if(!m_new)
        setNoms(m_bdd.getList<TypeEtablissement>(TypeEtablissement::Nom));
    m_mainLayout->addWidget(m_selectNiv);

}

void TypeEtablissementNewModifForm::save() {
    if(m_new) {
        TypeEtablissement te(nc(),nom());
        m_bdd.save(te);
        auto niv = m_selectNiv->value().second;
        for (auto i =niv.cbegin(); i != niv.cend(); ++i)
            m_bdd.save(NiveauTypeEtablissement(i->toUInt(),te.id()));
    }
    else {
        TypeEtablissement te(id());
        if(m_bdd.getAutorisation(te,bddMPS::Modif)) {
            te.setNc(nc());
            te.setNom(nom());
            m_bdd.save(te);
        }
        auto nivOld = m_bdd.getList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,te.id());
        auto nivNew = m_selectNiv->value().second;
        for(auto i = nivOld.cbegin(); i != nivOld.cend(); ++i) {
            auto j = nivNew.begin();
            while(j != nivNew.end() && i->idNiveau() != j->toUInt())
                ++j;
            if(j == nivNew.end()) {
                NiveauTypeEtablissement nivTE(i->idNiveau(),te.id());
                m_bdd.getUnique(nivTE);
                m_bdd.del(nivTE);
            }
            else
                j->setValue(0);
        }
        for (auto j = nivNew.cbegin(); j != nivNew.cend(); ++j) {
            if(j->toUInt() != 0)
                m_bdd.save(NiveauTypeEtablissement(j->toUInt(),te.id()));
        }
    }
}

void TypeEtablissementNewModifForm::updateData() {
    if(m_new) {
        auto vecNiv = m_bdd.getList<Niveau>();
        std::map<QVariant, QString> nivOut;
        for(auto i = vecNiv.cbegin(); i != vecNiv.cend(); ++i)
            nivOut[i->id()] = i->nom();
        m_selectNiv->setValue(nivOut,std::map<QVariant, QString>());
    }
    else {
        TypeEtablissement te;
        updateTemp<TypeEtablissement>(te);
        auto vecNiv = m_bdd.getList<Niveau>();
        std::map<QVariant, QString> nivIn;
        std::map<QVariant, QString> nivOut;
        for(auto i = vecNiv.cbegin(); i != vecNiv.cend(); ++i) {
            if(m_bdd.existsUnique(NiveauTypeEtablissement(i->id(),te.id())))
                nivIn[i->id()] = i->nom();
            else
                nivOut[i->id()] = i->nom();
        }
        m_selectNiv->setValue(nivOut,nivIn);
    }
}
