#include "entity_note.h"

using namespace note_mps;

DESTR_VIDE_DEF(annee)
DESTR_VIDE_DEF(classe)
DESTR_VIDE_DEF(classe_eleve)
DESTR_VIDE_DEF(controle)
DESTR_VIDE_DEF(controle_epreuve)
DESTR_VIDE_DEF(controle_epreuve_attribut)
DESTR_VIDE_DEF(eleve)
DESTR_VIDE_DEF(eleve_groupe)
DESTR_VIDE_DEF(enonce_point)
DESTR_VIDE_DEF(ens_point_attribut)
DESTR_VIDE_DEF(entree_attribut)
DESTR_VIDE_DEF(epreuve)
DESTR_VIDE_DEF(etablissement_type)
DESTR_VIDE_DEF(etablissement_niveau)
DESTR_VIDE_DEF(exercice)
DESTR_VIDE_DEF(fille_attribut)
DESTR_VIDE_DEF(filiation_niveau)
DESTR_VIDE_DEF(groupe)
DESTR_VIDE_DEF(minima_attribut)
DESTR_VIDE_DEF(niveau_type_etablissement)
DESTR_VIDE_DEF(num_ce_attribut)
DESTR_VIDE_DEF(note)
DESTR_VIDE_DEF(prenom_attribut)
DESTR_VIDE_DEF(sortie_attribut)
DESTR_VIDE_DEF(type_controle)
DESTR_VIDE_DEF(valide)

bool eleve::operator < (const eleve & eleve) const {
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
