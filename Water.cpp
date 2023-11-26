#ifndef _WATER_CPP
#define _WATER_CPP

#include"function.hpp"

void send_data(Info *data){
    string data_str = "";
    data->get_data(data_str);
   
    
    write(STDOUT_FILENO , data_str.c_str() , data_str.size());

}

int main(int argc , const char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string my_folder = (string)  argv[1];
    string file_string = my_folder + "/" + WATER_CSV;
    fstream *file = new fstream(file_string.c_str());
    
    Info *data = read_data(file , WATER_TYPE);
    data->calc_average_total_and_dist();
    send_data(data);



}



#endif
