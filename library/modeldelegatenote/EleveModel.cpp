#include "EleveModel.h"

using namespace noteMPS;

/////////////AbstractEleveModel /////////////
void AbstractEleveModel::addNomPrenom(Tableau::iterator iter) {
    insertColumn(Nom,std::make_unique<EntityColumn<Eleve>>(tr("Nom"),iter,
            [](const Eleve & eleve,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.nom();
                return QVariant();},
            [](const QVariant & value, Eleve & eleve,int role)->bool {
                if(role == Qt::EditRole) {
                    eleve.setNom(value.toString());
                    return true;}
                return false;}));
    insertColumn(Prenom,std::make_unique<EntityColumn<Eleve>>(tr("Prénom"),iter,
            [](const Eleve & eleve,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.prenom();
                return QVariant();},
            [](const QVariant & value, Eleve & eleve,int role)->bool {
                if(role == Qt::EditRole) {
                    eleve.setPrenom(value.toString());
                    return true;}
                return false;}));
}

void AbstractEleveModel::addNaissanceSexe(Tableau::iterator iter) {
    insertColumn(Naissance,std::make_unique<EntityColumn<Eleve>>(tr("Date de Naissance"),iter,
            [](const Eleve & eleve,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.date();
                return QVariant();},
            [](const QVariant & value, Eleve & eleve,int role)->bool {
                if(role == Qt::EditRole) {
                    eleve.setDate(value.toDate());
                    return true;}
                return false;}));
    insertColumn(Sexe,std::make_unique<EntityColumn<Eleve>>(tr("Sexe"),iter,
            [](const Eleve & eleve,int role)->QVariant {
                if(role == Qt::CheckStateRole)
                    return eleve.fille()? Qt::Checked : Qt::Unchecked;
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.fille()?QString("F"):QString("M");
                return QVariant();},
            [](const QVariant & value, Eleve & eleve,int role)->bool {
                if(role == Qt::CheckStateRole) {
                    eleve.setFille(value.toBool());
                    return true;}
                return false;}));
}

//////////////////// EleveModel /////////

void EleveModel::find(const FindEleves & fe){
    auto noReset = (m_findEleves.nom.pattern().isEmpty() || fe.nom == m_findEleves.nom)
            && (m_findEleves.prenom.pattern().isEmpty() || fe.prenom == m_findEleves.prenom)
            && m_findEleves.apres <= fe.apres
            && m_findEleves.avant >= fe.avant
            && (m_findEleves.femme || !fe.femme)
            && (m_findEleves.homme || !fe.homme);
    auto nomValid = fe.nom.isValid();
    auto prenomValid = fe.prenom.isValid();
    AbstractEleveModel::find([this,&fe,nomValid,prenomValid](szt id){
        auto & eleve = static_cast<const Eleve &>(m_data.front()[id]);
        return (!nomValid || eleve.nom().contains(fe.nom))
                && (!prenomValid || eleve.prenom().contains(fe.prenom))
                && eleve.date() >= fe.apres
                && eleve.date() <= fe.avant
                && ((fe.femme && fe.homme) || (fe.femme && eleve.fille()) || (fe.homme && !eleve.fille()));}
    , !noReset);
    m_findEleves = fe;
}

//QVariant EleveModel::headerData(int section, Qt::Orientation orientation, int role) const {
//    if(orientation == Qt::Vertical && role == Qt::BackgroundRole) {
//        auto & data = m_data.front()[idRow(section)]; //static_cast<const Eleve &>(m_data.front()[static_cast<szt>(section)]);
//        if(!data.isValid())
//            return QBrush(QColor(Qt::red));
//        else if (data.isNew() || !bdd().sameInBdd(data)) {
//            auto idR = idRow(section);
//            szt i = 0;
//            auto & e1 = static_cast<const Eleve &>(data);
//            auto controle = true;
//            while (i != nbrRow() && controle) {
//                if(i != idR){
//                    auto & e2 = static_cast<const Eleve &>(m_data.front()[i]);
//                    controle = e1.nom() != e2.nom() || e1.prenom() != e2.prenom() || e1.date() != e2.date();
//                }
//                ++i;
//            }
//            if(i == nbrRow())
//                return QBrush(QColor(Qt::green));
//            else {
//                szt j = 0;
//                while(j < nbrRow() && m_idRow[j] != i)
//                    j++;
//                if(j != nbrRow())
//                    //emit headerDataChanged(Qt::Vertical,j,j);
//                return QBrush(QColor(255,125,0));
//            }
//        }
//    }
//    return AbstractEleveModel::headerData(section,orientation,role);
//}
