#include "BddNote.h"

using namespace noteMPS;

void BddNote::listeMiseAJourBdd(int version) {
    BddPredef::listeMiseAJourBdd(version);
    switch (version) {
    case bmps::bddVersion::initialePredef:
        creerTable<Annee>();
        creerTable<Bareme>();
        creerTable<TypeEtablissement>();
        creerTable<Etablissement>();
        creerTable<Niveau>();
        creerTable<Classe>();
        creerTable<Eleve>();
        creerTable<ClasseEleve>();
        creerTable<Groupe>();
        creerTable<ClasseGroupe>();
        creerTable<TypeControle>();
        creerTable<Controle>();
        creerTable<Enonce>();
        creerTable<Epreuve>();
        creerTable<ControleEpreuve>();
        creerTable<EleveGroupe>();
        creerTable<Point>();
        creerTable<EnoncePoint>();
        creerTable<EtablissementNiveau>();
        creerTable<FiliationNiveau>();
        creerTable<NiveauTypeEtablissement>();
        creerTable<Note>();
        creerTable<Valide>();

        m_manager->saveVersion(bmps::bddVersion::initialeNote);
    }
}
