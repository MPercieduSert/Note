#include "EntitySelectWidget.h"

using namespace noteMPS;

AnneeSelectWidget::AnneeSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : AbstractEntitySelectWidget(bdd,orientation,parent) {
    m_label = new QLabel(tr("Annee scolaire : "));
    m_spinBox = new SpinBoxAnneeScolaire(m_bdd.getList<Annee>(Annee::Num));
    m_spinBox->setNowValue();
    m_spinBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_spinBox,&SpinBoxAnneeScolaire::valueChanged,this,[this](){emit idChanged(id());});
    m_mainLayout->addWidget(m_label);
    m_mainLayout->addWidget(m_spinBox);
}

ClasseSelectWidget::ClasseSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : ComboBoxEntitySelectWidget(bdd,tr("Classe : "),orientation,parent) {
    m_anneeSelect = new AnneeSelectWidget(bdd);
    m_etabSelect = new EtablissementSelectWidget(bdd);
    connect(m_anneeSelect,&AnneeSelectWidget::idChanged,this,&ClasseSelectWidget::updateClasse);
    connect(m_etabSelect,&EtablissementSelectWidget::idChanged,this,&ClasseSelectWidget::updateClasse);
    m_classeLayout = new QHBoxLayout;
    m_classeLayout->addWidget(m_label);
    m_classeLayout->addWidget(m_box);
    m_mainLayout->addWidget(m_anneeSelect);
    m_mainLayout->addWidget(m_etabSelect);
    m_mainLayout->addLayout(m_classeLayout);
}

void ClasseSelectWidget::setEnabledEtab(bool bb) {
    m_etabSelect->setEnabled(bb);
    m_label->setEnabled(bb);
    m_box->setEnabled(bb);
    if(bb)
        updateClasse();
    else
        m_box->clear();
}

void ClasseSelectWidget::updateClasse() {
    m_box->clear();
    if(m_anneeSelect->id() != 0 && m_etabSelect->id() != 0)
        m_box->addText(m_bdd.getList<Classe>(Classe::IdAn,m_anneeSelect->id(),Classe::IdEtab,m_etabSelect->id(),Classe::Nom),
                 [](const Classe & cl)->QString {return cl.nc();});
}

void NiveauxSelectWidget::setIdEtab(idt id) {
    m_box->clear();
    if(id == 0)
        m_box->addText(m_bdd.getList<Niveau>(Niveau::Nom),
                     [](const Niveau & niv)->QString {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
    else
        m_box->addText(m_bdd.getList<Niveau,EtablissementNiveau>(EtablissementNiveau::IdNiveau,EtablissementNiveau::IdEtab,id),
                     [](const Niveau & niv)->QString {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
}
