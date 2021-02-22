#include "SpinBoxAnneeScolaire.h"

using namespace noteMPS;

SpinBoxAnneeScolaire::SpinBoxAnneeScolaire(QWidget * parent)
    :QAbstractSpinBox (parent) {
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    setButtonSymbols(QAbstractSpinBox::PlusMinus);
    printValue();
    auto fm = lineEdit()->fontMetrics();
    setMinimumSize(QSize(fm.averageCharWidth()*12,fm.height()));
}

void SpinBoxAnneeScolaire::printValue() {
    lineEdit()->set_text(QString::number(value().num()).append("-").append(QString::number(value().num()+1)));
    emit valueChanged();
}

void SpinBoxAnneeScolaire::setNowValue() {
    auto date = QDate::currentDate();
    Annee an;
    an.set_num(date.month()<=6 ? date.year() - 1
                              : date.year());
    set_value(an);
    if(!m_vec.empty() && m_value.is_new()){
        m_pos = static_cast<szt>(m_vec.size())-1;
        printValue();
    }
}

void SpinBoxAnneeScolaire::set_value(const Annee & annee, bool byNum){
    if(m_vec.empty()) {
        m_value = annee;
        m_value.set_id(0);
        printValue();
    }
    else {
        szt pos = 0;
        if(byNum)
            while(pos != m_vec.size() && annee.num() != m_vec.at(pos).num())
                ++pos;
        else
            while(pos != m_vec.size() && annee.id() != m_vec.at(pos).id())
                ++pos;
        if(pos != m_vec.size()){
            m_pos = pos;
            m_value = m_vec.at(pos);
            printValue();
        }
    }
}

QSize SpinBoxAnneeScolaire::sizeHint() const
    {return QSize(fontMetrics().size(Qt::TextSingleLine,text()));}

void SpinBoxAnneeScolaire::stepBy(int steps) {
    if(m_vec.empty()) {
        m_value.set_num(m_value.num() + steps);
        printValue();
    }
    else if (steps < 0 && static_cast<int>(m_pos) + steps >= 0) {
        m_pos -= static_cast<szt>(-steps);
        printValue();
    }
    else if (steps > 0 && m_pos + static_cast<szt>(steps) < m_vec.size()) {
        m_pos += static_cast<szt>(steps);
        printValue();
    }
}

QAbstractSpinBox::StepEnabled SpinBoxAnneeScolaire::stepEnabled() const
    {return (StepUpEnabled| StepDownEnabled);}
