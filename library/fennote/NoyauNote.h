/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef NOYAUNOTE_H
#define NOYAUNOTE_H

#include "BddNote.h"
#include "ConfigNote.h"
#include "AbstractNoyau.h"

/*! \defgroup groupeFenNote Fenêtre note
 * \brief Ensemble des fenêtre et du noyau de l'application note.
 */

namespace noteMPS {
/*! \ingroup groupeFenInv
 * \brief Noyau de l'application note.
 *
 * Noyau de l'application note:
 *
 */
class NoyauNote : public fenMPS::AbstractNoyau {
public:
    NoyauNote()
        : AbstractNoyau() {}

    //!Ouvre le gestionnaire de configuration au chemin indiquer.
    void setConfigByPath(const QString & configPath, QWidget * modalParent = nullptr) override;
};
}
#endif // NOYAUNOTE_H
