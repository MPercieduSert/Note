#include "ModelNote.h"

using namespace noteMPS;

ClasseEleveModel::ClasseEleveModel(BddNote & bdd, szt idClasse, QObject * parent) : TableModel(false,false,parent) {
    auto tableaux = std::make_unique<ClasseEleveCompositionTableau>(bdd,idClasse);
    setTableau(std::move(tableaux));
    insertColonne(Nom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                EleveVecTableau::Nom,tr("Nom"),0});
    insertColonne(Prenom,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                   EleveVecTableau::Prenom,tr("Prenom"),0});
    insertColonne(Naissance,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                      EleveVecTableau::Naissance,tr("Date de Naissance"),0});
    insertColonne(Sexe,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                 EleveVecTableau::Sexe,tr("Sexe"),0});
    insertColonne(Entree,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                     ClasseEleveVecTableau::Entree,tr("Entrée"),0,1});
    insertColonne(Sortie,{Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable,
                                     ClasseEleveVecTableau::Sortie,tr("Sortie"),0,1});
}

void ClasseEleveModel::add(szt idEleve){
    szt ligne = 0;
    while(ligne != nbrLignes()
          && static_cast<EleveVecTableau&>(static_cast<ClasseEleveCompositionTableau&>(*m_data)
                                           .tableau(ClasseEleveCompositionTableau::EleveTableau)).internalData(ligne).id() != idEleve)
        ++ligne;
    if(ligne == nbrLignes()) {
        insertRow(rowCount());
        static_cast<ClasseEleveVecTableau&>(static_cast<ClasseEleveCompositionTableau&>(*m_data)
                                                   .tableau(ClasseEleveCompositionTableau::EleveTableau)).internalData(ligne).setId(idEleve);
        m_data->hydrate(ligne);
    }
}
