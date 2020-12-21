#include "node.h"

void line::delayedConstruction(node *n, uint sub)
{
    this->n=n;
    this->sub=sub;
}

bool line::getIsEvaling()
{
    if(isConst)
        return false;
    else
        return this->n->isEvaling;
}

bool line::get()
{
    if(isConst)
        return constVal;
    else
        return n->eval()[sub];
}

void line::stru(uint tabNum)
{
    if(isConst)
    {
        cout<<getName();
        return;
    }
    else
        n->stru(tabNum);
}
