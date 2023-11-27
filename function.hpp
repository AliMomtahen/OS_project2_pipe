#ifndef _FUNCTION_HPP
#define _FUNCTION_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <strings.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include"function.hpp"





using namespace std;


#define GAS_TYPE 1
#define WATER_TYPE 2
#define ELEC_TYPE 3 

const string ELEC_EXE =  "./Elec.out";
const string GAS_EXE =  "./Gas.out";
const string WATER_EXE =  "./Water.out";
const string  OFFICE_EXE = "./office.out";

const string WATER_CSV = "Water.csv";
const string ELEC_CSV = "Electricity.csv";
const string GAS_CSV = "Gas.csv";
const string BILLS = "bills.csv";


const vector<string> Indicators = {
    "average",
    "total usage",
    "high usage huor",
    "bill",
    "diff peak and average",
};


vector<string> getWords(string s , string spliter){
    vector<string> res;
    int pos = 0;
    while(pos < s.size()){
        pos = s.find(spliter);
        if(pos < 0 ){
            res.push_back(s.substr(0,s.size()));
            break;
        }
        res.push_back(s.substr(0,pos));
        s.erase(0,pos+1);
        pos=0; 
    }
    return res;
}


class MonthInfo
{
private:
    /* data */
public:
    int id;
    int total_usage;
    int average;
    int high_h;
    vector<vector<int> > day_lst;
    MonthInfo(/* args */);
    ~MonthInfo();
    void add_info(int d , vector<int> info){
        day_lst[d] = info;
    }

    int calc_usage(int& total , int& high_hour){
        total = 0;high_hour = 0;
        vector<int> h(6 , 0);
        int curr =0;
        for(int i=0 ;i < 6;i++){
            for(auto d : day_lst){
                h[i] += d[i];
            }
            total += h[i];
            if(h[i] > curr){
                curr = h[i];
                high_hour = i;
            }

        }

        total_usage = total;
        int average= total / (30 ); 
        high_h = high_hour;
        return average;
    }

    int calc_total_in_high(){
        int tot=0;
        for(auto d : day_lst){
            tot+=d[high_h];
        }
        return tot;
    }

    
};

MonthInfo::MonthInfo(/* args */)
{
    day_lst.resize(31);
    for(auto &d : day_lst){
        d.resize(6);
        
    }
}

MonthInfo::~MonthInfo()
{
    day_lst.clear();
}



class Info
{
private:
    /* data */
public:
    int year;
    int type;
    vector<MonthInfo> month_lst;
    vector<int> aver_lst;
    vector<int> high_usage_lst;
    vector<int> total_usage_in_high_hour;
    vector<int> total_usage_less_than_aver;

    vector<int> total_usage;
    vector<int> cost_lst;
    vector<int> diff_high_aver;

    
    Info(int t);
    ~Info();
    void add_info_day(int m , int d , vector<int> info){
        month_lst[m].add_info(d , info);
    }
    void calc_average_total_and_dist(){
        aver_lst.resize(13);
        total_usage.resize(13);
        high_usage_lst.resize(13);
        total_usage_in_high_hour.resize(13);
        total_usage_less_than_aver.resize(13);
        diff_high_aver.resize(13);
        
        cost_lst.resize(13);
        aver_lst.resize(13);
        for(int i=1;i<13;i++){
            aver_lst[i] = month_lst[i].calc_usage(total_usage[i] , high_usage_lst[i]);
        }
        for(int i=1; i < 13; i++){
            total_usage_in_high_hour[i] = month_lst[i].calc_total_in_high();
        }

    }

    void calc_diff(){

    }
    


    void get_data(string &str){
        str="";
        str += (to_string(type) + "\n");
        for(int i=1;i<13;i++){
            str += (to_string(aver_lst[i]) + " ");
        }
        str += "\n";
        for(int i=1;i<13;i++){
            str += (to_string(total_usage[i]) + " ");
        }
        str += "\n";

        for(int i=1;i<13;i++){
            str += (to_string(high_usage_lst[i]) + " ");
        }
        str += "\n";
        for(int i=1;i<13;i++){
            str += (to_string(total_usage_in_high_hour[i]) + " ");
        }
        str += "\n";
        for(int i=1;i<13;i++){
            str += (to_string(diff_high_aver[i]) + " ");
        }
        str += "\n";
        
        //todo
    }
};

Info::Info(int t)
{
    type= t;
    month_lst.resize(13);
}

Info::~Info()
{
    month_lst.clear();
}




class Build_info
{
private:
    /* data */
public:
    Info *elec;
    Info *Water;
    Info *Gas;
    Build_info(/* args */);
    ~Build_info();

};

Build_info::Build_info(/* args */)
{
    
}

Build_info::~Build_info()
{
}



Info * read_data(fstream *file , int type){
    Info *data = new Info(type);
    string line;
    char inp4[1024];
    file->getline(inp4 , 1024);
    
    while (1)
    {
        char inp[128];
        file->getline(inp , 128);
        line = (string) inp;
        if(line.size() < 3)
            break;
        vector<string> words = getWords(line , ",");
        int d = stoi(words[2]);
        vector<int> day_info;
        int m = stoi(words[1]);
        
       
        for(int i=3 ; i < words.size(); i++){
            day_info.push_back(stoi(words[i]));
            
        }
        data->add_info_day(m , d , day_info);
    }


    return data;
    
}



#endif