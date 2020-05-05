#include "TableauNote.h"

using namespace noteMPS;

EleveVecTableau::NomColonne::NomColonne(const QString &name, Qt::ItemFlags flags, conteneurMPS::VectorPtr<Eleve> & vec)
    : modelMPS::VectorPtrColonne<Eleve>(name, flags,modelMPS::TexteColonne,vec,
    [](const Eleve & eleve, int role)->QVariant{
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return eleve.nom();
        return QVariant();},
    [](const QVariant & value, Eleve & eleve, int role){
        if(role == Qt::EditRole){
            eleve.setNom(value.toString());
            return true;
        }
        return false;}) {}

bool EleveVecTableau::NomColonne::compare(szt ligne1, szt ligne2) const {
    if(m_vec[ligne1].nom() < m_vec[ligne2].nom())
        return true;
    if(m_vec[ligne1].nom() == m_vec[ligne2].nom()){
        if(m_vec[ligne1].prenom() < m_vec[ligne2].prenom())
            return true;
        else if (m_vec[ligne1].prenom() == m_vec[ligne2].prenom() && m_vec[ligne1].date() < m_vec[ligne2].date())
            return true;
    }
    return false;
}

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
        return std::make_unique<modelMPS::VectorPtrColonne<Eleve>>(info.name,info.flags,modelMPS::TexteColonne,m_vec,
            [](const Eleve & eleve,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.prenom();
                return QVariant();},
            [](const QVariant & value, Eleve & eleve,int role)->bool {
                if(role == Qt::EditRole) {
                    eleve.setPrenom(value.toString());
                    return true;}
                return false;});
    case Naissance:
        return std::make_unique<modelMPS::VectorPtrColonne<Eleve>>(info.name,info.flags,modelMPS::DateColonne,m_vec,
           [](const Eleve & eleve,int role)->QVariant {
               if(role == Qt::DisplayRole || role == Qt::EditRole)
                   return eleve.date();
               return QVariant();},
           [](const QVariant & value, Eleve & eleve,int role)->bool {
               if(role == Qt::EditRole) {
                   eleve.setDate(value.toDate());
                   return true;}
               return false;});
    case Sexe:
        return std::make_unique<modelMPS::VectorPtrColonne<Eleve>>(info.name,info.flags,modelMPS::BooleenColonne,m_vec,
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
               return false;});
    default:
        return nullptr;
    }
}
