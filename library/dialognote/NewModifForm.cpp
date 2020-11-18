#include "NewModifForm.h"

using namespace noteMPS;

// AbstractControle
AbstractControleNewModifForm::AbstractControleNewModifForm(bddMPS::Bdd &bdd, const QString & LabelParent,
                                                           const QString &labelNc, const QString &labelNom,
                                                           bool newEnt, QWidget * parent)
    : AbstractParentNcNomNewModifForm(bdd, {"nom","nom abrégé"},
                                      LabelParent,
                                      labelNc,
                                      labelNom,
                                      newEnt,parent) {
    //note
    m_noteGr = new QGroupBox(tr("Notation :"));
    m_noteCheck = new QCheckBox(tr("Devoir noté"));
    m_typeLabel = new QLabel(tr("Type de notation :"));
    m_chiffreRadio = new QRadioButton(tr("Nombre"));
    m_chiffreRadio->setChecked(true);
    m_lettreRadio = new QRadioButton(tr("Lettre"));
    m_totalLabel = new QLabel;
    m_totalSpinBox = new widgetMPS::SpinBoxLettre;
    m_depassementCheck = new QCheckBox(tr("Depassement"));
    m_decimalLabel = new QLabel;
    m_decimaleCB = new QComboBox;
    m_courbeCheck = new QCheckBox(tr("Courbe d'ajustement"));
    m_classementCheck = new QCheckBox(tr("Classement"));
    m_barreCheck = new QCheckBox(tr("Barre de classement :"));
    m_minimaSpinBox = new widgetMPS::SpinBoxDecimale;
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
    m_noteLayout = new QGridLayout(m_noteGr);
    m_noteLayout->addWidget(m_noteCheck,CheckNoteLigne,LabelColonne);
    m_noteLayout->addWidget(m_typeLabel,TypeLigne,LabelColonne);
    m_noteLayout->addWidget(m_chiffreRadio,TypeLigne,ColonneUne);
    m_noteLayout->addWidget(m_lettreRadio,TypeLigne,ColonneDeux);
    m_noteLayout->addWidget(m_totalLabel,TotalLigne,LabelColonne);
    m_noteLayout->addWidget(m_totalSpinBox,TotalLigne,ColonneUne);
    m_noteLayout->addWidget(m_depassementCheck,TotalLigne,ColonneDeux);
    m_noteLayout->addWidget(m_decimalLabel,DecimaleLigne,LabelColonne);
    m_noteLayout->addWidget(m_decimaleCB,DecimaleLigne,ColonneUne);
    m_noteLayout->addWidget(m_courbeCheck,DecimaleLigne,ColonneDeux);
    m_noteLayout->addWidget(m_classementCheck,ClassementLigne,ColonneZero);
    m_noteLayout->addWidget(m_barreCheck,BarreLigne,LabelColonne);
    m_noteLayout->addWidget(m_minimaSpinBox,BarreLigne,ColonneUne);

    m_optLayout = new QGridLayout(m_optGr);
    m_optLayout->addWidget(m_appreciationCheck,AppreciationCommentaireLigne,ColonneZero);
    m_optLayout->addWidget(m_commentaireCheck,AppreciationCommentaireLigne,ColonneUne);
    m_optLayout->addWidget(m_competenceCheck,CapaciteCompetenceLigne,ColonneZero);
    m_optLayout->addWidget(m_capaciteCheck,CapaciteCompetenceLigne,ColonneUne);

    m_mainLayout->addWidget(m_noteGr);
    m_mainLayout->addWidget(m_optGr);
}

void AbstractControleNewModifForm::barreEnable() {
    auto enable = m_barreCheck->isChecked();
    m_minimaSpinBox->setEnabled(enable);
    if(enable) {
        decimaleChange();
        totalChange();
        m_minimaSpinBox->setValueDouble(m_totalSpinBox->value() / 2.);
        connect(m_decimaleCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&TypeControleNewModifForm::decimaleChange);
        connect(m_totalSpinBox,qOverload<int>(&widgetMPS::SpinBoxLettre::valueChanged),this,&TypeControleNewModifForm::totalChange);
    }
    else {
        disconnect(m_decimaleCB,nullptr,this,nullptr);
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
    m_typeLabel->setEnabled(enable);
    m_chiffreRadio->setEnabled(enable);
    m_lettreRadio->setEnabled(enable);
    m_totalLabel->setEnabled(enable);
    m_totalSpinBox->setEnabled(enable);
    m_depassementCheck->setEnabled(enable);
    m_decimalLabel->setEnabled(enable);
    m_decimaleCB->setEnabled(enable);
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
        m_decimaleCB->clear();
    }
}

void AbstractControleNewModifForm::typeNoteChange() {
    using attDecimale = attributMPS::AttributDecimale;
    if(m_chiffreRadio->isChecked()) {
        m_totalLabel->setText("Total :");
        m_totalSpinBox->setStyle(diversMPS::NumToTexte::Arabe);
        m_totalSpinBox->setMinimum(1);
        m_totalSpinBox->setMaximum(1000);
        m_totalSpinBox->setValue(20);
        m_depassementCheck->setEnabled(m_noteCheck->isChecked());
        m_courbeCheck->setEnabled(m_noteCheck->isChecked());
        m_decimalLabel->setText(tr("Décimale :"));
        m_decimaleCB->clear();
        for (szt i = 0; i != attDecimale::NbrValues; ++i)
            m_decimaleCB->addItem(QString::number(1./ attDecimale::Decimale.at(i),'f',attDecimale::Precision.at(i)),
                                  attDecimale::Decimale.at(i));
        m_classementCheck->setEnabled(m_noteCheck->isChecked());
    }
    else {
        m_totalLabel->setText("De A à :");
        m_totalSpinBox->setStyle(diversMPS::NumToTexte::Majuscule);
        m_totalSpinBox->setMinimum(0);
        m_totalSpinBox->setMaximum(25);
        m_totalSpinBox->setValue(3);
        m_depassementCheck->setEnabled(false);
        m_depassementCheck->setChecked(false);
        m_courbeCheck->setEnabled(false);
        m_courbeCheck->setChecked(false);
        m_decimalLabel->setText(tr("Ajustement :"));
        m_classementCheck->setEnabled(false);
        m_classementCheck->setChecked(false);
        m_decimaleCB->clear();
        m_decimaleCB->addItem(" ",Controle::NoPlus);
        m_decimaleCB->addItem("+",Controle::Plus);
        m_decimaleCB->addItem("++",Controle::PPlus);
        m_decimaleCB->addItem("+++",Controle::PPPlus);
    }
}

// Annee
AnneeNewModifForm::AnneeNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractNewModifForm (bdd,newEnt,parent) {
    // Annee
    m_anneeLabel = new QLabel(newEnt ? tr("Nouvelle année scolaire :")
                                     : tr("Année scolaire courante :"));
    if(newEnt) {
        m_spinBox = new SpinBoxAnneeScolaire();
        m_anneeCourante = new QCheckBox(tr("Année courante"));
    }
    else
        m_spinBox = new SpinBoxAnneeScolaire(m_bdd.getList<Annee>(Annee::Num));
    m_spinBox->setNowValue();

    // Calque
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_anneeLabel);
    if(m_new) {
        m_mainLayout->addWidget(m_spinBox);
        m_mainLayout->addWidget(m_anneeCourante);
    }
    else if (!m_spinBox->values().empty())
        m_mainLayout->addWidget(m_spinBox);
    else
        m_anneeLabel->setText("Vous devez préalablement créer une année scolaire dans le menu :\n "
                              "fichier>Nouveau>Année scolaire");
}

void AnneeNewModifForm::connexion() {
    if(m_new){
        connect(m_spinBox,&SpinBoxAnneeScolaire::valueChanged,this,[this](){emit savePermis(!m_bdd.existsUnique(m_spinBox->value()));});
        emit savePermis(!m_bdd.existsUnique(m_spinBox->value()));
    }
    else if (m_spinBox->values().empty())
        emit savePermis(false);
}

void AnneeNewModifForm::save() {
    if(m_new)
        m_bdd.save(m_spinBox->value());
}

// Classe
ClasseNewModifForm::ClasseNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom abrégé de la classe :"),
                                            tr("Nom de la classe :"),
                                            newEnt,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.getList<Classe>(Classe::Nom));

    // Annee - Etab - Niveaux
    m_anneeSelect = new AnneeSelectWidget(m_bdd);
    m_etabSelect = new EtablissementSelectWidget(m_bdd);
    m_nivSelect = new NiveauxSelectWidget(m_bdd);

    // Num
    m_numLabel = new QLabel(tr("Numéro :"));
    m_numSpinBox = new QSpinBox();
    m_numSpinBox->setMinimum(0);
    m_numSpinBox->setSpecialValueText(" ");

    // Debut et fin des cours
    m_debutLabel = new QLabel(tr("Début des cours:"));
    m_finLabel = new QLabel(tr("Fin des cours:"));
    m_debutCalendar = new QCalendarWidget;
    m_finCalendar = new QCalendarWidget;

    // Calque
    m_mainLayout->addWidget(m_anneeSelect);
    m_mainLayout->addWidget(m_etabSelect);
    m_mainLayout->addWidget(m_nivSelect);
    m_mainLayout->addWidget(m_numLabel);
    m_mainLayout->addWidget(m_numSpinBox);
    m_debutLayout = new QVBoxLayout;
    m_debutLayout->addWidget(m_debutLabel);
    m_debutLayout->addWidget(m_debutCalendar);
    m_finLayout = new QVBoxLayout;
    m_finLayout->addWidget(m_finLabel);
    m_finLayout->addWidget(m_finCalendar);
    m_calendarLayout = new QHBoxLayout;
    m_calendarLayout->addLayout(m_debutLayout);
    m_calendarLayout->addLayout(m_finLayout);
    m_mainLayout->addLayout(m_calendarLayout);

}

void ClasseNewModifForm::connexion() {
    AbstractNcNomNewModifForm::connexion();
    connect(m_etabSelect,&EtablissementSelectWidget::idChanged,m_nivSelect,&NiveauxSelectWidget::setIdEtab);
    connect(m_anneeSelect,&AnneeSelectWidget::idChanged,this,&ClasseNewModifForm::updateCalendar);
    connect(m_debutCalendar,&QCalendarWidget::selectionChanged,this,[this](){emit savePermis(valide());});
    connect(m_finCalendar,&QCalendarWidget::selectionChanged,this,[this](){emit savePermis(valide());});
}

void ClasseNewModifForm::save() {
    Classe cl;
    if(!m_new)
        cl.setId(id());
    cl.setNc(nc());
    cl.setNom(nom());
    cl.setIdAn(m_anneeSelect->id());
    cl.setIdEtab(m_etabSelect->id());
    cl.setIdNiveau(m_nivSelect->id());
    cl.setNum(m_numSpinBox->value());
    m_bdd.save(cl);
    if(m_new)
        m_idClasse = cl.id();
    DonneeCible dnCb;
    dnCb.setIdDonnee(bdd().refToId<Donnee>("date_defaut_dn"));
    dnCb.setIdCible(m_idClasse);
    dnCb.setCible(bdd().cible<Classe>());
    dnCb.setNum(donnee::EntreeNum);
    bdd().getUnique(dnCb);
    dnCb.setValeur(m_debutCalendar->selectedDate());
    bdd().save(dnCb);
    dnCb.setId(0);
    dnCb.setNum(donnee::SortieNum);
    bdd().getUnique(dnCb);
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

void ClasseNewModifForm::updateData() {
    if(m_new) {
        m_nivSelect->setIdEtab(m_etabSelect->id());
        updateCalendar();
    }
    else {
        Classe cl;
        updateTemp<Classe>(cl);
        m_idClasse = cl.id();
        m_anneeSelect->setId(cl.idAn());
        m_etabSelect->setId(cl.idEtab());
        m_nivSelect->setIdEtab(cl.idEtab());
        m_nivSelect->setId(cl.idNiveau());
        m_numSpinBox->setValue(cl.num());
        DonneeCible dnCb;
        dnCb.setIdDonnee(bdd().refToId<Donnee>("date_defaut_dn"));
        dnCb.setIdCible(m_idClasse);
        dnCb.setCible(bdd().cible<Classe>());
        dnCb.setNum(donnee::EntreeNum);
        bdd().getUnique(dnCb);
        m_debutCalendar->setSelectedDate(dnCb.valeur().toDate());
        dnCb.setId(0);
        dnCb.setNum(donnee::SortieNum);
        bdd().getUnique(dnCb);
        m_finCalendar->setSelectedDate(dnCb.valeur().toDate());
    }
}

bool ClasseNewModifForm::valide() const {
    return AbstractNcNomNewModifForm::valide()
            && m_nivSelect->id() && m_etabSelect->id()
            && m_debutCalendar->selectedDate() < m_finCalendar->selectedDate();
}

// Controle
ControleNewModifForm::ControleNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent)
    : AbstractControleNewModifForm(bdd,
                                      "Type :",
                                      "Nom abrégé du controle :",
                                      "Nom du controle :",
                                      newEnt,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.getList<Controle>(Controle::Nom));

    // Type
    m_parentTree->setTreeRef(bdd.getArbre<TypeControle>(),
                          [](const TypeControle & tc)->QTreeWidgetItem * {
        auto item = new QTreeWidgetItem({tc.nom(),tc.nc()});
        item->setData(widgetMPS::TreeWidget::IdColonne,widgetMPS::TreeWidget::IdRole,tc.id());
        if(!tc.code().test(TypeControle::Categorie))
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        else
            item->setFlags(Qt::ItemIsEnabled);
        return item;
    });

    // Destination
    m_classeRadio = new QRadioButton(tr("Classe"));
    m_classeRadio->setChecked(true);
    m_eleveRadio = new QRadioButton(tr("Eleve"));
    m_groupeRadio = new QRadioButton(tr("Groupe"));
    m_pourBG = new QButtonGroup(this);
    m_pourBG->addButton(m_classeRadio,Classe::ID);
    m_pourBG->addButton(m_eleveRadio,Eleve::ID);
    m_pourBG->addButton(m_groupeRadio,Groupe::ID);
    //m_entitySelect = new
    m_pourGr = new QGroupBox("Pour :");
    m_pourButtonLayout = new QHBoxLayout;
    m_pourButtonLayout->addWidget(m_classeRadio);
    m_pourButtonLayout->addWidget(m_groupeRadio);
    m_pourButtonLayout->addWidget(m_eleveRadio);
    m_pourLayout = new QVBoxLayout(m_pourGr);
    m_pourLayout->addLayout(m_pourButtonLayout);

    m_dateLabel = new QLabel("Date et heure du controle :");
    m_dateTimeEdit = new QDateTimeEdit;
    m_numLabel = new QLabel("Numéro de controle :");
    m_numSpinBox = new QSpinBox;
    m_mainLayout->insertWidget(4,m_pourGr);
    m_mainLayout->insertWidget(5,m_dateLabel);
    m_mainLayout->insertWidget(6,m_dateTimeEdit);
    m_mainLayout->insertWidget(7,m_numLabel);
    m_mainLayout->insertWidget(8,m_numSpinBox);

}

void ControleNewModifForm::connexion() {
    AbstractControleNewModifForm::connexion();

}


void ControleNewModifForm::save() {
    auto ctr = entityNoteOption<Controle>(true);
    ctr.setIdType(idParent());

    m_bdd.save(ctr);
}

void ControleNewModifForm::updateData() {
    if(!m_new){
        Controle ctr;
        AbstractNcNomNewModifForm::updateTemp(ctr);
        setParent(ctr.idType());
    }
}

// Etablissement
EtablissementNewModifForm::EtablissementNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom abrégé de l'établissement :"),
                                            tr("Nom de l'établissement :"),
                                            newEnt,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.getList<Etablissement>());

    // Type d'établissment
    m_TELabel = new QLabel(tr("Type(s) de l'établissement :"));
    m_TEList = new widgetMPS::Checklist(m_bdd.getList<TypeEtablissement>());

    // Niveaux
    m_nivLabel = new QLabel(tr("Niveaux présent dans l'établissement:"));
    m_nivList = new widgetMPS::Checklist(m_bdd.getList<Niveau>());

    // Calque
    m_mainLayout->addWidget(m_TELabel);
    m_mainLayout->addWidget(m_TEList);
    m_mainLayout->addWidget(m_nivLabel);
    m_mainLayout->addWidget(m_nivList);
}

void EtablissementNewModifForm::connexion() {
    AbstractNcNomNewModifForm::connexion();
    connect(m_TEList,&widgetMPS::Checklist::itemChanged,this,&EtablissementNewModifForm::updateNiveau);
}

void EtablissementNewModifForm::save() {
    Etablissement etab;
    if(!m_new)
        etab.setId(id());
    etab.setNc(nc());
    etab.setNom(nom());
    m_bdd.save(etab);
    widgetMPS::Checklist::Save<EtablissementType>::save(m_bdd,m_TEList,
                                                        [&etab](EtablissementType & etabType, idt idTE){
                                                                        etabType.setIdEtab(etab.id());
                                                                        etabType.setIdTpEtab(idTE);
                                                                    });
    widgetMPS::Checklist::Save<EtablissementNiveau>::save(m_bdd,m_nivList,
                                                          [&etab](EtablissementNiveau & etabNiv, idt idNiv){
                                                                        etabNiv.setIdEtab(etab.id());
                                                                        etabNiv.setIdNiveau(idNiv);
                                                                    });
}

void EtablissementNewModifForm::updateData() {
    if(!m_new) {
        Etablissement etab;
        updateTemp<Etablissement>(etab);
        m_TEList->setChecksById<EtablissementType>(m_bdd.getList<EtablissementType>(EtablissementType::IdEtab,etab.id()),
                       [](const EtablissementType& entity)->idt{return entity.idTpEtab();});
        m_nivList->setChecksById<EtablissementNiveau>(m_bdd.getList<EtablissementNiveau>(EtablissementNiveau::IdEtab,etab.id()),
                       [](const EtablissementNiveau& entity)->idt{return entity.idNiveau();});
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
        auto vecNivTE = m_bdd.getList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,*iter,NiveauTypeEtablissement::IdNiveau);
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
GroupeNewModifForm::GroupeNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent)
    : AbstractTypeNcNomNewModifForm(static_cast<bddMPS::BddPredef &>(bdd),
                                    "groupe_root_tp",
                                    Groupe::ID,
                                    tr("Type de groupe :"),
                                    tr("Nom abrégé du groupe :"),
                                    tr("Nom du groupe :"),
                                    newEnt,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.getList<Groupe>(Groupe::Nom));

    // Choix de groupe année - classe
    m_anClGr = new QGroupBox(tr("Groupe pour une :"));
    m_anRadio = new QRadioButton(tr("Année"));
    m_clRadio = new QRadioButton(tr("Classe"));
    m_clRadio->setChecked(true);
    m_classeSelect = new ClasseSelectWidget(bdd,Qt::Vertical);

    // Identifiant des groupes.
    m_alphaLabel = new QLabel(tr("Identifiant des groupes :"));
    m_alphaCB = new QComboBox;
    m_alphaCB->addItem(tr("Nombre arabe (1)"),diversMPS::NumToTexte::ArabeSuivant);
    m_alphaCB->addItem(tr("Nombre romain (I)"),diversMPS::NumToTexte::RomainSuivant);
    m_alphaCB->addItem(tr("Minuscule (a)"),diversMPS::NumToTexte::Minuscule);
    m_alphaCB->addItem(tr("Majuscule (A)"),diversMPS::NumToTexte::Majuscule);
    m_alphaCB->addItem(tr("Minuscule grec (\u03B1)"),diversMPS::NumToTexte::GrecMinuscule);
    m_alphaCB->addItem(tr("Majuscule grec (\u0394)"),diversMPS::NumToTexte::GrecMajuscule);

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

    m_mainLayout->addWidget(m_anClGr);
    m_mainLayout->addWidget(m_classeSelect);
    m_mainLayout->addWidget(m_alphaLabel);
    m_mainLayout->addWidget(m_alphaCB);
    m_mainLayout->addWidget(m_optGr);
}

void GroupeNewModifForm::connexion() {
    AbstractTypeNcNomNewModifForm::connexion();
    connect(m_clRadio,&QRadioButton::toggled,m_classeSelect,&ClasseSelectWidget::setEnabledEtabClasse);
}

void GroupeNewModifForm::save() {
    Groupe groupe;
    if(!m_new)
        groupe.setId(id());
    groupe.setNc(nc());
    groupe.setNom(nom());
    groupe.setType(idType());
    groupe.setStyleNum(m_alphaCB->currentData().toUInt());
    if(m_exclusifCheck->isChecked())
        groupe.add(Groupe::Exclusif);
    if(m_totalCheck->isChecked())
        groupe.add(Groupe::Total);
    if(m_clRadio->isChecked()) {
        groupe.setIdAn(0);
        groupe.setIdClasse(m_classeSelect->id());
    }
    else {
        groupe.setIdAn(m_classeSelect->idAn());
        groupe.setIdClasse(0);
    }
    m_bdd.save(groupe);
}

void GroupeNewModifForm::updateData() {
    if(!m_new) {
        Groupe groupe;
        updateTemp<Groupe>(groupe);
        m_alphaCB->setCurrentIndex(m_alphaCB->findData(groupe.styleNum()));
        m_exclusifCheck->setChecked(groupe.code().test(Groupe::Exclusif));
        m_totalCheck->setChecked(groupe.code().test(Groupe::Total));
        m_clRadio->setChecked(groupe.idClasse());
        if(groupe.idClasse()){
            Classe cl(groupe.idClasse());
            m_bdd.get(cl);
            m_classeSelect->setIdAn(cl.idAn());
            m_classeSelect->setIdEtab(cl.idEtab());
            m_classeSelect->setId(cl.id());
        }
        else
            m_classeSelect->setIdAn(groupe.idAn());
    }
    else {
        m_exclusifCheck->setChecked(true);
        m_totalCheck->setChecked(true);
    }
}


// Niveau
NiveauNewModifForm::NiveauNewModifForm(bddMPS::Bdd & bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractTypeNcNomNewModifForm (static_cast<bddMPS::BddPredef &>(bdd),
                                                "etude_root_tp",
                                                Niveau::ID,
                                                tr("Type de niveau :"),
                                                tr("Nom abrégé du niveau :"),
                                                tr("Nom du niveau :"),
                                                newEnt,parent)
{

    // Nom
    auto niveaux = m_bdd.getList<Niveau>();
    if(!m_new)
        setNoms(niveaux);

    // Type d'établissment
    m_listLabel = new QLabel(tr("Type d'établissement pouvant accueillir le niveau:"));
    m_TEList = new widgetMPS::Checklist(m_bdd.getList<TypeEtablissement>());

    // Niveaux précédents
    m_nivPrecLabel = new QLabel(tr("Niveaux précédents:"));
    m_nivPrecList = new widgetMPS::Checklist(niveaux);

    // Niveaux suivants
    m_nivSuivLabel = new QLabel(tr("Niveaux suivants:"));
    m_nivSuivList = new widgetMPS::Checklist(niveaux);

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

    m_mainLayout->addWidget(m_listLabel);
    m_mainLayout->addWidget(m_TEList);
    m_mainLayout->addLayout(m_nivLayout);
}

void NiveauNewModifForm::save() {
    Niveau niv;
    if(!m_new)
        niv.setId(id());
    niv.setNc(nc());
    niv.setNom(nom());
    niv.setType(idType());
    m_bdd.save(niv);
    widgetMPS::Checklist::Save<NiveauTypeEtablissement>::save(m_bdd,m_TEList,[&niv](NiveauTypeEtablissement & nivTE, idt idTE){
                                                                                        nivTE.setIdNiveau(niv.id());
                                                                                        nivTE.setIdTpEtab(idTE);
                                                                                    });
    widgetMPS::Checklist::Save<FiliationNiveau>::save(m_bdd,m_nivPrecList,[&niv](FiliationNiveau & filNiv, idt idNiv){
                                                                                        filNiv.setIdPrecedent(idNiv);
                                                                                        filNiv.setIdSuivant(niv.id());
                                                                                    });
    widgetMPS::Checklist::Save<FiliationNiveau>::save(m_bdd,m_nivSuivList,[&niv](FiliationNiveau & filNiv, idt idNiv){
                                                                                        filNiv.setIdPrecedent(niv.id());
                                                                                        filNiv.setIdSuivant(idNiv);
                                                                                    });
}

void NiveauNewModifForm::updateData() {
    if(!m_new) {
        Niveau niv;
        updateTemp<Niveau>(niv);
        m_TEList->setChecksById<NiveauTypeEtablissement>(m_bdd.getList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdNiveau,niv.id()),
                       [](const NiveauTypeEtablissement& entity)->idt{return entity.idTpEtab();});
        m_nivPrecList->setChecksById<FiliationNiveau>(m_bdd.getList<FiliationNiveau>(FiliationNiveau::IdSuivant,niv.id()),
                       [](const FiliationNiveau & entity){return entity.idPrecedent();});
        m_nivSuivList->setChecksById<FiliationNiveau>(m_bdd.getList<FiliationNiveau>(FiliationNiveau::IdPrecedent,niv.id()),
                       [](const FiliationNiveau & entity){return entity.idSuivant();});
    }
}

// Typecontrole
TypeControleNewModifForm::TypeControleNewModifForm(bddMPS::Bdd &bdd, bool newEnt, QWidget * parent)
    : AbstractControleNewModifForm(bdd,
                                      "Parent :",
                                      "Nom abrégé du type de controle :",
                                      "Nom du type de controle :",
                                      newEnt,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.getList<TypeControle>(TypeControle::Nom));

    // parent
    m_parentTree->setTreeNcNomId(bdd.getArbre<TypeControle>());
    m_parentTree->expandAll();

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

    m_mainLayout->insertWidget(4,m_categorieCheck);
    m_mainLayout->addWidget(m_modifGr);
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
    tpc.setParent(idParent());
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

void TypeControleNewModifForm::updateData() {
    if(!m_new){
        TypeControle tpc;
        updateTemp(tpc);
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
TypeEtablissementNewModifForm::TypeEtablissementNewModifForm(bddMPS::Bdd & bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractNcNomNewModifForm (bdd,
                                            tr("Nom abrégé du type d'établissment :"),
                                            tr("Nom du type d'établissement :"),
                                            newEnt,parent) {
    // Nom
    if(!m_new)
        setNoms(m_bdd.getList<TypeEtablissement>(TypeEtablissement::Nom));

    // Niveaux
    m_selectNiv = new widgetMPS::SelectInListBox(tr("Niveaux existents :"),tr("Niveaux présents :"));

    // Calque
    m_mainLayout->addWidget(m_selectNiv);
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
        if(m_bdd.testAutorisation(te,bddMPS::Modif)) {
            te.setNc(nc());
            te.setNom(nom());
            m_bdd.save(te);
        }
        auto nivOld = m_bdd.getList<NiveauTypeEtablissement>(NiveauTypeEtablissement::IdTpEtab,te.id());
        auto nivNew = m_selectNiv->value().second;
        for(auto i = nivOld.cbegin(); i != nivOld.cend(); ++i) {
            auto j = nivNew.begin();
            while(j != nivNew.end() && i->idNiveau() != j->toUInt())
                ++j;
            if(j == nivNew.end()) {
                NiveauTypeEtablissement nivTE(i->idNiveau(),te.id());
                m_bdd.getUnique(nivTE);
                m_bdd.del(nivTE);
            }
            else
                j->setValue(0);
        }
        for (auto j = nivNew.cbegin(); j != nivNew.cend(); ++j) {
            if(j->toUInt() != 0)
                m_bdd.save(NiveauTypeEtablissement(j->toUInt(),te.id()));
        }
    }
}

void TypeEtablissementNewModifForm::updateData() {
    if(m_new) {
        auto vecNiv = m_bdd.getList<Niveau>();
        std::map<QVariant, QString> nivOut;
        for(auto i = vecNiv.cbegin(); i != vecNiv.cend(); ++i)
            nivOut[i->id()] = i->nom();
        m_selectNiv->setValue(nivOut,std::map<QVariant, QString>());
    }
    else {
        TypeEtablissement te;
        updateTemp<TypeEtablissement>(te);
        auto vecNiv = m_bdd.getList<Niveau>();
        std::map<QVariant, QString> nivIn;
        std::map<QVariant, QString> nivOut;
        for(auto i = vecNiv.cbegin(); i != vecNiv.cend(); ++i) {
            if(m_bdd.existsUnique(NiveauTypeEtablissement(i->id(),te.id())))
                nivIn[i->id()] = i->nom();
            else
                nivOut[i->id()] = i->nom();
        }
        m_selectNiv->setValue(nivOut,nivIn);
    }
}
