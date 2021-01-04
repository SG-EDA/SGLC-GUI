#pragma once
#include <string>
using namespace std;

class Excep {
protected:
    string inform;
public:
    Excep(string inform) : inform(inform) {}
    string getInform() { return inform; }
};


class undefinedVariableExcep : public Excep {
public:
    undefinedVariableExcep() : Excep("Undefined variable") {}
};
