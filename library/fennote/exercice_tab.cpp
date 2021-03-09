#include "exercice_tab.h"

using namespace note_mps;

exercice_tab::exercice_tab(bdd_note & bdd, tab_index pair, QWidget * parent)
    : abstract_tab_module_with_bdd(bdd,pair,parent) {
    //model-view
    m_view = new mps::widget::node_view(std::make_unique<mps::widget::rounded_arc_painter>());
    m_model = new edit_exercice_model(static_cast<idt>(pair.second), static_cast<bdd_note &>(bdd),this);
    m_view->set_model(m_model);
    auto delegate = new mps::delegate::standard_node_delegate(this);
    m_view->set_delegate(delegate);
    m_type_exo = delegate->create_sub_node(m_model->index(mps::model_base::node_index(),0,edit_exercice_model::Type_Cible));
    m_type_exo->update_data(mps::model_base::All_Role);
    connect(m_model,&edit_exercice_model::data_changed,this,[this](const mps::model_base::node_index &index, flag role){
        if(index.cible() == edit_exercice_model::Type_Cible && index.is_root())
            m_type_exo->update_data(role);
    });
    //bouton
    m_save_bouton = new QPushButton(tr("Sauvegarder"));
    connect(m_save_bouton,&QPushButton::clicked,this,&exercice_tab::sauver);

    // Calque
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_type_exo);
    m_main_layout->addWidget(m_view);
    m_main_layout->addWidget(m_save_bouton);
}

void exercice_tab::become_current()
    {emit action_permise(0);}

void exercice_tab::update_type() {
    auto types = m_model->data(m_model->index(mps::model_base::node_index(),0,edit_exercice_model::Type_Cible),
                               mps::model_base::Map_Role);

}
