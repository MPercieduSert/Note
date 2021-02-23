#include "ExerciceTab.h"

using namespace noteMPS;

ExerciceTab::ExerciceTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent)
    : AbstractTabModuleWithBdd(bdd,pair,parent) {
    // Widget
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_view = new widget::node_view(std::make_unique<widget::rounded_arc_painter>());
    m_model = new ExerciceEditModel(static_cast<idt>(pair.second), static_cast<BddNote &>(bdd),this);
    m_view->setModel(m_model);
    m_view->setDelegate(new delegate::standard_node_delegate(this));
    connect(m_saveButton,&QPushButton::clicked,this,&ExerciceTab::sauver);

    // Calque
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addWidget(m_saveButton);
}

void ExerciceTab::becomeCurrent()
    {emit actionPermise(0);}
