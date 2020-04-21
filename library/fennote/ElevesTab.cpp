#include "ElevesTab.h"

using namespace noteMPS;

ElevesTab::ElevesTab(BddNote & bdd, std::pair<int,int> pair, QWidget *parent)
    : AbstractTabTableau(bdd,pair,parent) {
    //Model et vue
    m_model = new EleveModel(bdd,this);
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSortingEnabled(true);
    m_model->sort(EleveModel::Nom);
    m_view->horizontalHeader()->setSectionsMovable(true);
    //Recheche
    m_findGroup = new QGroupBox(tr("Rechercher"));
    m_findLayout = new QGridLayout;
    m_findGroup->setLayout(m_findLayout);
    m_apresLabel = new QLabel(tr("Né après le :"));
    m_avantLabel = new QLabel(tr("Né avant le :"));
    m_apresEdit = new QDateEdit;
    m_avantEdit = new QDateEdit;
    m_apresEdit->setCalendarPopup(true);
    m_avantEdit->setCalendarPopup(true);
    m_apresEdit->setDate(model()->min(EleveModel::Naissance).toDate());
    m_avantEdit->setDate(model()->max(EleveModel::Naissance).toDate());
    m_nomLabel = new QLabel(tr("Nom :"));
    m_nomEdit = new QLineEdit;
    m_prenomLabel = new QLabel(tr("Prenom :"));
    m_prenomEdit = new QLineEdit;
    m_sexeLabel = new QLabel(tr("Sexe :"));
    m_sexeFemBox = new QCheckBox(tr("F"));
    m_sexeMasBox = new QCheckBox(tr("M"));
    m_sexeFemBox->setCheckState(Qt::Checked);
    m_sexeMasBox->setCheckState(Qt::Checked);
    m_resetFindButton = new QPushButton(tr("Réinitialiser"));
    connect(m_nomEdit,&QLineEdit::editingFinished,this,&ElevesTab::find);
    connect(m_prenomEdit,&QLineEdit::editingFinished,this,&ElevesTab::find);
    connect(m_apresEdit,&QDateEdit::dateChanged,this,&ElevesTab::find);
    connect(m_avantEdit,&QDateEdit::dateChanged,this,&ElevesTab::find);
    connect(m_sexeFemBox,&QCheckBox::stateChanged,this,&ElevesTab::find);
    connect(m_sexeMasBox,&QCheckBox::stateChanged,this,&ElevesTab::find);
    connect(m_resetFindButton,&QPushButton::clicked,this,
            [this](){m_findEnabled = false;
                model()->resetRow();
                m_nomEdit->clear();
                m_prenomEdit->clear();
                m_apresEdit->setDate(model()->min(EleveModel::Naissance).toDate());
                m_avantEdit->setDate(model()->max(EleveModel::Naissance).toDate());
                m_sexeFemBox->setCheckState(Qt::Checked);
                m_sexeMasBox->setCheckState(Qt::Checked);
                m_findEnabled = true;});
    //Ajout d'éléves
    m_addGroup = new QGroupBox(tr("Nombre d'éléves à ajouter :"));
    m_addLayout = new QHBoxLayout;
    m_addGroup->setLayout(m_addLayout);
    m_addGroup->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_addButton = new QPushButton(tr("Ajouter"));
    m_addSpinBox = new QSpinBox;
    connect(m_addButton,&QPushButton::clicked,this,[this](){m_model->insertRows(-1,m_addSpinBox->value());});
    //Save
    m_saveButton = new QPushButton(tr("Sauvegarder les éléves validés"));
    connect(m_saveButton,&QPushButton::clicked,this,&ElevesTab::save);
    //Del
    m_delButton = new QPushButton(tr("Supprimer les éléves selectionnés"));
    connect(m_delButton,&QPushButton::clicked,this,&ElevesTab::remove);

    //Calque
    m_addLayout->addWidget(m_addSpinBox);
    m_addLayout->addWidget(m_addButton);
    m_findLayout->addWidget(m_nomLabel,0,0);
    m_findLayout->addWidget(m_prenomLabel,1,0);
    m_findLayout->addWidget(m_apresLabel,2,0);
    m_findLayout->addWidget(m_avantLabel,3,0);
    m_findLayout->addWidget(m_sexeLabel,4,0);
    m_findLayout->addWidget(m_nomEdit,0,1,1,2);
    m_findLayout->addWidget(m_prenomEdit,1,1,1,2);
    m_findLayout->addWidget(m_apresEdit,2,1,1,2);
    m_findLayout->addWidget(m_avantEdit,3,1,1,2);
    m_findLayout->addWidget(m_sexeFemBox,4,1);
    m_findLayout->addWidget(m_sexeMasBox,4,2);
    m_findLayout->addWidget(m_resetFindButton,5,0,1,3);
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

void ElevesTab::find() {
    if(m_findEnabled) {
        EleveModel::FindEleves fe;
        fe.nom.setPattern(m_nomEdit->text());
        fe.prenom.setPattern(m_prenomEdit->text());
        fe.apres = m_apresEdit->date();
        fe.avant = m_avantEdit->date();
        fe.femme = m_sexeFemBox->isChecked();
        fe.homme = m_sexeMasBox->isChecked();
        model()->find(fe);
    }
}

void ElevesTab::remove(){
    if(model()->removeRowsSelected(m_view->selectionModel()->selectedIndexes()))
        m_view->selectionModel()->clear();
    else
        QMessageBox::warning(this,tr("Erreur de sélection"),
                             tr("Vous devez séléctioner des lignes entières pour supprimer des éléves"));
}
