#ifndef _GAS_CPP
#define _GAS_CPP

#include"function.hpp"

using namespace std;

Info * read_data(fstream *file){
    Info *data = new Info(GAS_TYPE);
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
        //cout << m << " " << d << "word size =" << words.size() <<  endl;
       
        for(int i=3 ; i < words.size(); i++){
            day_info.push_back(stoi(words[i]));
            
        }
        data->add_info_day(m , d , day_info);
    }


    return data;
    
}


void send_data(Info *data){
    string data_str = "";
    data->get_data(data_str);

}

int main(int argc , const char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    //cout << "in gas" << endl; 
    string my_folder = (string)  argv[1];
    string file_string = my_folder + "/" + GAS_CSV;
    fstream *file = new fstream(file_string.c_str());
    
    Info *data = read_data(file);
    data->calc_average_total_and_dist();
    
    send_data(data);
    
    
    
}



#endif
