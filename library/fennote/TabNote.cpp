#include "TabNote.h"

using namespace noteMPS;

fmps::AbstractTabModule * TabNote::createTab(std::pair<int,int> pair) const {
    if(pair.first < PredefTab::NbrTabId)
        return PredefTab::createTab(pair);
    else
        return nullptr;
}
