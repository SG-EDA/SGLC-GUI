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
    void recuTriTrue(uint sub = 0)
    {
        tri* t=(tri*)(allTri[sub]->g);
        t->setQ(0);

        if(sub==allTri.size()-1)
            run(true);
        else
            recuTriTrue(sub+1);

        t->setQ(1);

        if(sub==allTri.size()-1)
            run(true);
        else
            recuTriTrue(sub+1);
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

    void gateNum()
    {
        cout<<"Gate:"<<allNode.size()<<endl;
        map<string,int> num;
        for(node* i : allNode)
            num[i->g->getName()]+=1;
        for(auto i : num)
            cout<<i.first<<":"<<i.second<<endl;
    }

    void trueTable(uint sub = 0, bool staRecu=false)
    {
        auto conti=[&]()
        {
            if(sub==allInput.size()-1)
            {
                if(staRecu)
                    recuTriTrue();
                else
                    run();
            }
            else
                trueTable(sub+1,staRecu);
        };

        allInput[sub]->constVal=0;
        conti();
        allInput[sub]->constVal=1;
        conti();
    }

    void run(bool outputSta=false)
    {
        resetChunk();
        for(line* i : allInput)
            cout<<"["<<i->getName()<<"]"<<i->get()<<" ";

        if(outputSta)
        {
            cout<<" || ";
            for(uint i=0;i<allTri.size();i++)
            {
                tri* t=(tri*)(allTri[i]->g);
                cout<<"["<<i<<"]"<<t->getQ()<<" ";
            }
        }

        cout<<" -> ";
        for(uint i=0;i<allOutput.size();i++)
            cout<<"["<<i<<"]"<<allOutput[i]->get()<<" ";

        cout<<endl;
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

    void multiplexing()
    {
        uint lineNum=0;
        map<string,int>result;
        for(node* i : allNode)
        {
            for(line* j : i->inputLine)
                result[j->getName()]++;
        }
        for(auto i : result)
        {
            cout<<"["<<i.first<<"]"<<i.second<<"\t"; 
            lineNum+=i.second;
        }
        cout<<endl<<"lineNum:"<<lineNum<<endl;
    }
};

