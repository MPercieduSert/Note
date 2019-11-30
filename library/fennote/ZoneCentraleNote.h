/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef ZONECENTRALENOTE_H
#define ZONECENTRALENOTE_H

#include "TabNote.h"
#include "AbstractTabZoneCentrale.h"

namespace noteMPS {
/*! \ingroup groupeFenNote
 * \brief Zone centrale de l'application note.
 */
class ZoneCentraleNote : public fmps::AbstractTabZoneCentrale
{
public:
    //! Constructeur.
    explicit ZoneCentraleNote(TabNote * tab = new TabNote, QWidget * parent = nullptr);
};
}
#endif // ZONECENTRALENOTE_H
