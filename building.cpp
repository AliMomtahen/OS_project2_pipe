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


using namespace std;

int main(int argc , const char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string my_data_path = (string) argv[1];
    //int father_wfd = stoi(arg);
    // std::cout <<"in child" << endl;
    // cin >> s;
   write(STDOUT_FILENO ,argv[1] , 15);


    //write(father_wfd , "helow f" , 8);
    // string h;
    // cin >> h;
    // cout << "my name is " << h << '\n';
    FILE *k = fopen("ooo.txt" , "w");
    
    

    return 0;
}


#endif