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
    switch (cible) {
    case ExerciceEditModel::TexteCible:
        if(role == StringRole)
            return m_texte;
        break;
    case ExerciceEditModel::TitreCible:
        if(role == OrientationRole)
            return Qt::Horizontal;
        if(role == LabelRole)
            return "Titre :";
        if(role == StringRole)
            return m_titre;
        break;
    case ExerciceEditModel::VersionCible:
        if(role == NumRole)
            return m_version;
        break;
    case modelMPS::SubNodeCible:
        if(role == SubNodeRole) {
            QList<QVariant> init;
            switch (num) {
            case ExerciceEditModel::TextePosition:
                init.append(ExerciceEditModel::TexteCible);
                init.append(0);
                init.append(TexteEditSubNode);
                return init;
            case ExerciceEditModel::TitrePosition:
                init.append(ExerciceEditModel::TitreCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            case ExerciceEditModel::VersionPosition:
                init.append(ExerciceEditModel::VersionCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
        }
        break;
    case modelMPS::NodeCible:
        if(role == OrientationRole)
            return Qt::Vertical;
    }
    return ItemBddNode::data(cible, role, num);
}

numt ExerciceEditModel::dataCount(const NodeIndex & index) const {
    switch (index.cible()) {
    case ExerciceEditModel::TexteCible:
    case ExerciceEditModel::TitreCible:
    case ExerciceEditModel::VersionCible:
        return 1;
    case SubNodeCible:
        return NbrCible;
    }
    return ItemNodeModel::dataCount(index);
}

modelMPS::Node ExerciceEditModel::nodeFactory(const modelMPS::NodeIndex & /*parent*/, numt /*pos*/, int /*type*/)
    {return std::make_unique<ExerciceEditNode>(this);}

flag ExerciceNode::flags(int cible, numt num) const {
    if(m_iter.root())
        return NoFlagNode;
    return ItemBddNode::flags(cible,num);
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

