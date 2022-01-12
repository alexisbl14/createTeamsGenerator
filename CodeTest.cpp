#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;
using namespace nlohmann;
int main(int argc, char** argv) {
    ifstream file1;        //initialize file to be opened

    json buildHeapData_object;
    file1.open(argv[1]);    //open file, if opened successfully, put into buildHeapData_object
    if (file1.is_open()){
        file1 >> buildHeapData_object;
    }
    file1.close();    //close file

    int arraySize = buildHeapData_object["metadata"]["arraySize"];
    buildHeapData_object.erase("metadata");
    int x_value, data_count;
    data_count = 0;
    for(auto itr = buildHeapData_object.begin(); itr != buildHeapData_object.end(); itr++){
        if (data_count == 0){
            x_value = itr.value()[arraySize-1];
        }
        else{
            if(itr.value()[arraySize - 1] != x_value){
                cout << "false\n";
                return false;
            }
        }
        data_count ++;
    }
    cout << "true\n";
    return true;
}