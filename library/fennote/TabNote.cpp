#include "TabNote.h"

using namespace noteMPS;

fmps::abstract_tab_module * TabNote::create_tab(const tab_index& pair, const std::vector<QVariant> &args) const {
    if(pair.first < predef_tab::Nbr_Predef_Tab_Id)
        return predef_tab::create_tab(pair,args);
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
