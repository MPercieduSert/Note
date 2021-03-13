#include "exercice_tab.h"

using namespace note_mps;

edit_exercice_tab::edit_exercice_tab(bdd_note & bdd, tab_index pair, QWidget * parent)
    : abstract_tab_module_with_bdd(bdd,pair,parent) {
    //model-view
    m_view = new mps::widget::node_view(std::make_unique<mps::widget::rounded_arc_painter>());
    m_model = new model_exo::edit_exercice_model(static_cast<idt>(pair.second), static_cast<bdd_note &>(bdd),this);
    m_view->set_model(m_model);
    auto delegate = new mps::delegate::standard_node_delegate(this);
    m_view->set_delegate(delegate);
    m_type_exo = delegate->create_sub_node(m_model->index(mps::model_base::node_index(),0,model_exo::Type_Cible));
    m_type_exo->update_data(mps::model_base::All_Data_Change_Flag);
    connect(m_model,&model_exo::edit_exercice_model::data_changed,this,[this](const mps::model_base::node_index &index, flag role){
        if(index.cible() == model_exo::Type_Cible && index.is_root())
            m_type_exo->update_data(role);
    });
    //bouton
    m_save_bouton = new QPushButton(tr("Sauvegarder"));
    connect(m_save_bouton,&QPushButton::clicked,this,&edit_exercice_tab::sauver);

    // Calque
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_type_exo);
    m_main_layout->addWidget(m_view);
    m_main_layout->addWidget(m_save_bouton);
}

void edit_exercice_tab::become_current()
    {emit action_permise(0);}

void edit_exercice_tab::update_type() {
    auto types = m_model->data(m_model->index(mps::model_base::node_index(),0,model_exo::Type_Cible),
                               mps::model_base::Map_Role);

}

find_exercice_tab::find_exercice_tab(bdd_note & bd, tab_index pair, QWidget * parent)
    : abstract_tab_module_with_bdd(bd,pair,parent) {
    //model-view
    m_view = new mps::widget::node_view(std::make_unique<mps::widget::rounded_arc_painter>());
    m_model = new model_exo::read_exercice_model(static_cast<idt>(pair.second), bdd(),this);
    m_view->set_model(m_model);
    auto delegate = new mps::delegate::standard_node_delegate(this);
    m_view->set_delegate(delegate);
    //bouton
    m_select_bouton = new QPushButton(tr("Sélectionner"));
    connect(m_select_bouton,&QPushButton::clicked,this,[this](){
        if(m_view->selection_model()->has_selection())
            emit open_tab_requested({info_tab_note::Edit_Exercice_Tab_Id,
                                     m_view->selection_model()->current_index().data(mps::model_base::Id_Role).toInt()});
    });

    // Calque
    //m_exo_layout = new QVBoxLayout;
    //m_exo_layout->addWidget(m_view);
    m_find_layout = new QVBoxLayout;
    m_find_layout->addWidget(m_select_bouton);

    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(m_find_layout);
}

void find_exercice_tab::become_current()
    {emit action_permise(0);}
