#include "EleveModel.h"

//////////////////// EleveModel /////////

//void EleveModel::find(const FindEleves & fe){
//    auto noReset = (m_findEleves.nom.pattern().isEmpty() || fe.nom == m_findEleves.nom)
//            && (m_findEleves.prenom.pattern().isEmpty() || fe.prenom == m_findEleves.prenom)
//            && m_findEleves.apres <= fe.apres
//            && m_findEleves.avant >= fe.avant
//            && (m_findEleves.femme || !fe.femme)
//            && (m_findEleves.homme || !fe.homme);
//    auto nomValid = fe.nom.isValid();
//    auto prenomValid = fe.prenom.isValid();
//    AbstractEleveModel::find([this,&fe,nomValid,prenomValid](szt id){
//        auto & eleve = static_cast<const Eleve &>(m_data.front()[id]);
//        return (!nomValid || eleve.nom().contains(fe.nom))
//                && (!prenomValid || eleve.prenom().contains(fe.prenom))
//                && eleve.date() >= fe.apres
//                && eleve.date() <= fe.avant
//                && ((fe.femme && fe.homme) || (fe.femme && eleve.fille()) || (fe.homme && !eleve.fille()));}
//    , !noReset);
//    m_findEleves = fe;
//}
