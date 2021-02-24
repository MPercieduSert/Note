#include "entity_select_widget.h"

using namespace note_mps;

annee_select_widget::annee_select_widget(mps::b2d::bdd &bdd, Qt::Orientations orientation, QWidget *parent)
    : abstract_entity_select_widget(bdd,orientation,parent) {
    m_label = new QLabel(tr("annee scolaire : "));
    m_spin_box = new spin_box_annee_scolaire(m_bdd.get_list<annee>(annee::Num));
    m_spin_box->setNowValue();
    m_spin_box->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_spin_box,&spin_box_annee_scolaire::value_changed,this,[this](){emit id_changed(id());});
    m_main_layout->addWidget(m_label);
    m_main_layout->addWidget(m_spin_box);
}

classe_select_widget::classe_select_widget(mps::b2d::bdd & bdd, Qt::Orientations orientation, QWidget *parent)
    : combo_box_entity_select_widget(bdd,tr("Classe : "),orientation,parent) {
    m_annee_select = new annee_select_widget(bdd);
    m_etab_select = new etablissement_select_widget(bdd);
    set_enabled_etab_classe(true);
    connect(m_etab_select,&etablissement_select_widget::id_changed,this,&classe_select_widget::update_classe);
    m_classe_layout = new QHBoxLayout;
    m_classe_layout->addWidget(m_label);
    m_classe_layout->addWidget(m_box);
    m_main_layout->addWidget(m_annee_select);
    m_main_layout->addWidget(m_etab_select);
    m_main_layout->addLayout(m_classe_layout);
}

void classe_select_widget::set_enabled_etab_classe(bool bb) {
    m_etab_select->setEnabled(bb);
    m_label->setEnabled(bb);
    m_box->setEnabled(bb);
    m_annee_select->disconnect(this);
    if(bb) {
        update_classe();
        connect(m_annee_select,&annee_select_widget::id_changed,this,&classe_select_widget::update_classe);
    }
    else {
        m_box->clear();
        connect(m_annee_select,&annee_select_widget::id_changed,this,&classe_select_widget::id_an_changed);
    }
}

void classe_select_widget::set_id(idt id) {
    if(m_box->isEnabled()) {
        classe cl(id);
        if(m_bdd.get(cl)) {
            m_annee_select->set_id(cl.id_an());
            m_etab_select->set_id(cl.id_etab());
            combo_box_entity_select_widget::set_id(id);
        }
    }
}

void classe_select_widget::update_classe() {
    m_box->clear();
    if(m_annee_select->id() != 0 && m_etab_select->id() != 0)
        m_box->add_text(m_bdd.get_list<classe>(classe::Id_An,m_annee_select->id(),classe::Id_Etab,m_etab_select->id(),classe::Nom),
                 [](const classe & cl)->QString {return cl.nc();});
}

eleve_select_widget::eleve_select_widget(mps::b2d::bdd & bdd, Qt::Orientations orientation, QWidget *parent)
    : combo_box_entity_select_widget(bdd,tr("eleve : "),orientation,parent) {
    m_classe_select = new classe_select_widget(bdd,orientation);
    connect(m_classe_select,&classe_select_widget::id_changed,this,&eleve_select_widget::update_eleve);
    m_eleve_layout = new QHBoxLayout;
    m_eleve_layout->addWidget(m_label);
    m_eleve_layout->addWidget(m_box);
    m_main_layout->addWidget(m_classe_select);
    m_main_layout->addLayout(m_eleve_layout);
}

void eleve_select_widget::set_id(idt id) {
    auto list = m_bdd.get_list<classe_eleve>(classe_eleve::Id_Eleve,id,classe_eleve::Id_Classe);
    if(!list.empty())
        m_classe_select->set_id(list.back().id_classe());
    combo_box_entity_select_widget::set_id(id);
}

void eleve_select_widget::update_eleve() {
    m_box->clear();
    if(m_classe_select->id() != 0)
        m_box->add_text(m_bdd.get_list<eleve,classe_eleve>(classe_eleve::Id_Eleve,
                                                           classe_eleve::Id_Classe,m_classe_select->id(),eleve::Nom),
                 [](const eleve & el)->QString {return QString(el.nom()).append(" ")
                                                                        .append(el.prenom())
                                                                        .append(" (")
                                                                        .append(el.date().toString("dd/MM/yyyy"))
                                                                        .append(")");});
}

groupe_select_widget::groupe_select_widget(mps::b2d::bdd & bdd, Qt::Orientations orientation, QWidget *parent)
    : combo_box_entity_select_widget(bdd,tr("Groupe : "),orientation,parent) {
    m_cat_label = new QLabel("Catégorie :");
    m_catCB = new QComboBox;
    m_catCB->addItem("Année",false);
    m_catCB->addItem("Classe",true);
    m_catCB->setCurrentIndex(1);
    m_catCB->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_classe_select = new classe_select_widget(bdd,orientation);
    m_classe_select->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_type_label = new QLabel("Type :");
    m_type_cb = new mps::widget::id_combo_box;
    m_type_cb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    update_type();
    update_groupe();
    cat_change();
    connect(m_catCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&groupe_select_widget::cat_change);
    connect(m_type_cb,qOverload<int>(&QComboBox::currentIndexChanged),this,&groupe_select_widget::update_groupe);

    //Calque
    m_cat_layout = new QHBoxLayout;
    m_cat_layout->addWidget(m_cat_label);
    m_cat_layout->addWidget(m_catCB);
    m_type_layout = new QHBoxLayout;
    m_type_layout->addWidget(m_type_label);
    m_type_layout->addWidget(m_type_cb);
    m_groupe_layout = new QHBoxLayout;
    m_groupe_layout->addWidget(m_label);
    m_groupe_layout->addWidget(m_box);

    m_main_layout->addLayout(m_cat_layout);
    m_main_layout->addWidget(m_classe_select);
    m_main_layout->addLayout(m_type_layout);
    m_main_layout->addLayout(m_groupe_layout);
}

void groupe_select_widget::cat_change() {
    auto bb = m_catCB->currentData().toBool();
    m_classe_select->set_enabled_etab_classe(bb);
    m_classe_select->disconnect(this);
    if(bb) {
        connect(m_classe_select,&classe_select_widget::id_changed,this,&groupe_select_widget::update_type);
        connect(m_classe_select,&classe_select_widget::id_changed,this,&groupe_select_widget::update_groupe);
    }
    else {
        connect(m_classe_select,&classe_select_widget::id_an_changed,this,&groupe_select_widget::update_type);
        connect(m_classe_select,&classe_select_widget::id_an_changed,this,&groupe_select_widget::update_groupe);
    }
    update_type();
}

void groupe_select_widget::set_id(idt id){
    groupe gr(id);
    if(m_bdd.get(gr)) {
        if(gr.id_an()) {
            m_catCB->setCurrentIndex(0);
            m_classe_select->set_id_an(gr.id_an());
        }
        else {
            m_catCB->setCurrentIndex(1);
            m_classe_select->set_id(gr.id_classe());
        }
        m_type_cb->set_current_index_id(gr.type());
        combo_box_entity_select_widget::set_id(id);
    }
}

void groupe_select_widget::update_groupe() {
    m_box->clear();
    if(m_catCB->currentData().toBool())
        m_box->add_text(m_bdd.get_list<groupe>(groupe::Id_Classe,m_classe_select->id(),groupe::Type,m_type_cb->id(),groupe::Nom),
                      [](const groupe & gr)->QString
                            {return QString(gr.nom()).append(" (").append(gr.nc()).append(")");});
    else
        m_box->add_text(m_bdd.get_list<groupe>(groupe::Id_An,m_classe_select->id_an(),groupe::Type,m_type_cb->id(),groupe::Nom),
                      [](const groupe & gr)->QString
                            {return QString(gr.nom()).append(" (").append(gr.nc()).append(")");});
}

void groupe_select_widget::update_type() {
    m_type_cb->clear();
    std::map<type_permission::position,QVariant> where;
    where[type_permission::Cible] = bdd().cible<groupe>();
    std::vector<std::pair<type::position,bool>> order;
    order.push_back({type::Nom,true});
    auto vec = m_bdd.get_list<type,type_permission>(type::Id,type_permission::Id_Type,std::map<type::position,QVariant>(),
                                                    where,order);
    std::map<groupe::position,QVariant> map;
    if(m_catCB->currentData().toBool())
        map[groupe::Id_Classe] = m_classe_select->id();
    else
        map[groupe::Id_An] = m_classe_select->id_an();
    auto it = vec.cbegin();
    while(it != vec.cend()) {
        map[groupe::Type] = it->id();
        if(m_bdd.exists<groupe>(map))
            ++it;
        else
            vec.erase(it);
    }
    m_type_cb->add_text(vec,[](const type & tp)->QString {return QString(tp.nom()).append(" (").append(tp.nc()).append(")");});
}

void niveaux_select_widget::set_id_etab(idt id) {
    m_box->clear();
    if(id == 0)
        m_box->add_text(m_bdd.get_list<niveau>(niveau::Nom),
                     [](const niveau & niv)->QString {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
    else
        m_box->add_text(m_bdd.get_list<niveau,etablissement_niveau>(etablissement_niveau::Id_Niveau,
                                                                    etablissement_niveau::Id_Etab,id),
                     [](const niveau & niv)->QString {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
}
