/*Auteur: PERCIE DU SERT Maxime
 *Date: 10/03/2021
 */
#ifndef ENUM_FEN_NOTE_H
#define ENUM_FEN_NOTE_H

#include "predef_tab.h"

namespace note_mps {
    namespace info_tab_note {
    enum tab_id {Classe_Eleve_Tab_Id = mps::fenetre::predef_tab::Nbr_Predef_Tab_Id,
                    Eleves_Tab_Id,
                    Edit_Exercice_Tab_Id,
                    Find_Exercice_Tab_Id,
                    Groupe_Tab_Id
                    };
    }
}

#endif // ENUM_FEN_NOTE_H
