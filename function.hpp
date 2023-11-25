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
#include"function.hpp"





using namespace std;


const string ELEC_EXE =  "./Elec.out";
const string GAS_EXE =  "./Gas.out";
const string WATER_EXE =  "./Water.out";




vector<string> getWords(string s , string spliter){
    vector<string> res;
    int pos = 0;
    while(pos < s.size()){
        pos = s.find(spliter);
        res.push_back(s.substr(0,pos));
        s.erase(0,pos+3); 
    }
    return res;
}




#endif