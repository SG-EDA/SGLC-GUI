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

pos line::stru(QGraphicsScene* scene,uint tabNum,uint tabNum1)
{
    if(isConst)
    {
        cout<<getName();
    }
    else
       return n->stru(scene,tabNum,tabNum1);
}
