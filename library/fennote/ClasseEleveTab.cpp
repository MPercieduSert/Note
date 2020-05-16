#include "ClasseEleveTab.h"

using namespace noteMPS;

ClasseEleveTab::ClasseEleveTab(BddNote & bdd, std::pair<int,int> pair, QWidget *parent)
    : AbstractTabTableau(bdd,pair,parent) {
    // Classe
    m_anLabel = new QLabel(tr("Année de la classe :"));
    m_classeLabel = new QLabel(tr("Classe :"));
    m_anSpinBox = new SpinBoxAnneeScolaire(bdd.getList<Annee>(Annee::Num));
    m_anSpinBox->setNowValue();
    m_classeComboBox = new widgetMPS::IdComboBox;
    updateClasseListe();
    connect(m_anSpinBox,&SpinBoxAnneeScolaire::valueChanged,this,&ClasseEleveTab::updateClasseListe);

    // Calque
    m_classeLayout = new QHBoxLayout;
    m_classeLayout->addWidget(m_anLabel);
    m_classeLayout->addWidget(m_anSpinBox);
    m_classeLayout->addWidget(m_classeLabel);
    m_classeLayout->addWidget(m_classeComboBox);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addLayout(m_classeLayout);
}

void ClasseEleveTab::updateClasseListe(){
    m_classeComboBox->clear();
    m_classeComboBox->addText(bdd().getList<Classe>(Classe::IdAn,m_anSpinBox->value().id(),Classe::Nom),
                              [](const Classe & cl)->QString{return QString(cl.nom()).append(" (").append(cl.nc()).append(")");});
}
