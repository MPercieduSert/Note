#include "NewModifForm.h"

using namespace noteMPS;

// AbstractControle
AbstractControleNewModifForm::AbstractControleNewModifForm(b2d::Bdd &bdd, const QString & LabelParent,
                                                           const QString &labelNc, const QString &labelNom,
                                                           bool new_ent, QWidget * parent)
    : AbstractParentNcNomNewModifForm(bdd, {"nom","nom abrégé"},
                                      LabelParent,
                                      labelNc,
                                      labelNom,
                                      new_ent,parent) {
    //note
    m_noteGr = new QGroupBox(tr("Notation :"));
    m_noteCheck = new QCheckBox(tr("Devoir noté"));
    m_type_label = new QLabel(tr("Type de notation :"));
    m_chiffreRadio = new QRadioButton(tr("Nombre"));
    m_chiffreRadio->setChecked(true);
    m_lettreRadio = new QRadioButton(tr("Lettre"));
    m_totalLabel = new QLabel;
    m_totalSpinBox = new widget::spin_box_lettre;
    m_depassementCheck = new QCheckBox(tr("Depassement"));
    m_decimalLabel = new QLabel;
    m_decimale_cb = new QComboBox;
    m_courbeCheck = new QCheckBox(tr("Courbe d'ajustement"));
    m_classementCheck = new QCheckBox(tr("Classement"));
    m_barreCheck = new QCheckBox(tr("Barre de classement :"));
    m_minimaSpinBox = new widget::spin_box_decimale;
    barreEnable();
    classementEnable();
    typeNoteChange();
    noteEnable();

    //option
    m_optGr = new QGroupBox(tr("Options :"));
    m_appreciationCheck = new QCheckBox(tr("Appréciation"));
    m_capaciteCheck = new QCheckBox(tr("Capacités"));
    m_commentaireCheck = new QCheckBox(tr("Commentaire"));
    m_competenceCheck = new QCheckBox(tr("Compétences"));

    //Calque
    m_note_layout = new QGridLayout(m_noteGr);
    m_note_layout->addWidget(m_noteCheck,CheckNoteLigne,LabelColonne);
    m_note_layout->addWidget(m_type_label,TypeLigne,LabelColonne);
    m_note_layout->addWidget(m_chiffreRadio,TypeLigne,ColonneUne);
    m_note_layout->addWidget(m_lettreRadio,TypeLigne,ColonneDeux);
    m_note_layout->addWidget(m_totalLabel,TotalLigne,LabelColonne);
    m_note_layout->addWidget(m_totalSpinBox,TotalLigne,ColonneUne);
    m_note_layout->addWidget(m_depassementCheck,TotalLigne,ColonneDeux);
    m_note_layout->addWidget(m_decimalLabel,DecimaleLigne,LabelColonne);
    m_note_layout->addWidget(m_decimale_cb,DecimaleLigne,ColonneUne);
    m_note_layout->addWidget(m_courbeCheck,DecimaleLigne,ColonneDeux);
    m_note_layout->addWidget(m_classementCheck,ClassementLigne,ColonneZero);
    m_note_layout->addWidget(m_barreCheck,BarreLigne,LabelColonne);
    m_note_layout->addWidget(m_minimaSpinBox,BarreLigne,ColonneUne);

    m_optLayout = new QGridLayout(m_optGr);
    m_optLayout->addWidget(m_appreciationCheck,AppreciationCommentaireLigne,ColonneZero);
    m_optLayout->addWidget(m_commentaireCheck,AppreciationCommentaireLigne,ColonneUne);
    m_optLayout->addWidget(m_competenceCheck,CapaciteCompetenceLigne,ColonneZero);
    m_optLayout->addWidget(m_capaciteCheck,CapaciteCompetenceLigne,ColonneUne);

    m_main_layout->addWidget(m_noteGr);
    m_main_layout->addWidget(m_optGr);
}

void AbstractControleNewModifForm::barreEnable() {
    auto enable = m_barreCheck->isChecked();
    m_minimaSpinBox->setEnabled(enable);
    if(enable) {
        decimaleChange();
        totalChange();
        m_minimaSpinBox->set_value_double(m_totalSpinBox->value() / 2.);
        connect(m_decimale_cb,qOverload<int>(&QComboBox::currentIndexChanged),this,&TypeControleNewModifForm::decimaleChange);
        connect(m_totalSpinBox,qOverload<int>(&widget::spin_box_lettre::valueChanged),this,&TypeControleNewModifForm::totalChange);
    }
    else {
        disconnect(m_decimale_cb,nullptr,this,nullptr);
        disconnect(m_totalSpinBox,nullptr,this,nullptr);
        m_minimaSpinBox->clear();
    }

}

void AbstractControleNewModifForm::classementEnable() {
    auto enable = m_classementCheck->isChecked();
    m_barreCheck->setEnabled(enable);
    if(!enable)
        m_barreCheck->setChecked(false);
}

void AbstractControleNewModifForm::connexion() {
    AbstractParentNcNomNewModifForm::connexion();
    connect(m_noteCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::noteEnable);
    connect(m_chiffreRadio,&QRadioButton::toggled,this,&TypeControleNewModifForm::typeNoteChange);
    connect(m_classementCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::classementEnable);
    connect(m_barreCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::barreEnable);
}

void AbstractControleNewModifForm::noteEnable() {
    auto enable = m_noteCheck->isChecked();
    m_type_label->setEnabled(enable);
    m_chiffreRadio->setEnabled(enable);
    m_lettreRadio->setEnabled(enable);
    m_totalLabel->setEnabled(enable);
    m_totalSpinBox->setEnabled(enable);
    m_depassementCheck->setEnabled(enable);
    m_decimalLabel->setEnabled(enable);
    m_decimale_cb->setEnabled(enable);
    m_courbeCheck->setEnabled(enable);
    if(enable)
        typeNoteChange();
    else {
        m_chiffreRadio->setChecked(true);
        m_depassementCheck->setChecked(false);
        m_classementCheck->setEnabled(false);
        m_classementCheck->setChecked(false);
        m_courbeCheck->setChecked(false);
        m_totalSpinBox->clear();
        m_decimale_cb->clear();
    }
}

void AbstractControleNewModifForm::typeNoteChange() {
    using attDecimale = attributMPS::attribut_decimale;
    if(m_chiffreRadio->isChecked()) {
        m_totalLabel->set_text("Total :");
        m_totalSpinBox->set_style(divers::num_to_string::Arabe);
        m_totalSpinBox->setMinimum(1);
        m_totalSpinBox->setMaximum(1000);
        m_totalSpinBox->set_value(20);
        m_depassementCheck->setEnabled(m_noteCheck->isChecked());
        m_courbeCheck->setEnabled(m_noteCheck->isChecked());
        m_decimalLabel->set_text(tr("Décimale :"));
        m_decimale_cb->clear();
        for (szt i = 0; i != attDecimale::Nbr_Values; ++i)
            m_decimale_cb->addItem(QString::number(1./ attDecimale::Decimale.at(i),'f',attDecimale::Precision.at(i)),
                                  attDecimale::Decimale.at(i));
        m_classementCheck->setEnabled(m_noteCheck->isChecked());
    }
    else {
        m_totalLabel->set_text("De A à :");
        m_totalSpinBox->set_style(divers::num_to_string::Majuscule);
        m_totalSpinBox->setMinimum(0);
        m_totalSpinBox->setMaximum(25);
        m_totalSpinBox->set_value(3);
        m_depassementCheck->setEnabled(false);
        m_depassementCheck->setChecked(false);
        m_courbeCheck->setEnabled(false);
        m_courbeCheck->setChecked(false);
        m_decimalLabel->set_text(tr("Ajustement :"));
        m_classementCheck->setEnabled(false);
        m_classementCheck->setChecked(false);
        m_decimale_cb->clear();
        m_decimale_cb->addItem(" ",Controle::NoPlus);
        m_decimale_cb->addItem("+",Controle::Plus);
        m_decimale_cb->addItem("++",Controle::PPlus);
        m_decimale_cb->addItem("+++",Controle::PPPlus);
    }
}

// Annee
AnneeNewModifForm::AnneeNewModifForm(b2d::Bdd &bdd, bool new_ent, QWidget * parent)
    : dialogue::abstract_new_modif_form (bdd,new_ent,parent) {
    // Annee
    m_annee_label = new QLabel(new_ent ? tr("Nouvelle année scolaire :")
                                     : tr("Année scolaire courante :"));
    if(new_ent) {
        m_spinBox = new spin_box_anneeScolaire();
        m_anneeCourante = new QCheckBox(tr("Année courante"));
    }
    else
        m_spinBox = new spin_box_anneeScolaire(m_bdd.get_list<Annee>(Annee::Num));
    m_spinBox->setNowValue();

    // Calque
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_annee_label);
    if(m_new) {
        m_main_layout->addWidget(m_spinBox);
        m_main_layout->addWidget(m_anneeCourante);
    }
    else if (!m_spinBox->values().empty())
        m_main_layout->addWidget(m_spinBox);
    else
        m_annee_label->set_text("Vous devez préalablement créer une année scolaire dans le menu :\n "
                              "fichier>Nouveau>Année scolaire");
}

void AnneeNewModifForm::connexion() {
    if(m_new){
        connect(m_spinBox,&spin_box_anneeScolaire::valueChanged,this,[this](){emit save_permis(!m_bdd.exists_unique(m_spinBox->value()));});
        emit save_permis(!m_bdd.exists_unique(m_spinBox->value()));
    }
    else if (m_spinBox->values().empty())
        emit save_permis(false);
}

void AnneeNewModifForm::save() {
    if(m_new)
        m_bdd.save(m_spinBox->value());
}

// Classe
ClasseNewModifForm::ClasseNewModifForm(b2d::Bdd &bdd, bool new_ent, QWidget * parent)
    : dialogue::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom abrégé de la classe :"),
                                            tr("Nom de la classe :"),
                                            new_ent,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.get_list<Classe>(Classe::Nom));

    // Annee - Etab - Niveaux
    m_anneeSelect = new AnneeSelectWidget(m_bdd);
    m_etabSelect = new EtablissementSelectWidget(m_bdd);
    m_nivSelect = new NiveauxSelectWidget(m_bdd);

    // Num
    m_num_label = new QLabel(tr("Numéro :"));
    m_numSpinBox = new QSpinBox();
    m_numSpinBox->setMinimum(0);
    m_numSpinBox->setSpecialValueText(" ");

    // Debut et fin des cours
    m_debutLabel = new QLabel(tr("Début des cours:"));
    m_finLabel = new QLabel(tr("Fin des cours:"));
    m_debutCalendar = new QCalendarWidget;
    m_finCalendar = new QCalendarWidget;

    // Calque
    m_main_layout->addWidget(m_anneeSelect);
    m_main_layout->addWidget(m_etabSelect);
    m_main_layout->addWidget(m_nivSelect);
    m_main_layout->addWidget(m_num_label);
    m_main_layout->addWidget(m_numSpinBox);
    m_debut_layout = new QVBoxLayout;
    m_debut_layout->addWidget(m_debutLabel);
    m_debut_layout->addWidget(m_debutCalendar);
    m_fin_layout = new QVBoxLayout;
    m_fin_layout->addWidget(m_finLabel);
    m_fin_layout->addWidget(m_finCalendar);
    m_calendarLayout = new QHBoxLayout;
    m_calendarLayout->addLayout(m_debut_layout);
    m_calendarLayout->addLayout(m_fin_layout);
    m_main_layout->addLayout(m_calendarLayout);

}

void ClasseNewModifForm::connexion() {
    AbstractNcNomNewModifForm::connexion();
    connect(m_etabSelect,&EtablissementSelectWidget::id_changed,m_nivSelect,&NiveauxSelectWidget::set_idEtab);
    connect(m_anneeSelect,&AnneeSelectWidget::id_changed,this,&ClasseNewModifForm::updateCalendar);
    connect(m_debutCalendar,&QCalendarWidget::selectionChanged,this,[this](){emit save_permis(valide());});
    connect(m_finCalendar,&QCalendarWidget::selectionChanged,this,[this](){emit save_permis(valide());});
}

void ClasseNewModifForm::save() {
    Classe cl;
    if(!m_new)
        cl.set_id(id());
    cl.setNc(nc());
    cl.setNom(nom());
    cl.set_idAn(m_anneeSelect->id());
    cl.set_idEtab(m_etabSelect->id());
    cl.set_idNiveau(m_nivSelect->id());
    cl.set_num(m_numSpinBox->value());
    m_bdd.save(cl);
    if(m_new)
        m_idClasse = cl.id();
    donnee_cible dnCb;
    dnCb.set_iddonnee(bdd().ref_to_id<donnee>("date_defaut_dn"));
    dnCb.set_id_cible(m_idClasse);
    dnCb.set_cible(bdd().cible<Classe>());
    dnCb.set_num(donnee::EntreeNum);
    bdd().get_unique(dnCb);
    dnCb.setValeur(m_debutCalendar->selectedDate());
    bdd().save(dnCb);
    dnCb.set_id(0);
    dnCb.set_num(donnee::SortieNum);
    bdd().get_unique(dnCb);
    dnCb.setValeur(m_finCalendar->selectedDate());
    bdd().save(dnCb);
}

void ClasseNewModifForm::updateCalendar() {
    if(m_new) {
        auto an = m_anneeSelect->num();
        m_debutCalendar->setDateRange(QDate(an,9,1),QDate(an+1,8,31));
        m_debutCalendar->setSelectedDate(QDate(an,9,1));
        m_finCalendar->setDateRange(QDate(an,9,1),QDate(an+1,8,31));
        m_finCalendar->setSelectedDate(QDate(an+1,8,31));
    }
}

void ClasseNewModifForm::update_data() {
    if(m_new) {
        m_nivSelect->set_idEtab(m_etabSelect->id());
        updateCalendar();
    }
    else {
        Classe cl;
        update_temp<Classe>(cl);
        m_idClasse = cl.id();
        m_anneeSelect->set_id(cl.idAn());
        m_etabSelect->set_id(cl.idEtab());
        m_nivSelect->set_idEtab(cl.idEtab());
        m_nivSelect->set_id(cl.idNiveau());
        m_numSpinBox->set_value(cl.num());
        donnee_cible dnCb;
        dnCb.set_iddonnee(bdd().ref_to_id<donnee>("date_defaut_dn"));
        dnCb.set_id_cible(m_idClasse);
        dnCb.set_cible(bdd().cible<Classe>());
        dnCb.set_num(donnee::EntreeNum);
        bdd().get_unique(dnCb);
        m_debutCalendar->setSelectedDate(dnCb.valeur().toDate());
        dnCb.set_id(0);
        dnCb.set_num(donnee::SortieNum);
        bdd().get_unique(dnCb);
        m_finCalendar->setSelectedDate(dnCb.valeur().toDate());
    }
}

bool ClasseNewModifForm::valide() const {
    return AbstractNcNomNewModifForm::valide()
            && m_nivSelect->id() && m_etabSelect->id()
            && m_debutCalendar->selectedDate() < m_finCalendar->selectedDate();
}

// Controle
ControleNewModifForm::ControleNewModifForm(b2d::Bdd &bdd, bool new_ent, QWidget * parent)
    : AbstractControleNewModifForm(bdd,
                                      "Type :",
                                      "Nom abrégé du controle :",
                                      "Nom du controle :",
                                      new_ent,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.get_list<Controle>(Controle::Nom));

    // Type
    m_parent_tree->set_tree_ref(bdd.getArbre<TypeControle>(),
                          [](const TypeControle & tc)->QTreeWidgetItem * {
        auto item = new QTreeWidgetItem({tc.nom(),tc.nc()});
        item->set_data(widget::tree_widget::Id_Colonne,widget::tree_widget::Id_Role,tc.id());
        if(!tc.code().test(TypeControle::Categorie))
            item->set_flags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        else
            item->set_flags(Qt::ItemIsEnabled);
        return item;
    });

    // Destination
    m_classeRadio = new QRadioButton(tr("Classe"));
    m_classeRadio->setChecked(true);
    m_eleveRadio = new QRadioButton(tr("Eleve"));
    m_groupeRadio = new QRadioButton(tr("Groupe"));
    m_pourBG = new QButtonGroup(this);
    m_pourBG->addButton(m_classeRadio,static_cast<b2d::bdd_predef&>(m_bdd).cible(Classe::ID));
    m_pourBG->addButton(m_eleveRadio,static_cast<b2d::bdd_predef&>(m_bdd).cible(Eleve::ID));
    m_pourBG->addButton(m_groupeRadio,static_cast<b2d::bdd_predef&>(m_bdd).cible(Groupe::ID));
    m_entitySelect = new ClasseSelectWidget(m_bdd,Qt::Vertical);
    m_cibleEntitySelect = static_cast<b2d::bdd_predef&>(m_bdd).cible(Classe::ID);
    m_pourGr = new QGroupBox("Pour :");
    m_pourButton_layout = new QHBoxLayout;
    m_pourButton_layout->addWidget(m_classeRadio);
    m_pourButton_layout->addWidget(m_groupeRadio);
    m_pourButton_layout->addWidget(m_eleveRadio);
    m_pourLayout = new QVBoxLayout(m_pourGr);
    m_pourLayout->addLayout(m_pourButton_layout);
    m_pourLayout->addWidget(m_entitySelect);

    m_date_label = new QLabel("Date et heure du controle :");
    m_dateTimeEdit = new QDateTimeEdit;
    m_num_label = new QLabel("Numéro de controle :");
    m_numSpinBox = new widget::spin_box_num_exclu;
    m_numSpinBox->set_offset(1);
    m_numSpinBox->setMinimum(0);
    m_numSpinBox->set_minimum_visible(true);
    m_main_layout->insertWidget(4,m_pourGr);
    m_main_layout->insertWidget(5,m_num_label);
    m_main_layout->insertWidget(6,m_numSpinBox);
    m_main_layout->insertWidget(7,m_date_label);
    m_main_layout->insertWidget(8,m_dateTimeEdit);
}

void ControleNewModifForm::connexion() {
    AbstractControleNewModifForm::connexion();
    connect(m_pourBG,qOverload<int>(&QButtonGroup::buttonClicked),this,&ControleNewModifForm::updateSelectEntity);
    connect(m_entitySelect,&widget::AbstractEntitySelectWidget::id_changed,this,&ControleNewModifForm::updateNum);
    connect(m_parent_tree->selectionModel(),&QItemSelectionModel::selectionChanged,this,&ControleNewModifForm::updateNum);
}

void ControleNewModifForm::save() {
    auto ctr = entityNoteOption<Controle>(true);
    ctr.set_id_type(id_parent());
    ctr.set_cible(m_pourBG->checkedId());
    ctr.set_id_cible(m_entitySelect->id());
    ctr.set_num(m_numSpinBox->value());
    ctr.setDateTime(m_dateTimeEdit->dateTime());
    m_bdd.save(ctr);
}

void ControleNewModifForm::update_data() {
    if(!m_new){
        Controle ctr;
        AbstractNcNomNewModifForm::update_temp(ctr);
        set_parent(ctr.id_type());
        m_pourBG->button(ctr.cible())->setChecked(true);
        updateSelectEntity(ctr.cible());
        m_entitySelect->set_id(ctr.id_cible());
        updateNum();
        m_numSpinBox->set_value(ctr.num());
        m_dateTimeEdit->setDateTime(ctr.dateTime());
    }
    else
        updateNum();
}

void ControleNewModifForm::updateNum() {
    auto listCtr = m_bdd.get_list<Controle>(Controle::Cible,m_cibleEntitySelect,
                                           Controle::Id_Cible,m_entitySelect->id(),
                                           Controle::id_type,id_parent());
    if(listCtr.empty()) {
        m_numSpinBox->clear_list();
        m_numSpinBox->set_value(0);
    }
    else {
        auto listNum = std::list<int>();
        for(auto iter = listCtr.cbegin(); iter != listCtr.cend(); ++iter)
            listNum.push_back(iter->num());
        m_numSpinBox->set_list(listNum);
        auto max = std::max_element(listNum.cbegin(),listNum.cend());
        m_numSpinBox->set_value(*max + 1);
    }
}

void ControleNewModifForm::updateSelectEntity(int cible){
    if(cible != m_cibleEntitySelect) {
        widget::AbstractEntitySelectWidget * new_entitySelect = nullptr;
        if(cible == static_cast<b2d::bdd_predef&>(m_bdd).cible(Classe::ID))
            new_entitySelect = new ClasseSelectWidget(m_bdd,Qt::Vertical);
        else if (cible == static_cast<b2d::bdd_predef&>(m_bdd).cible(Eleve::ID))
            new_entitySelect = new EleveSelectWidget(m_bdd,Qt::Vertical);
        else if (cible == static_cast<b2d::bdd_predef&>(m_bdd).cible(Groupe::ID))
            new_entitySelect = new GroupeSelectWidget(m_bdd,Qt::Vertical);
        m_pourLayout->replaceWidget(m_entitySelect,new_entitySelect,Qt::FindDirectChildrenOnly);
        m_cibleEntitySelect = cible;
        m_entitySelect->deleteLater();
        m_entitySelect = new_entitySelect;
        updateNum();
        connect(m_entitySelect,&widget::AbstractEntitySelectWidget::id_changed,this,&ControleNewModifForm::updateNum);
    }
}
// Etablissement
EtablissementNewModifForm::EtablissementNewModifForm(b2d::Bdd &bdd, bool new_ent, QWidget * parent)
    : dialogue::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom abrégé de l'établissement :"),
                                            tr("Nom de l'établissement :"),
                                            new_ent,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.get_list<Etablissement>());

    // Type d'établissment
    m_TELabel = new QLabel(tr("Type(s) de l'établissement :"));
    m_TEList = new widget::check_list(m_bdd.get_list<TypeEtablissement>());

    // Niveaux
    m_nivLabel = new QLabel(tr("Niveaux présent dans l'établissement:"));
    m_nivList = new widget::check_list(m_bdd.get_list<Niveau>());

    // Calque
    m_main_layout->addWidget(m_TELabel);
    m_main_layout->addWidget(m_TEList);
    m_main_layout->addWidget(m_nivLabel);
    m_main_layout->addWidget(m_nivList);
}

void EtablissementNewModifForm::connexion() {
    AbstractNcNomNewModifForm::connexion();
    connect(m_TEList,&widget::check_list::itemChanged,this,&EtablissementNewModifForm::updateNiveau);
}

void EtablissementNewModifForm::save() {
    Etablissement etab;
    if(!m_new)
        etab.set_id(id());
    etab.setNc(nc());
    etab.setNom(nom());
    m_bdd.save(etab);
    widget::check_list::Save<EtablissementType>::save(m_bdd,m_TEList,
                                                        [&etab](EtablissementType & etabType, idt idTE){
                                                                        etabType.set_idEtab(etab.id());
                                                                        etabType.set_idTpEtab(idTE);
                                                                    });
    widget::check_list::Save<EtablissementNiveau>::save(m_bdd,m_nivList,
                                                          [&etab](EtablissementNiveau & etabNiv, idt idNiv){
                                                                        etabNiv.set_idEtab(etab.id());
                                                                        etabNiv.set_idNiveau(idNiv);
                                                                    });
}

void EtablissementNewModifForm::update_data() {
    if(!m_new) {
        Etablissement etab;
        update_temp<Etablissement>(etab);
        m_TEList->set_checks_by_id<EtablissementType>(m_bdd.get_list<EtablissementType>(EtablissementType::IdEtab,etab.id()),
                       [](const EtablissementType& entity)->idt{return ent.idTpEtab();});
        m_nivList->set_checks_by_id<EtablissementNiveau>(m_bdd.get_list<EtablissementNiveau>(EtablissementNiveau::IdEtab,etab.id()),
                       [](const EtablissementNiveau& entity)->idt{return ent.idNiveau();});
    }
    updateNiveau();
}

void EtablissementNewModifForm::updateNiveau() {
    std::forward_list<idt> listIdTE;
    for(auto i = 0; i != m_TEList->count(); ++i)
        if(m_TEList->item(i)->checkState() == Qt::Checked)
            listIdTE.push_front(m_TEList->id(i));
    std::list<idt> listIdNiv;
    for(auto iter = listIdTE.cbegin(); iter != listIdTE.cend(); ++iter) {
        auto vecNivTE = m_bdd.get_list<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,*iter,NiveauTypeEtablissement::IdNiveau);
        std::list<idt> listIdNivOther;
        for (auto iterNiv = vecNivTE.cbegin(); iterNiv != vecNivTE.cend(); ++iterNiv)
            listIdNivOther.push_back(iterNiv->idNiveau());
        listIdNiv.merge(listIdNivOther);
    }
    listIdNiv.unique();
    for (auto i = 0; i != m_nivList->count(); ++i) {
        if(listIdNiv.empty() || listIdNiv.front() != m_nivList->id(i)) {
            m_nivList->item(i)->setCheckState(Qt::Unchecked);
            m_nivList->item(i)->setHidden(true);
            }
        else {
            m_nivList->item(i)->setHidden(false);
            listIdNiv.pop_front();
        }
    }
}

// Groupe
GroupeNewModifForm::GroupeNewModifForm(b2d::Bdd &bdd, bool new_ent, QWidget * parent)
    : AbstractTypeNcNomNewModifForm(static_cast<b2d::bdd_predef &>(bdd),
                                    "groupe_root_tp",
                                    Groupe::ID,
                                    tr("Type de groupe :"),
                                    tr("Nom abrégé du groupe :"),
                                    tr("Nom du groupe :"),
                                    new_ent,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.get_list<Groupe>(Groupe::Nom));

    // Choix de groupe année - classe
    m_anClGr = new QGroupBox(tr("Groupe pour une :"));
    m_anRadio = new QRadioButton(tr("Année"));
    m_clRadio = new QRadioButton(tr("Classe"));
    m_clRadio->setChecked(true);
    m_classeSelect = new ClasseSelectWidget(bdd,Qt::Vertical);

    // Identifiant des groupes.
    m_alphaLabel = new QLabel(tr("Identifiant des groupes :"));
    m_alphaCB = new QComboBox;
    m_alphaCB->addItem(tr("Nombre arabe (1)"),divers::num_to_string::ArabeSuivant);
    m_alphaCB->addItem(tr("Nombre romain (I)"),divers::num_to_string::RomainSuivant);
    m_alphaCB->addItem(tr("Minuscule (a)"),divers::num_to_string::Minuscule);
    m_alphaCB->addItem(tr("Majuscule (A)"),divers::num_to_string::Majuscule);
    m_alphaCB->addItem(tr("Minuscule grec (\u03B1)"),divers::num_to_string::GrecMinuscule);
    m_alphaCB->addItem(tr("Majuscule grec (\u0394)"),divers::num_to_string::GrecMajuscule);

    // Option
    m_optGr = new QGroupBox(tr("Options :"));
    m_exclusifCheck = new QCheckBox(tr("Exclusif"));
    m_totalCheck = new QCheckBox(tr("Total"));

    // Calque
    m_anClLayout = new QHBoxLayout(m_anClGr);
    m_anClLayout->addWidget(m_clRadio);
    m_anClLayout->addWidget(m_anRadio);

    m_optLayout = new QHBoxLayout(m_optGr);
    m_optLayout->addWidget(m_exclusifCheck);
    m_optLayout->addWidget(m_totalCheck);

    m_main_layout->addWidget(m_anClGr);
    m_main_layout->addWidget(m_classeSelect);
    m_main_layout->addWidget(m_alphaLabel);
    m_main_layout->addWidget(m_alphaCB);
    m_main_layout->addWidget(m_optGr);
}

void GroupeNewModifForm::connexion() {
    AbstractTypeNcNomNewModifForm::connexion();
    connect(m_clRadio,&QRadioButton::toggled,m_classeSelect,&ClasseSelectWidget::setEnabledEtabClasse);
}

void GroupeNewModifForm::save() {
    Groupe groupe;
    if(!m_new)
        groupe.set_id(id());
    groupe.setNc(nc());
    groupe.setNom(nom());
    groupe.setType(id_type());
    groupe.set_styleNum(m_alphaCB->currentData().toUInt());
    if(m_exclusifCheck->isChecked())
        groupe.add(Groupe::Exclusif);
    if(m_totalCheck->isChecked())
        groupe.add(Groupe::Total);
    if(m_clRadio->isChecked()) {
        groupe.set_idAn(0);
        groupe.set_idClasse(m_classeSelect->id());
    }
    else {
        groupe.set_idAn(m_classeSelect->idAn());
        groupe.set_idClasse(0);
    }
    m_bdd.save(groupe);
}

void GroupeNewModifForm::update_data() {
    if(!m_new) {
        Groupe groupe;
        update_temp<Groupe>(groupe);
        m_alphaCB->setCurrentIndex(m_alphaCB->findData(groupe.styleNum()));
        m_exclusifCheck->setChecked(groupe.code().test(Groupe::Exclusif));
        m_totalCheck->setChecked(groupe.code().test(Groupe::Total));
        m_clRadio->setChecked(groupe.idClasse());
        if(groupe.idClasse()){
            Classe cl(groupe.idClasse());
            m_bdd.get(cl);
            m_classeSelect->set_idAn(cl.idAn());
            m_classeSelect->set_idEtab(cl.idEtab());
            m_classeSelect->set_id(cl.id());
        }
        else
            m_classeSelect->set_idAn(groupe.idAn());
    }
    else {
        m_exclusifCheck->setChecked(true);
        m_totalCheck->setChecked(true);
    }
}


// Niveau
NiveauNewModifForm::NiveauNewModifForm(b2d::Bdd & bdd, bool new_ent, QWidget * parent)
    : dialogue::AbstractTypeNcNomNewModifForm (static_cast<b2d::bdd_predef &>(bdd),
                                                "etude_root_tp",
                                                Niveau::ID,
                                                tr("Type de niveau :"),
                                                tr("Nom abrégé du niveau :"),
                                                tr("Nom du niveau :"),
                                                new_ent,parent)
{

    // Nom
    auto niveaux = m_bdd.get_list<Niveau>();
    if(!m_new)
        setNoms(niveaux);

    // Type d'établissment
    m_listLabel = new QLabel(tr("Type d'établissement pouvant accueillir le niveau:"));
    m_TEList = new widget::check_list(m_bdd.get_list<TypeEtablissement>());

    // Niveaux précédents
    m_nivPrecLabel = new QLabel(tr("Niveaux précédents:"));
    m_nivPrecList = new widget::check_list(niveaux);

    // Niveaux suivants
    m_nivSuivLabel = new QLabel(tr("Niveaux suivants:"));
    m_nivSuivList = new widget::check_list(niveaux);

    // Calques
    m_nivPrecLayout = new QVBoxLayout;
    m_nivPrecLayout->addWidget(m_nivPrecLabel);
    m_nivPrecLayout->addWidget(m_nivPrecList);

    m_nivSuivLayout = new QVBoxLayout;
    m_nivSuivLayout->addWidget(m_nivSuivLabel);
    m_nivSuivLayout->addWidget(m_nivSuivList);

    m_nivLayout = new QHBoxLayout;
    m_nivLayout->addLayout(m_nivPrecLayout);
    m_nivLayout->addLayout(m_nivSuivLayout);

    m_main_layout->addWidget(m_listLabel);
    m_main_layout->addWidget(m_TEList);
    m_main_layout->addLayout(m_nivLayout);
}

void NiveauNewModifForm::save() {
    Niveau niv;
    if(!m_new)
        niv.set_id(id());
    niv.setNc(nc());
    niv.setNom(nom());
    niv.setType(id_type());
    m_bdd.save(niv);
    widget::check_list::Save<NiveauTypeEtablissement>::save(m_bdd,m_TEList,[&niv](NiveauTypeEtablissement & nivTE, idt idTE){
                                                                                        nivTE.set_idNiveau(niv.id());
                                                                                        nivTE.set_idTpEtab(idTE);
                                                                                    });
    widget::check_list::Save<FiliationNiveau>::save(m_bdd,m_nivPrecList,[&niv](FiliationNiveau & filNiv, idt idNiv){
                                                                                        filNiv.set_idPrecedent(idNiv);
                                                                                        filNiv.set_idSuivant(niv.id());
                                                                                    });
    widget::check_list::Save<FiliationNiveau>::save(m_bdd,m_nivSuivList,[&niv](FiliationNiveau & filNiv, idt idNiv){
                                                                                        filNiv.set_idPrecedent(niv.id());
                                                                                        filNiv.set_idSuivant(idNiv);
                                                                                    });
}

void NiveauNewModifForm::update_data() {
    if(!m_new) {
        Niveau niv;
        update_temp<Niveau>(niv);
        m_TEList->set_checks_by_id<NiveauTypeEtablissement>(m_bdd.get_list<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdNiveau,niv.id()),
                       [](const NiveauTypeEtablissement& entity)->idt{return ent.idTpEtab();});
        m_nivPrecList->set_checks_by_id<FiliationNiveau>(m_bdd.get_list<FiliationNiveau>(FiliationNiveau::IdSuivant,niv.id()),
                       [](const FiliationNiveau & entity){return ent.idPrecedent();});
        m_nivSuivList->set_checks_by_id<FiliationNiveau>(m_bdd.get_list<FiliationNiveau>(FiliationNiveau::IdPrecedent,niv.id()),
                       [](const FiliationNiveau & entity){return ent.idSuivant();});
    }
}

// Typecontrole
TypeControleNewModifForm::TypeControleNewModifForm(b2d::Bdd &bdd, bool new_ent, QWidget * parent)
    : AbstractControleNewModifForm(bdd,
                                      "Parent :",
                                      "Nom abrégé du type de controle :",
                                      "Nom du type de controle :",
                                      new_ent,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.get_list<TypeControle>(TypeControle::Nom));

    // parent
    m_parent_tree->set_treeNcNomId(bdd.getArbre<TypeControle>());
    m_parent_tree->expandAll();

    //Catégorie
    m_categorieCheck = new QCheckBox("Catégorie");

    //Modif
    m_modifGr = new QGroupBox(tr("Modifiable dans les controles du type :"));
    m_appreciationModifCheck = new  QCheckBox(tr("Appreciation"));
    m_barreModifCheck = new QCheckBox(tr("Barre de classement"));
    m_capaciteModifCheck = new QCheckBox(tr("Capacités"));
    m_classementModifCheck = new QCheckBox(tr("Classement"));
    m_commentaireModifCheck = new QCheckBox(tr("Commentaire"));
    m_competenceModifCheck = new QCheckBox(tr("Compétence"));
    m_courbeModifCheck = new QCheckBox(tr("Courbe d'ajustement"));
    m_decimaleModifCheck = new QCheckBox(tr("Décimale"));
    m_depassementModifCheck = new QCheckBox(tr("Dépassement"));
    m_minimaModifCheck = new QCheckBox(tr("Minima"));
    m_noteModifCheck = new QCheckBox(tr("Noté"));
    m_totalModifCheck = new QCheckBox(tr("Total"));
    m_typeNoteModifCheck = new QCheckBox(tr("Nombre-Lettre"));
    noteModifEnable();
    optionEnable();

    //Calque
    m_modifLayout = new QGridLayout(m_modifGr);
    m_modifLayout->addWidget(m_appreciationModifCheck,LigneZero,OptionModifColonne);
    m_modifLayout->addWidget(m_commentaireModifCheck,LigneUne,OptionModifColonne);
    m_modifLayout->addWidget(m_competenceModifCheck,LigneDeux,OptionModifColonne);
    m_modifLayout->addWidget(m_capaciteModifCheck,LigneTrois,OptionModifColonne);
    m_modifLayout->addWidget(m_noteModifCheck,LigneZero,NoteModifColonne);
    m_modifLayout->addWidget(m_typeNoteModifCheck,LigneUne,NoteModifColonne);
    m_modifLayout->addWidget(m_totalModifCheck,LigneDeux,NoteModifColonne);
    m_modifLayout->addWidget(m_decimaleModifCheck,LigneTrois,NoteModifColonne);
    m_modifLayout->addWidget(m_depassementModifCheck,LigneZero,NombreModifColonne);
    m_modifLayout->addWidget(m_courbeModifCheck,LigneUne,NombreModifColonne);
    m_modifLayout->addWidget(m_classementModifCheck,LigneDeux,NombreModifColonne);
    m_modifLayout->addWidget(m_barreModifCheck,LigneTrois,NombreModifColonne);
    m_modifLayout->addWidget(m_minimaModifCheck,LigneQuatre,NombreModifColonne);

    m_main_layout->insertWidget(4,m_categorieCheck);
    m_main_layout->addWidget(m_modifGr);
}

void TypeControleNewModifForm::connexion() {
    AbstractControleNewModifForm::connexion();
    connect(m_categorieCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::optionEnable);
    connect(m_noteCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::noteModifEnable);
    connect(m_chiffreRadio,&QRadioButton::toggled,this,&TypeControleNewModifForm::noteModifEnable);
    connect(m_classementCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::noteModifEnable);
    connect(m_barreCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::noteModifEnable);
    connect(m_noteModifCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::noteModifEnable);
    connect(m_typeNoteModifCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::noteModifEnable);
    connect(m_classementModifCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::noteModifEnable);
    connect(m_barreModifCheck,&QCheckBox::stateChanged,this,&TypeControleNewModifForm::noteModifEnable);
}

void TypeControleNewModifForm::noteModifEnable() {
    auto enable = m_noteCheck->isChecked() || m_noteModifCheck->isChecked();
    m_typeNoteModifCheck->setEnabled(enable);
    m_totalModifCheck->setEnabled(enable);
    m_decimaleModifCheck->setEnabled(enable);
    if(!enable) {
        m_typeNoteModifCheck->setChecked(false);
        m_totalModifCheck->setChecked(false);
        m_decimaleModifCheck->setChecked(false);
    }
    enable &= m_chiffreRadio->isChecked() || m_typeNoteModifCheck->isChecked();
    m_depassementModifCheck->setEnabled(enable);
    m_courbeModifCheck->setEnabled(enable);
    m_classementModifCheck->setEnabled(enable);
    if(!enable) {
        m_depassementModifCheck->setChecked(false);
        m_courbeModifCheck->setChecked(false);
        m_classementModifCheck->setChecked(false);
    }
    enable &= m_classementCheck->isChecked() || m_classementModifCheck->isChecked();
    m_barreModifCheck->setEnabled(enable);
    if(!enable)
        m_barreModifCheck->setChecked(false);
    enable &= m_barreCheck->isChecked() || m_barreModifCheck->isChecked();
    m_minimaModifCheck->setEnabled(enable);
    if(!enable)
        m_minimaModifCheck->setChecked(false);
}

void TypeControleNewModifForm::optionEnable() {
    auto enable = ! m_categorieCheck->isChecked();
    m_noteGr->setEnabled(enable);
    m_optGr->setEnabled(enable);
    m_modifGr->setEnabled(enable);
    if(!enable) {
        m_noteCheck->setChecked(false);
        m_appreciationCheck->setChecked(false);
        m_capaciteCheck->setChecked(false);
        m_commentaireCheck->setChecked(false);
        m_competenceCheck->setChecked(false);
        m_appreciationModifCheck->setChecked(false);
        m_capaciteModifCheck->setChecked(false);
        m_commentaireModifCheck->setChecked(false);
        m_competenceModifCheck->setChecked(false);
        m_noteModifCheck->setChecked(false);
    }
}

void TypeControleNewModifForm::save() {
    auto tpc = entityNoteOption<TypeControle>(!m_categorieCheck->isChecked());
    tpc.set_parent(id_parent());
    if(m_categorieCheck->isChecked())
        tpc.add(TypeControle::Categorie);
    else {
        //Option modification
        if(m_appreciationModifCheck->isChecked())
            tpc.add(TypeControle::AppreciationModifiable);
        if(m_capaciteModifCheck->isChecked())
            tpc.add(TypeControle::CapacitesModifiable);
        if(m_commentaireModifCheck->isChecked())
            tpc.add(TypeControle::CommentaireModifiable);
        if(m_competenceModifCheck->isChecked())
            tpc.add(TypeControle::CompetencesModifiable);
        if(m_noteModifCheck->isChecked())
            tpc.add(TypeControle::NoteModifiable);
        if(m_noteCheck->isChecked() || m_noteModifCheck->isChecked()) {
            if(m_typeNoteModifCheck->isChecked())
                tpc.add(TypeControle::TypeNoteModifiable);
            if(m_totalModifCheck->isChecked())
                tpc.add(TypeControle::TotalModifiable);
            if(m_decimaleModifCheck->isChecked())
                tpc.add(TypeControle::DecimaleModifiable);
            if(m_chiffreRadio->isChecked() || m_typeNoteModifCheck->isChecked()){
                if(m_depassementCheck->isChecked())
                    tpc.add(TypeControle::DepassementModifiable);
                if(m_courbeModifCheck->isChecked())
                    tpc.add(TypeControle::CourbeModifiable);
                if(m_classementModifCheck->isChecked())
                    tpc.add(TypeControle::ClassementModifiable);
                if(m_classementCheck->isChecked() || m_classementModifCheck->isChecked()){
                    if(m_barreModifCheck->isChecked())
                        tpc.add(TypeControle::BarreModifiable);
                    if((m_barreCheck->isChecked() || m_barreModifCheck->isChecked())
                            && m_minimaModifCheck->isChecked())
                        tpc.add(TypeControle::MinimaModifiable);
                }
            }
        }
    }
    m_bdd.save(tpc);
}

void TypeControleNewModifForm::update_data() {
    if(!m_new){
        TypeControle tpc;
        update_temp(tpc);
        m_categorieCheck->setChecked(tpc.code().test(TypeControle::Categorie));
        if(!m_categorieCheck->isChecked()) {
            updateNoteOption(tpc);
            //Option modification
            m_appreciationModifCheck->setChecked(tpc.code().test(TypeControle::AppreciationModifiable));
            m_capaciteModifCheck->setChecked(tpc.code().test(TypeControle::CapacitesModifiable));
            m_commentaireModifCheck->setChecked(tpc.code().test(TypeControle::CommentaireModifiable));
            m_competenceModifCheck->setChecked(tpc.code().test(TypeControle::CompetencesModifiable));
            m_noteModifCheck->setChecked(tpc.code().test(TypeControle::NoteModifiable));
            if(m_noteCheck->isChecked() || m_noteModifCheck->isChecked()) {
                m_typeNoteModifCheck->setChecked(tpc.code().test(TypeControle::TypeNoteModifiable));
                m_totalModifCheck->setChecked(tpc.code().test(TypeControle::TotalModifiable));
                m_decimaleModifCheck->setChecked(tpc.code().test(TypeControle::DecimaleModifiable));
                if(m_chiffreRadio->isChecked() || m_typeNoteModifCheck->isChecked()){
                    m_depassementModifCheck->setChecked(tpc.code().test(TypeControle::DepassementModifiable));
                    m_courbeModifCheck->setChecked(tpc.code().test(TypeControle::CourbeModifiable));
                    m_classementModifCheck->setChecked(tpc.code().test(TypeControle::ClassementModifiable));
                    if(m_classementCheck->isChecked() || m_classementModifCheck->isChecked()){
                        m_barreModifCheck->setChecked(tpc.code().test(TypeControle::BarreModifiable));
                        if(m_barreCheck->isChecked() || m_barreModifCheck->isChecked())
                            m_minimaModifCheck->setChecked(TypeControle::MinimaModifiable);
                    }
                }
            }
        }
    }
}

// TypeEtablissement
TypeEtablissementNewModifForm::TypeEtablissementNewModifForm(b2d::Bdd & bdd, bool new_ent, QWidget * parent)
    : dialogue::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom abrégé du type d'établissment :"),
                                            tr("Nom du type d'établissement :"),
                                            new_ent,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.get_list<TypeEtablissement>(TypeEtablissement::Nom));

    // Niveaux
    m_selectNiv = new widget::select_in_list_box(tr("Niveaux existents :"),tr("Niveaux présents :"));

    // Calque
    m_main_layout->addWidget(m_selectNiv);
}

void TypeEtablissementNewModifForm::save() {
    if(m_new) {
        TypeEtablissement te(nc(),nom());
        m_bdd.save(te);
        auto niv = m_selectNiv->value().second;
        for (auto i =niv.cbegin(); i != niv.cend(); ++i)
            m_bdd.save(NiveauTypeEtablissement(i->toUInt(),te.id()));
    }
    else {
        TypeEtablissement te(id());
        if(m_bdd.test_autorisation(te,b2d::Modif)) {
            te.setNc(nc());
            te.setNom(nom());
            m_bdd.save(te);
        }
        auto nivOld = m_bdd.get_list<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,te.id());
        auto nivNew = m_selectNiv->value().second;
        for(auto i = nivOld.cbegin(); i != nivOld.cend(); ++i) {
            auto j = nivNew.begin();
            while(j != nivNew.end() && i->idNiveau() != j->toUInt())
                ++j;
            if(j == nivNew.end()) {
                NiveauTypeEtablissement nivTE(i->idNiveau(),te.id());
                m_bdd.get_unique(nivTE);
                m_bdd.del(nivTE);
            }
            else
                j->set_value(0);
        }
        for (auto j = nivNew.cbegin(); j != nivNew.cend(); ++j) {
            if(j->toUInt() != 0)
                m_bdd.save(NiveauTypeEtablissement(j->toUInt(),te.id()));
        }
    }
}

void TypeEtablissementNewModifForm::update_data() {
    if(m_new) {
        auto vecNiv = m_bdd.get_list<Niveau>();
        std::map<QVariant, QString> nivOut;
        for(auto i = vecNiv.cbegin(); i != vecNiv.cend(); ++i)
            nivOut[i->id()] = i->nom();
        m_selectNiv->set_value(nivOut,std::map<QVariant, QString>());
    }
    else {
        TypeEtablissement te;
        update_temp<TypeEtablissement>(te);
        auto vecNiv = m_bdd.get_list<Niveau>();
        std::map<QVariant, QString> nivIn;
        std::map<QVariant, QString> nivOut;
        for(auto i = vecNiv.cbegin(); i != vecNiv.cend(); ++i) {
            if(m_bdd.exists_unique(NiveauTypeEtablissement(i->id(),te.id())))
                nivIn[i->id()] = i->nom();
            else
                nivOut[i->id()] = i->nom();
        }
        m_selectNiv->set_value(nivOut,nivIn);
    }
}
