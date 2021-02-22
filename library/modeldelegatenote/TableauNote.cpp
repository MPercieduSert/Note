#include "TableauNote.h"

using namespace noteMPS;

////////////////////////////////////////// CandidatGroupeTableau ////////////////////////////////
CandidatGroupeTableau::CandidatGroupeTableau(BddNote & bdd, idt idGroupe) : m_bdd(bdd) {
    push_back(std::make_unique<EleveVecTableau>(bdd),false);
    m_groupe.set_id(idGroupe);
    m_bdd.get(m_groupe);
    if(m_groupe.idClasse())
        static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .setVecData(m_bdd.get_list<Eleve,ClasseEleve>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,m_groupe.idClasse()));
    else
        static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .setVecData(m_bdd.get_list<Eleve,ClasseEleve,Classe>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,Classe::IdAn,m_groupe.idAn()));
    if(m_groupe.test(Groupe::Exclusif)) {
        push_back(std::make_unique<EleveGroupeVecTableau>(m_bdd,tableau(EleveTableau).size()),true);
        static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).setGroupe(m_groupe);
    }
    else {
        push_back(std::make_unique<EleveGroupeListTableau>(m_bdd,tableau(EleveTableau).size()),true);
        static_cast<EleveGroupeListTableau&>(tableau(EleveGroupeTableau)).setGroupe(m_groupe);
    }
    for(szt ligne = 0; ligne != size(); ++ligne)
        hydrateEleveGroupe(ligne);
}

void CandidatGroupeTableau::hydrateEleveGroupe(szt ligne) {
    auto idEl = static_cast<EleveVecTableau&>(tableau(EleveTableau)).internalData(ligne).id();
    auto vec = m_bdd.get_list<EleveGroupe>(EleveGroupe::IdEleve,idEl,EleveGroupe::IdGroupe,m_groupe.id(),EleveGroupe::Num);
    if(m_groupe.test(Groupe::Exclusif)) {
        auto & elGr = static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).internalData(ligne);
        if(vec.empty()) {
            elGr.set_idEleve(idEl);
            elGr.set_idGroupe(m_groupe.id());
            elGr.set_num(NonAffecter);
        }
        else
            elGr = vec.front();
    }
    else {
        auto & elGr = static_cast<EleveGroupeListTableau&>(tableau(EleveGroupeTableau)).internalData(ligne);
        elGr.clear();
        if(!vec.empty())
            for (auto iter = vec.cbegin(); iter != vec.cend(); ++iter)
                elGr.push_back(*iter);
    }
}

void CandidatGroupeTableau::remove(std::vector<std::pair<idt, int> > &&vecIdNum) {
    for (auto iter = vecIdNum.cbegin(); iter != vecIdNum.cend(); ++iter) {
        szt i = 0;
        while (i != size() &&
               static_cast<EleveVecTableau&>(tableau(EleveTableau)).internalData(i).id() != iter->first)
            ++i;
        if(i != size()) {
            if(m_groupe.test(Groupe::Exclusif))
                static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).internalData(i).set_num(NonAffecter);
            else
                static_cast<EleveGroupeListTableau&>(tableau(EleveGroupeTableau)).internalData(i).remove_if(
                        [iter](const EleveGroupe & elGr)->bool{return elGr.num() == iter->second;});
        }
    }
}

void CandidatGroupeTableau::save(szt ligne) {
    if(m_groupe.test(Groupe::Exclusif)) {
        auto & elGr = static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).internalData(ligne);
        if(elGr.num() >=0)
            m_bdd.save(elGr);
        else if (!elGr.is_new())
            m_bdd.del(elGr);
    }
    else {
        auto & elGr = static_cast<EleveGroupeListTableau&>(tableau(EleveGroupeTableau)).internalData(ligne);
        auto idEl = static_cast<EleveVecTableau&>(tableau(EleveTableau)).internalData(ligne).id();
        for (auto iter = elGr.begin(); iter != elGr.end(); ++iter)
            iter->set_idEleve(idEl);
        auto vec = m_bdd.get_list<EleveGroupe>(EleveGroupe::IdEleve,idEl,EleveGroupe::IdGroupe,m_groupe.id(),EleveGroupe::Num);
        auto iterTab = elGr.begin();
        auto iterBdd = vec.begin();
        while(iterTab != elGr.end() && iterBdd != vec.end()){
            if(iterTab->num() == iterBdd->num()){
                ++iterTab;
                ++iterBdd;
            }
            else if (iterTab->num() < iterBdd->num()) {
                m_bdd.save(*iterTab);
                ++iterTab;
            }
            else {
                m_bdd.del(*iterBdd);
                ++iterBdd;
            }
        }
        while(iterTab != elGr.end()){
            m_bdd.save(*iterTab);
            ++iterTab;
        }
        while(iterBdd != vec.end()){
            m_bdd.del(*iterBdd);
            ++iterBdd;
        }
    }
}

////////////////////////////////////////// ClasseEleveCompositionTableau ////////////////////////////////
ClasseEleveCompositionTableau::ClasseEleveCompositionTableau(BddNote & bdd, idt idClasse) : m_bdd(bdd) {
    push_back(std::make_unique<EleveVecTableau>(bdd),false);
    push_back(std::make_unique<ClasseEleveVecTableau>(bdd));
    set_idClasse(idClasse);
}

void ClasseEleveCompositionTableau::hydrateClasseEleve(szt ligne){
    auto & clEl = static_cast<ClasseEleveVecTableau&>(tableau(ClasseEleveTableau)).internalData(ligne);
    clEl.set_idEleve(static_cast<EleveVecTableau&>(tableau(EleveTableau)).internalData(ligne).id());
    clEl.set_idClasse(m_idClasse);
    if(!m_bdd.get_unique(clEl)){
        donnee_cible dnCb;
        dnCb.set_iddonnee(m_bdd.ref_to_id<donnee>("date_defaut_dn"));
        dnCb.set_cible(m_bdd.cible<Classe>());
        dnCb.set_id_cible(m_idClasse);
        dnCb.set_num(donnee::EntreeNum);
        if(m_bdd.get_unique(dnCb))
            clEl.setEntree(dnCb.valeur().toDate());
        dnCb.set_id(0);
        dnCb.set_num(donnee::SortieNum);
        if(m_bdd.get_unique(dnCb))
            clEl.setSortie(dnCb.valeur().toDate());
    }
}

void ClasseEleveCompositionTableau::set_idClasse(idt id) {
    m_idClasse = id;
    static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .setVecData(m_bdd.get_list<Eleve,ClasseEleve>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,m_idClasse));
    static_cast<ClasseEleveVecTableau&>(tableau(ClasseEleveTableau)).setVecData(conteneurMPS::vector_ptr<ClasseEleve>(size()));
    for(szt ligne = 0; ligne != size(); ++ligne)
        hydrateClasseEleve(ligne);
}

////////////////////////////////////////////// ClasseEleveVecTableau ////////////////////////////////////////
std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
ClasseEleveVecTableau::makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) {
    switch (info.id) {
    case Entree:
        return std::make_unique<modelMPS::vector_ptrColonne<ClasseEleve>>(info.name,info.flags,
                                                                         modelMPS::DateColonneType,m_vec,
            [](const ClasseEleve & clEl,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return clEl.entree();
                return QVariant();},
            [](const ClasseEleve & clEl)->QVariant{return clEl.entree();},
            [](ClasseEleve & clEl, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    clEl.setEntree(value.toDate());
                    return true;}
                return false;});
    case Sortie:
        return std::make_unique<modelMPS::vector_ptrColonne<ClasseEleve>>(info.name,info.flags,
                                                                         modelMPS::DateColonneType,m_vec,
            [](const ClasseEleve & clEl,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return clEl.sortie();
                return QVariant();},
            [](const ClasseEleve & clEl)->QVariant{return clEl.sortie();},
            [](ClasseEleve & clEl, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    clEl.setSortie(value.toDate());
                    return true;}
                return false;});
    default:
        return nullptr;
    }
}

/////////////////////////////////////////// EleveVecTableau ///////////////////////////////////////////////////
EleveVecTableau::NomColonne::NomColonne(const QString &name, Qt::ItemFlags flags, conteneurMPS::vector_ptr<Eleve> & vec)
    : modelMPS::vector_ptrIdColonne<Eleve>(name, flags,modelMPS::TexteColonneType,vec,
    [](const Eleve & eleve, int role)->QVariant{
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return eleve.nom();
        return QVariant();},
    [](const Eleve & eleve)->QVariant{return eleve.nom();},
    [](Eleve & eleve, const QVariant & value, int role){
        if(role == Qt::EditRole){
            eleve.setNom(value.to_string());
            return true;}
        return false;}) {}

bool EleveVecTableau::egal(szt ligne1, szt ligne2) const {
    return m_vec[ligne1].nom() == m_vec[ligne2].nom()
            && m_vec[ligne1].prenom() == m_vec[ligne2].prenom()
            && m_vec[ligne1].date() == m_vec[ligne2].date();
}

std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
EleveVecTableau::makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) {
    switch (info.id) {
    case Nom:
        return std::make_unique<NomColonne>(info.name,info.flags,m_vec);
    case Prenom:
        return std::make_unique<modelMPS::vector_ptrColonne<Eleve>>(info.name,info.flags,
                                                                   modelMPS::TexteColonneType,m_vec,
            [](const Eleve & eleve,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.prenom();
                return QVariant();},
            [](const Eleve & eleve)->QVariant{return eleve.prenom();},
            [](Eleve & eleve, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    eleve.setPrenom(value.to_string());
                    return true;}
                return false;});
    case Naissance:
        return std::make_unique<modelMPS::vector_ptrColonne<Eleve>>(info.name,info.flags,
                                                                   modelMPS::DateColonneType,m_vec,
           [](const Eleve & eleve,int role)->QVariant {
               if(role == Qt::DisplayRole || role == Qt::EditRole)
                   return eleve.date();
               return QVariant();},
           [](const Eleve & eleve)->QVariant{return eleve.date();},
           [](Eleve & eleve, const QVariant & value, int role)->bool {
               if(role == Qt::EditRole) {
                   eleve.setDate(value.toDate());
                   return true;}
               return false;});
    case Sexe:
        return std::make_unique<modelMPS::vector_ptrBoolColonne<Eleve>>(info.name,info.flags,
                                                                       modelMPS::BoolColonneType,m_vec,
           [](const Eleve & eleve)->bool {return eleve.fille();},
           [](Eleve & eleve, bool value) {eleve.setFille(value);},
           "F","M");
    default:
        return nullptr;
    }
}

///////////////////////////////////////////////// EleveGroupeTableau ///////////////////////////////////////////////////
void EleveGroupeTableau::addEleve(const std::list<idt> &listEl, szt num) {
    if(num < sizeColumn()) {
        auto & refCol = colonne(num);
        for (auto iterId = listEl.cbegin(); iterId != listEl.cend(); ++iterId) {
            refCol.push_back(Eleve(*iterId));
            m_bdd.get(refCol.back());
        }
        std::sort(refCol.begin(),refCol.end(),
                  [](const Eleve & el1,const Eleve el2)->bool{return el1 < el2;});
        if(refCol.size() > m_nbrLine)
            m_nbrLine = static_cast<szt>(refCol.size());
    }
}

void EleveGroupeTableau::delEleve(const std::map<szt, std::forward_list<idt>> &mapDel) {
    for (auto iter = mapDel.cbegin(); iter != mapDel.cend(); ++iter) {
        for (auto iterId = iter->second.cbegin(); iterId != iter->second.cend(); ++iterId) {
            auto iterVec = colonneAt(iter->first).cbegin();
            while (iterVec->id() != *iterId)
                ++iterVec;
            colonne(iter->first).erase(iterVec);
        }
    }
    updateNbrLine();
}

std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
EleveGroupeTableau::makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) {
    return std::make_unique<modelMPS::VectorRefHeterogeneTailleIdColonne<Eleve>>(info.name,info.flags,
                                                                               modelMPS::TexteColonneType,
                                                                               colonneAt(info.tableau),
    [](const Eleve & el,int role)->QVariant {
        if(role == Qt::DisplayRole)
            return QString().append(el.nom()).append(" ").append(el.prenom());
        return QVariant();},
    [](const Eleve &)->QVariant {return QVariant();},
    [](Eleve &, const QVariant &, int)->bool {return false;});
}

void EleveGroupeTableau::remove(const std::map<szt,std::list<idt>> & map) {
    for (auto iter = map.cbegin(); iter != map.cend(); ++iter)
        for (auto iterList = iter->second.crbegin(); iterList != iter->second.crend(); ++iterList)
            m_tableau[iter->first]->erase(std::next(m_tableau[iter->first]->cbegin(),*iterList));
    updateNbrLine();
}

void EleveGroupeTableau::set_idGroupe(idt id) {
    m_groupe.set_id(id);
    m_bdd.get(m_groupe);
    m_styleNum.set_style(m_groupe.styleNum());
    m_tableau.clear();
    szt nbrGr = 0;
    if(m_bdd.exists<EleveGroupe>(EleveGroupe::IdGroupe,m_groupe.id())) {
        nbrGr = 1 + m_bdd.fonction_agrega<EleveGroupe>(b2d::agrega::Max,EleveGroupe::Num,EleveGroupe::IdGroupe,m_groupe.id()).toUInt();
        for (szt num = 0; num != nbrGr; ++num) {
            auto vecEl = m_bdd.get_list<Eleve,EleveGroupe>(EleveGroupe::IdEleve,
                                                          EleveGroupe::IdGroupe,m_groupe.id(),
                                                          EleveGroupe::Num,static_cast<uint>(num));
            push_back(vecEl.size());
            auto iterWrite = colonne(num).begin();
            for (auto iterRead = vecEl.cbegin(); iterRead != vecEl.cend(); ++iterRead, ++iterWrite)
                *iterWrite = *iterRead;
        }
    }
    updateNbrLine();
}

///////////////////////////////////////////////// EleveGroupeListTableau ///////////////////////////////////////////////
std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
EleveGroupeListTableau::makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) {
    auto read = [this](const std::list<EleveGroupe> & listElGr,int role)->QVariant {
        switch (role) {
        case Qt::DisplayRole: {
            QString str;
            for(auto iter = listElGr.cbegin(); iter != listElGr.cend(); ++iter)
                str.append(numToTexte(iter->num())).append(", ");
            str.chop(2);
            return str;
        }
        case Qt::TextAlignmentRole:
            return Qt::AlignHCenter;
        case CandidatGroupeTableau::NumRole: {
            QList<QVariant> lq;
            for (auto iter = listElGr.cbegin(); iter != listElGr.cend(); ++iter)
                lq.append(iter->num());
            return lq;
            }
        default:
            return QVariant();}};
    auto find = [read](const std::list<EleveGroupe>& listElGr)->QVariant {return read(listElGr,CandidatGroupeTableau::NumRole);};
    auto write = [this](std::list<EleveGroupe> & elGr, const QVariant & value, int role)->bool {
        if(role == Qt::EditRole){
            EleveGroupe eg;
            eg.set_idGroupe(m_groupe.id());
            eg.set_num(value.toInt());
            elGr.push_back(eg);
            elGr.sort([](const EleveGroupe & elGr1, const EleveGroupe & elGr2)->bool
                        {return elGr1.num() < elGr2.num();});
            return true;
        }
        return false;};
    return std::make_unique<modelMPS::_tempBaseColonne<decltype (read), decltype (find), decltype (write),
                                                      std::vector<std::list<EleveGroupe>>>>(info.name,info.flags,
                                                                     modelMPS::TexteColonneType,m_vec,
                                                                     read,find,write);
}

///////////////////////////////////////////////// EleveGroupeVecTableau ///////////////////////////////////////////////
std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
EleveGroupeVecTableau::makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) {
    auto backFore = [](const EleveGroupe & elGr)->szt {return static_cast<szt>(elGr.num() + 1);};
    auto read = [this](const EleveGroupe & elGr,int role)->QVariant {
        switch (role) {
        case Qt::DisplayRole:
            return numToTexte(elGr.num());
        case Qt::TextAlignmentRole:
            return Qt::AlignHCenter;
        case CandidatGroupeTableau::NumRole:
            return elGr.num();
        default:
            return QVariant();}};
    auto find = [](const EleveGroupe & elGr)->QVariant {return elGr.num();};
    auto write = [](EleveGroupe & elGr, const QVariant & value, int role)->bool {
        if(role == Qt::EditRole){
            elGr.set_num(value.toInt());
            return true;}
        return false;};
        auto colonne = std::make_unique<modelMPS::_tempBaseColorColonne<decltype (backFore), decltype (backFore),
                                                               decltype (read), decltype (find), decltype (write),
                                                               conteneurMPS::vector_ptr<EleveGroupe>>>(info.name,info.flags,
                                                                         modelMPS::TexteColonneType,m_vec,
                                                                         backFore,backFore,
                                                                         read,find,write);
        colonne->backGroundVector().push_back(QBrush(Qt::black));
        colonne->backGroundVector().push_back(QBrush(Qt::red));
        colonne->backGroundVector().push_back(QBrush(Qt::green));
        colonne->backGroundVector().push_back(QBrush(Qt::blue));
        colonne->backGroundVector().push_back(QBrush(Qt::yellow));
        colonne->backGroundVector().push_back(QBrush(Qt::cyan));
        colonne->backGroundVector().push_back(QBrush(Qt::magenta));
        colonne->foreGroundVector().push_back(QBrush(Qt::white));
        colonne->foreGroundVector().push_back(QBrush(Qt::white));
        colonne->foreGroundVector().push_back(QBrush(Qt::black));
        colonne->foreGroundVector().push_back(QBrush(Qt::white));
        colonne->foreGroundVector().push_back(QBrush(Qt::black));
        colonne->foreGroundVector().push_back(QBrush(Qt::black));
        colonne->foreGroundVector().push_back(QBrush(Qt::white));
        return std::move(colonne);
}


