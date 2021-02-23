#include "GroupeTab.h"

using namespace noteMPS;

GroupeTab::GroupeTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent)
    : AbstractTabTableau(bdd,pair,parent) {
    // Choix du groupe.
    m_groupeSelect = new GroupeSelectWidget(bdd);

    // Model et vue.
    m_model = new Eleve_groupeModel(bdd,this);
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_view->setselection_mode(QAbstractItemView::Extended_Selection);
    m_candidatView = new QTableView;
    m_candidatView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_candidatView->setselection_mode(QAbstractItemView::Extended_Selection);
    m_candidatView->setSortingEnabled(true);
    m_candidatView->horizontalHeader()->setSectionsMovable(true);
    m_candidatView->horizontalHeader()->setSectionsClickable(true);

    // Recheche.
    m_findWidget = new widget::find_widget();
    m_findLayout = new QVBoxLayout;
    m_findGroup = new QGroupBox(tr("Filtrer :"));
    m_findLayout->addWidget(m_findWidget);
    m_findGroup->setLayout(m_findLayout);

    // Boutons
    m_delEleveButton = new QPushButton(tr("Retirer les éléves"));
    m_add_groupeButton = new QPushButton(tr("Ajouter un groupe"));
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_propGrLabel = new QLabel(tr("Propriétés du groupe :"));
    m_exclusifCheck = new QCheckBox(tr("Exclusif"));
    m_exclusifCheck->setDisabled(true);
    m_totalCheck = new QCheckBox(tr("Total"));
    m_totalCheck->setDisabled(true);

    // Peuple et connect
    updateEleve();
    connect(m_groupeSelect,&GroupeSelectWidget::id_changed,this,&GroupeTab::updateEleve);
    connect(m_add_groupeButton,&QPushButton::clicked,this,&GroupeTab::add_groupe);
    connect(m_delEleveButton,&QPushButton::clicked,this,&GroupeTab::remove);
    connect(m_saveButton,&QPushButton::clicked,this,&GroupeTab::sauver);
    connect(m_view->horizontalHeader(),&QHeaderView::sectionClicked,this,&GroupeTab::affecte);


    // Calque
    m_eleve_layout = new QHBoxLayout;
    m_eleve_layout->addWidget(m_candidatView);
    m_eleve_layout->addWidget(m_findGroup);
    m_bouton_layout = new QHBoxLayout;
    m_bouton_layout->addWidget(m_add_groupeButton);
    m_bouton_layout->addWidget(m_delEleveButton);
    m_bouton_layout->addWidget(m_saveButton);
    m_bouton_layout->addWidget(m_propGrLabel);
    m_bouton_layout->addWidget(m_exclusifCheck);
    m_bouton_layout->addWidget(m_totalCheck);
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_groupeSelect);
    m_main_layout->addLayout(m_eleve_layout);
    m_main_layout->addLayout(m_bouton_layout);
    m_main_layout->addWidget(m_view);
}

void GroupeTab::affecte(int column) {
    if(column >= 0) {
        auto col = static_cast<numt>(column);
        auto selection = m_candidatView->selectionModel()->selectedRows();
        std::list<idt> listId;
        std::map<szt,std::forward_list<idt>> delGrId;
        if(static_cast<Eleve_groupeModel&>(*m_model).groupe().code().test(Groupe::Exclusif)){
            m_candidatView->selectionModel()->clear();
            for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter) {
                auto num = m_candidatModel->data(iter->siblingAtColumn(CandidatGroupeModel::GroupeColonne),
                                                 CandidatGroupeModel::Num_Role).toInt();
                if(num != column) {
                    auto id = m_candidatModel->data(*iter,model_base::abstract_colonnes_model::Id_Role).toUInt();
                    listId.push_front(id);
                    m_candidatModel->set_data(iter->siblingAtColumn(CandidatGroupeModel::GroupeColonne),col);
                    if(num >= 0)
                        delGrId[static_cast<szt>(num)].push_front(id);
                }
            }
        }
        else {
            for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter) {
                auto listNum = m_candidatModel->data(iter->siblingAtColumn(CandidatGroupeModel::GroupeColonne),
                                                     CandidatGroupeModel::Num_Role).toList();
                auto iterNum = listNum.cbegin();
                while(iterNum != listNum.cend() && iterNum->toUInt() < col)
                    ++iterNum;
                if(iterNum == listNum.cend() || iterNum->toUInt() != col) {
                    listId.push_front(m_candidatModel->data(*iter,model_base::abstract_colonnes_model::Id_Role).toUInt());
                    m_candidatModel->set_data(iter->siblingAtColumn(CandidatGroupeModel::GroupeColonne),col);
                }
            }
        }
        static_cast<Eleve_groupeModel&>(*m_model).updateEleve(listId,col,delGrId);
    }
}

void GroupeTab::becomeCurrent()
    {emit actionPermise(fenMPS::EffacerAction|fenMPS::SauverAction);}

void GroupeTab::remove(){
    auto selection = m_view->selectionModel()->selectedIndexes();
    m_candidatModel->remove(static_cast<Eleve_groupeModel&>(*m_model).remove(selection));
}

void GroupeTab::updateEleve() {
    auto model = m_candidatModel;
    m_candidatModel = new CandidatGroupeModel(bdd(),m_groupeSelect->id(),this);
    m_candidatModel->sort(CandidatGroupeModel::NomColonne);
    m_findWidget->setModel(m_candidatModel);
    m_candidatView->setModel(m_candidatModel);
    if(model)
        delete model;
    static_cast<Eleve_groupeModel&>(*m_model).set_id_groupe(m_groupeSelect->id());
    m_exclusifCheck->setChecked(static_cast<Eleve_groupeModel&>(*m_model).groupe().code().test(Groupe::Exclusif));
    m_totalCheck->setChecked(static_cast<Eleve_groupeModel&>(*m_model).groupe().code().test(Groupe::Total));
}
