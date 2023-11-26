#ifndef _MASTER_CPP
#define _MASTER_CPP


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


#define Red_COLOR "\033[0;31m"
#define Green_COLOR "\033[0;32m"
#define Yellow_COLOR "\033[0;33m"
#define Blue_COLOR "\033[0;34m"
#define RESET_COLOR "\033[0m"

const string BIULD_EXE = "./building.out";

void color_rst(){
    cout << RESET_COLOR;
}

vector<string> find_builds(string building_dir){
    vector<string> build_lst;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (building_dir.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_DIR && ((string) ent->d_name) != "." && 
                ((string )ent->d_name) != "..") {
            

                build_lst.push_back((string) ent->d_name);
            }
        }
        closedir (dir);
    } else {
        cout << "directory "  << building_dir << "not exist\n";
       
    }
    return build_lst;
}

int creat_one_procces(int &wp , int& rp , string bname){
    int p[2];
    int p2[2];
    if ( pipe(p2) == -1) {
        return -1;
    }
    int pid = fork();
    if (pid == 0) {
        cout << "in childdd" << endl;
        
        //dup2(p2[1] ,STDOUT_FILENO );
        
        
        close(p2[0]);
        close(p2[1]);
        
        static char *args[] = { (char *) BIULD_EXE.c_str() ,(char*) bname.c_str()  , NULL };
        string data = to_string(p[1]);
        int en = execv(BIULD_EXE.c_str() , args);
        exit(1);
        //cout << "after it" << en << "\n" ;
    } else if (pid > 0) {
        // dup2(p[0], STDIN_FILENO);
        rp = p2[0];
        close(p2[1]);
        // wp = p[1];
        // close(p[0]);
        // write(wp , "hasssss" , 8);
        
    }else{
        cout << "can't make child\n";
    }
    return pid;
}


int make_office_proc(int& wp , string bname){
    int p2[2];
    if ( pipe(p2) == -1) {
        return -1;
    }
    int pid = fork();
    if (pid == 0) {
        cout << "in office childdd" << endl;
        
        dup2(p2[0] ,STDIN_FILENO );
        
        
        close(p2[0]);
        close(p2[1]);
        
        static char *args[] = { (char *) OFFICE_EXE.c_str(),(char*) bname.c_str()  , NULL };
        string data = to_string(p2[1]);
        int en = execv(OFFICE_EXE.c_str() , args);
        exit(1);
    } else if (pid > 0) {
        
        close(p2[0]);
        wp = p2[1];
        
    }else{
        cout << "can't make child\n";
    }
    return pid;
}

void   send_line_to_pipe(int office_pipe_w , vector<string>& build_lst){
    string data = "";
    for(auto s : build_lst){
        data += s;
        data += " ";
    }
    cout << "in send" << endl;
    write(office_pipe_w , data.c_str() , data.size());
}



int   creat_proccess(vector<string> &build_lst ,vector<int>& build_pid_lst ,
        vector<int> &read_pipe_lst,string building_dir , int& office_pid){
    cout << "number of buildings is " << Red_COLOR << build_lst.size() << 
    Yellow_COLOR << "\nbuildings: \n" <<RESET_COLOR;
    for(int i=0;i<build_lst.size() ; i++){
        cout <<  Yellow_COLOR<< i+1<<"- " << RESET_COLOR << build_lst[i] << endl;
        
    }

    int office_pipe_w;
    office_pid=  make_office_proc(office_pipe_w , building_dir);
    send_line_to_pipe(office_pipe_w , build_lst);
    for(int i=0;i<build_lst.size() ; i++){
        int wp=-1 , rp=-1;
        build_pid_lst[i] = creat_one_procces(wp , rp ,building_dir+ "/"+ build_lst[i]);
        char g[100];
        int r = read(rp , g , 100);
        string s = (string) g;
        cout <<  s + "Ali" << endl ;
        // write(wp , build_lst[i].c_str() ,build_lst[i].size());
        read_pipe_lst[i] = rp;

        

        close(rp);
    }

    cout << endl;
    return -1;
}



void get_input_and_send_data(vector<string> &build_lst ,vector<int>& build_pid_lst ,
        vector<int> &read_pipe_lst){

            string lin1 , lin2;
            cout << Green_COLOR <<  "pleas inter sources:" << RESET_COLOR << endl;
            getline(cin , lin1);
            cout << Green_COLOR <<  "pleas inter buildings:" << RESET_COLOR << endl;
            getline(cin , lin2);
            vector<string> req_src = getWords(lin1 , " ");
            vector<string> req_build = getWords(lin2 , " ");


}



int main(int argc , const char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if(argc < 2){
        cout << "input not valid\n";
        return 1;
    }
    string building_dir = (string)  argv[1];
    vector<string> build_lst = find_builds(building_dir);
    vector<int> build_pid_lst(build_lst.size());
    vector<int> read_pipe_lst(build_lst.size());

    int office_pid=-1;
    creat_proccess(build_lst , build_pid_lst ,read_pipe_lst ,  building_dir , office_pid);
    //get_input_and_send_data(build_lst , build_pid_lst ,read_pipe_lst);
   
    cout << "end" << endl;
    for(int i=0 ; i < build_lst.size();i++){
        int status;
        cout << build_pid_lst[i] << endl;
        waitpid(build_pid_lst[i] , &status , 0);
    }
    
}


#endif