#include "NewModifForm.h"

using namespace noteMPS;

//! Niveau
NiveauNewModifForm::NiveauNewModifForm(bddMPS::BddPredef & bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractTypeNcNomNewModifForm (bdd,
                                                bddMPS::idProg::EtudeType,
                                                Niveau::ID,
                                                tr("type de niveau:"),
                                                tr("Nom court du niveau :"),
                                                tr("Nom du niveau"),
                                                newEnt,parent)
{
    auto niveaux = m_bdd.getList<Niveau>();

    // Type d'établissment
    m_listLabel = new QLabel(tr("Type d'établissement pouvant accueillir le niveau:"));
    m_TEList = new QListWidget;
    nomUncheckedList(m_TEList,m_bdd.getList<TypeEtablissement>());
    //m_TEList->setSelectionMode(QAbstractItemView::SingleSelection);

    // Niveaux précédents
    m_nivPrecLabel = new QLabel(tr("Niveaux Précédents:"));
    m_nivPrecList = new QListWidget;
    nomUncheckedList(m_nivPrecList,niveaux);

    // Niveaux suivants
    m_nivSuivLabel = new QLabel(tr("Niveaux Suivants:"));
    m_nivSuivList = new QListWidget;
    nomUncheckedList(m_nivSuivList,niveaux);

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
    for (auto i = 0; i != m_nivPrecList->count(); ++i) {
        if(m_nivPrecList->item(i)->checkState() == Qt::Checked)
            m_bdd.saveUnique(FiliationNiveau(m_nivPrecList->item(i)->data(Qt::UserRole).toUInt(),niv.id()));
        if(m_nivSuivList->item(i)->checkState() == Qt::Checked)
            m_bdd.saveUnique(FiliationNiveau(niv.id(),m_nivSuivList->item(i)->data(Qt::UserRole).toUInt()));
    }
}

void NiveauNewModifForm::updateData() {
    if(!m_new) {
        Niveau niv;
        updateTemp<Niveau>(niv);
        setCheckedList<NiveauTypeEtablissement>(m_TEList,m_bdd.getList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdNiveau,niv.id()),
                       [](const NiveauTypeEtablissement& entity)->idt{return entity.idTpEtab();});
        setCheckedList<FiliationNiveau>(m_nivPrecList,m_bdd.getList<FiliationNiveau>(FiliationNiveau::IdSuivant,niv.id()),
                       [](const FiliationNiveau & entity){return entity.idPrecedent();});
        setCheckedList<FiliationNiveau>(m_nivSuivList,m_bdd.getList<FiliationNiveau>(FiliationNiveau::IdPrecedent,niv.id()),
                       [](const FiliationNiveau & entity){return entity.idSuivant();});
    }
}

//! TypeEtablissement
TypeEtablissementNewModifForm::TypeEtablissementNewModifForm(bddMPS::Bdd & bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom court du type d'établissment :"),
                                            tr("Nom du type d'établissement"),
                                            newEnt,parent)
{
    m_selectNiv = new widgetMPS::SelectInListBox(tr("Niveaux existents:"),tr("Niveaux présents"));
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
