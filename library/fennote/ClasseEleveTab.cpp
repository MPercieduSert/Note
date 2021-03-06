#include "ClasseEleveTab.h"

using namespace noteMPS;

ClasseEleveTab::ClasseEleveTab(BddNote & bdd, std::pair<int,int> pair, QWidget *parent)
    : AbstractTabTableau(bdd,pair,parent) {
    m_classeSelect = new ClasseSelectWidget(bdd);
    m_model = new ClasseEleveModel(bdd,m_classeSelect->id(),this);
    connect(m_classeSelect,&ClasseSelectWidget::idChanged,static_cast<ClasseEleveModel*>(m_model),&ClasseEleveModel::setIdClasse);
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_view->setSortingEnabled(true);
    m_model->sort(ClasseEleveModel::NomColonne);
    m_view->horizontalHeader()->setSectionsMovable(true);

    // Bouton
    m_addButton = new QPushButton(tr("Ajouter à la classe"));
    connect(m_addButton,&QPushButton::clicked,this,[this](){
        auto selection = m_eleveView->selectionModel()->selectedRows();
        for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter)
            static_cast<ClasseEleveModel*>(m_model)->add(m_eleveModel->data(*iter,modelMPS::AbstractColonnesModel::IdRole).toUInt());
    });
    m_delButton = new QPushButton(tr("Retirer de la classe"));
    connect(m_delButton,&QPushButton::clicked,this,&ClasseEleveTab::supprimer);
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    connect(m_saveButton,&QPushButton::clicked,this,&ClasseEleveTab::sauver);

    // Eleve
    m_eleveModel = new modelMPS::TableModel(false,false,this);
    m_eleveModel->setTableau(std::make_unique<EleveVecTableau>(bdd,bdd.getList<Eleve>()));
    m_eleveModel->insertColonne(NomEl,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                EleveVecTableau::Nom,tr("Nom"),0});
    m_eleveModel->insertColonne(PrenomEl,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0});
    m_eleveModel->insertColonne(NaissanceEl,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                      EleveVecTableau::Naissance,tr("Date de Naissance"),0});
    m_eleveModel->insertColonne(SexeEl,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                 EleveVecTableau::Sexe,tr("Sexe"),0});
    m_eleveView = new QTableView;
    m_eleveView->setModel(m_eleveModel);
    m_eleveView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_eleveView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_eleveView->setSortingEnabled(true);
    m_eleveModel->sort(NomEl);
    m_eleveView->horizontalHeader()->setSectionsMovable(true);
    m_eleveFind = new widgetMPS::FindWidget(m_eleveModel);

    // Calque
    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->addWidget(m_addButton);
    m_buttonLayout->addWidget(m_delButton);
    m_buttonLayout->addWidget(m_saveButton);
    m_eleveLayout = new QHBoxLayout;
    m_eleveLayout->addWidget(m_eleveView);
    m_eleveLayout->addWidget(m_eleveFind);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_classeSelect);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addLayout(m_eleveLayout);
}
