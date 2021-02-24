/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef NOYAU_NOTE_H
#define NOYAU_NOTE_H

#include "bdd_note.h"
#include "config_note.h"
#include "abstract_noyau.h"

/*! \defgroup groupe_fenNote Fenêtre note
 * \brief Ensemble des fenêtre et du noyau de l'application note.
 */

namespace note_mps {
/*! \ingroup groupe_fenInv
 * \brief Noyau de l'application note.
 *
 * Noyau de l'application note:
 *
 */
class noyau_note : public mps::fenetre::abstract_noyau {
    Q_OBJECT
protected:
    annee m_annee;      //!< annee Courante.

public:
    //! Constructeur.
    noyau_note() {}

    //! Destructeur.
    ~noyau_note() override = default;

    //! Accesseur de l'année courante.
    const annee &get_annee() const noexcept
        {return m_annee;}

    //! Accesseur de la base de donnée.
    bdd_note &bdd() override
        {return static_cast<bdd_note &>(abstract_noyau::bdd());}

    //! Mutateur de l'année courante.
    void set_annee(const annee &an);// = get_annee());

    //!Ouvre le gestionnaire de configuration au chemin indiquer.
    void set_config_by_path(const QString & config_path, QWidget *modal_parent = nullptr) override;
};
}
#endif // noyau_note_H
