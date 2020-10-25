#include "EntitySelectWidget.h"

using namespace noteMPS;

AnneeSelectWidget::AnneeSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : AbstractEntitySelectWidget(bdd,orientation,parent) {
    m_anneeLabel = new QLabel(tr("Annee scolaire : "));
    m_anneeSpinBox = new SpinBoxAnneeScolaire(m_bdd.getList<Annee>(Annee::Num));
    m_anneeSpinBox->setNowValue();
    connect(m_anneeSpinBox,&SpinBoxAnneeScolaire::valueChanged,this,[this](){emit idChanged(id());});

    m_mainLayout->addWidget(m_anneeLabel);
    m_mainLayout->addWidget(m_anneeSpinBox);
    m_mainLayout->setAlignment(m_anneeLabel,Qt::AlignLeft);
    m_mainLayout->setAlignment(m_anneeSpinBox,Qt::AlignLeft);
}
