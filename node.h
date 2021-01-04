#pragma once
#include "gate.h"
#include "help.h"
#include <map>

class node;

class line
{
private:
    string name;
    uint sub;
    bool isConst=false;

public:
    node *n;
    bool constVal;

    line(string name, node *n,uint sub=0) : name(name), n(n), sub(sub) {}
    void delayedConstruction(node *n,uint sub=0);
    line(string name,bool constVal=0) : name(name), isConst(true), constVal(constVal) {}
    bool get();
    string getName() { return this->name; }
    bool getIsEvaling();
    void stru(uint tabNum=0);
};


class node
{
private:
    blist getInputPar()
    {
        blist par;
        for(line* i : inputLine)
        {
            if(i->getIsEvaling()) //检查是否有环形
            {
                 gate* iTri=i->n->g;
                 if(iTri->getIsTri())
                 {
                     tri* t=(tri*)iTri;
                     par.push_back(t->getStat()[tri::Q]);
                 }
                 else
                     throw string("Illegal Ring Circuit");
            }
            else
                par.push_back(i->get());
        }
        return par;
    }

public:
    vector<line*> inputLine; //与g的input个数相等。每个连接需指明插槽位
    bool isEvaled=false;
    bool isEvaling=false;
    blist result;
    gate *g;

    node(gate* g) : g(g) {}
    ~node() { delete g; }
    void addInputLine(line* l) { inputLine.push_back(l); }

    blist eval()
    {
        isEvaling=true;
        if(isEvaled==false)
        {
            this->result=g->calu(getInputPar());
            this->isEvaled=true;
        }
        isEvaling=false;
        return this->result;
    }

    void stru(uint tabNum=0)
    {
        cout<<g->getName()<<"-";
        for(uint i=0;i<inputLine.size();i++)
        {
            inputLine[i]->stru(tabNum+1);
            if(i!=inputLine.size()-1)
            {
                cout<<endl;
                help::tab(tabNum+1);
            }
        }
        if(tabNum==0)
            cout<<endl;
    }
};


class nodeManager
{
private:
    list<node*> allNode;
    list<line*> allLine;
    vector<line*> allInput;
    vector<line*> allOutput;
    vector<node*> allTri;

    string recuTriTrue(uint sub = 0)
    {
        string result;

        tri* t=(tri*)(allTri[sub]->g);
        t->setQ(0);

        if(sub==allTri.size()-1) //产生一个所有触发器值的组合之后再run
            result=run(true)+result;
        else
            result=recuTriTrue(sub+1)+result;

        t->setQ(1);

        if(sub==allTri.size()-1)
            result=run(true)+result;
        else
            result=recuTriTrue(sub+1)+result;

        return result;
    }

    void resetChunk()
    {
        for(node* i : allNode)
            i->isEvaled=false;
    }

    void addTri(node* n) { allTri.push_back(n); }

public:
    ~nodeManager()
    {
        for(line* i : allLine)
            delete i;
        for(node* i : allNode)
            delete i;
        allNode.clear();
        allLine.clear();
        allInput.clear();
        allOutput.clear();
        allTri.clear();
    }

    void addNode(node* n)
    {
        allNode.push_back(n);
        if(n->g->getIsTri())
            this->addTri(n);
    }
    void addLine(line* n) { allLine.push_back(n); }
    void addInputLine(line* n) { allInput.push_back(n); }
    void addOutputLine(line* n) { allOutput.push_back(n); }

    string gateNum()
    {
        string result;
        result+="Gate:"+to_string(allNode.size())+"\n";
        map<string,int> num;
        for(node* i : allNode)
            num[i->g->getName()]+=1;
        for(auto i : num)
            result+=i.first+":"+to_string(i.second)+"\n";
        return result;
    }

    string trueTable(uint sub = 0, bool staRecu=false)
    {
        if(staRecu && this->allTri.empty())
            return "There is no trigger in this circuit\n";

        string result;

        auto conti=[&]()
        {
            if(sub==allInput.size()-1)
            {
                if(staRecu)
                    result+=recuTriTrue();
                else
                    result+=run();
            }
            else
                result+=trueTable(sub+1,staRecu);
        };

        allInput[sub]->constVal=0;
        conti();
        allInput[sub]->constVal=1;
        conti();

        return result;
    }

    string run(bool outputSta=false)
    {
        string result;

        resetChunk();
        for(line* i : allInput)
            result+="["+i->getName()+"]"+to_string(i->get())+" ";

        if(outputSta)
        {
            result+=" || ";
            for(uint i=0;i<allTri.size();i++)
            {
                tri* t=(tri*)(allTri[i]->g);
                result+="["+to_string(i)+"]"+to_string(t->getQ())+" ";
            }
        }

        result+=" -> ";
        for(uint i=0;i<allOutput.size();i++)
            result+="["+to_string(i)+"]"+to_string(allOutput[i]->get())+" ";

        result+="\n";
        return result;
    }

    void resetTri()
    {
        for(node* i : allTri)
        {
            tri* t=(tri*)(i->g);
            t->setQ(0);
        }
    }

    void stru()
    {
        for(line* i : allOutput)
            i->stru();
    }

    void middleVar()
    {
        for(line* i : allLine)
            cout<<"["<<i->getName()<<"]"<<i->get()<<"\t";
        cout<<endl;
    }

    map<line*,int> getLineMultiplexing()
    {
        map<line*,int> result;
        for(node* i : allNode)
        {
            for(line* j : i->inputLine)
                result[j]++;
        }
        return result;
    }

    map<node*,int> getNodeMultiplexing()
    {
        map<node*,int> result;
        for(line* i : allLine) //allLine记录的是输出
            result[i->n]++; //看看它是从哪个元件连出来的，给那个元件加

        for(node* i : allNode)
            result[i]+=i->inputLine.size(); //inputLine记录的是输入，有几个输入加几

        return result;
    }

    string multiplexing()
    {
        string strResult;

        uint lineNum=0;
        for(auto i : this->getLineMultiplexing())
        {
            strResult+="["+i.first->getName()+"]"+to_string(i.second)+"\t";
            lineNum+=i.second;
        }
        strResult+="\nlineNum:"+to_string(lineNum)+"\n";

        return strResult;
    }
};

