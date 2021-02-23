/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef SPINBOXANNEESCOLAIRE_H
#define SPINBOXANNEESCOLAIRE_H

#include <QMessageBox>
#include <QAbstractSpinBox>
#include <QLineEdit>
#include "EntityNote.h"
#include "conteneur_ptr.h"

namespace  noteMPS{
/*! \defgroup groupeWidgetNote Dialogues de note
 * \brief Ensemble des widgets de l'application note.
 */

/*! \ingroup groupeWidgetNote
 * \brief Widget de choix d'une Annees.
 */
class spin_box_anneeScolaire : public QAbstractSpinBox
{
    Q_OBJECT
protected:
    szt m_pos = 0;
    Annee m_value;          //!< Annee courante.
    vector_ptr<Annee> m_vec;   //!< Tableau d'année.

public:
    //! Constructeur.
    spin_box_anneeScolaire(QWidget * parent = nullptr);

    //! Constructeur.
    spin_box_anneeScolaire(const Annee & an, QWidget * parent = nullptr)
        : spin_box_anneeScolaire(parent)
        {set_value(an);}

    //! Constructeur.
    spin_box_anneeScolaire(const vector_ptr<Annee> & vec, QWidget * parent = nullptr)
        : spin_box_anneeScolaire(parent)
        {set_values(vec);}

    //! Constructeur.
    spin_box_anneeScolaire(vector_ptr<Annee> && vec, QWidget * parent = nullptr)
        : spin_box_anneeScolaire(parent)
        {set_values(std::move(vec));}

    //! Place la valeur sur l'annee actuelle si possible ou sur la dernière année de la liste sinon.
    void setNowValue();

    //! Mutateur de l'année courante (recherche par num (byNum=true) ou par id (byNum=false).
    void set_value(const Annee & annee, bool byNum = true);

    //! Mutateur de la liste des année disponible courante.
    void set_values(const vector_ptr<Annee> & vec)
        {m_vec = vec;}

    //! Mutateur de la liste des année disponible courante.
    void set_values(vector_ptr<Annee> && vec)
        {m_vec = std::move(vec);}

    //! Taille du widget.
    QSize sizeHint() const override;

    //! incrémente de steps fois la valeur.
    void stepBy(int steps) override;

    //! Accesseur de l'année courante.
    const Annee & value() const {
        return m_vec.empty() ? m_value
                              : m_vec.at(m_pos);
    }

    //! Accesseur de la liste des valeurs.
    const vector_ptr<Annee> & values() const
        {return m_vec;}

signals:
    //! La valeur afficher change.
    void valueChanged();

public slots:
    //! Affiche la valeur.
    void printValue();

protected:
    QAbstractSpinBox::StepEnabled stepEnabled() const override;
};
}
#endif // SPINBOXANNEESCOLAIRE_H
