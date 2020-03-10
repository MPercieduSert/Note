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
    lineEdit()->setText(QString::number(value().num()).append("-").append(QString::number(value().num()+1)));
    emit valueChanged();
}

QSize SpinBoxAnneeScolaire::sizeHint() const {
    return QSize(fontMetrics().size(Qt::TextSingleLine,text()));
}

void SpinBoxAnneeScolaire::stepBy(int steps) {
    if(m_vec.empty()) {
        m_value.setNum(m_value.num() + steps);
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
