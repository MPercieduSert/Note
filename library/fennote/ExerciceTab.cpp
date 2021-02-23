#include "ExerciceTab.h"

using namespace noteMPS;

ExerciceTab::ExerciceTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent)
    : AbstractTabModuleWithBdd(bdd,pair,parent) {
    // Widget
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_view = new widgetMPS::node_ptrView(std::make_unique<widgetMPS::RoundedArcPainter>());
    m_model = new ExerciceEditModel(static_cast<idt>(pair.second), static_cast<BddNote &>(bdd),this);
    m_view->setModel(m_model);
    m_view->setDelegate(new delegateMPS::Standardnode_ptrDelegate(this));
    connect(m_saveButton,&QPushButton::clicked,this,&ExerciceTab::sauver);

    // Calque
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addWidget(m_saveButton);
}

void ExerciceTab::becomeCurrent()
    {emit actionPermise(0);}
