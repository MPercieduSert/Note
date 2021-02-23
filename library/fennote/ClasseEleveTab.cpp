#include "ClasseEleveTab.h"

using namespace noteMPS;

ClasseEleveTab::ClasseEleveTab(BddNote & bdd, std::pair<int,int> pair, QWidget *parent)
    : AbstractTabTableau(bdd,pair,parent) {
    m_classeSelect = new ClasseSelectWidget(bdd);
    m_model = new ClasseEleveModel(bdd,m_classeSelect->id(),this);
    connect(m_classeSelect,&ClasseSelectWidget::id_changed,static_cast<ClasseEleveModel*>(m_model),&ClasseEleveModel::set_idClasse);
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setselection_mode(QAbstractItemView::Extended_Selection);
    m_view->setSortingEnabled(true);
    m_model->sort(ClasseEleveModel::NomColonne);
    m_view->horizontalHeader()->setSectionsMovable(true);

    // Bouton
    m_addButton = new QPushButton(tr("Ajouter à la classe"));
    connect(m_addButton,&QPushButton::clicked,this,[this](){
        auto selection = m_eleveView->selectionModel()->selectedRows();
        for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter)
            static_cast<ClasseEleveModel*>(m_model)->add(m_eleveModel->data(*iter,model_base::abstract_colonnes_model::Id_Role).toUInt());
    });
    m_delButton = new QPushButton(tr("Retirer de la classe"));
    connect(m_delButton,&QPushButton::clicked,this,&ClasseEleveTab::supprimer);
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    connect(m_saveButton,&QPushButton::clicked,this,&ClasseEleveTab::sauver);

    // Eleve
    m_eleveModel = new model_base::table_model(false,false,this);
    m_eleveModel->set_tableau(std::make_unique<EleveVecTableau>(bdd,bdd.get_list<Eleve>()));
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
    m_eleveView->setselection_mode(QAbstractItemView::Extended_Selection);
    m_eleveView->setSortingEnabled(true);
    m_eleveModel->sort(NomEl);
    m_eleveView->horizontalHeader()->setSectionsMovable(true);
    m_eleveFind = new widget::find_widget(m_eleveModel);

    // Calque
    m_bouton_layout = new QHBoxLayout;
    m_bouton_layout->addWidget(m_addButton);
    m_bouton_layout->addWidget(m_delButton);
    m_bouton_layout->addWidget(m_saveButton);
    m_eleve_layout = new QHBoxLayout;
    m_eleve_layout->addWidget(m_eleveView);
    m_eleve_layout->addWidget(m_eleveFind);
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_classeSelect);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(m_bouton_layout);
    m_main_layout->addLayout(m_eleve_layout);
}
