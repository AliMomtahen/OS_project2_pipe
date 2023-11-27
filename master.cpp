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
#include<set>
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
    print_log("read building name");
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
        
        
        dup2(p2[1] ,STDOUT_FILENO );
        
        
        close(p2[0]);
        close(p2[1]);
        
        static char *args[] = { (char *) BIULD_EXE.c_str() ,(char*) bname.c_str()  , NULL };
        string data = to_string(p[1]);
        int en = execv(BIULD_EXE.c_str() , args);
        exit(1);
        
    } else if (pid > 0) {
        
        rp = p2[0];
        close(p2[1]);
        
        
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
    print_log("send data to pipe from main");
    
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
    print_log("main make procces");
    office_pid=  make_office_proc(office_pipe_w , building_dir);
    send_line_to_pipe(office_pipe_w , build_lst);
    for(int i=0;i<build_lst.size() ; i++){
        int wp=-1 , rp=-1;
        build_pid_lst[i] = creat_one_procces(wp , rp ,building_dir+ "/"+ build_lst[i]);
        
        read_pipe_lst[i] = rp;
    }

    cout << endl;
    return -1;
}


struct Set_sets{
    set<string> req_build;
    set<string> req_src;
    set<int> req_indicator;
};



void print_indec_first(){
    cout << Green_COLOR <<  "we can calculate 6 indicator\n" << RESET_COLOR << endl;
    for(int i=0 ;i < 6 ;i++){
        cout << Yellow_COLOR << i << "- " << Indicators[i] << RESET_COLOR << endl; 
    }

}

void get_input_and_send_data(vector<string> &build_lst ,vector<int>& build_pid_lst ,
        vector<int> &read_pipe_lst , struct Set_sets *req_set){
    print_log("get input");

            string lin1 , lin2 , lin3;
            cout << Green_COLOR <<  "the sources is 1-gas 2-water 3-electricity\npleas inter id of sources:" << RESET_COLOR << endl;
            getline(cin , lin1);
            cout << Green_COLOR <<  "pleas inter buildings:" << RESET_COLOR << endl;
            getline(cin , lin2);
            print_indec_first();
            cout << Green_COLOR <<  "pleas inter id of indecator you need:" << RESET_COLOR << endl;
            getline(cin , lin3);
            vector<string> req_src = getWords(lin1 , " ");
            vector<string> req_build = getWords(lin2 , " ");
            vector<string> req_indic = getWords(lin3 , " ");
            for(auto r : req_src){
                req_set->req_src.insert(r);
            }
            for(auto r : req_build){
                req_set->req_build.insert(r);
            }
            for(auto r : req_indic){
                req_set->req_indicator.insert(stoi(r));
            }


}

void print_a_line(){
    cout << Yellow_COLOR ;
    cout << "============================";
    for(int i=0 ;i <= 12 ;i++){
        cout << "==========";
    }
    cout << RESET_COLOR << endl;
}
void print_space(string c  ,int ff){
    cout << Yellow_COLOR;
    int k=10;
    if(ff == 0){
        k=25;
    }
    for(int i=0 ;i < k - c.size()  ;i++){
        cout << " ";
    }
    cout << RESET_COLOR;
}


void print_buty(vector<vector<string>> lst){
    int f=0;
    print_a_line();
    for(auto l : lst)
        {
            int ff=0;
            for(auto c : l){
                cout << Yellow_COLOR << "|" << RESET_COLOR;
                if(f==0)
                    cout << Green_COLOR;
                cout << c;
                print_space(c , ff);
                ff=1;
            }
            cout << Yellow_COLOR << "|" << RESET_COLOR<<  endl;
            print_a_line();
            f=1;
        }
}
void print_src(struct Set_sets *req_set , string data){
    
    auto word = getWords(data , "\n");
    vector<vector<string>> str_d(1);
    str_d[0].push_back("indicator");
    switch (stoi(word[0]))
    {
    case 1:
        cout << Red_COLOR << "\n\ngas report:" << RESET_COLOR << endl;
        break;
    case 2:
        cout << Red_COLOR << "\n\nwater report:" << RESET_COLOR << endl;

    break;
    case 3:
        cout << Red_COLOR << "\n\nelectricity report:" << RESET_COLOR << endl;

    break;
    default:
        break;
    }
    for(int i=0;i<12;i++){
        string s="month ";
        if(i < 9)
            s += "0";
        str_d[0].push_back(s + to_string(i+1));

    }
    int size=0;
    for(auto i : req_set->req_indicator){
        if(i >= 6)
            continue;
        str_d.push_back({});
        size++;
        str_d[size].push_back(Indicators[i]);
        auto d = getWords(word[i+1] , " ");
        for(auto ds : d){
            str_d[size].push_back(ds);
        }
    }
    print_buty(str_d);
    
}

void print_result(string build_name , string data , struct Set_sets *req_set){
    if(req_set->req_build.find(build_name) == req_set->req_build.end())
        return ;
    print_log("print result " + build_name);
    cout << Green_COLOR << "report for " << build_name << ":" << RESET_COLOR << endl;
    vector<string> srcs = getWords(data , "$");
    for(auto r : req_set->req_src){
        int src_id= stoi(r);
        
        print_src(req_set , srcs[src_id -1]);
    }

    cout << Blue_COLOR << "\n\n***********************\n\n" << endl;  
    

}


int main(int argc , const char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    print_log("open file")
    print_log("start work main");
    if(argc < 2){
        cout << "input not valid\n";
        return 1;
    }

    string building_dir = (string)  argv[1];
    vector<string> build_lst = find_builds(building_dir);
    vector<int> build_pid_lst(build_lst.size());
    vector<int> read_pipe_lst(build_lst.size());
    struct Set_sets *req_set = new struct Set_sets;
    int office_pid=-1;
    creat_proccess(build_lst , build_pid_lst ,read_pipe_lst ,  building_dir , office_pid);
    get_input_and_send_data(build_lst , build_pid_lst ,read_pipe_lst , req_set);
    cout << Blue_COLOR << "\n\n***********************\n\n" << endl;  
    for(int i=0 ; i < build_lst.size();i++){
        int status;
        
        char inp[2048];
        read(read_pipe_lst[i] , inp , 2048);
        close(read_pipe_lst[i]);
        print_result(build_lst[i] ,(string) inp , req_set);
        
        waitpid(build_pid_lst[i] , &status , 0);
        
    }
    cout << Red_COLOR<< "end" << RESET_COLOR<< endl;
    
}


#endif