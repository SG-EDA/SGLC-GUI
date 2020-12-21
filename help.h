#pragma once
#include "QString.h"
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

class help
{
public:
    static bool VerifyNumber(qstring str)
    {
        std::string temp = str.str;
        for (int i = 0; i < str.size(); i++)
        {
            if ((temp[i]<'0' || temp[i]>'9') && temp[i]!='.')
                return false;
        }
        return true;
    }

    static qstringList splitSpace(qstring stri)
    {
        stri=stri.replace("  "," ");
        return stri.split(" ");
    }

    static qstring getLastElm(qstring stri, qstring keyword)
    {
        qstringList strList=help::splitSpace(stri);
        for(int i=0;i<strList.size();i++)
        {
            if(strList[i]==keyword)
                return strList[i+1];
        }
    }

    static void tab(unsigned int num=0)
    {
        for(unsigned int i=0;i<num;i++)
            cout<<"   ";
    }

    static int toint(string str)
    {
        return atoi(str.c_str());
    }

    static string readTxt(string file)
    {
        ifstream infile;
        infile.open(file.data());   //将文件流对象与文件连接起来
        assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行

        string result;
        string s;
        while(getline(infile,s))
        {
            result+=s+"\n";
        }
        infile.close();             //关闭文件输入流
        return result;
    }

    static vector<string> split(string text, string qseperator)
    {
        qstring qtext(text);
        qstringList qlist=qtext.split(qseperator);
        vector<string> result;
        for(qstring i : qlist)
            result.push_back(i.str);
        return result;
    }
};
