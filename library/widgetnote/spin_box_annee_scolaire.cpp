#include "spin_box_annee_scolaire.h"

using namespace note_mps;

spin_box_annee_scolaire::spin_box_annee_scolaire(QWidget *parent)
    :QAbstractSpinBox (parent) {
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    setButtonSymbols(QAbstractSpinBox::PlusMinus);
    print_value();
    auto fm = lineEdit()->fontMetrics();
    setMinimumSize(QSize(fm.averageCharWidth()*12,fm.height()));
}

void spin_box_annee_scolaire::print_value() {
    lineEdit()->setText(QString::number(value().num()).append("-").append(QString::number(value().num()+1)));
    emit value_changed();
}

void spin_box_annee_scolaire::setNowValue() {
    auto date = QDate::currentDate();
    annee an;
    an.set_num(date.month()<=6 ? date.year() - 1
                              : date.year());
    set_value(an);
    if(!m_vec.empty() && m_value.is_new()){
        m_pos = static_cast<szt>(m_vec.size())-1;
        print_value();
    }
}

void spin_box_annee_scolaire::set_value(const annee & an, bool by_num){
    if(m_vec.empty()) {
        m_value = an;
        m_value.set_id(0);
        print_value();
    }
    else {
        szt pos = 0;
        if(by_num)
            while(pos != m_vec.size() && an.num() != m_vec.at(pos).num())
                ++pos;
        else
            while(pos != m_vec.size() && an.id() != m_vec.at(pos).id())
                ++pos;
        if(pos != m_vec.size()){
            m_pos = pos;
            m_value = m_vec.at(pos);
            print_value();
        }
    }
}

void spin_box_annee_scolaire::stepBy(int steps) {
    if(m_vec.empty()) {
        m_value.set_num(m_value.num() + steps);
        print_value();
    }
    else if (steps < 0 && static_cast<int>(m_pos) + steps >= 0) {
        m_pos -= static_cast<szt>(-steps);
        print_value();
    }
    else if (steps > 0 && m_pos + static_cast<szt>(steps) < m_vec.size()) {
        m_pos += static_cast<szt>(steps);
        print_value();
    }
}  
