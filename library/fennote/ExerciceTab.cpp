#include "ExerciceTab.h"

using namespace noteMPS;

ExerciceTab::ExerciceTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent)
    : AbstractTabModuleWithBdd(bdd,pair,parent) {
    m_view = new widgetMPS::NodeView(std::make_unique<widgetMPS::RoundedArcPainter>());
    m_model = new ExerciceEditModel(static_cast<idt>(pair.second), static_cast<BddNote &>(bdd),this);
    m_view->setModel(m_model);
    m_view->setDelegate(new delegateMPS::StandardNodeDelegate(this));
}

void ExerciceTab::becomeCurrent()
    {emit actionPermise(fenMPS::EffacerAction|fenMPS::SauverAction);}
