#include "tableau_note.h"

using namespace note_mps;

////////////////////////////////////////// candidat_groupe_tableau ////////////////////////////////
candidat_groupe_tableau::candidat_groupe_tableau(bdd_note & bdd, idt id_groupe) : m_bdd(bdd) {
    push_back(std::make_unique<eleve_vec_tableau>(bdd),false);
    m_groupe.set_id(id_groupe);
    m_bdd.get(m_groupe);
    if(m_groupe.id_classe())
        static_cast<eleve_vec_tableau&>(tableau(Eleve_Tableau))
            .set_vec_data(m_bdd.get_list<eleve,classe_eleve>(classe_eleve::Id_Eleve,classe_eleve::Id_Classe,m_groupe.id_classe()));
    else
        static_cast<eleve_vec_tableau&>(tableau(Eleve_Tableau))
            .set_vec_data(m_bdd.get_list<eleve,classe_eleve,classe>(classe_eleve::Id_Eleve,classe_eleve::Id_Classe,
                                                                    classe::Id_An,m_groupe.id_an()));
    if(m_groupe.test(groupe::Exclusif)) {
        push_back(std::make_unique<eleve_groupe_vec_tableau>(m_bdd,tableau(Eleve_Tableau).size()),true);
        static_cast<eleve_groupe_vec_tableau&>(tableau(Eleve_Groupe_Tableau)).set_groupe(m_groupe);
    }
    else {
        push_back(std::make_unique<eleve_groupe_list_tableau>(m_bdd,tableau(Eleve_Tableau).size()),true);
        static_cast<eleve_groupe_list_tableau&>(tableau(Eleve_Groupe_Tableau)).set_groupe(m_groupe);
    }
    for(szt ligne = 0; ligne != size(); ++ligne)
        hydrate_eleve_groupe(ligne);
}

void candidat_groupe_tableau::hydrate_eleve_groupe(szt ligne) {
    auto id_el = static_cast<eleve_vec_tableau&>(tableau(Eleve_Tableau)).internal_data(ligne).id();
    auto vec = m_bdd.get_list<eleve_groupe>(eleve_groupe::Id_Eleve,id_el,eleve_groupe::Id_Groupe,m_groupe.id(),eleve_groupe::Num);
    if(m_groupe.test(groupe::Exclusif)) {
        auto & el_gr = static_cast<eleve_groupe_vec_tableau&>(tableau(Eleve_Groupe_Tableau)).internal_data(ligne);
        if(vec.empty()) {
            el_gr.set_id_eleve(id_el);
            el_gr.set_id_groupe(m_groupe.id());
            el_gr.set_num(Non_Affecter);
        }
        else
            el_gr = vec.front();
    }
    else {
        auto & el_gr = static_cast<eleve_groupe_list_tableau&>(tableau(Eleve_Groupe_Tableau)).internal_data(ligne);
        el_gr.clear();
        if(!vec.empty())
            for (auto it = vec.cbegin(); it != vec.cend(); ++it)
                el_gr.push_back(*it);
    }
}

void candidat_groupe_tableau::remove(std::vector<std::pair<idt, int> > &&vec_id_num) {
    for (auto it = vec_id_num.cbegin(); it != vec_id_num.cend(); ++it) {
        szt i = 0;
        while (i != size() &&
               static_cast<eleve_vec_tableau&>(tableau(Eleve_Tableau)).internal_data(i).id() != it->first)
            ++i;
        if(i != size()) {
            if(m_groupe.test(groupe::Exclusif))
                static_cast<eleve_groupe_vec_tableau&>(tableau(Eleve_Groupe_Tableau)).internal_data(i).set_num(Non_Affecter);
            else
                static_cast<eleve_groupe_list_tableau&>(tableau(Eleve_Groupe_Tableau)).internal_data(i).remove_if(
                        [it](const eleve_groupe & el_gr)->bool{return el_gr.num() == it->second;});
        }
    }
}

void candidat_groupe_tableau::save(szt ligne) {
    if(m_groupe.test(groupe::Exclusif)) {
        auto & el_gr = static_cast<eleve_groupe_vec_tableau&>(tableau(Eleve_Groupe_Tableau)).internal_data(ligne);
        if(el_gr.num() >=0)
            m_bdd.save(el_gr);
        else if (!el_gr.is_new())
            m_bdd.del(el_gr);
    }
    else {
        auto & el_gr = static_cast<eleve_groupe_list_tableau&>(tableau(Eleve_Groupe_Tableau)).internal_data(ligne);
        auto id_el = static_cast<eleve_vec_tableau&>(tableau(Eleve_Tableau)).internal_data(ligne).id();
        for (auto it = el_gr.begin(); it != el_gr.end(); ++it)
            it->set_id_eleve(id_el);
        auto vec = m_bdd.get_list<eleve_groupe>(eleve_groupe::Id_Eleve,id_el,eleve_groupe::Id_Groupe,
                                                m_groupe.id(),eleve_groupe::Num);
        auto tab_it = el_gr.begin();
        auto bdd_it = vec.begin();
        while(tab_it != el_gr.end() && bdd_it != vec.end()){
            if(tab_it->num() == bdd_it->num()){
                ++tab_it;
                ++bdd_it;
            }
            else if (tab_it->num() < bdd_it->num()) {
                m_bdd.save(*tab_it);
                ++tab_it;
            }
            else {
                m_bdd.del(*bdd_it);
                ++bdd_it;
            }
        }
        while(tab_it != el_gr.end()){
            m_bdd.save(*tab_it);
            ++tab_it;
        }
        while(bdd_it != vec.end()){
            m_bdd.del(*bdd_it);
            ++bdd_it;
        }
    }
}

////////////////////////////////////////// classe_eleve_composition_tableau ////////////////////////////////
classe_eleve_composition_tableau::classe_eleve_composition_tableau(bdd_note & bdd, idt id_classe) : m_bdd(bdd) {
    push_back(std::make_unique<eleve_vec_tableau>(bdd),false);
    push_back(std::make_unique<classe_eleve_vec_tableau>(bdd));
    set_id_classe(id_classe);
}

void classe_eleve_composition_tableau::hydrate_classe_eleve(szt ligne){
    auto & cl_el = static_cast<classe_eleve_vec_tableau&>(tableau(Classe_Eleve_Tableau)).internal_data(ligne);
    cl_el.set_id_eleve(static_cast<eleve_vec_tableau&>(tableau(Eleve_Tableau)).internal_data(ligne).id());
    cl_el.set_id_classe(m_id_classe);
    if(!m_bdd.get_unique(cl_el)){
        donnee_cible dn_cb;
        dn_cb.set_id_donnee(m_bdd.ref_to_id<donnee>("date_defaut_dn"));
        dn_cb.set_cible(m_bdd.cible<classe>());
        dn_cb.set_id_cible(m_id_classe);
        dn_cb.set_num(donnee_mps::Entree_Num);
        if(m_bdd.get_unique(dn_cb))
            cl_el.set_entree(dn_cb.valeur().toDate());
        dn_cb.set_id(0);
        dn_cb.set_num(donnee_mps::Sortie_Num);
        if(m_bdd.get_unique(dn_cb))
            cl_el.set_sortie(dn_cb.valeur().toDate());
    }
}

void classe_eleve_composition_tableau::set_id_classe(idt id) {
    m_id_classe = id;
    static_cast<eleve_vec_tableau&>(tableau(Eleve_Tableau))
            .set_vec_data(m_bdd.get_list<eleve,classe_eleve>(classe_eleve::Id_Eleve,classe_eleve::Id_Classe,m_id_classe));
    static_cast<classe_eleve_vec_tableau&>(tableau(Classe_Eleve_Tableau)).set_vec_data(mps::vector_ptr<classe_eleve>(size()));
    for(szt ligne = 0; ligne != size(); ++ligne)
        hydrate_classe_eleve(ligne);
}

////////////////////////////////////////////// classe_eleve_vec_tableau ////////////////////////////////////////
std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
classe_eleve_vec_tableau::make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) {
    switch (info.id) {
    case Entree:
        return std::make_unique<mps::model_base::vector_ptr_colonne<classe_eleve>>(info.name,info.flags,
                                                                         mps::model_base::Date_Colonne_Type,m_vec,
            [](const classe_eleve & cl_el,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return cl_el.entree();
                return QVariant();},
            [](const classe_eleve & cl_el)->QVariant{return cl_el.entree();},
            [](classe_eleve & cl_el, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    cl_el.set_entree(value.toDate());
                    return true;}
                return false;});
    case Sortie:
        return std::make_unique<mps::model_base::vector_ptr_colonne<classe_eleve>>(info.name,info.flags,
                                                                         mps::model_base::Date_Colonne_Type,m_vec,
            [](const classe_eleve & cl_el,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return cl_el.sortie();
                return QVariant();},
            [](const classe_eleve & cl_el)->QVariant{return cl_el.sortie();},
            [](classe_eleve & cl_el, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    cl_el.set_sortie(value.toDate());
                    return true;}
                return false;});
    default:
        return nullptr;
    }
}

/////////////////////////////////////////// eleve_vec_tableau ///////////////////////////////////////////////////
eleve_vec_tableau::nom_colonne::nom_colonne(const QString &name, Qt::ItemFlags flags, mps::vector_ptr<eleve> & vec)
    : mps::model_base::vector_ptr_id_colonne<eleve>(name, flags,mps::model_base::Texte_Colonne_Type,vec,
    [](const eleve & eleve, int role)->QVariant{
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return eleve.nom();
        return QVariant();},
    [](const eleve & eleve)->QVariant{return eleve.nom();},
    [](eleve & eleve, const QVariant & value, int role){
        if(role == Qt::EditRole){
            eleve.set_nom(value.toString());
            return true;}
        return false;}) {}

bool eleve_vec_tableau::egal(szt ligne_1, szt ligne_2) const {
    return m_vec[ligne_1].nom() == m_vec[ligne_2].nom()
            && m_vec[ligne_1].prenom() == m_vec[ligne_2].prenom()
            && m_vec[ligne_1].date() == m_vec[ligne_2].date();
}

std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
eleve_vec_tableau::make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) {
    switch (info.id) {
    case Nom:
        return std::make_unique<nom_colonne>(info.name,info.flags,m_vec);
    case Prenom:
        return std::make_unique<mps::model_base::vector_ptr_colonne<eleve>>(info.name,info.flags,
                                                                   mps::model_base::Texte_Colonne_Type,m_vec,
            [](const eleve & eleve,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.prenom();
                return QVariant();},
            [](const eleve & eleve)->QVariant{return eleve.prenom();},
            [](eleve & eleve, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    eleve.set_prenom(value.toString());
                    return true;}
                return false;});
    case Naissance:
        return std::make_unique<mps::model_base::vector_ptr_colonne<eleve>>(info.name,info.flags,
                                                                   mps::model_base::Date_Colonne_Type,m_vec,
           [](const eleve & eleve,int role)->QVariant {
               if(role == Qt::DisplayRole || role == Qt::EditRole)
                   return eleve.date();
               return QVariant();},
           [](const eleve & eleve)->QVariant{return eleve.date();},
           [](eleve & eleve, const QVariant & value, int role)->bool {
               if(role == Qt::EditRole) {
                   eleve.set_date(value.toDate());
                   return true;}
               return false;});
    case Sexe:
        return std::make_unique<mps::model_base::vector_ptr_bool_colonne<eleve>>(info.name,info.flags,
                                                                       mps::model_base::Bool_Colonne_Type,m_vec,
           [](const eleve & eleve)->bool {return eleve.fille();},
           [](eleve & eleve, bool value) {eleve.set_fille(value);},
           "F","M");
    default:
        return nullptr;
    }
}

///////////////////////////////////////////////// Eleve_Groupe_Tableau ///////////////////////////////////////////////////
void eleve_groupe_tableau::add_eleve(const std::list<idt> &list_el, szt num) {
    if(num < size_column()) {
        auto & ref_col = colonne(num);
        for (auto id_it = list_el.cbegin(); id_it != list_el.cend(); ++id_it) {
            ref_col.push_back(eleve(*id_it));
            m_bdd.get(ref_col.back());
        }
        std::sort(ref_col.begin(),ref_col.end(),
                  [](const eleve & el_1,const eleve el_2)->bool{return el_1 < el_2;});
        if(ref_col.size() > m_nbrLine)
            m_nbrLine = static_cast<szt>(ref_col.size());
    }
}

void eleve_groupe_tableau::del_eleve(const std::map<szt, std::forward_list<idt>> &map_del) {
    for (auto it = map_del.cbegin(); it != map_del.cend(); ++it) {
        for (auto id_it = it->second.cbegin(); id_it != it->second.cend(); ++id_it) {
            auto vec_it = colonne_at(it->first).cbegin();
            while (vec_it->id() != *id_it)
                ++vec_it;
            colonne(it->first).erase(vec_it);
        }
    }
    update_nbr_line();
}

std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
eleve_groupe_tableau::make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) {
    return std::make_unique<mps::model_base::vector_ref_heterogene_taille_id_colonne<eleve>>(info.name,info.flags,
                                                                               mps::model_base::Texte_Colonne_Type,
                                                                               colonne_at(info.tableau),
    [](const eleve & el,int role)->QVariant {
        if(role == Qt::DisplayRole)
            return QString().append(el.nom()).append(" ").append(el.prenom());
        return QVariant();},
    [](const eleve &)->QVariant {return QVariant();},
    [](eleve &, const QVariant &, int)->bool {return false;});
}

void eleve_groupe_tableau::remove(const std::map<szt,std::list<idt>> & map) {
    for (auto it = map.cbegin(); it != map.cend(); ++it)
        for (auto list_it = it->second.crbegin(); list_it != it->second.crend(); ++list_it)
            m_tableau[it->first]->erase(std::next(m_tableau[it->first]->cbegin(),*list_it));
    update_nbr_line();
}

void eleve_groupe_tableau::set_id_groupe(idt id) {
    m_groupe.set_id(id);
    m_bdd.get(m_groupe);
    m_style_num.set_style(m_groupe.style_num());
    m_tableau.clear();
    szt nbr_gr = 0;
    if(m_bdd.exists<eleve_groupe>(eleve_groupe::Id_Groupe,m_groupe.id())) {
        nbr_gr = 1 + m_bdd.fonction_agrega<eleve_groupe>(mps::b2d::agrega::Max,eleve_groupe::Num,
                                                        eleve_groupe::Id_Groupe,m_groupe.id()).toUInt();
        for (szt num = 0; num != nbr_gr; ++num) {
            auto vec_el = m_bdd.get_list<eleve,eleve_groupe>(eleve_groupe::Id_Eleve,
                                                          eleve_groupe::Id_Groupe,m_groupe.id(),
                                                          eleve_groupe::Num,static_cast<uint>(num));
            push_back(vec_el.size());
            auto write_it = colonne(num).begin();
            for (auto read_it = vec_el.cbegin(); read_it != vec_el.cend(); ++read_it, ++write_it)
                *write_it = *read_it;
        }
    }
    update_nbr_line();
}

///////////////////////////////////////////////// eleve_groupe_list_tableau ///////////////////////////////////////////////
std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
eleve_groupe_list_tableau::make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) {
    auto read = [this](const std::list<eleve_groupe> & list_el_gr,int role)->QVariant {
        switch (role) {
        case Qt::DisplayRole: {
            QString str;
            for(auto it = list_el_gr.cbegin(); it != list_el_gr.cend(); ++it)
                str.append(num_to_texte(it->num())).append(", ");
            str.chop(2);
            return str;
        }
        case Qt::TextAlignmentRole:
            return Qt::AlignHCenter;
        case candidat_groupe_tableau::Num_Role: {
            QList<QVariant> lq;
            for (auto it = list_el_gr.cbegin(); it != list_el_gr.cend(); ++it)
                lq.append(it->num());
            return lq;
            }
        default:
            return QVariant();}};
    auto find = [read](const std::list<eleve_groupe>& list_el_gr)->QVariant
        {return read(list_el_gr,candidat_groupe_tableau::Num_Role);};
    auto write = [this](std::list<eleve_groupe> & el_gr, const QVariant & value, int role)->bool {
        if(role == Qt::EditRole){
            eleve_groupe eg;
            eg.set_id_groupe(m_groupe.id());
            eg.set_num(value.toInt());
            el_gr.push_back(eg);
            el_gr.sort([](const eleve_groupe & el_gr_1, const eleve_groupe & el_gr_2)->bool
                        {return el_gr_1.num() < el_gr_2.num();});
            return true;
        }
        return false;};
    return std::make_unique<mps::model_base::base_colonne_temp<decltype (read), decltype (find), decltype (write),
                                                      std::vector<std::list<eleve_groupe>>>>(info.name,info.flags,
                                                                     mps::model_base::Texte_Colonne_Type,m_vec,
                                                                     read,find,write);
}

///////////////////////////////////////////////// eleve_groupe_vec_tableau ///////////////////////////////////////////////
std::unique_ptr<mps::model_base::abstract_colonnes_model::abstract_colonne>
eleve_groupe_vec_tableau::make_colonne(const mps::model_base::abstract_colonnes_model::new_colonne_info & info) {
    auto backFore = [](const eleve_groupe & el_gr)->szt {return static_cast<szt>(el_gr.num() + 1);};
    auto read = [this](const eleve_groupe & el_gr,int role)->QVariant {
        switch (role) {
        case Qt::DisplayRole:
            return num_to_texte(el_gr.num());
        case Qt::TextAlignmentRole:
            return Qt::AlignHCenter;
        case candidat_groupe_tableau::Num_Role:
            return el_gr.num();
        default:
            return QVariant();}};
    auto find = [](const eleve_groupe & el_gr)->QVariant {return el_gr.num();};
    auto write = [](eleve_groupe & el_gr, const QVariant & value, int role)->bool {
        if(role == Qt::EditRole){
            el_gr.set_num(value.toInt());
            return true;}
        return false;};
        auto colonne = std::make_unique<mps::model_base::base_color_colonne_temp<decltype (backFore), decltype (backFore),
                                                               decltype (read), decltype (find), decltype (write),
                                                               mps::vector_ptr<eleve_groupe>>>(info.name,info.flags,
                                                                         mps::model_base::Texte_Colonne_Type,m_vec,
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
