#include "NewModifForm.h"

using namespace noteMPS;

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

    // Annee
    m_anneeLabel = new QLabel(tr("Annee scolaire : "));
    m_anneeSpinBox = new SpinBoxAnneeScolaire(m_bdd.getList<Annee>(Annee::Num));
    m_anneeSpinBox->setNowValue();

    // Etab
    m_etabLabel = new QLabel(tr("Etablissement :"));
    m_etabCB = new widgetMPS::IdComboBox;
    m_etabCB->setInsertPolicy(QComboBox::NoInsert);
    m_etabCB->addText(m_bdd.getList<Etablissement>(Etablissement::Nom),
                      [](const Etablissement & etab)->QString
                            {return QString(etab.nom()).append(" (").append(etab.nc()).append(")");});
    // Niveau
    m_nivLabel = new QLabel(tr("Niveau :"));
    m_nivCB = new widgetMPS::IdComboBox;
    m_nivCB->setInsertPolicy(QComboBox::NoInsert);
    updateNiveau();

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
    m_mainLayout->addWidget(m_anneeLabel);
    m_mainLayout->addWidget(m_anneeSpinBox);
    m_mainLayout->addWidget(m_etabLabel);
    m_mainLayout->addWidget(m_etabCB);
    m_mainLayout->addWidget(m_nivLabel);
    m_mainLayout->addWidget(m_nivCB);
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
    connect(m_etabCB,qOverload<int>(&QComboBox::currentIndexChanged),this,&ClasseNewModifForm::updateNiveau);
    connect(m_anneeSpinBox,&SpinBoxAnneeScolaire::valueChanged,this,&ClasseNewModifForm::updateCalendar);
}

void ClasseNewModifForm::save() {
    Classe cl;
    if(!m_new)
        cl.setId(id());
    cl.setNc(nc());
    cl.setNom(nom());
    cl.setIdAn(m_anneeSpinBox->value().id());
    cl.setIdEtab(m_etabCB->currentData().toUInt());
    cl.setIdNiveau(m_nivCB->currentData().toUInt());
    cl.setNum(m_numSpinBox->value());
    m_bdd.save(cl);
    if(m_new)
        m_idClasse = cl.id();
    DonneeCible dnCb;
    dnCb.setIdDonnee(bdd().idProgToId<Donnee>(donnee::DefaultDateId));
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
        auto an = m_anneeSpinBox->value().num();
        m_debutCalendar->setDateRange(QDate(an,9,1),QDate(an+1,8,31));
        m_debutCalendar->setSelectedDate(QDate(an,9,1));
        m_finCalendar->setDateRange(QDate(an,9,1),QDate(an+1,8,31));
        m_finCalendar->setSelectedDate(QDate(an+1,8,31));
    }
}

void ClasseNewModifForm::updateData() {
    if(m_new) {
        updateNiveau();
        updateCalendar();
    }
    else {
        Classe cl;
        updateTemp<Classe>(cl);
        m_idClasse = cl.id();
        m_anneeSpinBox->setValue(Annee(cl.idAn()), false);
        m_etabCB->setCurrentIndex(m_etabCB->findData(cl.idEtab()));
        m_nivCB->setCurrentIndex(m_nivCB->findData(cl.idNiveau()));
        m_numSpinBox->setValue(cl.num());
        DonneeCible dnCb;
        dnCb.setIdDonnee(bdd().idProgToId<Donnee>(donnee::DefaultDateId));
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

void ClasseNewModifForm::updateNiveau() {
    m_nivCB->clear();
    m_nivCB->addText(m_bdd.getList<Niveau,EtablissementNiveau>(EtablissementNiveau::IdNiveau,
                                                          EtablissementNiveau::IdEtab,
                                                          m_etabCB->currentData(Qt::UserRole).toUInt()),
                     [](const Niveau & niv)->QString
                            {return QString(niv.nom()).append(" (").append(niv.nc()).append(")");});
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

// Niveau
NiveauNewModifForm::NiveauNewModifForm(bddMPS::Bdd & bdd, bool newEnt, QWidget * parent)
    : dialogMPS::AbstractTypeNcNomNewModifForm (static_cast<bddMPS::BddPredef &>(bdd),
                                                bddMPS::idProg::EtudeType,
                                                Niveau::ID,
                                                tr("type de niveau :"),
                                                tr("Nom abrégé du niveau :"),
                                                tr("Nom du niveau"),
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
        if(m_bdd.getAutorisation(te,bddMPS::Modif)) {
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
