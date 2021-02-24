#include "model_note.h"

using namespace note_mps;

candidat_groupe_model::candidat_groupe_model(bdd_note & bdd, idt id_groupe, QObject *parent)
    : table_model(false,false,parent),
      m_groupe_info({Qt::ItemIsEnabled|Qt::ItemIsSelectable,0,tr("Groupe"),0,candidat_groupe_tableau::Eleve_Groupe_Tableau}){
    set_tableau(std::make_unique<candidat_groupe_tableau>(bdd,id_groupe));
    insert_colonne(Nom_Colonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                eleve_vec_tableau::Nom,tr("Nom"),0,candidat_groupe_tableau::Eleve_Tableau});
    insert_colonne(Prenom_Colonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   eleve_vec_tableau::Prenom,tr("Prenom"),0,candidat_groupe_tableau::Eleve_Tableau});
    insert_colonne(Groupe_Colonne,m_groupe_info);
}

classe_eleve_model::classe_eleve_model(bdd_note & bdd, idt id_classe, QObject *parent) : table_model(false,true,parent) {
    set_tableau(std::make_unique<classe_eleve_composition_tableau>(bdd,id_classe));
    insert_colonne(Nom_Colonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                eleve_vec_tableau::Nom,tr("Nom"),0,classe_eleve_composition_tableau::Eleve_Tableau});
    insert_colonne(Prenom_Colonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   eleve_vec_tableau::Prenom,tr("Prenom"),0,classe_eleve_composition_tableau::Eleve_Tableau});
    insert_colonne(Naissance_Colonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                      eleve_vec_tableau::Naissance,tr("Date de Naissance"),0,classe_eleve_composition_tableau::Eleve_Tableau});
    insert_colonne(Sexe_Colonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                 eleve_vec_tableau::Sexe,tr("Sexe"),0,classe_eleve_composition_tableau::Eleve_Tableau});
    insert_colonne(Entree_Colonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                     classe_eleve_vec_tableau::Entree,tr("Entrée"),0,classe_eleve_composition_tableau::Classe_Eleve_Tableau});
    insert_colonne(Sortie_Colonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                     classe_eleve_vec_tableau::Sortie,tr("Sortie"),0,classe_eleve_composition_tableau::Classe_Eleve_Tableau});
}

void classe_eleve_model::add(idt id_eleve){
    szt ligne = 0;
    while(ligne != nbr_lignes()
          && static_cast<eleve_vec_tableau&>(static_cast<classe_eleve_composition_tableau&>(*m_data)
                                           .tableau(classe_eleve_composition_tableau::Eleve_Tableau)).internal_data(ligne).id() != id_eleve)
        ++ligne;
    if(ligne == nbr_lignes()) {
        auto row = rowCount();
        insertRow(row);
        static_cast<classe_eleve_vec_tableau&>(static_cast<classe_eleve_composition_tableau&>(*m_data)
                                                   .tableau(classe_eleve_composition_tableau::Eleve_Tableau)).internal_data(ligne).set_id(id_eleve);
        hydrate(row);
    }
}

void eleve_groupe_model::push_back(){
    static_cast<eleve_groupe_tableau&>(*m_data).push_back();
    new_colonne_info info;
    info.flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
    info.name = static_cast<eleve_groupe_tableau&>(*m_data).num_to_texte(columnCount());
    info.tableau = static_cast<numt>(columnCount());
    push_back_colonne(info);
}

std::vector<std::pair<idt, int>> eleve_groupe_model::remove(const QModelIndexList & selection){
    std::map<szt,std::list<idt>> map;
    std::vector<std::pair<idt, int>> vec_id_num;
    for (auto it = selection.cbegin(); it != selection.cend(); ++it) {
        map[static_cast<szt>(it->column())].push_back(static_cast<idt>(it->row()));
        std::pair<szt,int> id_num;
        id_num.first = data(*it, abstract_colonnes_model::Id_Role).toUInt();
        id_num.second = it->column();
        vec_id_num.push_back(id_num);
    }
    for(auto list_it = map.begin(); list_it != map.end(); ++list_it)
        list_it->second.sort();
    beginResetModel();
        static_cast<eleve_groupe_tableau&>(*m_data).remove(map);
        reset_row_to_ligne();
    endResetModel();
    return vec_id_num;
}

void eleve_groupe_model::set_id_groupe(idt id_groupe){
    beginResetModel();
        static_cast<eleve_groupe_tableau&>(*m_data).set_id_groupe(id_groupe);
        m_colonnes.clear();
        new_colonne_info info;
        info.flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
        for (numt num = 0; num != static_cast<eleve_groupe_tableau&>(*m_data).size_column(); ++num){
            info.name = static_cast<eleve_groupe_tableau&>(*m_data).num_to_texte(static_cast<int>(num));
            info.tableau = num;
            push_back_colonne(info);
        }
        reset_row_to_ligne();
    endResetModel();
}

void eleve_groupe_model::update_eleve(const std::list<idt> & list_el, szt num, const std::map<szt,std::forward_list<idt>> & map_del) {
    auto & tableau = static_cast<eleve_groupe_tableau&>(*m_data);
    if(num < tableau.size_column()){
        if(map_del.empty()) {
            if(tableau.size_of_column(num) + list_el.size() < nbr_lignes()){
                tableau.add_eleve(list_el,num);
                dataChanged(index(0,static_cast<int>(num)),index(
                                tableau.size_of_column(num) < nbr_row() ? static_cast<int>(tableau.size_of_column(num))
                                                                     : rowCount(),
                                static_cast<int>(num)));
            }
            else {
                beginInsertRows(QModelIndex(), rowCount(), static_cast<int>(tableau.size_of_column(num) + list_el.size() - 1));
                    tableau.add_eleve(list_el,num);
                    reset_row_to_ligne();
                endInsertRows();
                dataChanged(index(0,static_cast<int>(num)),index(rowCount() - 1,static_cast<int>(num)));
            }
        }
        else {
            beginResetModel();
                tableau.add_eleve(list_el,num);
                tableau.del_eleve(map_del);
                reset_row_to_ligne();
            endResetModel();
        }
    }
}
