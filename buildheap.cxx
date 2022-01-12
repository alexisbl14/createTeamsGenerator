#include "priorityqueue.h"
#include <iostream>
#include <fstream>

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

    int maxHeapSize = buildHeapData_object["metadata"]["maxHeapSize"];   //set max heap size to json object max heap size
    int numOperations = buildHeapData_object["metadata"]["numOperations"];  //set number of operations to json object numOperations
    buildHeapData_object.erase("metadata");     //erase metadata to iterate easier

    PriorityQueue myQueue(maxHeapSize);            //myQueue is queue to perform opertations on
    for(auto itr = buildHeapData_object.begin(); itr != buildHeapData_object.end(); itr++){  //iterate through json object to read operations
        string operation = itr.value()["operation"];      //set operation to value of operation in json object
        if (operation == "insert"){               //if operation is insert, set value of key in json object to variable key
            double key = itr.value()["key"];
            myQueue.insert(key);                   //and insert the key into myQueue
        }
        else if(operation == "removeMin") {    //if operation is removeMin, remove min in my queue
            myQueue.removeMin();
        }
    }

    json buildHeap_json = myQueue.JSON();   //organize buildHeap_json into JSON format for output

    buildHeap_json["metadata"]["numOperations"] = numOperations;   //add numOperations and maxHeapSize to buildHeap_json for output
    buildHeap_json["metadata"]["maxHeapSize"] = maxHeapSize;

    cout << buildHeap_json.dump(2) << endl;  //output final json


}
