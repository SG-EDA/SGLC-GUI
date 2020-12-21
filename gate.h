#pragma once
#include <vector>
#include <list>
#include <string>
#include <iostream>
using namespace std;

typedef vector<bool> blist;
typedef unsigned int uint;

class gate
{
protected:
    string name;
    uint inputNum;
    uint outputNum=1;
    bool isTri=false;

public:
    virtual blist realCalu(blist par)=0;
    gate(string name,uint inputNum, uint outputNum=1) :
        name(name), inputNum(inputNum), outputNum(outputNum) {}
    uint getInputNum() { return inputNum; }
    uint getOutputNum() { return outputNum; }
    string getName() { return this->name; }
    bool getIsTri() { return isTri; }

    blist calu(blist par)
    {
        if(this->inputNum!=par.size())
            throw string("The number of parameters does not match");
        else
            return realCalu(par);
    }

    virtual ~gate() {}
};


class andGate : public gate
{
public:
    andGate() : gate("and",2) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        result.push_back(par[0]&&par[1]);
        return result;
    }
};


class and4Gate : public gate
{
public:
    and4Gate() : gate("and4",4) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        result.push_back(par[0]&&par[1]&&par[2]&&par[3]);
        return result;
    }
};


class orGate : public gate
{
public:
    orGate() : gate("or",2) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        result.push_back(par[0]||par[1]);
        return result;
    }
};


class or4Gate : public gate
{
public:
    or4Gate() : gate("or4",4) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        result.push_back(par[0]||par[1]||par[2]||par[3]);
        return result;
    }
};


class notGate : public gate
{
public:
    notGate() : gate("not",1) {}

    virtual blist realCalu(blist par)
    {
        par[0]=!par[0];
        return par;
    }
};


class NAGate : public gate
{
public:
    NAGate() : gate("NA",2) {}

    virtual blist realCalu(blist par)
    {
        andGate g1;
        notGate g2;
        par=g1.calu(par);
        return g2.calu(par);
    }
};


class NA4Gate : public gate
{
public:
    NA4Gate() : gate("NA4",4) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        result.push_back(!(par[0]&&par[1]&&par[2]&&par[3]));
        return result;
    }
};


class NOGate : public gate
{
public:
    NOGate() : gate("NO",2) {}

    virtual blist realCalu(blist par)
    {
        orGate g1;
        notGate g2;
        par=g1.calu(par);
        return g2.calu(par);
    }
};

class NOAGate : public gate
{
public:
    NOAGate() : gate("NOA",4) {}

    virtual blist realCalu(blist par)
    {
        andGate a1;
        andGate a2;
        orGate g1;
        notGate g2;

        blist par1;
        par1.push_back(par[0]);
        par1.push_back(par[1]);
        blist par2;
        par2.push_back(par[2]);
        par2.push_back(par[3]);

        //长度都为1
        par1=a1.calu(par1);
        par2=a2.calu(par2);

        blist r;
        r.push_back(par1[0]);
        r.push_back(par2[0]);

        r=g1.calu(r);
        return g2.calu(r);
    }
};

class xorGate : public gate
{
public:
    xorGate() : gate("xor",2) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        if(par[0]==par[1])
            result.push_back(1);
        else
            result.push_back(0);
        return result;
    }
};

class tri : public gate
{
protected:
    blist stat;

public:
    tri(string name,uint inputNum, uint outputNum=1) : gate(name,inputNum,outputNum) { isTri=true; }

    const static uint Q=0;
    virtual void setQ(bool q) { stat[Q]=q; }
    virtual bool getQ() { return stat[Q]; }
    blist getStat() { return stat; }
};

class RSTri : public tri
{
public:
    RSTri() : tri("RS",2,2)
    {
        stat.push_back(0);
        stat.push_back(1);
    }
    virtual void setQ(bool q)
    {
        stat[Q]=q;
        stat[Q2]=!q;
    }

    const uint R=0;
    const uint S=1;

    const uint Q2=1;

    virtual blist realCalu(blist par) //第一个是r第二个是s。返回值也是对应，第一个Q2第二个Q
    {

        if(par[R]==0&&par[S]==0) {} //结果不确定
        else if(par[R]==0&&par[S]==1)
        {
            setQ(0);
        }
        else if(par[R]==1&&par[S]==0)
        {
            setQ(1);
        }
        return stat;
    }
};

class RSCTri : public tri
{
private:
    RSTri rstg;

public:
    RSCTri() : tri("RSC",3,2) {}
    virtual void setQ(bool q) { rstg.setQ(q); }
    virtual bool getQ() { return rstg.getQ(); }

    const uint R=0;
    const uint S=1;
    const uint CP=2;

    virtual blist realCalu(blist par)
    {
        blist r;
        if(par[CP]==1)
        {
            //按书上电路，连的是与非门，所以这里取非
            r.push_back(!par[R]);
            r.push_back(!par[S]);
        }
        else
        {
            r.push_back(0);
            r.push_back(0);
        }
        return rstg.calu(r);
    }
};

class DTri : public tri
{
public:
    DTri() : tri("D",2)
    {
        stat.push_back(0);
    }

    const uint D=0;
    const uint CP=1;

    virtual blist realCalu(blist par)
    {
        if(par[CP]==1)
            stat[D]=par[D];
        return stat;
    }
};

class JKTri : public tri
{
public:
    JKTri() : tri("JK",3,2)
    {
        stat.push_back(0);
        stat.push_back(1);
    }
    virtual void setQ(bool q)
    {
        stat[Q]=q;
        stat[Q2]=!q;
    }

    const uint J=0;
    const uint K=1;
    const uint CP=2;

    const uint Q2=1;

    virtual blist realCalu(blist par)
    {
        if(par[CP]==1)
        {
            if(par[J]==0&&par[K]==1)
            {
                setQ(0);
            }
            else if(par[J]==1&&par[K]==0)
            {
                setQ(1);
            }
            else if(par[J]==1&&par[K]==1)
            {
                setQ(!stat[Q]);
            }
        }
        return stat;
    }
};

class TTri : public tri
{
public:
    TTri() : tri("T",2,2)
    {
        stat.push_back(0);
        stat.push_back(1);
    }
    virtual void setQ(bool q)
    {
        stat[Q]=q;
        stat[Q2]=!q;
    }
	
	const uint T=0;
    const uint CP=1;
	
    const uint Q2=1;
	
	virtual blist realCalu(blist par)
    {
        if(par[CP]==1)
        {
            if(par[T]==1)
            {
                setQ(!stat[Q]);
            }
        }
        return stat;
    }
};
