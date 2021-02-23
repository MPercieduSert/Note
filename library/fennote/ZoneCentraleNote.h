/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef ZONECENTRALENOTE_H
#define ZONECENTRALENOTE_H

#include "TabNote.h"
#include "predef_zone_centrale.h"

namespace noteMPS {
/*! \ingroup groupe_fenNote
 * \brief Zone centrale de l'application note.
 */
class ZoneCentraleNote : public fmps::predef_zone_centrale
{
public:
    //! Constructeur.
    explicit ZoneCentraleNote(TabNote * tab, QWidget * parent = nullptr);
};
}
#endif // ZONECENTRALENOTE_H
