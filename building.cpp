#ifndef _BUILDING_CPP
#define _BUILDING_CPP

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

struct Data_src{
    string gas_data;
    string water_data;
    string elec_data;
};




int make_proc(string my_data_path ,string exe_file  ,int & water_r){
    int p2[2];
    if ( pipe(p2) == -1) {
        return -1;
    }
    int pid = fork();
    if (pid == 0) {
        dup2(p2[1] ,STDOUT_FILENO );
        close(p2[0]);
        close(p2[1]);
        static char *args[] = { (char *) exe_file.c_str() ,(char*) my_data_path.c_str()  , NULL };
        int en = execv(exe_file.c_str() , args);
        exit(1);
    } else if (pid > 0) {
        water_r = p2[0];
        close(p2[1]);
    }else{
        cerr << "can't make child\n";
    }
    return pid;
}

void get_data_file(string my_data_path , struct Data_src * data){
    
    
    ofstream *fd = new ofstream("ggg.txt");
     
    
    *fd << "hkhk5555hkhk";
    fd->close();
    
    int water_r , gas_r , elec_r; 
    int pid_water = make_proc(my_data_path ,WATER_EXE  , water_r);
    int pid_gas = make_proc(my_data_path ,GAS_EXE  , gas_r);
    int pid_elec = make_proc(my_data_path ,ELEC_EXE  , elec_r);
    int status;
    
    char inp[1024] , inp1[1024] , inp2[1024];
    read(gas_r , inp , 1024);
    data->gas_data = (string) inp;
    read(water_r , inp1 , 1024);
    data->water_data = (string) inp1;
    read(elec_r , inp2 , 1024);
    data->elec_data = (string) inp2;

    
    waitpid(pid_elec , &status , 0);
    waitpid(pid_gas , &status , 0);
    waitpid(pid_water , &status , 0);

}



void send_to_office(string fifo_office , Data_src *data){

    string data_str = data->gas_data + "$" + data->water_data + "$"
        + data->elec_data;
    mkfifo(fifo_office.c_str(), 0666);
    char inp[1024];
    int fifo_id = open(fifo_office.c_str() , O_WRONLY);
    write(fifo_id , data_str.c_str() , data_str.size());
    close(fifo_id);

    fifo_id = open(fifo_office.c_str() , O_RDONLY);
    int size =0;
    while (size < 2)
        size =read(fifo_id , inp , 1024);

    vector<string> bills = getWords((string) inp , "$");

    //cerr<< "goooooood " << bills[2] << "enddd" << endl;
    
    close(fifo_id);
    data->gas_data += (bills[0] + "\n");
    data->water_data += (bills[1] + "\n");
    data->elec_data += (bills[2] + "\n");


}



void send_to_main(Data_src * data){
    string data_str = data->gas_data + "$" + data->water_data + "$" + data->elec_data + "$";
    write(STDOUT_FILENO , data_str.c_str(), data_str.size());
}

int main(int argc , const char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string my_data_path = (string) argv[1];
    
    struct Data_src *data = new struct Data_src;
    get_data_file(my_data_path , data);
    
    
    string fifo_office = "np/" +  my_data_path + "_FIFO1";
    
    
    send_to_office(fifo_office , data);
    send_to_main(data);
    // cout << "khkkkkkkkkkkkk "<< data->elec_data << endl;
    // read(fifo_id , inp ,1024);
    //cout << "rsv "<< my_data_path<< " "<< (string) inp << endl;
    //cout << "hhhhh" << endl;
    //write(STDOUT_FILENO ,argv[1] , 15);
    //FILE *k = fopen("ooo.txt" , "w");
    
    

    return 0;
}


#endif