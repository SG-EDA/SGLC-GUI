#pragma once
#include "node.h"

class script
{
private:
    void runFile(vector<string> com);
    void equExp(string sen);
    void colonExp(string sen);
    void commandExp(string sen);
    nodeManager* manager;

public:
    map<string,line*> lineMap; //没有所有权
    script() { this->manager=new nodeManager; }
    ~script()
    {
        delete this->manager;
        lineMap.clear();
    }
    nodeManager* getManager() { return this->manager; }

    void clear();
    void eval(string sen);
    void evalAll(string code)
    {
        auto com=help::split(code,"\n");
        for(auto i : com)
            eval(i);
    }
};
