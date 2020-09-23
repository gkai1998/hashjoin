#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include<benchmark/benchmark.h>
using namespace std;
vector<vector<int>> readDataFromFile(string path)
{
    fstream read;
    read.open(path);
    cout << "reading the file:" << endl;
    //int data;
    int const LINE_LENGTH = 50;
    char str[LINE_LENGTH];
    vector<vector<int>> data;
    while (read.getline(str, LINE_LENGTH))
    {
        const char *patten = ",";
        char *buf;
        char *save_ptr = NULL;
        buf = strtok_r(str, patten, &save_ptr);
        vector<int> temp;
        while (buf)
        {
            string s = buf;
            temp.push_back(stoi(s));
            buf = strtok_r(NULL, patten, &save_ptr);
        }
        data.push_back(temp);
    }
    read.close();
    return data;
}
void printData(vector<vector<int>> data)
{
    for (vector<vector<int>>::iterator it = data.begin(); it != data.end(); it++)
    {
        for (vector<int>::iterator iti = (*it).begin(); iti != (*it).end(); iti++)
        {
            cout << *iti << " ";
        }
        cout << "" << endl;
    }
}

string vectorToString(vector<int> v)
{
    string String = "";
    for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
    {
        String += to_string(*it);
    }
    return String;
}

vector<int> getDataByIndex(vector<int> &data, vector<int> &index)
{
    vector<int> ret;
    for (vector<int>::iterator it = index.begin(); it != index.end(); it++)
    {
        ret.insert(ret.end(), data[*it]);
    }
    return ret;
}
vector<int> removeOffsetData(vector<int> data, vector<int> &offset)
{
    for (vector<int>::iterator it = offset.begin(); it != offset.end(); it++)
    {
        data.erase(data.begin() + (*it));
    }
    return data;
}
vector<vector<int>> hashJoin(vector<vector<int>> &data1, vector<vector<int>> &data2, vector<int> &offset1, vector<int> &offset2)
{
    // if (index1 >= data1[0].size() || index2 >= data2[0].size())
    // {
    // cout << "索引越界" << endl;
    // return;
    // }

    vector<vector<int>> &bigtable = data1;
    vector<vector<int>> &smalltable = data2;
    vector<int> &bigindex = offset1;
    vector<int> &smallindex = offset2;
    if (data1.size() < data2.size())
    {
        bigtable = data2;
        bigindex = offset2;
        smalltable = data1;
        smallindex = offset1;
    }
    unordered_map<string, vector<vector<int>>> hashtable;
    for (vector<vector<int>>::iterator it = smalltable.begin(); it != smalltable.end(); it++)
    {
        string String = vectorToString(getDataByIndex(*it, smallindex));
        unordered_map<string, vector<vector<int>>>::iterator mapit = hashtable.find(String);
        if (mapit == hashtable.end())
        {
            hashtable.insert(pair<string, vector<vector<int>>>(String, {*it}));
        }
        else
        {
            (*mapit).second.push_back(*it);
        }
        //hashtable.insert(pair<int, vector<int>>((*it)[smallindex], *it));
    }
    vector<vector<int>> jointable;
    unordered_map<string, vector<vector<int>>>::iterator mapit;
    for (vector<vector<int>>::iterator it = bigtable.begin(); it != bigtable.end(); it++)
    {
        string String = vectorToString(getDataByIndex(*it, bigindex));
        mapit = hashtable.find(String);
        if (mapit != hashtable.end())
        {
            vector<int> temp;
            //(*it).erase((*it).begin() + bigindex);
            //temp.insert(temp.end(), (mapit->second).begin(), (mapit->second).end());
            //temp.insert(temp.end(), (*it).begin(), (*it).end());
            for (vector<vector<int>>::iterator itii = (*mapit).second.begin(); itii != (*mapit).second.end(); itii++)
            {
                temp.insert(temp.end(), (*itii).begin(), (*itii).end());
                vector<int> removedata=removeOffsetData((*it), bigindex);
                temp.insert(temp.end(), removedata.begin(), removedata.end());
                jointable.push_back(temp);
            }
        }
    }
    return jointable;
}
void storeToFile(vector<vector<int>> data, string path)
{
    ofstream outfile(path);
    if (outfile)
    {
        for (vector<vector<int>>::iterator it = data.begin(); it != data.end(); it++)
        {
            vector<int>::iterator iti;
            for (iti = (*it).begin(); iti != ((*it).end() - 1); iti++)
            {
                outfile << (*iti) << ",";
            }
            outfile << (*iti);
            outfile << endl;
        }
    }
    outfile.close();
}

static void joinr0r1(benchmark::State& state){
    string r0 = "./data/r0.tbl";
    string r1 = "./data/r1.tbl";
    vector<vector<int>> data0 = readDataFromFile(r0);
    vector<vector<int>> data1 = readDataFromFile(r1);
    vector<int> offset1 = {0};
    vector<int> offset0 = {0};
    for (auto _: state) {
        vector<vector<int>> jointable = hashJoin(data0, data1, offset0, offset1);
        storeToFile(jointable, "./data/r1r2.tbl");
    }
    data0.clear();
    data1.clear();
}

BENCHMARK(joinr0r1);
BENCHMARK_MAIN();





/*
int main()
{
    string r0 = "./data/r0.tbl";
    string r1 = "./data/r1.tbl";
    string r2 = "./data/r2.tbl";
    vector<vector<int>> data0 = readDataFromFile(r0);
    vector<vector<int>> data1 = readDataFromFile(r1);
    vector<vector<int>> data2 = readDataFromFile(r2);
    //printdata(data);
    vector<int> offset1 = {0};
    vector<int> offset2 = {0};
    vector<vector<int>> jointable = hashJoin(data1, data2, offset1, offset2);
    storeToFile(jointable, "./data/r1r2.tbl");
    data0.clear();
    data1.clear();
    data2.clear();
    jointable.clear();
    return 0;
}
*/