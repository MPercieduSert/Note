#include "ElevesTab.h"

using namespace noteMPS;

ElevesTab::ElevesTab(BddNote & bdd, std::pair<int,int> pair, QWidget *parent)
    : AbstractTabTableau(bdd,pair,parent) {
    //Model et vue
    m_model = new modelMPS::TableModel(true,true,this);
    m_model->setTableau(std::make_unique<EleveVecTableau>(bdd,bdd.getList<Eleve>()));
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
    m_findWidget = new widgetMPS::FindWidget(m_model);
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
    connect(m_addButton,&QPushButton::clicked,this,[this](){m_model->insertRows(m_model->rowCount(),m_addSpinBox->value());});
    //Save
    m_saveButton = new QPushButton(tr("Sauvegarder les éléves validés"));
    connect(m_saveButton,&QPushButton::clicked,this,&ElevesTab::sauver);
    //Del
    m_delButton = new QPushButton(tr("Supprimer les éléves selectionnés"));
    connect(m_delButton,&QPushButton::clicked,this,&ElevesTab::supprimer);

    //Calque
    m_addLayout->addWidget(m_addSpinBox);
    m_addLayout->addWidget(m_addButton);
    m_secondLayout = new QVBoxLayout;
    m_secondLayout->addWidget(m_findGroup);
    m_secondLayout->addWidget(m_addGroup);
    m_secondLayout->addWidget(m_saveButton);
    m_secondLayout->addWidget(m_delButton);
    m_secondLayout->addStretch();
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_secondLayout);
}
