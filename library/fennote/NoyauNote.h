/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef NOYAUNOTE_H
#define NOYAUNOTE_H

#include "BddNote.h"
#include "ConfigNote.h"
#include "abstract_noyau.h"

/*! \defgroup groupe_fenNote Fenêtre note
 * \brief Ensemble des fenêtre et du noyau de l'application note.
 */

namespace noteMPS {
/*! \ingroup groupe_fenInv
 * \brief Noyau de l'application note.
 *
 * Noyau de l'application note:
 *
 */
class NoyauNote : public fenetre::abstract_noyau {
    Q_OBJECT
protected:
    Annee m_annee;      //!< Annee Courante.

public:
    //! Constructeur.
    NoyauNote() {}

    //! Destructeur.
    ~NoyauNote() override = default;

    //! Accesseur de l'année courante.
    const Annee & annee() const noexcept
        {return m_annee;}

    //! Accesseur de la base de donnée.
    BddNote & bdd() override
        {return static_cast<BddNote &>(fenetre::abstract_noyau::bdd());}

    //! Mutateur de l'année courante.
    void setAnnee(const Annee & annee = Annee());

    //!Ouvre le gestionnaire de configuration au chemin indiquer.
    void set_config_by_path(const QString & config_path, QWidget * modal_parent = nullptr) override;
};
}
#endif // NOYAUNOTE_H
