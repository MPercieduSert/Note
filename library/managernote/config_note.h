/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/11/2019
 */
#ifndef CONFIG_NOTE_H
#define CONFIG_NOTE_H

#include "config.h"

/*! \ingroup groupe_noyau
 * \brief Chemin XML du dossier de l'application par default.
 */
#define DEFAULT_DIRECTORY "conf/directories/default"

/*! \ingroup groupe_noyau
 * \brief Chemin XML de la base de donnée.
 */
#define DATA_BASE "conf/files/databases/database"

/*! \ingroup groupe_noyau
 * \brief Chemin XML de l'année par default.
 */
#define DEFAULT_ANNEE "conf/parametre/initialisation/anneeDefaut"

namespace note_mps {
/*! \ingroup groupe_managerInv
 * \brief Gestionnaire du fichier de configuration.
 *
 * Gestionnaire du fichier de configuration enregistré sous la forme d'un document XML. Tous les noeuds ont des chemins uniques.
 */
class config_note : public mps::fichier::config {
public:
    using config::config;

    //! Créer un fichier de configuration avec les entrées par défault.
    bool creer() override;

    //! Renvoie le chemin du dossier par default.
    QString default_directory() override;
};
}
#endif // CONFIG_NOTE_H
