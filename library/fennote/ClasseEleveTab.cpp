#include "ClasseEleveTab.h"

using namespace noteMPS;

ClasseEleveTab::ClasseEleveTab(BddNote & bdd, std::pair<int,int> pair, QWidget *parent)
    : AbstractTabTableau(bdd,pair,parent) {
    // Classe
    m_anLabel = new QLabel(tr("Année de la classe :"));
    m_classeLabel = new QLabel(tr("Classe :"));
    m_anSpinBox = new SpinBoxAnneeScolaire(bdd.getList<Annee>(Annee::Num));
    m_anSpinBox->setNowValue();
    m_classeComboBox = new widgetMPS::IdComboBox;
    updateClasseListe();
    connect(m_anSpinBox,&SpinBoxAnneeScolaire::valueChanged,this,&ClasseEleveTab::updateClasseListe);

    m_model = new modelMPS::TableModel(false,false,this);
    m_model->setTableau(std::make_unique<EleveVecTableau>(bdd,bdd.getList<Eleve,ClasseEleve>(ClasseEleve::IdEleve,
                                                                                             ClasseEleve::IdClasse,m_classeComboBox->id())));
    m_model->insertColonne(Nom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                EleveVecTableau::Nom,tr("Nom"),0});
    m_model->insertColonne(Prenom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0});
    m_model->insertColonne(Naissance,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                      EleveVecTableau::Naissance,tr("Date de Naissance"),0});
    m_model->insertColonne(Sexe,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                 EleveVecTableau::Sexe,tr("Sexe"),0});
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_view->setSortingEnabled(true);
    m_model->sort(Nom);
    m_view->horizontalHeader()->setSectionsMovable(true);

    // Bouton
    m_addButton = new QPushButton(tr("Ajouter à la classe"));
    m_delButton = new QPushButton(tr("Retirer de la classe"));

    // Eleve
    m_eleveModel = new modelMPS::TableModel(false,false,this);
    m_eleveModel->setTableau(std::make_unique<EleveVecTableau>(bdd,bdd.getList<Eleve>()));
    m_eleveModel->insertColonne(Nom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                EleveVecTableau::Nom,tr("Nom"),0});
    m_eleveModel->insertColonne(Prenom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0});
    m_eleveModel->insertColonne(Naissance,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                      EleveVecTableau::Naissance,tr("Date de Naissance"),0});
    m_eleveModel->insertColonne(Sexe,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                 EleveVecTableau::Sexe,tr("Sexe"),0});
    m_eleveView = new QTableView;
    m_eleveView->setModel(m_eleveModel);
    m_eleveView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_eleveView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_eleveView->setSortingEnabled(true);
    m_eleveModel->sort(Nom);
    m_eleveView->horizontalHeader()->setSectionsMovable(true);
    m_eleveFind = new widgetMPS::FindWidget;
    m_eleveFind->setModel(m_eleveModel);

    // Calque
    m_classeLayout = new QHBoxLayout;
    m_classeLayout->addWidget(m_anLabel);
    m_classeLayout->addWidget(m_anSpinBox);
    m_classeLayout->addWidget(m_classeLabel);
    m_classeLayout->addWidget(m_classeComboBox);
    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->addWidget(m_addButton);
    m_buttonLayout->addWidget(m_delButton);
    m_eleveLayout = new QHBoxLayout;
    m_eleveLayout->addWidget(m_eleveView);
    m_eleveLayout->addWidget(m_eleveFind);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addLayout(m_classeLayout);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addLayout(m_eleveLayout);
}

void ClasseEleveTab::updateClasseListe(){
    m_classeComboBox->clear();
    m_classeComboBox->addText(bdd().getList<Classe>(Classe::IdAn,m_anSpinBox->value().id(),Classe::Nom),
                              [](const Classe & cl)->QString{return QString(cl.nom()).append(" (").append(cl.nc()).append(")");});
}
