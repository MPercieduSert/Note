#include "EntityNote.h"

using namespace noteMPS;

DESTR_VIDE_DEF(Annee)
DESTR_VIDE_DEF(Classe)
DESTR_VIDE_DEF(ClasseEleve)
DESTR_VIDE_DEF(Controle)
DESTR_VIDE_DEF(ControleEpreuve)
DESTR_VIDE_DEF(ControleEpreuveAttribut)
DESTR_VIDE_DEF(Eleve)
DESTR_VIDE_DEF(EleveGroupe)
DESTR_VIDE_DEF(EnoncePoint)
DESTR_VIDE_DEF(EnsPointAttribut)
DESTR_VIDE_DEF(EntreeAttribut)
DESTR_VIDE_DEF(Epreuve)
DESTR_VIDE_DEF(EtablissementType)
DESTR_VIDE_DEF(EtablissementNiveau)
DESTR_VIDE_DEF(FilleAttribut)
DESTR_VIDE_DEF(FiliationNiveau)
DESTR_VIDE_DEF(Groupe)
DESTR_VIDE_DEF(MinimaAttribut)
DESTR_VIDE_DEF(NiveauTypeEtablissement)
DESTR_VIDE_DEF(NumCEAttribut)
DESTR_VIDE_DEF(Note)
DESTR_VIDE_DEF(PrenomAttribut)
DESTR_VIDE_DEF(SortieAttribut)
DESTR_VIDE_DEF(TypeControle)
DESTR_VIDE_DEF(Valide)

bool Eleve::operator < (const Eleve & eleve) const {
    if(nom() < eleve.nom())
        return true;
    if(nom() == eleve.nom()){
        if(prenom() < eleve.prenom())
            return true;
        else if (prenom() == eleve.prenom() && date() < eleve.date())
            return true;
    }
    return false;

}
