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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    std::cout << "in child\n";
    string h;
    cin >> h;
    cout << "my name is " << h << '\n';
    FILE *k = fopen("ooo.txt" , "w");
    fprintf(k , "yes yes");
    

    return 0;
}


#endif