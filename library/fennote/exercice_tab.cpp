#include "exercice_tab.h"

using namespace note_mps;

exercice_tab::exercice_tab(bdd_note & bdd, tab_index pair, QWidget * parent)
    : abstract_tab_module_with_bdd(bdd,pair,parent) {
    // Widget
    m_save_bouton = new QPushButton(tr("Sauvegarder"));
    m_view = new mps::widget::node_view(std::make_unique<mps::widget::rounded_arc_painter>());
    m_model = new exercice_edit_model(static_cast<idt>(pair.second), static_cast<bdd_note &>(bdd),this);
    m_view->set_model(m_model);
    m_view->set_delegate(new mps::delegate::standard_node_delegate(this));
    connect(m_save_bouton,&QPushButton::clicked,this,&exercice_tab::sauver);

    // Calque
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addWidget(m_save_bouton);
}

void exercice_tab::become_current()
    {emit action_permise(0);}
