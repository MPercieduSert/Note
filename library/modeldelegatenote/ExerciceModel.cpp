#include "ExerciceModel.h"

using namespace modelMPS;
using namespace noteMPS;

ExerciceEditModel::ExerciceEditModel(idt idRacineExo, BddNote & bdd, QObject * parent)
    : ItemNodeBddModel(bdd,parent) {
    if(idRacineExo == IdNew){
        m_data.setRoot(std::make_unique<ExerciceEditNode>(this));
        m_data.push_back(index(NodeIndex(),0), std::make_unique<ExerciceEditNode>(this));
    }
    else {

    }
}

QVariant ExerciceNode::data(int cible, int role, numt num) const {
    switch (role) {
    case ExerciceEditModel::TexteCible:
        if(role == StringRole)
            return m_texte;
        break;
    case ExerciceEditModel::TitreCible:
        if(role == StringRole)
            return m_titre;
        break;
    case ExerciceEditModel::VersionCible:
        if(role == NumRole)
            return m_version;
    }
    return ItemBddNode::data(cible, role, num);
}

flag ExerciceNode::setData(int cible, const QVariant & value, int role, numt num) {
    switch (role) {
    case ExerciceEditModel::TexteCible:
        if(role == StringRole){
            m_texte = value.toString();
            return StringRole;
        }

        break;
    case ExerciceEditModel::TitreCible:
        if(role == modelMPS::StringRole) {
            m_titre = value.toString();
            return StringRole;
        }
        break;
    case ExerciceEditModel::VersionCible:
        if(role == modelMPS::NumRole) {
            m_version = value.toUInt();
            return NumRole;
        }
    }
    return ItemBddNode::setData(cible, value, role, num);
}
