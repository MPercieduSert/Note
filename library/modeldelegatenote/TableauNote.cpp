#include "TableauNote.h"

using namespace noteMPS;

EleveVecTableau::NomColonne::NomColonne(const QString &name, Qt::ItemFlags flags, conteneurMPS::VectorPtr<Eleve> & vec)
    : modelMPS::IdVectorPtrColonne<Eleve>(name, flags,modelMPS::AbstractColonnesModel::TexteColonne,vec,
    [](const Eleve & eleve, int role)->QVariant{
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return eleve.nom();
        return QVariant();},
    [](const Eleve & eleve)->QVariant{return eleve.nom();},
    [](Eleve & eleve, const QVariant & value, int role){
        if(role == Qt::EditRole){
            eleve.setNom(value.toString());
            return true;}
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
        return std::make_unique<modelMPS::VectorPtrColonne<Eleve>>(info.name,info.flags,modelMPS::AbstractColonnesModel::TexteColonne,m_vec,
            [](const Eleve & eleve,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return eleve.prenom();
                return QVariant();},
            [](const Eleve & eleve)->QVariant{return eleve.prenom();},
            [](Eleve & eleve, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    eleve.setPrenom(value.toString());
                    return true;}
                return false;});
    case Naissance:
        return std::make_unique<modelMPS::VectorPtrColonne<Eleve>>(info.name,info.flags,modelMPS::AbstractColonnesModel::DateColonne,m_vec,
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
        return std::make_unique<modelMPS::VectorPtrBoolColonne<Eleve>>(info.name,info.flags,modelMPS::AbstractColonnesModel::BoolColonne,m_vec,
           [](const Eleve & eleve)->bool {return eleve.fille();},
           [](Eleve & eleve, bool value) {eleve.setFille(value);},
           "F","M");
    default:
        return nullptr;
    }
}

std::unique_ptr<modelMPS::AbstractColonnesModel::AbstractColonne>
ClasseEleveVecTableau::makeColonne(const modelMPS::AbstractColonnesModel::NewColonneInfo & info) {
    switch (info.id) {
    case Entree:
        return std::make_unique<modelMPS::IdVectorPtrColonne<ClasseEleve>>(info.name,info.flags,
                                                                         modelMPS::AbstractColonnesModel::DateColonne,m_vec,
            [](const ClasseEleve & clEl,int role)->QVariant {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return clEl.entree();
                return QVariant();},
            [](const ClasseEleve & clEl)->QVariant{return clEl.entree();},
            [](ClasseEleve & clEl, const QVariant & value, int role)->bool {
                if(role == Qt::EditRole) {
                    clEl.setEntree(value.toDate());
                    return true;}
                return false;});
    case Sortie:
        return std::make_unique<modelMPS::VectorPtrColonne<ClasseEleve>>(info.name,info.flags,
                                                                         modelMPS::AbstractColonnesModel::DateColonne,m_vec,
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

ClasseEleveCompositionTableau::ClasseEleveCompositionTableau(BddNote & bdd, szt idClasse) : m_bdd(bdd) {
    push_back(std::make_unique<EleveVecTableau>(bdd),false);
    push_back(std::make_unique<ClasseEleveVecTableau>(bdd));
    setIdClasse(idClasse);
}

void ClasseEleveCompositionTableau::hydrateClasseEleve(szt ligne){
    auto & clEl = static_cast<ClasseEleveVecTableau&>(*m_tableaux.back()).internalData(ligne);
    clEl.setIdEleve(static_cast<EleveVecTableau&>(*m_tableaux.front()).internalData(ligne).id());
    clEl.setIdClasse(m_idClasse);
    if(!m_bdd.getUnique(clEl)){
        DonneeCible dnCb;
        dnCb.setIdDonnee(m_bdd.idProgToId<Donnee>(donnee::DefaultDateId));
        dnCb.setCible(m_bdd.cible<Classe>());
        dnCb.setIdCible(m_idClasse);
        dnCb.setNum(donnee::EntreeNum);
        if(m_bdd.getUnique(dnCb))
            clEl.setEntree(dnCb.valeur().toDate());
        dnCb.setId(0);
        dnCb.setNum(donnee::SortieNum);
        if(m_bdd.getUnique(dnCb))
            clEl.setSortie(dnCb.valeur().toDate());
    }
}

void ClasseEleveCompositionTableau::setIdClasse(szt id) {
    m_idClasse = id;
    static_cast<EleveVecTableau&>(*m_tableaux.front())
            .setVecData(m_bdd.getList<Eleve,ClasseEleve>(ClasseEleve::IdEleve,ClasseEleve::IdClasse,m_idClasse));
    static_cast<ClasseEleveVecTableau&>(*m_tableaux.back()).setVecData(conteneurMPS::VectorPtr<ClasseEleve>(size()));
    for(szt ligne = 0; ligne != size(); ++ligne)
        hydrateClasseEleve(ligne);
}
