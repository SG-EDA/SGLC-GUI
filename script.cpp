#include "script.h"

void script::clear()
{
    delete this->manager;
    lineMap.clear();
    this->manager=new nodeManager;
    line* trueLine=new line("1",true);
    manager->addLine(trueLine);
    lineMap["1"]=trueLine;
    line* falseLine=new line("0",false);
    lineMap["0"]=falseLine;
    manager->addLine(falseLine);
}

void script::runFile(vector<string> com)
{
    this->clear();
    string path;
    if(com.size()==3)
       path=com[1]+":"+com[2];
    else
        path=com[1];

    string code=help::readTxt(path);
    cout<<code<<endl;
    cout<<"----- load:"<<path<<" -----"<<endl<<endl;
    script::evalAll(code);
    cout<<endl<<"-----end-----"<<endl;
}


void script::equExp(string sen)
{
    vector<string> com=help::split(sen,"=");
    vector<string> com2=help::split(com[1]," ");

    //先根据com2创建门电路
    gate *g;
    //确认门类型，实例化对象
    if(com2[0]=="and")
        g=new andGate();
    else if(com2[0]=="and4")
        g=new and4Gate();
    else if(com2[0]=="or")
        g=new orGate();
    else if(com2[0]=="or4")
        g=new or4Gate();
    else if(com2[0]=="not")
        g=new notGate();
    else if(com2[0]=="NA")
        g=new NAGate();
    else if(com2[0]=="NA4")
        g=new NA4Gate();
    else if(com2[0]=="NO")
        g=new NOGate();
    else if(com2[0]=="NOA")
        g=new NOAGate();
    else if(com2[0]=="xor")
        g=new xorGate();
    else if(com2[0]=="RS")
        g=new RSTri();
    else if(com2[0]=="RSC")
        g=new RSCTri();
    else if(com2[0]=="D")
        g=new DTri();
    else if(com2[0]=="JK")
        g=new JKTri();
    else if(com2[0]=="T")
        g=new TTri();
    else
        throw string("Unknown gate type");

    node *n=new node(g); //通过门对象初始化节点
    manager->addNode(n);
    for(uint i=1;i<com2.size();i++)
    {
        if(lineMap[com2[i]]==0) //目前这个输入线还没有被定义
        {
            line* newLine=new line(com2[i],nullptr); //先构造上对象本体，其它的等定义的时候再延迟构造
            lineMap[com2[i]]=newLine;
            manager->addLine(newLine);
            n->addInputLine(newLine);
        }
        else
            n->addInputLine(lineMap[com2[i]]); //设置节点输入
    }

    //创建该节点的输出导线
    vector<string> com3=help::split(com[0],",");
    for(uint i=0;i<com3.size();i++)
    {
        if(com3[i]=="_")
            continue;
        else
        {
           if(lineMap[com3[i]]==0) //目前这个输出线还没有被定义
           {
               line* newLine=new line(com3[i],n,i); //正常定义
               lineMap[com3[i]]=newLine;
               manager->addLine(newLine);
           }
           else
               lineMap[com3[i]]->delayedConstruction(n,i); //延迟构造
        }
    }
}

void script::colonExp(string sen)
{
    vector<string> com=help::split(sen,":");

    if(com[0]=="input")
    {
        line* newline=new line(com[1]);
        lineMap[com[1]]=newline;
        manager->addLine(newline);
        manager->addInputLine(newline);
    }
    else if(com[0]=="output")
        manager->addOutputLine(lineMap[com[1]]);
    else if(com[0]=="set")
    {
        vector<string> com2=help::split(com[1]," ");
        lineMap[com2[0]]->constVal=help::toint(com2[1]);
    }
    else if(com[0]=="load")
    {
        runFile(com);
    }
    else
        throw string("Unexpected line markup");
}

void script::commandExp(string sen)
{
    if(sen=="trueTable")
        manager->trueTable();
    else if(sen=="statTable")
        manager->trueTable(0,true);
    else if(sen=="gateNum")
        manager->gateNum();
    else if(sen=="run")
        manager->run();
    else if(sen=="stru")
        manager->stru();
    else if(sen=="clear")
        this->clear();
    else if(sen=="middleVar")
        manager->middleVar();
    else if(sen=="multiplexing")
        manager->multiplexing();
    else if(sen=="resetTri")
        manager->resetTri();
    else
        throw string("Unknow command");
}

void script::eval(string sen)
{
    if(sen=="")
        return;
    else if(sen.find(":")!=-1)
        colonExp(sen);
    else if(sen.find("=")!=-1)
        equExp(sen);
    else
        commandExp(sen);
}
