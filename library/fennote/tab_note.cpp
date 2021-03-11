#include "tab_note.h"

using namespace note_mps;

mps::fenetre::abstract_tab_module * tab_note::create_tab(const tab_index& pair, const std::vector<QVariant> &args) const {
    if(pair.first < predef_tab::Nbr_Predef_Tab_Id)
        return predef_tab::create_tab(pair,args);
    else
        switch (pair.first) {
        case info_tab_note::Classe_Eleve_Tab_Id:
            return new classe_eleve_tab(bdd(),pair);
        case info_tab_note::Eleves_Tab_Id:
            return new eleves_tab(bdd(),pair);
        case info_tab_note::Edit_Exercice_Tab_Id:
            return new edit_exercice_tab(bdd(),pair);
        case info_tab_note::Find_Exercice_Tab_Id:
            return new find_exercice_tab(bdd(),pair);
        case info_tab_note::Groupe_Tab_Id:
            return new groupe_tab(bdd(),pair);
        default:
            return nullptr;
        }

}
