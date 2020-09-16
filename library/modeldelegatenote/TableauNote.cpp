#include "TableauNote.h"

using namespace noteMPS;

////////////////////////////////////////// CandidatGroupeTableau ////////////////////////////////
CandidatGroupeTableau::CandidatGroupeTableau(BddNote & bdd, szt idGroupe) : m_bdd(bdd) {
    push_back(std::make_unique<EleveVecTableau>(bdd),false);
    m_groupe.setId(idGroupe);
    m_bdd.get(m_groupe);
    if(m_groupe.idClasse())
        static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .setVecData(m_bdd.getList<Eleve,ClasseEleve>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,m_groupe.idClasse()));
    else
        static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .setVecData(m_bdd.getList<Eleve,ClasseEleve,Classe>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,Classe::IdAn,m_groupe.idAn()));
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
    auto vec = m_bdd.getList<EleveGroupe>(EleveGroupe::IdEleve,idEl,EleveGroupe::IdGroupe,m_groupe.id(),EleveGroupe::Num);
    if(m_groupe.test(Groupe::Exclusif)) {
        auto & elGr = static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).internalData(ligne);
        if(vec.empty()) {
            elGr.setIdEleve(idEl);
            elGr.setIdGroupe(m_groupe.id());
            elGr.setNum(NonAffecter);
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

void CandidatGroupeTableau::remove(std::vector<std::pair<szt,int>> &&vecIdNum) {
    for (auto iter = vecIdNum.cbegin(); iter != vecIdNum.cend(); ++iter) {
        szt i = 0;
        while (i != size() &&
               static_cast<EleveVecTableau&>(tableau(EleveTableau)).internalData(i).id() != iter->first)
            ++i;
        if(i != size()) {
            if(m_groupe.test(Groupe::Exclusif))
                static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).internalData(i).setNum(NonAffecter);
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
        else if (!elGr.isNew())
            m_bdd.del(elGr);
    }
    else {
        auto & elGr = static_cast<EleveGroupeListTableau&>(tableau(EleveGroupeTableau)).internalData(ligne);
        auto idEl = static_cast<EleveVecTableau&>(tableau(EleveTableau)).internalData(ligne).id();
        for (auto iter = elGr.begin(); iter != elGr.end(); ++iter)
            iter->setIdEleve(idEl);
        auto vec = m_bdd.getList<EleveGroupe>(EleveGroupe::IdEleve,idEl,EleveGroupe::IdGroupe,m_groupe.id(),EleveGroupe::Num);
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
ClasseEleveCompositionTableau::ClasseEleveCompositionTableau(BddNote & bdd, szt idClasse) : m_bdd(bdd) {
    push_back(std::make_unique<EleveVecTableau>(bdd),false);
    push_back(std::make_unique<ClasseEleveVecTableau>(bdd));
    setIdClasse(idClasse);
}

void ClasseEleveCompositionTableau::hydrateClasseEleve(szt ligne){
    auto & clEl = static_cast<ClasseEleveVecTableau&>(tableau(ClasseEleveTableau)).internalData(ligne);
    clEl.setIdEleve(static_cast<EleveVecTableau&>(tableau(EleveTableau)).internalData(ligne).id());
    clEl.setIdClasse(m_idClasse);
    if(!m_bdd.getUnique(clEl)){
        DonneeCible dnCb;
        dnCb.setIdDonnee(m_bdd.refToId<Donnee>("date_defaut_dn"));
        dnCb.setCible(m_bdd.cible<Classe>());
        dnCb.setIdCible(m_idClasse);
        dnCb.setNum(donnee::EntreeNum);
        if(m_bdd.getUnique(dnCb))
            clEl.setEntree(dnCb.valeur().toDate());
        dnCb.setId(0);
        dnCb.setNum(donnee::SortieNum);
        if(m_bdd.getUnique(dnCb))
            clEl.setSortie(dnCb.valeur().toDate());
    }
}

void ClasseEleveCompositionTableau::setIdClasse(szt id) {
    m_idClasse = id;
    static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .setVecData(m_bdd.getList<Eleve,ClasseEleve>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,m_idClasse));
    static_cast<ClasseEleveVecTableau&>(tableau(ClasseEleveTableau)).setVecData(conteneurMPS::VectorPtr<ClasseEleve>(size()));
    for(szt ligne = 0; ligne != size(); ++ligne)
        hydrateClasseEleve(ligne);
}

////////////////////////////////////////////// ClasseEleveVecTableau ////////////////////////////////////////
std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
ClasseEleveVecTableau::makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) {
    switch (info.id) {
    case Entree:
        return std::make_unique<modelMPS::VectorPtrColonne<ClasseEleve>>(info.name,info.flags,
                                                                         modelMPS::AbstractColonnesModel::DateColonne,m_vec,
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
        return std::make_unique<modelMPS::VectorPtrColonne<ClasseEleve>>(info.name,info.flags,
                                                                         modelMPS::AbstractColonnesModel::DateColonne,m_vec,
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
EleveVecTableau::NomColonne::NomColonne(const QString &name, Qt::ItemFlags flags, conteneurMPS::VectorPtr<Eleve> & vec)
    : modelMPS::VectorPtrIdColonne<Eleve>(name, flags,modelMPS::AbstractColonnesModel::TexteColonne,vec,
    [](const Eleve & eleve, int role)->QVariant{
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return eleve.nom();
        return QVariant();},
    [](const Eleve & eleve)->QVariant{return eleve.nom();},
    [](Eleve & eleve, const QVariant & value, int role){
        if(role == Qt::EditRole){
            eleve.setNom(value.toString());
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
        return std::make_unique<modelMPS::VectorPtrColonne<Eleve>>(info.name,info.flags,
                                                                   modelMPS::AbstractColonnesModel::TexteColonne,m_vec,
            [](const Eleve & eleve,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.prenom();
                return QVariant();},
            [](const Eleve & eleve)->QVariant{return eleve.prenom();},
            [](Eleve & eleve, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    eleve.setPrenom(value.toString());
                    return true;}
                return false;});
    case Naissance:
        return std::make_unique<modelMPS::VectorPtrColonne<Eleve>>(info.name,info.flags,
                                                                   modelMPS::AbstractColonnesModel::DateColonne,m_vec,
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
        return std::make_unique<modelMPS::VectorPtrBoolColonne<Eleve>>(info.name,info.flags,
                                                                       modelMPS::AbstractColonnesModel::BoolColonne,m_vec,
           [](const Eleve & eleve)->bool {return eleve.fille();},
           [](Eleve & eleve, bool value) {eleve.setFille(value);},
           "F","M");
    default:
        return nullptr;
    }
}

///////////////////////////////////////////////// EleveGroupeTableau ///////////////////////////////////////////////////
void EleveGroupeTableau::addEleve(const std::list<szt> &listEl, szt num) {
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

void EleveGroupeTableau::delEleve(const std::map<szt, std::forward_list<szt>> &mapDel) {
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
                                                                               modelMPS::AbstractColonnesModel::TexteColonne,
                                                                               colonneAt(info.tableau),
    [](const Eleve & el,int role)->QVariant {
        if(role == Qt::DisplayRole)
            return QString().append(el.nom()).append(" ").append(el.prenom());
        return QVariant();},
    [](const Eleve &)->QVariant {return QVariant();},
    [](Eleve &, const QVariant &, int)->bool {return false;});
}

void EleveGroupeTableau::remove(const std::map<szt,std::list<szt>> & map) {
    for (auto iter = map.cbegin(); iter != map.cend(); ++iter)
        for (auto iterList = iter->second.crbegin(); iterList != iter->second.crend(); ++iterList)
            m_tableau[iter->first]->erase(std::next(m_tableau[iter->first]->cbegin(),*iterList));
    updateNbrLine();
}

void EleveGroupeTableau::setIdGroupe(szt id) {
    m_groupe.setId(id);
    m_bdd.get(m_groupe);
    m_tableau.clear();
    szt nbrGr = 0;
    if(m_bdd.exists<EleveGroupe>(EleveGroupe::IdGroupe,m_groupe.id())) {
        nbrGr = 1 + m_bdd.fonctionAgrega<EleveGroupe>(bddMPS::agrega::Max,EleveGroupe::Num,EleveGroupe::IdGroupe,m_groupe.id()).toUInt();
        for (szt num = 0; num != nbrGr; ++num) {
            auto vecEl = m_bdd.getList<Eleve,EleveGroupe>(EleveGroupe::IdEleve,
                                                          EleveGroupe::IdGroupe,m_groupe.id(),
                                                          EleveGroupe::Num,num);
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
                str.append(m_groupe.alphaTexte(iter->num())).append(", ");
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
            eg.setIdGroupe(m_groupe.id());
            eg.setNum(value.toInt());
            elGr.push_back(eg);
            elGr.sort([](const EleveGroupe & elGr1, const EleveGroupe & elGr2)->bool
                        {return elGr1.num() < elGr2.num();});
            return true;
        }
        return false;};
    return std::make_unique<modelMPS::TempBaseColonne<decltype (read), decltype (find), decltype (write),
                                                      std::vector<std::list<EleveGroupe>>>>(info.name,info.flags,
                                                                     modelMPS::AbstractColonnesModel::TexteColonne,m_vec,
                                                                     read,find,write);
}

///////////////////////////////////////////////// EleveGroupeVecTableau ///////////////////////////////////////////////
std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
EleveGroupeVecTableau::makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) {
    auto backFore = [](const EleveGroupe & elGr)->szt {return static_cast<szt>(elGr.num() + 1);};
    auto read = [this](const EleveGroupe & elGr,int role)->QVariant {
        switch (role) {
        case Qt::DisplayRole:
            return m_groupe.alphaTexte(elGr.num());
        case Qt::TextAlignmentRole:
            return Qt::AlignHCenter;
        case CandidatGroupeTableau::NumRole:
            return elGr.num();
        default:
            return QVariant();}};
    auto find = [](const EleveGroupe & elGr)->QVariant {return elGr.num();};
    auto write = [](EleveGroupe & elGr, const QVariant & value, int role)->bool {
        if(role == Qt::EditRole){
            elGr.setNum(value.toInt());
            return true;}
        return false;};
        auto colonne = std::make_unique<modelMPS::TempBaseColorColonne<decltype (backFore), decltype (backFore),
                                                               decltype (read), decltype (find), decltype (write),
                                                               conteneurMPS::VectorPtr<EleveGroupe>>>(info.name,info.flags,
                                                                         modelMPS::AbstractColonnesModel::TexteColonne,m_vec,
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


