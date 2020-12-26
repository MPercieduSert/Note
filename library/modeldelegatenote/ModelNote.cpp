#include "ModelNote.h"

using namespace noteMPS;

CandidatGroupeModel::CandidatGroupeModel(BddNote & bdd, idt idGroupe, QObject * parent)
    : TableModel(false,false,parent),
      m_groupeInfo({Qt::ItemIsEnabled|Qt::ItemIsSelectable,0,tr("Groupe"),0,CandidatGroupeTableau::EleveGroupeTableau}){
    setTableau(std::make_unique<CandidatGroupeTableau>(bdd,idGroupe));
    insertColonne(NomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                EleveVecTableau::Nom,tr("Nom"),0,CandidatGroupeTableau::EleveTableau});
    insertColonne(PrenomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0,CandidatGroupeTableau::EleveTableau});
    insertColonne(GroupeColonne,m_groupeInfo);
}

ClasseEleveModel::ClasseEleveModel(BddNote & bdd, idt idClasse, QObject * parent) : TableModel(false,true,parent) {
    setTableau(std::make_unique<ClasseEleveCompositionTableau>(bdd,idClasse));
    insertColonne(NomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                EleveVecTableau::Nom,tr("Nom"),0,ClasseEleveCompositionTableau::EleveTableau});
    insertColonne(PrenomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0,ClasseEleveCompositionTableau::EleveTableau});
    insertColonne(NaissanceColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                      EleveVecTableau::Naissance,tr("Date de Naissance"),0,ClasseEleveCompositionTableau::EleveTableau});
    insertColonne(SexeColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                 EleveVecTableau::Sexe,tr("Sexe"),0,ClasseEleveCompositionTableau::EleveTableau});
    insertColonne(EntreeColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                     ClasseEleveVecTableau::Entree,tr("Entrée"),0,ClasseEleveCompositionTableau::ClasseEleveTableau});
    insertColonne(SortieColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                     ClasseEleveVecTableau::Sortie,tr("Sortie"),0,ClasseEleveCompositionTableau::ClasseEleveTableau});
}

void ClasseEleveModel::add(idt idEleve){
    szt ligne = 0;
    while(ligne != nbrLignes()
          && static_cast<EleveVecTableau&>(static_cast<ClasseEleveCompositionTableau&>(*m_data)
                                           .tableau(ClasseEleveCompositionTableau::EleveTableau)).internalData(ligne).id() != idEleve)
        ++ligne;
    if(ligne == nbrLignes()) {
        auto row = rowCount();
        insertRow(row);
        static_cast<ClasseEleveVecTableau&>(static_cast<ClasseEleveCompositionTableau&>(*m_data)
                                                   .tableau(ClasseEleveCompositionTableau::EleveTableau)).internalData(ligne).setId(idEleve);
        hydrate(row);
    }
}

void EleveGroupeModel::push_back(){
    static_cast<EleveGroupeTableau&>(*m_data).push_back();
    NewColonneInfo info;
    info.flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
    info.name = static_cast<EleveGroupeTableau&>(*m_data).numToTexte(columnCount());
    info.tableau = static_cast<numt>(columnCount());
    push_backColonne(info);
}

std::vector<std::pair<idt, int>> EleveGroupeModel::remove(const QModelIndexList & selection){
    std::map<szt,std::list<idt>> map;
    std::vector<std::pair<idt, int>> vecIdNum;
    for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter) {
        map[static_cast<szt>(iter->column())].push_back(static_cast<idt>(iter->row()));
        std::pair<szt,int> idNum;
        idNum.first = data(*iter, AbstractColonnesModel::IdRole).toUInt();
        idNum.second = iter->column();
        vecIdNum.push_back(idNum);
    }
    for(auto iterList = map.begin(); iterList != map.end(); ++iterList)
        iterList->second.sort();
    beginResetModel();
        static_cast<EleveGroupeTableau&>(*m_data).remove(map);
        resetRowToLigne();
    endResetModel();
    return vecIdNum;
}

void EleveGroupeModel::setIdGroupe(idt idGroupe){
    beginResetModel();
        static_cast<EleveGroupeTableau&>(*m_data).setIdGroupe(idGroupe);
        m_colonnes.clear();
        NewColonneInfo info;
        info.flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
        for (numt num = 0; num != static_cast<EleveGroupeTableau&>(*m_data).sizeColumn(); ++num){
            info.name = static_cast<EleveGroupeTableau&>(*m_data).numToTexte(static_cast<int>(num));
            info.tableau = num;
            push_backColonne(info);
        }
        resetRowToLigne();
    endResetModel();
}

void EleveGroupeModel::updateEleve(const std::list<idt> & listEl, szt num, const std::map<szt,std::forward_list<idt>> & mapDel) {
    auto & tableau = static_cast<EleveGroupeTableau&>(*m_data);
    if(num < tableau.sizeColumn()){
        if(mapDel.empty()) {
            if(tableau.sizeOfColumn(num) + listEl.size() < nbrLignes()){
                tableau.addEleve(listEl,num);
                dataChanged(index(0,static_cast<int>(num)),index(
                                tableau.sizeOfColumn(num) < nbrRow() ? static_cast<int>(tableau.sizeOfColumn(num))
                                                                     : rowCount(),
                                static_cast<int>(num)));
            }
            else {
                beginInsertRows(QModelIndex(), rowCount(), static_cast<int>(tableau.sizeOfColumn(num) + listEl.size() - 1));
                    tableau.addEleve(listEl,num);
                    resetRowToLigne();
                endInsertRows();
                dataChanged(index(0,static_cast<int>(num)),index(rowCount() - 1,static_cast<int>(num)));
            }
        }
        else {
            beginResetModel();
                tableau.addEleve(listEl,num);
                tableau.delEleve(mapDel);
                resetRowToLigne();
            endResetModel();
        }
    }
}
