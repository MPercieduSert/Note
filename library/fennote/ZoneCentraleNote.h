/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef ZONECENTRALENOTE_H
#define ZONECENTRALENOTE_H

#include "TabNote.h"
#include "PredefZoneCentrale.h"

namespace noteMPS {
/*! \ingroup groupeFenNote
 * \brief Zone centrale de l'application note.
 */
class ZoneCentraleNote : public fmps::PredefZoneCentrale
{
public:
    //! Constructeur.
    explicit ZoneCentraleNote(TabNote * tab, QWidget * parent = nullptr);
};
}
#endif // ZONECENTRALENOTE_H
