#include "GroupeTab.h"

using namespace noteMPS;

GroupeTab::GroupeTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent)
    : AbstractTabTableau(bdd,pair,parent) {
    // Choix du groupe.
    m_groupeGroup = new QGroupBox("Sélection du groupe :");
    m_catLabel = new QLabel("Catégorie :");
    m_catLabel->setAlignment(Qt::AlignRight);
    m_catCB = new QComboBox;
    m_catCB->addItem("Année",false);
    m_catCB->addItem("Classe",true);
    m_catCB->setCurrentIndex(1);
    m_anLabel = new QLabel("Année :");
    m_anLabel->setAlignment(Qt::AlignRight);
    m_anSB = new SpinBoxAnneeScolaire(bdd.getList<Annee>(Annee::Num));
    m_anSB->setNowValue();
    m_etabLabel = new QLabel(tr("Établissement :"));
    m_etabLabel->setAlignment(Qt::AlignRight);
    m_etabCB = new widgetMPS::IdComboBox;
    m_etabCB->addText(m_bdd.getList<Etablissement>(Etablissement::Nom),
                      [](const Etablissement & etab)->QString
                            {return QString(etab.nom()).append(" (").append(etab.nc()).append(")");});
    m_classeLabel = new QLabel("Classe :");
    m_classeLabel->setAlignment(Qt::AlignRight);
    m_classeCB = new widgetMPS::IdComboBox;
    m_typeLabel = new QLabel("Type :");
    m_typeLabel->setAlignment(Qt::AlignRight);
    m_typeCB = new widgetMPS::IdComboBox;
    m_groupeLabel = new QLabel("Groupe :");
    m_groupeLabel->setAlignment(Qt::AlignRight);
    m_groupeCB = new widgetMPS::IdComboBox;

    // Model et vue.
    m_model = new EleveGroupeModel(bdd,this);
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_candidatView = new QTableView;
    m_candidatView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_candidatView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_candidatView->setSortingEnabled(true);
    m_candidatView->horizontalHeader()->setSectionsMovable(true);
    m_candidatView->horizontalHeader()->setSectionsClickable(true);

    // Recheche.
    m_findWidget = new widgetMPS::FindWidget();
    m_findLayout = new QVBoxLayout;
    m_findGroup = new QGroupBox(tr("Filtrer :"));
    m_findLayout->addWidget(m_findWidget);
    m_findGroup->setLayout(m_findLayout);

    // Boutons
    m_delEleveButton = new QPushButton(tr("Retirer les éléves"));
    m_addGroupeButton = new QPushButton(tr("Ajouter un groupe"));
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_propGrLabel = new QLabel(tr("Propriétés du groupe :"));
    m_exclusifCheck = new QCheckBox(tr("Exclusif"));
    m_exclusifCheck->setDisabled(true);
    m_totalCheck = new QCheckBox(tr("Total"));
    m_totalCheck->setDisabled(true);

    // Peuple et connect
    updateClasse();
    updateType();
    updateGroupe();
    updateEleve();
    catChange();
    connect(m_catCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeTab::catChange);
    connect(m_etabCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeTab::updateClasse);
    connect(m_typeCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeTab::updateGroupe);
    connect(m_groupeCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeTab::updateEleve);
    connect(m_addGroupeButton,&QPushButton::clicked,this,&GroupeTab::addGroupe);
    connect(m_delEleveButton,&QPushButton::clicked,this,&GroupeTab::remove);
    connect(m_saveButton,&QPushButton::clicked,this,&GroupeTab::sauver);
    connect(m_view->horizontalHeader(),&QHeaderView::sectionClicked,this,&GroupeTab::affecte);


    // Calque
    m_groupeLayout = new QHBoxLayout;
    m_groupeLayout->addWidget(m_catLabel);
    m_groupeLayout->addWidget(m_catCB);
    m_groupeLayout->addWidget(m_anLabel);
    m_groupeLayout->addWidget(m_anSB);
    m_groupeLayout->addWidget(m_etabLabel);
    m_groupeLayout->addWidget(m_etabCB);
    m_groupeLayout->addWidget(m_classeLabel);
    m_groupeLayout->addWidget(m_classeCB);
    m_groupeLayout->addWidget(m_typeLabel);
    m_groupeLayout->addWidget(m_typeCB);
    m_groupeLayout->addWidget(m_groupeLabel);
    m_groupeLayout->addWidget(m_groupeCB);
    m_eleveLayout = new QHBoxLayout;
    m_eleveLayout->addWidget(m_candidatView);
    m_eleveLayout->addWidget(m_findGroup);
    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->addWidget(m_addGroupeButton);
    m_buttonLayout->addWidget(m_delEleveButton);
    m_buttonLayout->addWidget(m_saveButton);
    m_buttonLayout->addWidget(m_propGrLabel);
    m_buttonLayout->addWidget(m_exclusifCheck);
    m_buttonLayout->addWidget(m_totalCheck);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addLayout(m_groupeLayout);
    m_mainLayout->addLayout(m_eleveLayout);
    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addWidget(m_view);
}

void GroupeTab::affecte(int column) {
    if(column >= 0) {
        auto col = static_cast<szt>(column);
        auto selection = m_candidatView->selectionModel()->selectedRows();
        std::list<szt> listId;
        std::map<szt,std::forward_list<szt>> delGrId;
        if(static_cast<EleveGroupeModel&>(*m_model).groupe().code().test(Groupe::Exclusif)){
            m_candidatView->selectionModel()->clear();
            for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter) {
                auto num = m_candidatModel->data(iter->siblingAtColumn(CandidatGroupeModel::GroupeColonne),
                                                 CandidatGroupeModel::NumRole).toInt();
                if(num != column) {
                    auto id = m_candidatModel->data(*iter,modelMPS::AbstractColonnesModel::IdRole).toUInt();
                    listId.push_front(id);
                    m_candidatModel->setData(iter->siblingAtColumn(CandidatGroupeModel::GroupeColonne),col);
                    if(num >= 0)
                        delGrId[static_cast<szt>(num)].push_front(id);
                }
            }
        }
        else {
            for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter) {
                auto listNum = m_candidatModel->data(iter->siblingAtColumn(CandidatGroupeModel::GroupeColonne),
                                                     CandidatGroupeModel::NumRole).toList();
                auto iterNum = listNum.cbegin();
                while(iterNum != listNum.cend() && iterNum->toUInt() < col)
                    ++iterNum;
                if(iterNum == listNum.cend() || iterNum->toUInt() != col) {
                    listId.push_front(m_candidatModel->data(*iter,modelMPS::AbstractColonnesModel::IdRole).toUInt());
                    m_candidatModel->setData(iter->siblingAtColumn(CandidatGroupeModel::GroupeColonne),col);
                }
            }
        }
        static_cast<EleveGroupeModel&>(*m_model).updateEleve(listId,col,delGrId);
    }
}

void GroupeTab::becomeCurrent()
    {emit actionPermise(fenMPS::EffacerAction|fenMPS::SauverAction);}

void GroupeTab::catChange() {
    auto bb = m_catCB->currentData().toBool();
    m_etabCB->setEnabled(bb);
    m_classeCB->setEnabled(bb);
    if(bb) {
        m_anSB->disconnect(this);
        updateClasse();
        connect(m_anSB,&SpinBoxAnneeScolaire::valueChanged,this,&GroupeTab::updateClasse);
        connect(m_classeCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeTab::updateType);
        connect(m_classeCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeTab::updateGroupe);
        updateType();
    }
    else {
        m_anSB->disconnect(this);
        m_classeCB->disconnect(this);
        m_classeCB->clear();
        connect(m_anSB,&SpinBoxAnneeScolaire::valueChanged,this,&GroupeTab::updateType);
        connect(m_anSB,&SpinBoxAnneeScolaire::valueChanged,this,&GroupeTab::updateGroupe);
        updateType();
    }
}

void GroupeTab::remove(){
    auto selection = m_view->selectionModel()->selectedIndexes();
    m_candidatModel->remove(static_cast<EleveGroupeModel&>(*m_model).remove(selection));
}

void GroupeTab::updateClasse() {
    m_classeCB->clear();
    m_classeCB->addText(m_bdd.getList<Classe>(Classe::IdAn,m_anSB->value().id(),Classe::IdEtab,m_etabCB->id(),Classe::Nom),
                      [](const Classe & cl)->QString
                            {return QString(cl.nom()).append(" (").append(cl.nc()).append(")");});
}

void GroupeTab::updateEleve() {
    auto model = m_candidatModel;
    m_candidatModel = new CandidatGroupeModel(bdd(),m_groupeCB->id(),this);
    m_candidatModel->sort(CandidatGroupeModel::NomColonne);
    m_findWidget->setModel(m_candidatModel);
    m_candidatView->setModel(m_candidatModel);
    if(model)
        delete model;
    static_cast<EleveGroupeModel&>(*m_model).setIdGroupe(m_groupeCB->id());
    m_exclusifCheck->setChecked(static_cast<EleveGroupeModel&>(*m_model).groupe().code().test(Groupe::Exclusif));
    m_totalCheck->setChecked(static_cast<EleveGroupeModel&>(*m_model).groupe().code().test(Groupe::Total));
}

void GroupeTab::updateGroupe() {
    m_groupeCB->clear();
    if(m_catCB->currentData().toBool())
        m_groupeCB->addText(m_bdd.getList<Groupe>(Groupe::IdClasse,m_classeCB->id(),Groupe::Type,m_typeCB->id(),Groupe::Nom),
                      [](const Groupe & groupe)->QString
                            {return QString(groupe.nom()).append(" (").append(groupe.nc()).append(")");});
    else
        m_groupeCB->addText(m_bdd.getList<Groupe>(Groupe::IdAn,m_anSB->value().id(),Groupe::Type,m_typeCB->id(),Groupe::Nom),
                      [](const Groupe & groupe)->QString
                            {return QString(groupe.nom()).append(" (").append(groupe.nc()).append(")");});
}

void GroupeTab::updateType() {
    m_typeCB->clear();
    std::map<TypePermission::Position,QVariant> where;
    where[TypePermission::Cible] = bdd().cible<Groupe>();
    std::vector<std::pair<Type::Position,bool>> order;
    order.push_back({Type::Nom,true});
    auto vec = m_bdd.getList<Type,TypePermission>(Type::Id,TypePermission::IdType,std::map<Type::Position,QVariant>(),where,order);
    std::map<Groupe::Position,QVariant> map;
    if(m_catCB->currentData().toBool())
        map[Groupe::IdClasse] = m_classeCB->id();
    else
        map[Groupe::IdAn] = m_anSB->value().id();
    auto iter = vec.cbegin();
    while(iter != vec.cend()) {
        map[Groupe::Type] = iter->id();
        if(bdd().exists<Groupe>(map))
            ++iter;
        else
            vec.erase(iter);
    }
    m_typeCB->addText(vec,[](const Type & type)->QString {return QString(type.nom()).append(" (").append(type.nc()).append(")");});
}
