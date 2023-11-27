#ifndef _OFFICE_CPP
#define _OFFICE_CPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <strings.h>
#include <dirent.h>
#include <sstream>
#include <fstream>

#include <sys/stat.h>
#include <unistd.h>

#include"function.hpp"



using namespace std;

void make_unnamed_pipe(vector<string>& build_lst ,string build_path){
    for(auto &p : build_lst){
        
        if(p == " " || p == "")
            continue;
        p = "np/" + build_path + "/" + p + "_FIFO1";
        
        int g = mkfifo(p.c_str() , 0666);
        
        
        
    }
}



void read_file(vector<int>& gas_coef , vector<int>& water_coef , vector<int>& elec_coef
    , string build_path){
        string file_string = build_path + "/" + BILLS;
        fstream *file = new fstream(file_string.c_str());

    string line;
    char inp4[1024];
    file->getline(inp4 , 1024);
   for(int i=1; i < 13;i++)
    {
        char inp[128];
        file->getline(inp , 128);
        line = (string) inp;
        if(line.size() < 3)
            break;
        vector<string> words = getWords(line , ",");
        water_coef[i] = stoi(words[2]);
        gas_coef[i] = stoi(words[3]);
        elec_coef[i] = stoi(words[4]);
        
    }
    print_log("office read bill coef file");

}


string calc_bill_gas(string data ,
   vector<int>& gas_coef , vector<int>& water_coef , vector<int>& elec_coef ){
   vector<string> word = getWords(data , "\n");
   vector<string> str_tot = getWords(word[2] , " ");
   int sum =0;
   string ans="";
   for(int i=0;i<12;i++){
        ans += (to_string(gas_coef[i+1] * stoi(str_tot[i])) + " "); 
   }
    
    return ans;
}

string calc_bill_elec(string data ,
   vector<int>& gas_coef , vector<int>& water_coef , vector<int>& elec_coef ){

    vector<string> word = getWords(data , "\n");
    vector<string> str_tot = getWords(word[2] , " ");
    vector<string> str_aver = getWords(word[1] , " ");
    vector<string> str_tot_high = getWords(word[4] , " ");

    int sum =0;
    string ans="";
    for(int i=0;i<12;i++){
            int tot = stoi(str_tot[i]);
            double aver= stoi(str_aver[i]);
            int peak_tot = stoi(str_tot_high[i]);
            double coef =1.25;
            if((tot / (30 * 6.0)) > (peak_tot / (30.0)))
                coef = 0.75;
            ans += (to_string((int) (((tot - peak_tot) +  peak_tot *0.75) * elec_coef[i+1])) + " "); 
    }
    
    return ans;

}

string calc_bill_water(string data ,
   vector<int>& gas_coef , vector<int>& water_coef , vector<int>& elec_coef ){

    vector<string> word = getWords(data , "\n");
    
   
    vector<string> str_tot = getWords(word[2] , " ");
    vector<string> str_aver = getWords(word[1] , " ");
    vector<string> str_tot_high = getWords(word[4] , " ");
    int sum =0;
    string ans="";
    for(int i=0;i<12;i++){
            
            int tot = stoi(str_tot[i]);
            int aver= stoi(str_aver[i]);
            int peak_tot = stoi(str_tot_high[i]);
            double res =(((tot - peak_tot) +  peak_tot *1.25) * water_coef[i+1]);
            ans += (to_string((int) res ) + " "); 
    }
    
    return ans;

}

void rsv_and_calc_bills(vector<string>& build_lst ,
    vector<int>& gas_coef , vector<int>& water_coef , vector<int>& elec_coef){

        for(int i=0 ; i < build_lst.size() ; i++){
            auto p = build_lst[i];
            int fd = open(p.c_str() , O_RDONLY);
            char inp[1024];
            int size =0;
            while (size < 2)
                size =read(fd , inp , 1024);
            auto data_lst = getWords((string) inp , "$");
            print_log("office rcv data from " + build_lst[i]);

            string gas_bill = calc_bill_gas(data_lst[0] , gas_coef , water_coef , elec_coef);
            
            string water_bill = calc_bill_water(data_lst[1] , gas_coef , water_coef , elec_coef);

            
            string elec_bill = calc_bill_elec(data_lst[2] , gas_coef , water_coef , elec_coef);
            print_log("office calc bills");
            string send_data_str = gas_bill + "$" + water_bill + "$" 
                + elec_bill + "$";
            close(fd);
            fd = open(p.c_str() , O_WRONLY);
            print_log("office send data to " + build_lst[i]);

            write(fd , send_data_str.c_str() , send_data_str.size());
            close(fd);
            
        }
}

int main(int argc , const char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
   
    string build_path = (string) argv[1];
    print_log("start office ");

    vector<int> gas_coef(13) , water_coef(13),elec_coef(13);
    read_file(gas_coef , water_coef , elec_coef , build_path);

    char inp[1024];
    read(STDIN_FILENO , inp , 1024);
    
    vector<string> build_lst = getWords((string) inp , " ");
    
    make_unnamed_pipe(build_lst , build_path);
    rsv_and_calc_bills(build_lst , gas_coef , water_coef , elec_coef);
    close(STDIN_FILENO);
    print_log("end office ");
    return 0;
}


#endif