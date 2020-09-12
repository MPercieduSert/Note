#include "ModelNote.h"

using namespace noteMPS;

ClasseEleveModel::ClasseEleveModel(BddNote & bdd, szt idClasse, QObject * parent) : TableModel(false,true,parent) {
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

void ClasseEleveModel::add(szt idEleve){
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
