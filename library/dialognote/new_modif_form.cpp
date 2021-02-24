#include "new_modif_form.h"

using namespace note_mps;

// Abstractcontrole
abstract_controle_new_modif_form::abstract_controle_new_modif_form(mps::b2d::bdd &bdd, const QString & LabelParent,
                                                           const QString &label_nc, const QString &label_nom,
                                                           bool new_ent, QWidget *parent)
    : abstract_parent_nc_nom_new_modif_form(bdd, {"nom","nom abrégé"},
                                      LabelParent,
                                      label_nc,
                                      label_nom,
                                      new_ent,parent) {
    //note
    m_note_gr = new QGroupBox(tr("Notation :"));
    m_note_check = new QCheckBox(tr("Devoir noté"));
    m_type_label = new QLabel(tr("Type de notation :"));
    m_chiffre_radio = new QRadioButton(tr("Nombre"));
    m_chiffre_radio->setChecked(true);
    m_lettre_radio = new QRadioButton(tr("Lettre"));
    m_total_label = new QLabel;
    m_total_spin_box = new mps::widget::spin_box_lettre;
    m_depassement_check = new QCheckBox(tr("Depassement"));
    m_decimal_label = new QLabel;
    m_decimale_cb = new QComboBox;
    m_courbe_check = new QCheckBox(tr("Courbe d'ajustement"));
    m_classement_check = new QCheckBox(tr("Classement"));
    m_barre_check = new QCheckBox(tr("Barre de classement :"));
    m_minima_spin_box = new mps::widget::spin_box_decimale;
    barre_enable();
    classement_enable();
    type_note_change();
    note_enable();

    //option
    m_opt_gr = new QGroupBox(tr("Options :"));
    m_appreciation_check = new QCheckBox(tr("Appréciation"));
    m_capacite_check = new QCheckBox(tr("Capacités"));
    m_commentaire_check = new QCheckBox(tr("Commentaire"));
    m_competence_check = new QCheckBox(tr("Compétences"));

    //Calque
    m_note_layout = new QGridLayout(m_note_gr);
    m_note_layout->addWidget(m_note_check,Check_Note_Ligne,Label_Colonne);
    m_note_layout->addWidget(m_type_label,Type_Ligne,Label_Colonne);
    m_note_layout->addWidget(m_chiffre_radio,Type_Ligne,Colonne_Une);
    m_note_layout->addWidget(m_lettre_radio,Type_Ligne,Colonne_Deux);
    m_note_layout->addWidget(m_total_label,Total_Ligne,Label_Colonne);
    m_note_layout->addWidget(m_total_spin_box,Total_Ligne,Colonne_Une);
    m_note_layout->addWidget(m_depassement_check,Total_Ligne,Colonne_Deux);
    m_note_layout->addWidget(m_decimal_label,Decimale_Ligne,Label_Colonne);
    m_note_layout->addWidget(m_decimale_cb,Decimale_Ligne,Colonne_Une);
    m_note_layout->addWidget(m_courbe_check,Decimale_Ligne,Colonne_Deux);
    m_note_layout->addWidget(m_classement_check,Classement_Ligne,Colonne_Zero);
    m_note_layout->addWidget(m_barre_check,Barre_Ligne,Label_Colonne);
    m_note_layout->addWidget(m_minima_spin_box,Barre_Ligne,Colonne_Une);

    m_opt_layout = new QGridLayout(m_opt_gr);
    m_opt_layout->addWidget(m_appreciation_check,Appreciation_Commentaire_Ligne,Colonne_Zero);
    m_opt_layout->addWidget(m_commentaire_check,Appreciation_Commentaire_Ligne,Colonne_Une);
    m_opt_layout->addWidget(m_competence_check,Capacite_Competence_Ligne,Colonne_Zero);
    m_opt_layout->addWidget(m_capacite_check,Capacite_Competence_Ligne,Colonne_Une);

    m_main_layout->addWidget(m_note_gr);
    m_main_layout->addWidget(m_opt_gr);
}

void abstract_controle_new_modif_form::barre_enable() {
    auto enable = m_barre_check->isChecked();
    m_minima_spin_box->setEnabled(enable);
    if(enable) {
        decimale_change();
        total_change();
        m_minima_spin_box->set_value_double(m_total_spin_box->value() / 2.);
        connect(m_decimale_cb,qOverload<int>(&QComboBox::currentIndexChanged),this,&type_controle_new_modif_form::decimale_change);
        connect(m_total_spin_box,qOverload<int>(&mps::widget::spin_box_lettre::valueChanged),this,&type_controle_new_modif_form::total_change);
    }
    else {
        disconnect(m_decimale_cb,nullptr,this,nullptr);
        disconnect(m_total_spin_box,nullptr,this,nullptr);
        m_minima_spin_box->clear();
    }

}

void abstract_controle_new_modif_form::classement_enable() {
    auto enable = m_classement_check->isChecked();
    m_barre_check->setEnabled(enable);
    if(!enable)
        m_barre_check->setChecked(false);
}

void abstract_controle_new_modif_form::connexion() {
    abstract_parent_nc_nom_new_modif_form::connexion();
    connect(m_note_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::note_enable);
    connect(m_chiffre_radio,&QRadioButton::toggled,this,&type_controle_new_modif_form::type_note_change);
    connect(m_classement_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::classement_enable);
    connect(m_barre_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::barre_enable);
}

void abstract_controle_new_modif_form::note_enable() {
    auto enable = m_note_check->isChecked();
    m_type_label->setEnabled(enable);
    m_chiffre_radio->setEnabled(enable);
    m_lettre_radio->setEnabled(enable);
    m_total_label->setEnabled(enable);
    m_total_spin_box->setEnabled(enable);
    m_depassement_check->setEnabled(enable);
    m_decimal_label->setEnabled(enable);
    m_decimale_cb->setEnabled(enable);
    m_courbe_check->setEnabled(enable);
    if(enable)
        type_note_change();
    else {
        m_chiffre_radio->setChecked(true);
        m_depassement_check->setChecked(false);
        m_classement_check->setEnabled(false);
        m_classement_check->setChecked(false);
        m_courbe_check->setChecked(false);
        m_total_spin_box->clear();
        m_decimale_cb->clear();
    }
}

void abstract_controle_new_modif_form::type_note_change() {
    using attDecimale = mps::attribut::attribut_decimale;
    if(m_chiffre_radio->isChecked()) {
        m_total_label->setText("Total :");
        m_total_spin_box->set_style(mps::divers::num_to_string::Arabe_Style);
        m_total_spin_box->setMinimum(1);
        m_total_spin_box->setMaximum(1000);
        m_total_spin_box->setValue(20);
        m_depassement_check->setEnabled(m_note_check->isChecked());
        m_courbe_check->setEnabled(m_note_check->isChecked());
        m_decimal_label->setText(tr("Décimale :"));
        m_decimale_cb->clear();
        for (szt i = 0; i != attDecimale::Nbr_Values; ++i)
            m_decimale_cb->addItem(QString::number(1./ attDecimale::Decimale.at(i),'f',attDecimale::Precision.at(i)),
                                  attDecimale::Decimale.at(i));
        m_classement_check->setEnabled(m_note_check->isChecked());
    }
    else {
        m_total_label->setText("De A à :");
        m_total_spin_box->set_style(mps::divers::num_to_string::Majuscule_Style);
        m_total_spin_box->setMinimum(0);
        m_total_spin_box->setMaximum(25);
        m_total_spin_box->setValue(3);
        m_depassement_check->setEnabled(false);
        m_depassement_check->setChecked(false);
        m_courbe_check->setEnabled(false);
        m_courbe_check->setChecked(false);
        m_decimal_label->setText(tr("Ajustement :"));
        m_classement_check->setEnabled(false);
        m_classement_check->setChecked(false);
        m_decimale_cb->clear();
        m_decimale_cb->addItem(" ",controle::NoPlus);
        m_decimale_cb->addItem("+",controle::Plus);
        m_decimale_cb->addItem("++",controle::PPlus);
        m_decimale_cb->addItem("+++",controle::PPPlus);
    }
}

// annee
annee_new_modif_form::annee_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent)
    : mps::dialogue::abstract_new_modif_form (bdd,new_ent,parent) {
    // annee
    m_annee_label = new QLabel(new_ent ? tr("Nouvelle année scolaire :")
                                     : tr("Année scolaire courante :"));
    if(new_ent) {
        m_spin_box = new spin_box_annee_scolaire();
        m_annee_courante = new QCheckBox(tr("Année courante"));
    }
    else
        m_spin_box = new spin_box_annee_scolaire(m_bdd.get_list<annee>(annee::Num));
    m_spin_box->setNowValue();

    // Calque
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_annee_label);
    if(m_new) {
        m_main_layout->addWidget(m_spin_box);
        m_main_layout->addWidget(m_annee_courante);
    }
    else if (!m_spin_box->values().empty())
        m_main_layout->addWidget(m_spin_box);
    else
        m_annee_label->setText("Vous devez préalablement créer une année scolaire dans le menu :\n "
                              "fichier>Nouveau>Année scolaire");
}

void annee_new_modif_form::connexion() {
    if(m_new){
        connect(m_spin_box,&spin_box_annee_scolaire::value_changed,this,[this]()
            {emit save_permis(!m_bdd.exists_unique(m_spin_box->value()));});
        emit save_permis(!m_bdd.exists_unique(m_spin_box->value()));
    }
    else if (m_spin_box->values().empty())
        emit save_permis(false);
}

void annee_new_modif_form::save() {
    if(m_new)
        m_bdd.save(m_spin_box->value());
}

// Classe
classe_new_modif_form::classe_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent)
    : mps::dialogue::abstract_nc_nom_new_modif_form (bdd,
                                            tr("Nom abrégé de la classe :"),
                                            tr("Nom de la classe :"),
                                            new_ent,parent) {
    // Nom
    if(!m_new)
        set_noms(m_bdd.get_list<classe>(classe::Nom));

    // annee - Etab - niveaux
    m_annee_select = new annee_select_widget(m_bdd);
    m_etab_select = new etablissement_select_widget(m_bdd);
    m_niv_select = new niveaux_select_widget(m_bdd);

    // Num
    m_num_label = new QLabel(tr("Numéro :"));
    m_num_spin_box = new QSpinBox();
    m_num_spin_box->setMinimum(0);
    m_num_spin_box->setSpecialValueText(" ");

    // Debut et fin des cours
    m_debut_label = new QLabel(tr("Début des cours:"));
    m_fin_label = new QLabel(tr("Fin des cours:"));
    m_debut_calendar = new QCalendarWidget;
    m_fin_calendar = new QCalendarWidget;

    // Calque
    m_main_layout->addWidget(m_annee_select);
    m_main_layout->addWidget(m_etab_select);
    m_main_layout->addWidget(m_niv_select);
    m_main_layout->addWidget(m_num_label);
    m_main_layout->addWidget(m_num_spin_box);
    m_debut_layout = new QVBoxLayout;
    m_debut_layout->addWidget(m_debut_label);
    m_debut_layout->addWidget(m_debut_calendar);
    m_fin_layout = new QVBoxLayout;
    m_fin_layout->addWidget(m_fin_label);
    m_fin_layout->addWidget(m_fin_calendar);
    m_calendar_layout = new QHBoxLayout;
    m_calendar_layout->addLayout(m_debut_layout);
    m_calendar_layout->addLayout(m_fin_layout);
    m_main_layout->addLayout(m_calendar_layout);

}

void classe_new_modif_form::connexion() {
    abstract_nc_nom_new_modif_form::connexion();
    connect(m_etab_select,&etablissement_select_widget::id_changed,m_niv_select,&niveaux_select_widget::set_id_etab);
    connect(m_annee_select,&annee_select_widget::id_changed,this,&classe_new_modif_form::update_calendar);
    connect(m_debut_calendar,&QCalendarWidget::selectionChanged,this,[this](){emit save_permis(valide());});
    connect(m_fin_calendar,&QCalendarWidget::selectionChanged,this,[this](){emit save_permis(valide());});
}

void classe_new_modif_form::save() {
    classe cl;
    if(!m_new)
        cl.set_id(id());
    cl.set_nc(nc());
    cl.set_nom(nom());
    cl.set_id_an(m_annee_select->id());
    cl.set_id_etab(m_etab_select->id());
    cl.set_id_niveau(m_niv_select->id());
    cl.set_num(m_num_spin_box->value());
    m_bdd.save(cl);
    if(m_new)
        m_id_classe = cl.id();
    donnee_cible dn_cb;
    dn_cb.set_id_donnee(bdd().ref_to_id<donnee>("date_defaut_dn"));
    dn_cb.set_id_cible(m_id_classe);
    dn_cb.set_cible(bdd().cible<classe>());
    dn_cb.set_num(donnee_mps::Entree_Num);
    bdd().get_unique(dn_cb);
    dn_cb.set_valeur(m_debut_calendar->selectedDate());
    bdd().save(dn_cb);
    dn_cb.set_id(0);
    dn_cb.set_num(donnee_mps::Sortie_Num);
    bdd().get_unique(dn_cb);
    dn_cb.set_valeur(m_fin_calendar->selectedDate());
    bdd().save(dn_cb);
}

void classe_new_modif_form::update_calendar() {
    if(m_new) {
        auto an = m_annee_select->num();
        m_debut_calendar->setDateRange(QDate(an,9,1),QDate(an+1,8,31));
        m_debut_calendar->setSelectedDate(QDate(an,9,1));
        m_fin_calendar->setDateRange(QDate(an,9,1),QDate(an+1,8,31));
        m_fin_calendar->setSelectedDate(QDate(an+1,8,31));
    }
}

void classe_new_modif_form::update_data() {
    if(m_new) {
        m_niv_select->set_id_etab(m_etab_select->id());
        update_calendar();
    }
    else {
        classe cl;
        update_temp<classe>(cl);
        m_id_classe = cl.id();
        m_annee_select->set_id(cl.id_an());
        m_etab_select->set_id(cl.id_etab());
        m_niv_select->set_id_etab(cl.id_etab());
        m_niv_select->set_id(cl.id_niveau());
        m_num_spin_box->setValue(cl.num());
        donnee_cible dn_cb;
        dn_cb.set_id_donnee(bdd().ref_to_id<donnee>("date_defaut_dn"));
        dn_cb.set_id_cible(m_id_classe);
        dn_cb.set_cible(bdd().cible<classe>());
        dn_cb.set_num(donnee_mps::Entree_Num);
        bdd().get_unique(dn_cb);
        m_debut_calendar->setSelectedDate(dn_cb.valeur().toDate());
        dn_cb.set_id(0);
        dn_cb.set_num(donnee_mps::Sortie_Num);
        bdd().get_unique(dn_cb);
        m_fin_calendar->setSelectedDate(dn_cb.valeur().toDate());
    }
}

bool classe_new_modif_form::valide() const {
    return abstract_nc_nom_new_modif_form::valide()
            && m_niv_select->id() && m_etab_select->id()
            && m_debut_calendar->selectedDate() < m_fin_calendar->selectedDate();
}

// controle
controle_new_modif_form::controle_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent)
    : abstract_controle_new_modif_form(bdd,
                                      "Type :",
                                      "Nom abrégé du controle :",
                                      "Nom du controle :",
                                      new_ent,parent) {
    // Nom
    if(!m_new)
        set_noms(m_bdd.get_list<controle>(controle::Nom));

    // Type
    m_parent_tree->set_tree_ref(bdd.get_arbre<type_controle>(),
                          [](const type_controle & tc)->QTreeWidgetItem * {
        auto item = new QTreeWidgetItem({tc.nom(),tc.nc()});
        item->setData(mps::widget::tree_widget::Id_Colonne,mps::widget::tree_widget::Id_Role,tc.id());
        if(!tc.code().test(type_controle::Categorie))
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        else
            item->setFlags(Qt::ItemIsEnabled);
        return item;
    });

    // Destination
    m_classe_radio = new QRadioButton(tr("Classe"));
    m_classe_radio->setChecked(true);
    m_eleve_radio = new QRadioButton(tr("eleve"));
    m_groupe_radio = new QRadioButton(tr("Groupe"));
    m_pour_bg = new QButtonGroup(this);
    m_pour_bg->addButton(m_classe_radio,static_cast<mps::b2d::bdd_predef&>(m_bdd).cible(classe::ID));
    m_pour_bg->addButton(m_eleve_radio,static_cast<mps::b2d::bdd_predef&>(m_bdd).cible(eleve::ID));
    m_pour_bg->addButton(m_groupe_radio,static_cast<mps::b2d::bdd_predef&>(m_bdd).cible(groupe::ID));
    m_entity_select = new classe_select_widget(m_bdd,Qt::Vertical);
    m_cible_entity_select = static_cast<mps::b2d::bdd_predef&>(m_bdd).cible(classe::ID);
    m_pour_gr = new QGroupBox("Pour :");
    m_pour_bouton_layout = new QHBoxLayout;
    m_pour_bouton_layout->addWidget(m_classe_radio);
    m_pour_bouton_layout->addWidget(m_groupe_radio);
    m_pour_bouton_layout->addWidget(m_eleve_radio);
    m_pour_layout = new QVBoxLayout(m_pour_gr);
    m_pour_layout->addLayout(m_pour_bouton_layout);
    m_pour_layout->addWidget(m_entity_select);

    m_date_label = new QLabel("Date et heure du controle :");
    m_date_time_edit = new QDateTimeEdit;
    m_num_label = new QLabel("Numéro de controle :");
    m_num_spin_box = new mps::widget::spin_box_num_exclu;
    m_num_spin_box->set_offset(1);
    m_num_spin_box->set_minimum(0);
    m_num_spin_box->set_minimum_visible(true);
    m_main_layout->insertWidget(4,m_pour_gr);
    m_main_layout->insertWidget(5,m_num_label);
    m_main_layout->insertWidget(6,m_num_spin_box);
    m_main_layout->insertWidget(7,m_date_label);
    m_main_layout->insertWidget(8,m_date_time_edit);
}

void controle_new_modif_form::connexion() {
    abstract_controle_new_modif_form::connexion();
    connect(m_pour_bg,qOverload<int>(&QButtonGroup::buttonClicked),this,&controle_new_modif_form::update_select_entity);
    connect(m_entity_select,&mps::widget::abstract_entity_select_widget::id_changed,this,&controle_new_modif_form::update_num);
    connect(m_parent_tree->selectionModel(),&QItemSelectionModel::selectionChanged,this,&controle_new_modif_form::update_num);
}

void controle_new_modif_form::save() {
    auto ctr = entity_note_option<controle>(true);
    ctr.set_id_type(id_parent());
    ctr.set_cible(m_pour_bg->checkedId());
    ctr.set_id_cible(m_entity_select->id());
    ctr.set_num(m_num_spin_box->value());
    ctr.set_date_time(m_date_time_edit->dateTime());
    m_bdd.save(ctr);
}

void controle_new_modif_form::update_data() {
    if(!m_new){
        controle ctr;
        abstract_nc_nom_new_modif_form::update_temp(ctr);
        set_parent(ctr.id_type());
        m_pour_bg->button(ctr.cible())->setChecked(true);
        update_select_entity(ctr.cible());
        m_entity_select->set_id(ctr.id_cible());
        update_num();
        m_num_spin_box->set_value(ctr.num());
        m_date_time_edit->setDateTime(ctr.date_time());
    }
    else
        update_num();
}

void controle_new_modif_form::update_num() {
    auto ctr_list = m_bdd.get_list<controle>(controle::Cible,m_cible_entity_select,
                                           controle::Id_Cible,m_entity_select->id(),
                                           controle::Id_Type,id_parent());
    if(ctr_list.empty()) {
        m_num_spin_box->clear_list();
        m_num_spin_box->set_value(0);
    }
    else {
        auto num_list = std::list<int>();
        for(auto it = ctr_list.cbegin(); it != ctr_list.cend(); ++it)
            num_list.push_back(it->num());
        m_num_spin_box->set_list(num_list);
        auto max = std::max_element(num_list.cbegin(),num_list.cend());
        m_num_spin_box->set_value(*max + 1);
    }
}

void controle_new_modif_form::update_select_entity(int id){
    if(id != m_cible_entity_select) {
        mps::widget::abstract_entity_select_widget * new_entity_select = nullptr;
        if(id == static_cast<mps::b2d::bdd_predef&>(m_bdd).cible(classe::ID))
            new_entity_select = new classe_select_widget(m_bdd,Qt::Vertical);
        else if (id == static_cast<mps::b2d::bdd_predef&>(m_bdd).cible(eleve::ID))
            new_entity_select = new eleve_select_widget(m_bdd,Qt::Vertical);
        else if (id == static_cast<mps::b2d::bdd_predef&>(m_bdd).cible(groupe::ID))
            new_entity_select = new groupe_select_widget(m_bdd,Qt::Vertical);
        m_pour_layout->replaceWidget(m_entity_select,new_entity_select,Qt::FindDirectChildrenOnly);
        m_cible_entity_select = id;
        m_entity_select->deleteLater();
        m_entity_select = new_entity_select;
        update_num();
        connect(m_entity_select,&mps::widget::abstract_entity_select_widget::id_changed,this,&controle_new_modif_form::update_num);
    }
}
// etablissement
etablissement_new_modif_form::etablissement_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent)
    : mps::dialogue::abstract_nc_nom_new_modif_form (bdd,
                                            tr("Nom abrégé de l'établissement :"),
                                            tr("Nom de l'établissement :"),
                                            new_ent,parent) {
    // Nom
    if(!m_new)
        set_noms(m_bdd.get_list<etablissement>());

    // Type d'établissment
    m_te_label = new QLabel(tr("Type(s) de l'établissement :"));
    m_te_list = new mps::widget::check_list(m_bdd.get_list<type_etablissement>());

    // niveaux
    m_niv_label = new QLabel(tr("Niveaux présent dans l'établissement:"));
    m_niv_list = new mps::widget::check_list(m_bdd.get_list<niveau>());

    // Calque
    m_main_layout->addWidget(m_te_label);
    m_main_layout->addWidget(m_te_list);
    m_main_layout->addWidget(m_niv_label);
    m_main_layout->addWidget(m_niv_list);
}

void etablissement_new_modif_form::connexion() {
    abstract_nc_nom_new_modif_form::connexion();
    connect(m_te_list,&mps::widget::check_list::itemChanged,this,&etablissement_new_modif_form::update_niveau);
}

void etablissement_new_modif_form::save() {
    etablissement etab;
    if(!m_new)
        etab.set_id(id());
    etab.set_nc(nc());
    etab.set_nom(nom());
    m_bdd.save(etab);
    mps::widget::check_list::Save<etablissement_type>::save(m_bdd,m_te_list,
                                                        [&etab](etablissement_type & etab_type, idt id_te){
                                                                        etab_type.set_id_etab(etab.id());
                                                                        etab_type.set_id_tp_etab(id_te);
                                                                    });
    mps::widget::check_list::Save<etablissement_niveau>::save(m_bdd,m_niv_list,
                                                          [&etab](etablissement_niveau & etab_niv, idt id_niv){
                                                                        etab_niv.set_id_etab(etab.id());
                                                                        etab_niv.set_id_niveau(id_niv);
                                                                    });
}

void etablissement_new_modif_form::update_data() {
    if(!m_new) {
        etablissement etab;
        update_temp<etablissement>(etab);
        m_te_list->set_checks_by_id<etablissement_type>(m_bdd.get_list<etablissement_type>(etablissement_type::Id_Etab,etab.id()),
                       [](const etablissement_type& ent)->idt{return ent.id_tp_etab();});
        m_niv_list->set_checks_by_id<etablissement_niveau>(m_bdd.get_list<etablissement_niveau>(etablissement_niveau::Id_Etab,etab.id()),
                       [](const etablissement_niveau& ent)->idt{return ent.id_niveau();});
    }
    update_niveau();
}

void etablissement_new_modif_form::update_niveau() {
    std::forward_list<idt> te_id_list;
    for(auto i = 0; i != m_te_list->count(); ++i)
        if(m_te_list->item(i)->checkState() == Qt::Checked)
            te_id_list.push_front(m_te_list->id(i));
    std::list<idt> niv_id_list;
    for(auto it = te_id_list.cbegin(); it != te_id_list.cend(); ++it) {
        auto te_niv_vec = m_bdd.get_list<niveau_type_etablissement>(niveau_type_etablissement::Id_Tp_Etab,*it,
                                                                  niveau_type_etablissement::Id_Niveau);
        std::list<idt> niv_id_list_other;
        for (auto niv_it = te_niv_vec.cbegin(); niv_it != te_niv_vec.cend(); ++niv_it)
            niv_id_list_other.push_back(niv_it->id_niveau());
        niv_id_list.merge(niv_id_list_other);
    }
    niv_id_list.unique();
    for (auto it = 0; it != m_niv_list->count(); ++it) {
        if(niv_id_list.empty() || niv_id_list.front() != m_niv_list->id(it)) {
            m_niv_list->item(it)->setCheckState(Qt::Unchecked);
            m_niv_list->item(it)->setHidden(true);
            }
        else {
            m_niv_list->item(it)->setHidden(false);
            niv_id_list.pop_front();
        }
    }
}

// Groupe
groupe_new_modif_form::groupe_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent)
    : abstract_type_nc_nom_new_modif_form(static_cast<mps::b2d::bdd_predef &>(bdd),
                                    "groupe_root",
                                    groupe::ID,
                                    tr("Type de groupe :"),
                                    tr("Nom abrégé du groupe :"),
                                    tr("Nom du groupe :"),
                                    new_ent,parent) {
    // Nom
    if(!m_new)
        set_noms(m_bdd.get_list<groupe>(groupe::Nom));

    // Choix de groupe année - classe
    m_an_cl_gr = new QGroupBox(tr("Groupe pour une :"));
    m_an_radio = new QRadioButton(tr("Année"));
    m_cl_radio = new QRadioButton(tr("Classe"));
    m_cl_radio->setChecked(true);
    m_classe_select = new classe_select_widget(bdd,Qt::Vertical);

    // Identifiant des groupes.
    m_alpha_label = new QLabel(tr("Identifiant des groupes :"));
    m_alpha_cb = new QComboBox;
    m_alpha_cb->addItem(tr("Nombre arabe (1)"),mps::divers::num_to_string::Arabe_Suivant_Style);
    m_alpha_cb->addItem(tr("Nombre romain (I)"),mps::divers::num_to_string::Romain_Suivant_Style);
    m_alpha_cb->addItem(tr("Minuscule (a)"),mps::divers::num_to_string::Minuscule_Style);
    m_alpha_cb->addItem(tr("Majuscule (A)"),mps::divers::num_to_string::Majuscule_Style);
    m_alpha_cb->addItem(tr("Minuscule grec (\u03B1)"),mps::divers::num_to_string::Grec_Minuscule_Style);
    m_alpha_cb->addItem(tr("Majuscule grec (\u0394)"),mps::divers::num_to_string::Grec_Majuscule_Style);

    // Option
    m_opt_gr = new QGroupBox(tr("Options :"));
    m_exclusif_check = new QCheckBox(tr("Exclusif"));
    m_total_check = new QCheckBox(tr("Total"));

    // Calque
    m_an_cl_layout = new QHBoxLayout(m_an_cl_gr);
    m_an_cl_layout->addWidget(m_cl_radio);
    m_an_cl_layout->addWidget(m_an_radio);

    m_opt_layout = new QHBoxLayout(m_opt_gr);
    m_opt_layout->addWidget(m_exclusif_check);
    m_opt_layout->addWidget(m_total_check);

    m_main_layout->addWidget(m_an_cl_gr);
    m_main_layout->addWidget(m_classe_select);
    m_main_layout->addWidget(m_alpha_label);
    m_main_layout->addWidget(m_alpha_cb);
    m_main_layout->addWidget(m_opt_gr);
}

void groupe_new_modif_form::connexion() {
    abstract_type_nc_nom_new_modif_form::connexion();
    connect(m_cl_radio,&QRadioButton::toggled,m_classe_select,&classe_select_widget::set_enabled_etab_classe);
}

void groupe_new_modif_form::save() {
    groupe groupe;
    if(!m_new)
        groupe.set_id(id());
    groupe.set_nc(nc());
    groupe.set_nom(nom());
    groupe.set_type(id_type());
    groupe.set_style_num(m_alpha_cb->currentData().toUInt());
    if(m_exclusif_check->isChecked())
        groupe.add(groupe::Exclusif);
    if(m_total_check->isChecked())
        groupe.add(groupe::Total);
    if(m_cl_radio->isChecked()) {
        groupe.set_id_an(0);
        groupe.set_id_classe(m_classe_select->id());
    }
    else {
        groupe.set_id_an(m_classe_select->id_an());
        groupe.set_id_classe(0);
    }
    m_bdd.save(groupe);
}

void groupe_new_modif_form::update_data() {
    if(!m_new) {
        groupe gr;
        update_temp<groupe>(gr);
        m_alpha_cb->setCurrentIndex(m_alpha_cb->findData(gr.style_num()));
        m_exclusif_check->setChecked(gr.code().test(groupe::Exclusif));
        m_total_check->setChecked(gr.code().test(groupe::Total));
        m_cl_radio->setChecked(gr.id_classe());
        if(gr.id_classe()){
            classe cl(gr.id_classe());
            m_bdd.get(cl);
            m_classe_select->set_id_an(cl.id_an());
            m_classe_select->set_id_etab(cl.id_etab());
            m_classe_select->set_id(cl.id());
        }
        else
            m_classe_select->set_id_an(gr.id_an());
    }
    else {
        m_exclusif_check->setChecked(true);
        m_total_check->setChecked(true);
    }
}


// niveau
niveau_new_modif_form::niveau_new_modif_form(mps::b2d::bdd & bdd, bool new_ent, QWidget *parent)
    : mps::dialogue::abstract_type_nc_nom_new_modif_form (static_cast<mps::b2d::bdd_predef &>(bdd),
                                                "etude_root",
                                                niveau::ID,
                                                tr("Type de niveau :"),
                                                tr("Nom abrégé du niveau :"),
                                                tr("Nom du niveau :"),
                                                new_ent,parent)
{

    // Nom
    auto niveaux = m_bdd.get_list<niveau>();
    if(!m_new)
        set_noms(niveaux);

    // Type d'établissment
    m_list_label = new QLabel(tr("Type d'établissement pouvant accueillir le niveau:"));
    m_te_list = new mps::widget::check_list(m_bdd.get_list<type_etablissement>());

    // niveaux précédents
    m_niv_prec_label = new QLabel(tr("Niveaux précédents:"));
    m_niv_prec_list = new mps::widget::check_list(niveaux);

    // niveaux suivants
    m_niv_suiv_label = new QLabel(tr("Niveaux suivants:"));
    m_niv_suiv_list = new mps::widget::check_list(niveaux);

    // Calques
    m_niv_prec_layout = new QVBoxLayout;
    m_niv_prec_layout->addWidget(m_niv_prec_label);
    m_niv_prec_layout->addWidget(m_niv_prec_list);

    m_niv_suiv_layout = new QVBoxLayout;
    m_niv_suiv_layout->addWidget(m_niv_suiv_label);
    m_niv_suiv_layout->addWidget(m_niv_suiv_list);

    m_niv_layout = new QHBoxLayout;
    m_niv_layout->addLayout(m_niv_prec_layout);
    m_niv_layout->addLayout(m_niv_suiv_layout);

    m_main_layout->addWidget(m_list_label);
    m_main_layout->addWidget(m_te_list);
    m_main_layout->addLayout(m_niv_layout);
}

void niveau_new_modif_form::save() {
    niveau niv;
    if(!m_new)
        niv.set_id(id());
    niv.set_nc(nc());
    niv.set_nom(nom());
    niv.set_type(id_type());
    m_bdd.save(niv);
    mps::widget::check_list::Save<niveau_type_etablissement>::save(m_bdd,m_te_list,[&niv](niveau_type_etablissement & nivTE, idt id_te){
                                                                                        nivTE.set_id_niveau(niv.id());
                                                                                        nivTE.set_id_tp_etab(id_te);
                                                                                    });
    mps::widget::check_list::Save<filiation_niveau>::save(m_bdd,m_niv_prec_list,[&niv](filiation_niveau & filNiv, idt id_niv){
                                                                                        filNiv.set_id_precedent(id_niv);
                                                                                        filNiv.set_id_suivant(niv.id());
                                                                                    });
    mps::widget::check_list::Save<filiation_niveau>::save(m_bdd,m_niv_suiv_list,[&niv](filiation_niveau & filNiv, idt id_niv){
                                                                                        filNiv.set_id_precedent(niv.id());
                                                                                        filNiv.set_id_suivant(id_niv);
                                                                                    });
}

void niveau_new_modif_form::update_data() {
    if(!m_new) {
        niveau niv;
        update_temp<niveau>(niv);
        m_te_list->set_checks_by_id<niveau_type_etablissement>(
                    m_bdd.get_list<niveau_type_etablissement>(niveau_type_etablissement::Id_Niveau,niv.id()),
                       [](const niveau_type_etablissement& ent)->idt{return ent.id_tp_etab();});
        m_niv_prec_list->set_checks_by_id<filiation_niveau>(
                    m_bdd.get_list<filiation_niveau>(filiation_niveau::Id_Suivant,niv.id()),
                       [](const filiation_niveau & ent){return ent.id_precedent();});
        m_niv_suiv_list->set_checks_by_id<filiation_niveau>(
                    m_bdd.get_list<filiation_niveau>(filiation_niveau::Id_Precedent,niv.id()),
                       [](const filiation_niveau & ent){return ent.id_suivant();});
    }
}

// type_controle
type_controle_new_modif_form::type_controle_new_modif_form(mps::b2d::bdd &bdd, bool new_ent, QWidget *parent)
    : abstract_controle_new_modif_form(bdd,
                                      "Parent :",
                                      "Nom abrégé du type de controle :",
                                      "Nom du type de controle :",
                                      new_ent,parent) {
    // Nom
    if(!m_new)
        set_noms(m_bdd.get_list<type_controle>(type_controle::Nom));

    // parent
    m_parent_tree->set_tree_nc_nom_id(bdd.get_arbre<type_controle>());
    m_parent_tree->expandAll();

    //Catégorie
    m_categorie_check = new QCheckBox("Catégorie");

    //Modif
    m_modif_gr = new QGroupBox(tr("Modifiable dans les controles du type :"));
    m_appreciation_modif_check = new  QCheckBox(tr("Appreciation"));
    m_barre_modif_check = new QCheckBox(tr("Barre de classement"));
    m_capacite_modif_check = new QCheckBox(tr("Capacités"));
    m_classement_modif_check = new QCheckBox(tr("Classement"));
    m_commentaire_modif_check = new QCheckBox(tr("Commentaire"));
    m_competence_modif_check = new QCheckBox(tr("Compétence"));
    m_courbe_modif_check = new QCheckBox(tr("Courbe d'ajustement"));
    m_decimale_modif_check = new QCheckBox(tr("Décimale"));
    m_depassement_modif_check = new QCheckBox(tr("Dépassement"));
    m_minima_modif_check = new QCheckBox(tr("Minima"));
    m_note_modif_check = new QCheckBox(tr("Noté"));
    m_total_modif_check = new QCheckBox(tr("Total"));
    m_type_note_modif_check = new QCheckBox(tr("Nombre-Lettre"));
    note_modif_enable();
    option_enable();

    //Calque
    m_modif_layout = new QGridLayout(m_modif_gr);
    m_modif_layout->addWidget(m_appreciation_modif_check,Ligne_Zero,Option_Modif_Colonne);
    m_modif_layout->addWidget(m_commentaire_modif_check,Ligne_Une,Option_Modif_Colonne);
    m_modif_layout->addWidget(m_competence_modif_check,Ligne_Deux,Option_Modif_Colonne);
    m_modif_layout->addWidget(m_capacite_modif_check,Ligne_Trois,Option_Modif_Colonne);
    m_modif_layout->addWidget(m_note_modif_check,Ligne_Zero,Note_Modif_Colonne);
    m_modif_layout->addWidget(m_type_note_modif_check,Ligne_Une,Note_Modif_Colonne);
    m_modif_layout->addWidget(m_total_modif_check,Ligne_Deux,Note_Modif_Colonne);
    m_modif_layout->addWidget(m_decimale_modif_check,Ligne_Trois,Note_Modif_Colonne);
    m_modif_layout->addWidget(m_depassement_modif_check,Ligne_Zero,Nombre_Modif_Colonne);
    m_modif_layout->addWidget(m_courbe_modif_check,Ligne_Une,Nombre_Modif_Colonne);
    m_modif_layout->addWidget(m_classement_modif_check,Ligne_Deux,Nombre_Modif_Colonne);
    m_modif_layout->addWidget(m_barre_modif_check,Ligne_Trois,Nombre_Modif_Colonne);
    m_modif_layout->addWidget(m_minima_modif_check,Ligne_Quatre,Nombre_Modif_Colonne);

    m_main_layout->insertWidget(4,m_categorie_check);
    m_main_layout->addWidget(m_modif_gr);
}

void type_controle_new_modif_form::connexion() {
    abstract_controle_new_modif_form::connexion();
    connect(m_categorie_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::option_enable);
    connect(m_note_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::note_modif_enable);
    connect(m_chiffre_radio,&QRadioButton::toggled,this,&type_controle_new_modif_form::note_modif_enable);
    connect(m_classement_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::note_modif_enable);
    connect(m_barre_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::note_modif_enable);
    connect(m_note_modif_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::note_modif_enable);
    connect(m_type_note_modif_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::note_modif_enable);
    connect(m_classement_modif_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::note_modif_enable);
    connect(m_barre_modif_check,&QCheckBox::stateChanged,this,&type_controle_new_modif_form::note_modif_enable);
}

void type_controle_new_modif_form::note_modif_enable() {
    auto enable = m_note_check->isChecked() || m_note_modif_check->isChecked();
    m_type_note_modif_check->setEnabled(enable);
    m_total_modif_check->setEnabled(enable);
    m_decimale_modif_check->setEnabled(enable);
    if(!enable) {
        m_type_note_modif_check->setChecked(false);
        m_total_modif_check->setChecked(false);
        m_decimale_modif_check->setChecked(false);
    }
    enable &= m_chiffre_radio->isChecked() || m_type_note_modif_check->isChecked();
    m_depassement_modif_check->setEnabled(enable);
    m_courbe_modif_check->setEnabled(enable);
    m_classement_modif_check->setEnabled(enable);
    if(!enable) {
        m_depassement_modif_check->setChecked(false);
        m_courbe_modif_check->setChecked(false);
        m_classement_modif_check->setChecked(false);
    }
    enable &= m_classement_check->isChecked() || m_classement_modif_check->isChecked();
    m_barre_modif_check->setEnabled(enable);
    if(!enable)
        m_barre_modif_check->setChecked(false);
    enable &= m_barre_check->isChecked() || m_barre_modif_check->isChecked();
    m_minima_modif_check->setEnabled(enable);
    if(!enable)
        m_minima_modif_check->setChecked(false);
}

void type_controle_new_modif_form::option_enable() {
    auto enable = ! m_categorie_check->isChecked();
    m_note_gr->setEnabled(enable);
    m_opt_gr->setEnabled(enable);
    m_modif_gr->setEnabled(enable);
    if(!enable) {
        m_note_check->setChecked(false);
        m_appreciation_check->setChecked(false);
        m_capacite_check->setChecked(false);
        m_commentaire_check->setChecked(false);
        m_competence_check->setChecked(false);
        m_appreciation_modif_check->setChecked(false);
        m_capacite_modif_check->setChecked(false);
        m_commentaire_modif_check->setChecked(false);
        m_competence_modif_check->setChecked(false);
        m_note_modif_check->setChecked(false);
    }
}

void type_controle_new_modif_form::save() {
    auto tpc = entity_note_option<type_controle>(!m_categorie_check->isChecked());
    tpc.set_parent(id_parent());
    if(m_categorie_check->isChecked())
        tpc.add(type_controle::Categorie);
    else {
        //Option modification
        if(m_appreciation_modif_check->isChecked())
            tpc.add(type_controle::Appreciation_Modifiable);
        if(m_capacite_modif_check->isChecked())
            tpc.add(type_controle::Capacites_Modifiable);
        if(m_commentaire_modif_check->isChecked())
            tpc.add(type_controle::Commentaire_Modifiable);
        if(m_competence_modif_check->isChecked())
            tpc.add(type_controle::Competences_Modifiable);
        if(m_note_modif_check->isChecked())
            tpc.add(type_controle::Note_Modifiable);
        if(m_note_check->isChecked() || m_note_modif_check->isChecked()) {
            if(m_type_note_modif_check->isChecked())
                tpc.add(type_controle::Type_Note_Modifiable);
            if(m_total_modif_check->isChecked())
                tpc.add(type_controle::Total_Modifiable);
            if(m_decimale_modif_check->isChecked())
                tpc.add(type_controle::Decimale_Modifiable);
            if(m_chiffre_radio->isChecked() || m_type_note_modif_check->isChecked()){
                if(m_depassement_check->isChecked())
                    tpc.add(type_controle::Depassement_Modifiable);
                if(m_courbe_modif_check->isChecked())
                    tpc.add(type_controle::Courbe_Modifiable);
                if(m_classement_modif_check->isChecked())
                    tpc.add(type_controle::Classement_Modifiable);
                if(m_classement_check->isChecked() || m_classement_modif_check->isChecked()){
                    if(m_barre_modif_check->isChecked())
                        tpc.add(type_controle::Barre_Modifiable);
                    if((m_barre_check->isChecked() || m_barre_modif_check->isChecked())
                            && m_minima_modif_check->isChecked())
                        tpc.add(type_controle::Minima_Modifiable);
                }
            }
        }
    }
    m_bdd.save(tpc);
}

void type_controle_new_modif_form::update_data() {
    if(!m_new){
        type_controle tpc;
        update_temp(tpc);
        m_categorie_check->setChecked(tpc.code().test(type_controle::Categorie));
        if(!m_categorie_check->isChecked()) {
            update_note_option(tpc);
            //Option modification
            m_appreciation_modif_check->setChecked(tpc.code().test(type_controle::Appreciation_Modifiable));
            m_capacite_modif_check->setChecked(tpc.code().test(type_controle::Capacites_Modifiable));
            m_commentaire_modif_check->setChecked(tpc.code().test(type_controle::Commentaire_Modifiable));
            m_competence_modif_check->setChecked(tpc.code().test(type_controle::Competences_Modifiable));
            m_note_modif_check->setChecked(tpc.code().test(type_controle::Note_Modifiable));
            if(m_note_check->isChecked() || m_note_modif_check->isChecked()) {
                m_type_note_modif_check->setChecked(tpc.code().test(type_controle::Type_Note_Modifiable));
                m_total_modif_check->setChecked(tpc.code().test(type_controle::Total_Modifiable));
                m_decimale_modif_check->setChecked(tpc.code().test(type_controle::Decimale_Modifiable));
                if(m_chiffre_radio->isChecked() || m_type_note_modif_check->isChecked()){
                    m_depassement_modif_check->setChecked(tpc.code().test(type_controle::Depassement_Modifiable));
                    m_courbe_modif_check->setChecked(tpc.code().test(type_controle::Courbe_Modifiable));
                    m_classement_modif_check->setChecked(tpc.code().test(type_controle::Classement_Modifiable));
                    if(m_classement_check->isChecked() || m_classement_modif_check->isChecked()){
                        m_barre_modif_check->setChecked(tpc.code().test(type_controle::Barre_Modifiable));
                        if(m_barre_check->isChecked() || m_barre_modif_check->isChecked())
                            m_minima_modif_check->setChecked(type_controle::Minima_Modifiable);
                    }
                }
            }
        }
    }
}

// type_etablissement
type_etablissement_new_modif_form::type_etablissement_new_modif_form(mps::b2d::bdd & bdd, bool new_ent, QWidget *parent)
    : mps::dialogue::abstract_nc_nom_new_modif_form (bdd,
                                            tr("Nom abrégé du type d'établissment :"),
                                            tr("Nom du type d'établissement :"),
                                            new_ent,parent) {
    // Nom
    if(!m_new)
        set_noms(m_bdd.get_list<type_etablissement>(type_etablissement::Nom));

    // Niveaux
    m_niv_select = new mps::widget::select_in_list_box(tr("Niveaux existents :"),tr("Niveaux présents :"));

    // Calque
    m_main_layout->addWidget(m_niv_select);
}

void type_etablissement_new_modif_form::save() {
    if(m_new) {
        type_etablissement te(nc(),nom());
        m_bdd.save(te);
        auto niv = m_niv_select->value().second;
        for (auto it =niv.cbegin(); it != niv.cend(); ++it)
            m_bdd.save(niveau_type_etablissement(it->toUInt(),te.id()));
    }
    else {
        type_etablissement te(id());
        if(m_bdd.test_autorisation(te,mps::b2d::Modif)) {
            te.set_nc(nc());
            te.set_nom(nom());
            m_bdd.save(te);
        }
        auto niv_old = m_bdd.get_list<niveau_type_etablissement>(niveau_type_etablissement::Id_Tp_Etab,te.id());
        auto niv_new = m_niv_select->value().second;
        for(auto old_it = niv_old.cbegin(); old_it != niv_old.cend(); ++old_it) {
            auto new_it = niv_new.begin();
            while(new_it != niv_new.end() && old_it->id_niveau() != new_it->toUInt())
                ++new_it;
            if(new_it == niv_new.end()) {
                niveau_type_etablissement nivTE(old_it->id_niveau(),te.id());
                m_bdd.get_unique(nivTE);
                m_bdd.del(nivTE);
            }
            else
                new_it->setValue(0);
        }
        for (auto new_it = niv_new.cbegin(); new_it != niv_new.cend(); ++new_it) {
            if(new_it->toUInt() != 0)
                m_bdd.save(niveau_type_etablissement(new_it->toUInt(),te.id()));
        }
    }
}

void type_etablissement_new_modif_form::update_data() {
    if(m_new) {
        auto niv_vec = m_bdd.get_list<niveau>();
        std::map<QVariant, QString> nivOut;
        for(auto it = niv_vec.cbegin(); it != niv_vec.cend(); ++it)
            nivOut[it->id()] = it->nom();
        m_niv_select->set_value(nivOut,std::map<QVariant, QString>());
    }
    else {
        type_etablissement te;
        update_temp<type_etablissement>(te);
        auto niv_vec = m_bdd.get_list<niveau>();
        std::map<QVariant, QString> nivIn;
        std::map<QVariant, QString> nivOut;
        for(auto it = niv_vec.cbegin(); it != niv_vec.cend(); ++it) {
            if(m_bdd.exists_unique(niveau_type_etablissement(it->id(),te.id())))
                nivIn[it->id()] = it->nom();
            else
                nivOut[it->id()] = it->nom();
        }
        m_niv_select->set_value(nivOut,nivIn);
    }
}
