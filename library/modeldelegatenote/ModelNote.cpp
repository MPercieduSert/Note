#include "ModelNote.h"

using namespace noteMPS;

CandidatGroupeModel::CandidatGroupeModel(BddNote & bdd, idt id_groupe, QObject * parent)
    : table_model(false,false,parent),
      m_groupeInfo({Qt::ItemIsEnabled|Qt::ItemIsSelectable,0,tr("Groupe"),0,CandidatGroupeTableau::Eleve_groupeTableau}){
    set_tableau(std::make_unique<CandidatGroupeTableau>(bdd,id_groupe));
    insertColonne(NomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                EleveVecTableau::Nom,tr("Nom"),0,CandidatGroupeTableau::EleveTableau});
    insertColonne(PrenomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0,CandidatGroupeTableau::EleveTableau});
    insertColonne(GroupeColonne,m_groupeInfo);
}

ClasseEleveModel::ClasseEleveModel(BddNote & bdd, idt idClasse, QObject * parent) : table_model(false,true,parent) {
    set_tableau(std::make_unique<ClasseElevecomposition_tableau>(bdd,idClasse));
    insertColonne(NomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                EleveVecTableau::Nom,tr("Nom"),0,ClasseElevecomposition_tableau::EleveTableau});
    insertColonne(PrenomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0,ClasseElevecomposition_tableau::EleveTableau});
    insertColonne(NaissanceColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                      EleveVecTableau::Naissance,tr("Date de Naissance"),0,ClasseElevecomposition_tableau::EleveTableau});
    insertColonne(SexeColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                 EleveVecTableau::Sexe,tr("Sexe"),0,ClasseElevecomposition_tableau::EleveTableau});
    insertColonne(EntreeColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                     ClasseEleveVecTableau::Entree,tr("Entrée"),0,ClasseElevecomposition_tableau::ClasseEleveTableau});
    insertColonne(SortieColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                     ClasseEleveVecTableau::Sortie,tr("Sortie"),0,ClasseElevecomposition_tableau::ClasseEleveTableau});
}

void ClasseEleveModel::add(idt idEleve){
    szt ligne = 0;
    while(ligne != nbr_lignes()
          && static_cast<EleveVecTableau&>(static_cast<ClasseElevecomposition_tableau&>(*m_data)
                                           .tableau(ClasseElevecomposition_tableau::EleveTableau)).internal_data(ligne).id() != idEleve)
        ++ligne;
    if(ligne == nbr_lignes()) {
        auto row = row_count();
        insertRow(row);
        static_cast<ClasseEleveVecTableau&>(static_cast<ClasseElevecomposition_tableau&>(*m_data)
                                                   .tableau(ClasseElevecomposition_tableau::EleveTableau)).internal_data(ligne).set_id(idEleve);
        hydrate(row);
    }
}

void Eleve_groupeModel::push_back(){
    static_cast<Eleve_groupeTableau&>(*m_data).push_back();
    new_colonne_info info;
    info.flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
    info.name = static_cast<Eleve_groupeTableau&>(*m_data).numToTexte(columnCount());
    info.tableau = static_cast<numt>(columnCount());
    push_back_colonne(info);
}

std::vector<std::pair<idt, int>> Eleve_groupeModel::remove(const QModelIndexList & selection){
    std::map<szt,std::list<idt>> map;
    std::vector<std::pair<idt, int>> vecIdNum;
    for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter) {
        map[static_cast<szt>(iter->column())].push_back(static_cast<idt>(iter->row()));
        std::pair<szt,int> idNum;
        idNum.first = data(*iter, abstract_colonnes_model::Id_Role).toUInt();
        idNum.second = iter->column();
        vecIdNum.push_back(idNum);
    }
    for(auto iterList = map.begin(); iterList != map.end(); ++iterList)
        iterList->second.sort();
    beginResetModel();
        static_cast<Eleve_groupeTableau&>(*m_data).remove(map);
        reset_row_to_ligne();
    end_reset_model();
    return vecIdNum;
}

void Eleve_groupeModel::set_id_groupe(idt id_groupe){
    beginResetModel();
        static_cast<Eleve_groupeTableau&>(*m_data).set_id_groupe(id_groupe);
        m_colonnes.clear();
        new_colonne_info info;
        info.flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
        for (numt num = 0; num != static_cast<Eleve_groupeTableau&>(*m_data).size_column(); ++num){
            info.name = static_cast<Eleve_groupeTableau&>(*m_data).numToTexte(static_cast<int>(num));
            info.tableau = num;
            push_back_colonne(info);
        }
        reset_row_to_ligne();
    end_reset_model();
}

void Eleve_groupeModel::updateEleve(const std::list<idt> & listEl, szt num, const std::map<szt,std::forward_list<idt>> & mapDel) {
    auto & tableau = static_cast<Eleve_groupeTableau&>(*m_data);
    if(num < tableau.size_column()){
        if(mapDel.empty()) {
            if(tableau.size_of_column(num) + listEl.size() < nbr_lignes()){
                tableau.addEleve(listEl,num);
                dataChanged(index(0,static_cast<int>(num)),index(
                                tableau.size_of_column(num) < nbr_row() ? static_cast<int>(tableau.size_of_column(num))
                                                                     : row_count(),
                                static_cast<int>(num)));
            }
            else {
                beginInsertRows(QModelIndex(), row_count(), static_cast<int>(tableau.size_of_column(num) + listEl.size() - 1));
                    tableau.addEleve(listEl,num);
                    reset_row_to_ligne();
                endInsertRows();
                dataChanged(index(0,static_cast<int>(num)),index(row_count() - 1,static_cast<int>(num)));
            }
        }
        else {
            beginResetModel();
                tableau.addEleve(listEl,num);
                tableau.delEleve(mapDel);
                reset_row_to_ligne();
            end_reset_model();
        }
    }
}
