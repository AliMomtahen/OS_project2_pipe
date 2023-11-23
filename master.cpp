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


vector<string> find_builds(string building_dir){
    vector<string> build_lst;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (building_dir.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_DIR) {
                build_lst.push_back((string) ent->d_name);
            }
        }
        closedir (dir);
    } else {
        printf("directory %s not exist\n");
       
    }
    return build_lst;
}

int   creat_proc(vector<string> &build_lst ,vector<int>& build_pid_lst ,string building_dir){
    for(int i=0;i<build_lst.size() ; i++){
        cout << build_lst[i] << " ";
    }

    cout << endl;
}



int main(int argc , const char *argv[]){
    if(argc < 2){
        cout << "input not valid\n";
        return 1;
    }
    string building_dir = (string)  argv[1];
    vector<string> build_lst = find_builds(building_dir);
    vector<int> build_pid_lst(build_lst.size());
    int office_pid=-1;
    creat_proc(build_lst , build_pid_lst , building_dir);
    

    
}


#endif