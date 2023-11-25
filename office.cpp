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
        
        //cout << "good" << g << endl;
        int fd1 = open(p.c_str() , O_WRONLY);
        write(fd1, "officcccc" , 10);
        close(fd1);
    }
}



int main(int argc , const char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
   
    string build_path = (string) argv[1];
    char inp[1024];
    read(STDIN_FILENO , inp , 1024);
    cout << "in offic " << (string) inp << endl;
    vector<string> build_lst = getWords((string) inp , " ");
    
    make_unnamed_pipe(build_lst , build_path);
    
    return 0;
}


#endif