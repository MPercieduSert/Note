#include "eleve_tab.h"

using namespace note_mps;

/////////////////////////////////////////// eleve_tab /////////////////////////////////////////////////
eleves_tab::eleves_tab(bdd_note & bdd, tab_index pair, QWidget *parent)
    : abstract_tab_tableau(bdd,pair,parent) {
    //Model et vue
    m_model = new mps::model_base::table_model(true,true,this);
    m_model->set_tableau(std::make_unique<eleve_vec_tableau>(bdd,bdd.get_list<eleve>()));
    m_model->insert_colonne(Nom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                eleve_vec_tableau::Nom,tr("Nom"),0});
    m_model->insert_colonne(Prenom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                   eleve_vec_tableau::Prenom,tr("Prenom"),0});
    m_model->insert_colonne(Naissance,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                      eleve_vec_tableau::Naissance,tr("Date de Naissance"),0});
    m_model->insert_colonne(Sexe,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable,
                                 eleve_vec_tableau::Sexe,tr("Sexe"),0});
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSortingEnabled(true);
    m_model->sort(Nom);
    m_view->horizontalHeader()->setSectionsMovable(true);
    //Recheche
    m_find_widget = new mps::widget::find_widget(m_model);
    //m_find_widget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    m_find_layout = new QVBoxLayout();
    m_find_group = new QGroupBox(tr("Filtrer :"));
    m_find_layout->addWidget(m_find_widget);
    m_find_group->setLayout(m_find_layout);
    //Ajout d'éléves
    m_add_group = new QGroupBox(tr("Nombre d'éléves à ajouter :"));
    m_addLayout = new QHBoxLayout;
    m_add_group->setLayout(m_addLayout);
    m_add_group->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_add_bouton = new QPushButton(tr("Ajouter"));
    m_add_spin_box = new QSpinBox;
    connect(m_add_bouton,&QPushButton::clicked,this,[this]()
        {m_model->insertRows(m_model->rowCount(),m_add_spin_box->value());});
    //Save
    m_save_bouton = new QPushButton(tr("Sauvegarder les éléves validés"));
    connect(m_save_bouton,&QPushButton::clicked,this,&eleves_tab::sauver);
    //Del
    m_del_bouton = new QPushButton(tr("Supprimer les éléves selectionnés"));
    connect(m_del_bouton,&QPushButton::clicked,this,&eleves_tab::supprimer);

    //Calque
    m_addLayout->addWidget(m_add_spin_box);
    m_addLayout->addWidget(m_add_bouton);
    m_second_layout = new QVBoxLayout;
    m_second_layout->addWidget(m_find_group,1);
    m_second_layout->addWidget(m_add_group);
    m_second_layout->addWidget(m_save_bouton);
    m_second_layout->addWidget(m_del_bouton);
    m_second_layout->addStretch();
    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(m_second_layout);
}

/////////////////////////////////////////// classe_eleve_tab //////////////////////////////////////////
classe_eleve_tab::classe_eleve_tab(bdd_note & bdd, tab_index pair, QWidget *parent)
    : abstract_tab_tableau(bdd,pair,parent) {
    m_classeSelect = new classe_select_widget(bdd);
    m_model = new classe_eleve_model(bdd,m_classeSelect->id(),this);
    connect(m_classeSelect,&classe_select_widget::id_changed,
            static_cast<classe_eleve_model*>(m_model),&classe_eleve_model::set_id_classe);
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_view->setSortingEnabled(true);
    m_model->sort(classe_eleve_model::Nom_Colonne);
    m_view->horizontalHeader()->setSectionsMovable(true);

    // Bouton
    m_add_bouton = new QPushButton(tr("Ajouter à la classe"));
    connect(m_add_bouton,&QPushButton::clicked,this,[this](){
        auto selection = m_eleveView->selectionModel()->selectedRows();
        for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter)
            static_cast<classe_eleve_model*>(m_model)->add(
                    m_eleveModel->data(*iter,mps::model_base::abstract_colonnes_model::Id_Role).toUInt());
    });
    m_del_bouton = new QPushButton(tr("Retirer de la classe"));
    connect(m_del_bouton,&QPushButton::clicked,this,&classe_eleve_tab::supprimer);
    m_save_bouton = new QPushButton(tr("Sauvegarder"));
    connect(m_save_bouton,&QPushButton::clicked,this,&classe_eleve_tab::sauver);

    // Eleve
    m_eleveModel = new mps::model_base::table_model(false,false,this);
    m_eleveModel->set_tableau(std::make_unique<eleve_vec_tableau>(bdd,bdd.get_list<eleve>()));
    m_eleveModel->insert_colonne(NomEl,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                eleve_vec_tableau::Nom,tr("Nom"),0});
    m_eleveModel->insert_colonne(PrenomEl,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   eleve_vec_tableau::Prenom,tr("Prenom"),0});
    m_eleveModel->insert_colonne(NaissanceEl,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                      eleve_vec_tableau::Naissance,tr("Date de Naissance"),0});
    m_eleveModel->insert_colonne(SexeEl,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                 eleve_vec_tableau::Sexe,tr("Sexe"),0});
    m_eleveView = new QTableView;
    m_eleveView->setModel(m_eleveModel);
    m_eleveView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_eleveView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_eleveView->setSortingEnabled(true);
    m_eleveModel->sort(NomEl);
    m_eleveView->horizontalHeader()->setSectionsMovable(true);
    m_eleveFind = new mps::widget::find_widget(m_eleveModel);

    // Calque
    m_bouton_layout = new QHBoxLayout;
    m_bouton_layout->addWidget(m_add_bouton);
    m_bouton_layout->addWidget(m_del_bouton);
    m_bouton_layout->addWidget(m_save_bouton);
    m_eleve_layout = new QHBoxLayout;
    m_eleve_layout->addWidget(m_eleveView);
    m_eleve_layout->addWidget(m_eleveFind);
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_classeSelect);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(m_bouton_layout);
    m_main_layout->addLayout(m_eleve_layout);
}
