/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef zone_centrale_note_H
#define zone_centrale_note_H

#include "tab_note.h"
#include "predef_zone_centrale.h"

namespace note_mps {
/*! \ingroup groupe_fenNote
 * \brief Zone centrale de l'application note.
 */
class zone_centrale_note : public mps::fenetre::predef_zone_centrale
{
public:
    //! Constructeur.
    explicit zone_centrale_note(tab_note * tab, QWidget *parent = nullptr);
};
}
#endif // zone_centrale_note_H
