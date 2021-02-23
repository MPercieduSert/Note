#include "ElevesTab.h"

using namespace noteMPS;

ElevesTab::ElevesTab(BddNote & bdd, tab_index pair, QWidget *parent)
    : abstract_tab_tableau(bdd,pair,parent) {
    //Model et vue
    m_model = new model_base::table_model(true,true,this);
    m_model->set_tableau(std::make_unique<EleveVecTableau>(bdd,bdd.get_list<Eleve>()));
    m_model->insertColonne(Nom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                EleveVecTableau::Nom,tr("Nom"),0});
    m_model->insertColonne(Prenom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0});
    m_model->insertColonne(Naissance,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                      EleveVecTableau::Naissance,tr("Date de Naissance"),0});
    m_model->insertColonne(Sexe,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable,
                                 EleveVecTableau::Sexe,tr("Sexe"),0});
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSortingEnabled(true);
    m_model->sort(Nom);
    m_view->horizontalHeader()->setSectionsMovable(true);
    //Recheche
    m_findWidget = new widget::find_widget(m_model);
    m_findLayout = new QVBoxLayout();
    m_findGroup = new QGroupBox(tr("Filtrer :"));
    m_findLayout->addWidget(m_findWidget);
    m_findGroup->setLayout(m_findLayout);
    //Ajout d'éléves
    m_addGroup = new QGroupBox(tr("Nombre d'éléves à ajouter :"));
    m_addLayout = new QHBoxLayout;
    m_addGroup->setLayout(m_addLayout);
    m_addGroup->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_addButton = new QPushButton(tr("Ajouter"));
    m_addSpinBox = new QSpinBox;
    connect(m_addButton,&QPushButton::clicked,this,[this](){m_model->insert_rows(m_model->row_count(),m_addSpinBox->value());});
    //Save
    m_save_bouton = new QPushButton(tr("Sauvegarder les éléves validés"));
    connect(m_save_bouton,&QPushButton::clicked,this,&ElevesTab::sauver);
    //Del
    m_delButton = new QPushButton(tr("Supprimer les éléves selectionnés"));
    connect(m_delButton,&QPushButton::clicked,this,&ElevesTab::supprimer);

    //Calque
    m_addLayout->addWidget(m_addSpinBox);
    m_addLayout->addWidget(m_addButton);
    m_secondLayout = new QVBoxLayout;
    m_secondLayout->addWidget(m_findGroup);
    m_secondLayout->addWidget(m_addGroup);
    m_secondLayout->addWidget(m_save_bouton);
    m_secondLayout->addWidget(m_delButton);
    m_secondLayout->addStretch();
    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(m_secondLayout);
}
