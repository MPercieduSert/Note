#include "EntitySelectWidget.h"

using namespace noteMPS;

AnneeSelectWidget::AnneeSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : AbstractEntitySelectWidget(bdd,orientation,parent) {
    m_label = new QLabel(tr("Annee scolaire : "));
    m_spinBox = new SpinBoxAnneeScolaire(m_bdd.getList<Annee>(Annee::Num));
    m_spinBox->setNowValue();
    m_spinBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_spinBox,&SpinBoxAnneeScolaire::valueChanged,this,[this](){emit idChanged(id());});
    m_mainLayout->addWidget(m_label);
    m_mainLayout->addWidget(m_spinBox);
}

ClasseSelectWidget::ClasseSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : ComboBoxEntitySelectWidget(bdd,tr("Classe : "),orientation,parent) {
    m_anneeSelect = new AnneeSelectWidget(bdd);
    m_etabSelect = new EtablissementSelectWidget(bdd);
    setEnabledEtabClasse(true);
    connect(m_etabSelect,&EtablissementSelectWidget::idChanged,this,&ClasseSelectWidget::updateClasse);
    m_classeLayout = new QHBoxLayout;
    m_classeLayout->addWidget(m_label);
    m_classeLayout->addWidget(m_box);
    m_mainLayout->addWidget(m_anneeSelect);
    m_mainLayout->addWidget(m_etabSelect);
    m_mainLayout->addLayout(m_classeLayout);
}

void ClasseSelectWidget::setEnabledEtabClasse(bool bb) {
    m_etabSelect->setEnabled(bb);
    m_label->setEnabled(bb);
    m_box->setEnabled(bb);
    m_anneeSelect->disconnect(this);
    if(bb) {
        updateClasse();
        connect(m_anneeSelect,&AnneeSelectWidget::idChanged,this,&ClasseSelectWidget::updateClasse);
    }
    else {
        m_box->clear();
        connect(m_anneeSelect,&AnneeSelectWidget::idChanged,this,&ClasseSelectWidget::idAnChanged);
    }
}

void ClasseSelectWidget::updateClasse() {
    m_box->clear();
    if(m_anneeSelect->id() != 0 && m_etabSelect->id() != 0)
        m_box->addText(m_bdd.getList<Classe>(Classe::IdAn,m_anneeSelect->id(),Classe::IdEtab,m_etabSelect->id(),Classe::Nom),
                 [](const Classe & cl)->QString {return cl.nc();});
}

GroupeSelectWidget::GroupeSelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : ComboBoxEntitySelectWidget(bdd,tr("Groupe : "),orientation,parent) {
    m_catLabel = new QLabel("Catégorie :");
    m_catCB = new QComboBox;
    m_catCB->addItem("Année",false);
    m_catCB->addItem("Classe",true);
    m_catCB->setCurrentIndex(1);
    m_catCB->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_classeSelect = new ClasseSelectWidget(bdd,orientation);
    m_classeSelect->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_typeLabel = new QLabel("Type :");
    m_typeCB = new widgetMPS::IdComboBox;
    m_typeCB->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    updateType();
    updateGroupe();
    catChange();
    connect(m_catCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeSelectWidget::catChange);
    connect(m_typeCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeSelectWidget::updateGroupe);

    //Calque
    m_catLayout = new QHBoxLayout;
    m_catLayout->addWidget(m_catLabel);
    m_catLayout->addWidget(m_catCB);
    m_typeLayout = new QHBoxLayout;
    m_typeLayout->addWidget(m_typeLabel);
    m_typeLayout->addWidget(m_typeCB);
    m_groupeLayout = new QHBoxLayout;
    m_groupeLayout->addWidget(m_label);
    m_groupeLayout->addWidget(m_box);

    m_mainLayout->addLayout(m_catLayout);
    m_mainLayout->addWidget(m_classeSelect);
    m_mainLayout->addLayout(m_typeLayout);
    m_mainLayout->addLayout(m_groupeLayout);
}

void GroupeSelectWidget::catChange() {
    auto bb = m_catCB->currentData().toBool();
    m_classeSelect->setEnabledEtabClasse(bb);
    m_classeSelect->disconnect(this);
    if(bb) {
        connect(m_classeSelect,&ClasseSelectWidget::idChanged,this,&GroupeSelectWidget::updateType);
        connect(m_classeSelect,&ClasseSelectWidget::idChanged,this,&GroupeSelectWidget::updateGroupe);
    }
    else {
        connect(m_classeSelect,&ClasseSelectWidget::idAnChanged,this,&GroupeSelectWidget::updateType);
        connect(m_classeSelect,&ClasseSelectWidget::idAnChanged,this,&GroupeSelectWidget::updateGroupe);
    }
    updateType();
}

void GroupeSelectWidget::updateGroupe() {
    m_box->clear();
    if(m_catCB->currentData().toBool())
        m_box->addText(m_bdd.getList<Groupe>(Groupe::IdClasse,m_classeSelect->id(),Groupe::Type,m_typeCB->id(),Groupe::Nom),
                      [](const Groupe & groupe)->QString
                            {return QString(groupe.nom()).append(" (").append(groupe.nc()).append(")");});
    else
        m_box->addText(m_bdd.getList<Groupe>(Groupe::IdAn,m_classeSelect->idAn(),Groupe::Type,m_typeCB->id(),Groupe::Nom),
                      [](const Groupe & groupe)->QString
                            {return QString(groupe.nom()).append(" (").append(groupe.nc()).append(")");});
}

void GroupeSelectWidget::updateType() {
    m_typeCB->clear();
    std::map<TypePermission::Position,QVariant> where;
    where[TypePermission::Cible] = bdd().cible<Groupe>();
    std::vector<std::pair<Type::Position,bool>> order;
    order.push_back({Type::Nom,true});
    auto vec = m_bdd.getList<Type,TypePermission>(Type::Id,TypePermission::IdType,std::map<Type::Position,QVariant>(),where,order);
    std::map<Groupe::Position,QVariant> map;
    if(m_catCB->currentData().toBool())
        map[Groupe::IdClasse] = m_classeSelect->id();
    else
        map[Groupe::IdAn] = m_classeSelect->idAn();
    auto iter = vec.cbegin();
    while(iter != vec.cend()) {
        map[Groupe::Type] = iter->id();
        if(m_bdd.exists<Groupe>(map))
            ++iter;
        else
            vec.erase(iter);
    }
    m_typeCB->addText(vec,[](const Type & type)->QString {return QString(type.nom()).append(" (").append(type.nc()).append(")");});
}

void NiveauxSelectWidget::setIdEtab(idt id) {
    m_box->clear();
    if(id == 0)
        m_box->addText(m_bdd.getList<Niveau>(Niveau::Nom),
                     [](const Niveau & niv)->QString {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
    else
        m_box->addText(m_bdd.getList<Niveau,EtablissementNiveau>(EtablissementNiveau::IdNiveau,EtablissementNiveau::IdEtab,id),
                     [](const Niveau & niv)->QString {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
}
