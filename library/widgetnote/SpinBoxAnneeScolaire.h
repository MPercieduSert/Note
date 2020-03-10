/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef SPINBOXANNEESCOLAIRE_H
#define SPINBOXANNEESCOLAIRE_H

#include <QMessageBox>
#include <QAbstractSpinBox>
#include <QLineEdit>
#include "EntityNote.h"
#include "ConteneurPtr.h"

namespace  noteMPS{
/*! \defgroup groupeWidgetNote Dialogues de note
 * \brief Ensemble des widgets de l'application note.
 */

/*! \ingroup groupeWidgetNote
 * \brief Formulaire de choix d'une Annees.
 */
class SpinBoxAnneeScolaire : public QAbstractSpinBox
{
    Q_OBJECT
protected:
    szt m_pos = 0;
    Annee m_value;          //!< Annee courante.
    conteneurMPS::VectorPtr<Annee> m_vec;   //!< Tableau d'année.

public:
    //! Constructeur.
    SpinBoxAnneeScolaire(QWidget * parent = nullptr);

    //! Constructeur.
    SpinBoxAnneeScolaire(const Annee & an, QWidget * parent = nullptr)
        : SpinBoxAnneeScolaire(parent)
        {setValue(an);}

    //! Constructeur.
    SpinBoxAnneeScolaire(const conteneurMPS::VectorPtr<Annee> & vec, QWidget * parent = nullptr)
        : SpinBoxAnneeScolaire(parent)
        {setValues(vec);}

    //! Constructeur.
    SpinBoxAnneeScolaire(conteneurMPS::VectorPtr<Annee> && vec, QWidget * parent = nullptr)
        : SpinBoxAnneeScolaire(parent)
        {setValues(std::move(vec));}

    //! Mutateur de l'année courante.
    void setValue(const Annee & annee){
        if(m_vec.empty()) {
            m_value = annee;
            m_value.setId(0);
            printValue();
        }
        else {
            szt pos = 0;
            while(pos != m_vec.size() && annee.num() != m_vec.at(pos).num())
                ++pos;
            if(pos != m_vec.size()){
                m_pos = pos;
                printValue();
            }
        }
    }

    //! Mutateur de la liste des année disponible courante.
    void setValues(const conteneurMPS::VectorPtr<Annee> & vec)
        {m_vec = vec;}

    //! Mutateur de la liste des année disponible courante.
    void setValues(conteneurMPS::VectorPtr<Annee> && vec)
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
    const conteneurMPS::VectorPtr<Annee> & values() const
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
