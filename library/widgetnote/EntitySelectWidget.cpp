#include "EntitySelectWidget.h"

using namespace noteMPS;

AnneeSelectWidget::AnneeSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : AbstractEntitySelectWidget(bdd,orientation,parent) {
    m_label = new QLabel(tr("Annee scolaire : "));
    m_spinBox = new spin_box_anneeScolaire(m_bdd.get_list<Annee>(Annee::Num));
    m_spinBox->setNowValue();
    m_spinBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_spinBox,&spin_box_anneeScolaire::valueChanged,this,[this](){emit id_changed(id());});
    m_main_layout->addWidget(m_label);
    m_main_layout->addWidget(m_spinBox);
}

ClasseSelectWidget::ClasseSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : ComboBoxEntitySelectWidget(bdd,tr("Classe : "),orientation,parent) {
    m_anneeSelect = new AnneeSelectWidget(bdd);
    m_etabSelect = new EtablissementSelectWidget(bdd);
    setEnabledEtabClasse(true);
    connect(m_etabSelect,&EtablissementSelectWidget::id_changed,this,&ClasseSelectWidget::updateClasse);
    m_classe_layout = new QHBoxLayout;
    m_classe_layout->addWidget(m_label);
    m_classe_layout->addWidget(m_box);
    m_main_layout->addWidget(m_anneeSelect);
    m_main_layout->addWidget(m_etabSelect);
    m_main_layout->addLayout(m_classe_layout);
}

void ClasseSelectWidget::setEnabledEtabClasse(bool bb) {
    m_etabSelect->setEnabled(bb);
    m_label->setEnabled(bb);
    m_box->setEnabled(bb);
    m_anneeSelect->disconnect(this);
    if(bb) {
        updateClasse();
        connect(m_anneeSelect,&AnneeSelectWidget::id_changed,this,&ClasseSelectWidget::updateClasse);
    }
    else {
        m_box->clear();
        connect(m_anneeSelect,&AnneeSelectWidget::id_changed,this,&ClasseSelectWidget::idAnChanged);
    }
}

void ClasseSelectWidget::set_id(idt id) {
    if(m_box->isEnabled()) {
        Classe cl(id);
        if(m_bdd.get(cl)) {
            m_anneeSelect->set_id(cl.idAn());
            m_etabSelect->set_id(cl.idEtab());
            ComboBoxEntitySelectWidget::set_id(id);
        }
    }
}

void ClasseSelectWidget::updateClasse() {
    m_box->clear();
    if(m_anneeSelect->id() != 0 && m_etabSelect->id() != 0)
        m_box->add_text(m_bdd.get_list<Classe>(Classe::IdAn,m_anneeSelect->id(),Classe::IdEtab,m_etabSelect->id(),Classe::Nom),
                 [](const Classe & cl)->QString {return cl.nc();});
}

EleveSelectWidget::EleveSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : ComboBoxEntitySelectWidget(bdd,tr("Eleve : "),orientation,parent) {
    m_classeSelect = new ClasseSelectWidget(bdd,orientation);
    connect(m_classeSelect,&ClasseSelectWidget::id_changed,this,&EleveSelectWidget::updateEleve);
    m_eleve_layout = new QHBoxLayout;
    m_eleve_layout->addWidget(m_label);
    m_eleve_layout->addWidget(m_box);
    m_main_layout->addWidget(m_classeSelect);
    m_main_layout->addLayout(m_eleve_layout);
}

void EleveSelectWidget::set_id(idt id) {
    auto list = m_bdd.get_list<ClasseEleve>(ClasseEleve::IdEleve,id,ClasseEleve::IdClasse);
    if(!list.empty())
        m_classeSelect->set_id(list.back().idClasse());
    ComboBoxEntitySelectWidget::set_id(id);
}

void EleveSelectWidget::updateEleve() {
    m_box->clear();
    if(m_classeSelect->id() != 0)
        m_box->add_text(m_bdd.get_list<Eleve,ClasseEleve>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,m_classeSelect->id(),Eleve::Nom),
                 [](const Eleve & el)->QString {return QString(el.nom()).append(" ")
                                                                        .append(el.prenom())
                                                                        .append(" (")
                                                                        .append(el.date().to_string("dd/MM/yyyy"))
                                                                        .append(")");});
}

GroupeSelectWidget::GroupeSelectWidget(b2d::Bdd & bdd, Qt::Orientations orientation, QWidget * parent)
    : ComboBoxEntitySelectWidget(bdd,tr("Groupe : "),orientation,parent) {
    m_catLabel = new QLabel("Catégorie :");
    m_catCB = new QComboBox;
    m_catCB->addItem("Année",false);
    m_catCB->addItem("Classe",true);
    m_catCB->setCurrentIndex(1);
    m_catCB->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_classeSelect = new ClasseSelectWidget(bdd,orientation);
    m_classeSelect->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_type_label = new QLabel("Type :");
    m_type_cb = new widget::id_combo_box;
    m_type_cb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    updateType();
    update_groupe();
    catChange();
    connect(m_catCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeSelectWidget::catChange);
    connect(m_type_cb,qOverload<int>(&QComboBox::currentIndexChanged),this,&GroupeSelectWidget::update_groupe);

    //Calque
    m_catLayout = new QHBoxLayout;
    m_catLayout->addWidget(m_catLabel);
    m_catLayout->addWidget(m_catCB);
    m_type_layout = new QHBoxLayout;
    m_type_layout->addWidget(m_type_label);
    m_type_layout->addWidget(m_type_cb);
    m_groupe_layout = new QHBoxLayout;
    m_groupe_layout->addWidget(m_label);
    m_groupe_layout->addWidget(m_box);

    m_main_layout->addLayout(m_catLayout);
    m_main_layout->addWidget(m_classeSelect);
    m_main_layout->addLayout(m_type_layout);
    m_main_layout->addLayout(m_groupe_layout);
}

void GroupeSelectWidget::catChange() {
    auto bb = m_catCB->currentData().toBool();
    m_classeSelect->setEnabledEtabClasse(bb);
    m_classeSelect->disconnect(this);
    if(bb) {
        connect(m_classeSelect,&ClasseSelectWidget::id_changed,this,&GroupeSelectWidget::updateType);
        connect(m_classeSelect,&ClasseSelectWidget::id_changed,this,&GroupeSelectWidget::update_groupe);
    }
    else {
        connect(m_classeSelect,&ClasseSelectWidget::idAnChanged,this,&GroupeSelectWidget::updateType);
        connect(m_classeSelect,&ClasseSelectWidget::idAnChanged,this,&GroupeSelectWidget::update_groupe);
    }
    updateType();
}

void GroupeSelectWidget::set_id(idt id){
    Groupe gr(id);
    if(m_bdd.get(gr)) {
        if(gr.idAn()) {
            m_catCB->setCurrentIndex(0);
            m_classeSelect->set_idAn(gr.idAn());
        }
        else {
            m_catCB->setCurrentIndex(1);
            m_classeSelect->set_id(gr.idClasse());
        }
        m_type_cb->set_current_index_id(gr.type());
        ComboBoxEntitySelectWidget::set_id(id);
    }
}

void GroupeSelectWidget::update_groupe() {
    m_box->clear();
    if(m_catCB->currentData().toBool())
        m_box->add_text(m_bdd.get_list<Groupe>(Groupe::IdClasse,m_classeSelect->id(),Groupe::Type,m_type_cb->id(),Groupe::Nom),
                      [](const Groupe & groupe)->QString
                            {return QString(groupe.nom()).append(" (").append(groupe.nc()).append(")");});
    else
        m_box->add_text(m_bdd.get_list<Groupe>(Groupe::IdAn,m_classeSelect->idAn(),Groupe::Type,m_type_cb->id(),Groupe::Nom),
                      [](const Groupe & groupe)->QString
                            {return QString(groupe.nom()).append(" (").append(groupe.nc()).append(")");});
}

void GroupeSelectWidget::updateType() {
    m_type_cb->clear();
    std::map<type_permission::position,QVariant> where;
    where[type_permission::Cible] = bdd().cible<Groupe>();
    std::vector<std::pair<Type::position,bool>> order;
    order.push_back({Type::Nom,true});
    auto vec = m_bdd.get_list<Type,type_permission>(Type::Id,type_permission::id_type,std::map<Type::position,QVariant>(),where,order);
    std::map<Groupe::position,QVariant> map;
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
    m_type_cb->add_text(vec,[](const Type & type)->QString {return QString(type.nom()).append(" (").append(type.nc()).append(")");});
}

void NiveauxSelectWidget::set_idEtab(idt id) {
    m_box->clear();
    if(id == 0)
        m_box->add_text(m_bdd.get_list<Niveau>(Niveau::Nom),
                     [](const Niveau & niv)->QString {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
    else
        m_box->add_text(m_bdd.get_list<Niveau,EtablissementNiveau>(EtablissementNiveau::IdNiveau,EtablissementNiveau::IdEtab,id),
                     [](const Niveau & niv)->QString {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
}
