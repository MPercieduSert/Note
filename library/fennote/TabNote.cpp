#include "TabNote.h"

using namespace noteMPS;

fmps::AbstractTabModule * TabNote::createTab(const std::pair<int,int>& pair, const std::vector<QVariant> &args) const {
    if(pair.first < PredefTab::NbrPredefTabId)
        return PredefTab::createTab(pair,args);
    else
        switch (pair.first) {
        case ClasseEleveTabId:
            return new ClasseEleveTab(bdd(),pair);
        case ElevesTabId:
            return new ElevesTab(bdd(),pair);
        case ExerciceTabId:
            return new ExerciceTab(bdd(),pair);
        case GroupeTabId:
            return new GroupeTab(bdd(),pair);
        default:
            return nullptr;
        }

}
