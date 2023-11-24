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

int creat_one_procces(int &wp){
    int p[2];
    if (pipe(p) == -1) {
        return -1;
    }
    int pid = fork();
    if (pid == 0) {
        cout << "in childdd\n";
        dup2(p[0], STDIN_FILENO);
        close(p[0]);
        close(p[1]);
        int en = execl(BIULD_EXE.c_str() ,  BIULD_EXE.c_str() , NULL);
        cout << "after it" << en << "\n" ;
    } else if (pid > 0) {
        close(p[0]);
        wp = p[1];
    }else{
        cout << "can't make child\n";
    }
    return pid;
}


int   creat_proccess(vector<string> &build_lst ,vector<int>& build_pid_lst ,
        vector<int> &write_pipe_lst,string building_dir){
    cout << "number of buildings is " << Red_COLOR << build_lst.size() << 
    RESET_COLOR << "\nbuildings: ";
    for(int i=0;i<build_lst.size() ; i++){
        cout << build_lst[i] << " ";
    }

    cout << "\n";
    for(int i=0;i<build_lst.size() ; i++){
        int wp=-1;
        build_pid_lst[i] = creat_one_procces(wp);
        write(wp , build_lst[i].c_str() ,build_lst[i].size());

        cout << "num"<< wp << "\n";
    }

    cout << endl;
    return -1;
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
    vector<int> write_pipe_lst(build_lst.size());

    int office_pid=-1;
    creat_proccess(build_lst , build_pid_lst ,write_pipe_lst ,  building_dir);
    

    
}


#endif