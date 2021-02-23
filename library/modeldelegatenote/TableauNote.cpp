#include "TableauNote.h"

using namespace noteMPS;

////////////////////////////////////////// CandidatGroupeTableau ////////////////////////////////
CandidatGroupeTableau::CandidatGroupeTableau(BddNote & bdd, idt idGroupe) : m_bdd(bdd) {
    push_back(std::make_unique<EleveVecTableau>(bdd),false);
    m_groupe.set_id(idGroupe);
    m_bdd.get(m_groupe);
    if(m_groupe.idClasse())
        static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .set_vec_data(m_bdd.get_list<Eleve,ClasseEleve>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,m_groupe.idClasse()));
    else
        static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .set_vec_data(m_bdd.get_list<Eleve,ClasseEleve,Classe>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,Classe::IdAn,m_groupe.idAn()));
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
    auto idEl = static_cast<EleveVecTableau&>(tableau(EleveTableau)).internal_data(ligne).id();
    auto vec = m_bdd.get_list<EleveGroupe>(EleveGroupe::IdEleve,idEl,EleveGroupe::IdGroupe,m_groupe.id(),EleveGroupe::Num);
    if(m_groupe.test(Groupe::Exclusif)) {
        auto & elGr = static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).internal_data(ligne);
        if(vec.empty()) {
            elGr.set_idEleve(idEl);
            elGr.set_idGroupe(m_groupe.id());
            elGr.set_num(NonAffecter);
        }
        else
            elGr = vec.front();
    }
    else {
        auto & elGr = static_cast<EleveGroupeListTableau&>(tableau(EleveGroupeTableau)).internal_data(ligne);
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
               static_cast<EleveVecTableau&>(tableau(EleveTableau)).internal_data(i).id() != iter->first)
            ++i;
        if(i != size()) {
            if(m_groupe.test(Groupe::Exclusif))
                static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).internal_data(i).set_num(NonAffecter);
            else
                static_cast<EleveGroupeListTableau&>(tableau(EleveGroupeTableau)).internal_data(i).remove_if(
                        [iter](const EleveGroupe & elGr)->bool{return elGr.num() == iter->second;});
        }
    }
}

void CandidatGroupeTableau::save(szt ligne) {
    if(m_groupe.test(Groupe::Exclusif)) {
        auto & elGr = static_cast<EleveGroupeVecTableau&>(tableau(EleveGroupeTableau)).internal_data(ligne);
        if(elGr.num() >=0)
            m_bdd.save(elGr);
        else if (!elGr.is_new())
            m_bdd.del(elGr);
    }
    else {
        auto & elGr = static_cast<EleveGroupeListTableau&>(tableau(EleveGroupeTableau)).internal_data(ligne);
        auto idEl = static_cast<EleveVecTableau&>(tableau(EleveTableau)).internal_data(ligne).id();
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

////////////////////////////////////////// ClasseElevecomposition_tableau ////////////////////////////////
ClasseElevecomposition_tableau::ClasseElevecomposition_tableau(BddNote & bdd, idt idClasse) : m_bdd(bdd) {
    push_back(std::make_unique<EleveVecTableau>(bdd),false);
    push_back(std::make_unique<ClasseEleveVecTableau>(bdd));
    set_idClasse(idClasse);
}

void ClasseElevecomposition_tableau::hydrateClasseEleve(szt ligne){
    auto & clEl = static_cast<ClasseEleveVecTableau&>(tableau(ClasseEleveTableau)).internal_data(ligne);
    clEl.set_idEleve(static_cast<EleveVecTableau&>(tableau(EleveTableau)).internal_data(ligne).id());
    clEl.set_idClasse(m_idClasse);
    if(!m_bdd.get_unique(clEl)){
        donnee_cible dnCb;
        dnCb.set_iddonnee(m_bdd.ref_to_id<donnee>("date_defaut_dn"));
        dnCb.set_cible(m_bdd.cible<Classe>());
        dnCb.set_id_cible(m_idClasse);
        dnCb.set_num(donnee::EntreeNum);
        if(m_bdd.get_unique(dnCb))
            clEl.set_entree(dnCb.valeur().toDate());
        dnCb.set_id(0);
        dnCb.set_num(donnee::SortieNum);
        if(m_bdd.get_unique(dnCb))
            clEl.setSortie(dnCb.valeur().toDate());
    }
}

void ClasseElevecomposition_tableau::set_idClasse(idt id) {
    m_idClasse = id;
    static_cast<EleveVecTableau&>(tableau(EleveTableau))
            .set_vec_data(m_bdd.get_list<Eleve,ClasseEleve>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,m_idClasse));
    static_cast<ClasseEleveVecTableau&>(tableau(ClasseEleveTableau)).set_vec_data(vector_ptr<ClasseEleve>(size()));
    for(szt ligne = 0; ligne != size(); ++ligne)
        hydrateClasseEleve(ligne);
}

////////////////////////////////////////////// ClasseEleveVecTableau ////////////////////////////////////////
std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
ClasseEleveVecTableau::make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) {
    switch (info.id) {
    case Entree:
        return std::make_unique<model_base::vector_ptr_colonne<ClasseEleve>>(info.name,info.flags,
                                                                         model_base::Date_Colonne_Type,m_vec,
            [](const ClasseEleve & clEl,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return clEl.entree();
                return QVariant();},
            [](const ClasseEleve & clEl)->QVariant{return clEl.entree();},
            [](ClasseEleve & clEl, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    clEl.set_entree(value.toDate());
                    return true;}
                return false;});
    case Sortie:
        return std::make_unique<model_base::vector_ptr_colonne<ClasseEleve>>(info.name,info.flags,
                                                                         model_base::Date_Colonne_Type,m_vec,
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
EleveVecTableau::NomColonne::NomColonne(const QString &name, Qt::ItemFlags flags, vector_ptr<Eleve> & vec)
    : model_base::vector_ptr_id_colonne<Eleve>(name, flags,model_base::Texte_Colonne_Type,vec,
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

bool EleveVecTableau::egal(szt ligne_1, szt ligne_2) const {
    return m_vec[ligne_1].nom() == m_vec[ligne_2].nom()
            && m_vec[ligne_1].prenom() == m_vec[ligne_2].prenom()
            && m_vec[ligne_1].date() == m_vec[ligne_2].date();
}

std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
EleveVecTableau::make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) {
    switch (info.id) {
    case Nom:
        return std::make_unique<NomColonne>(info.name,info.flags,m_vec);
    case Prenom:
        return std::make_unique<model_base::vector_ptr_colonne<Eleve>>(info.name,info.flags,
                                                                   model_base::Texte_Colonne_Type,m_vec,
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
        return std::make_unique<model_base::vector_ptr_colonne<Eleve>>(info.name,info.flags,
                                                                   model_base::Date_Colonne_Type,m_vec,
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
        return std::make_unique<model_base::vector_ptr_bool_colonne<Eleve>>(info.name,info.flags,
                                                                       model_base::Bool_Colonne_Type,m_vec,
           [](const Eleve & eleve)->bool {return eleve.fille();},
           [](Eleve & eleve, bool value) {eleve.setFille(value);},
           "F","M");
    default:
        return nullptr;
    }
}

///////////////////////////////////////////////// EleveGroupeTableau ///////////////////////////////////////////////////
void EleveGroupeTableau::addEleve(const std::list<idt> &listEl, szt num) {
    if(num < size_column()) {
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
            auto iterVec = colonne_at(iter->first).cbegin();
            while (iterVec->id() != *iterId)
                ++iterVec;
            colonne(iter->first).erase(iterVec);
        }
    }
    update_nbr_line();
}

std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
EleveGroupeTableau::make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) {
    return std::make_unique<model_base::vector_ref_heterogene_taille_id_colonne<Eleve>>(info.name,info.flags,
                                                                               model_base::Texte_Colonne_Type,
                                                                               colonne_at(info.tableau),
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
    update_nbr_line();
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
    update_nbr_line();
}

///////////////////////////////////////////////// EleveGroupeListTableau ///////////////////////////////////////////////
std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
EleveGroupeListTableau::make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) {
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
        case CandidatGroupeTableau::Num_Role: {
            QList<QVariant> lq;
            for (auto iter = listElGr.cbegin(); iter != listElGr.cend(); ++iter)
                lq.append(iter->num());
            return lq;
            }
        default:
            return QVariant();}};
    auto find = [read](const std::list<EleveGroupe>& listElGr)->QVariant {return read(listElGr,CandidatGroupeTableau::Num_Role);};
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
    return std::make_unique<model_base::base_colonne_temp<decltype (read), decltype (find), decltype (write),
                                                      std::vector<std::list<EleveGroupe>>>>(info.name,info.flags,
                                                                     model_base::Texte_Colonne_Type,m_vec,
                                                                     read,find,write);
}

///////////////////////////////////////////////// EleveGroupeVecTableau ///////////////////////////////////////////////
std::unique_ptr<model_base::abstract_colonnes_model::abstract_colonne>
EleveGroupeVecTableau::make_colonne(const model_base::abstract_colonnes_model::new_colonne_info & info) {
    auto backFore = [](const EleveGroupe & elGr)->szt {return static_cast<szt>(elGr.num() + 1);};
    auto read = [this](const EleveGroupe & elGr,int role)->QVariant {
        switch (role) {
        case Qt::DisplayRole:
            return numToTexte(elGr.num());
        case Qt::TextAlignmentRole:
            return Qt::AlignHCenter;
        case CandidatGroupeTableau::Num_Role:
            return elGr.num();
        default:
            return QVariant();}};
    auto find = [](const EleveGroupe & elGr)->QVariant {return elGr.num();};
    auto write = [](EleveGroupe & elGr, const QVariant & value, int role)->bool {
        if(role == Qt::EditRole){
            elGr.set_num(value.toInt());
            return true;}
        return false;};
        auto colonne = std::make_unique<model_base::base_color_colonne_temp<decltype (backFore), decltype (backFore),
                                                               decltype (read), decltype (find), decltype (write),
                                                               vector_ptr<EleveGroupe>>>(info.name,info.flags,
                                                                         model_base::Texte_Colonne_Type,m_vec,
                                                                         backFore,backFore,
                                                                         read,find,write);
        colonne->back_ground_vector().push_back(QBrush(Qt::black));
        colonne->back_ground_vector().push_back(QBrush(Qt::red));
        colonne->back_ground_vector().push_back(QBrush(Qt::green));
        colonne->back_ground_vector().push_back(QBrush(Qt::blue));
        colonne->back_ground_vector().push_back(QBrush(Qt::yellow));
        colonne->back_ground_vector().push_back(QBrush(Qt::cyan));
        colonne->back_ground_vector().push_back(QBrush(Qt::magenta));
        colonne->fore_ground_vector().push_back(QBrush(Qt::white));
        colonne->fore_ground_vector().push_back(QBrush(Qt::white));
        colonne->fore_ground_vector().push_back(QBrush(Qt::black));
        colonne->fore_ground_vector().push_back(QBrush(Qt::white));
        colonne->fore_ground_vector().push_back(QBrush(Qt::black));
        colonne->fore_ground_vector().push_back(QBrush(Qt::black));
        colonne->fore_ground_vector().push_back(QBrush(Qt::white));
        return std::move(colonne);
}


