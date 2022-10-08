//
//  main.cpp
//  proj3
//
//  Created by Ruby on 2022/5/11.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include<sys/time.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>


using namespace std;
typedef struct Data {
    int times = 0;
    int num;
} pData;

string filename;
int method, page_frame ;
vector<int> page_Reference;
ofstream outFile;


void readFile(){
    ifstream txtfile;
    string dataLine;
    int num;
    string temp;
    cout << "輸入檔案名稱: "<<endl;
    cin >> filename;
    txtfile.open(filename+ ".txt");
    while (!txtfile.is_open()) {
        cout << "Failed to open file.\n";
        cout << "輸入檔案名稱: "<<endl;
        cin >> filename;
        txtfile.open(filename + ".txt");
    }
    
    txtfile >> method >> page_frame;
    txtfile >> dataLine;
    for(int i =0 ; i < dataLine.length() ; i++){
        num = dataLine[i] - '0';
        page_Reference.push_back(num);
    }
    
}// readFile()


bool biggerT(pData p1 , pData p2) {

    return p1.times < p2.times;
    
}

bool findP(vector<pData> queue, int num, int &index){
    for(int i = 0 ; i < queue.size() ; i++){
        if(queue[i].num == num){
            index = i;
            return true;
        }
        
    }
    return false;
}

int sortT(vector<pData> queue){
    int index = 0 ;
    vector<pData> temp = queue;
    sort(temp.begin(), temp.end(), biggerT);
    findP(queue, temp[0].num, index);
    return index;
}


bool biggerM(pData p1 , pData p2) {
    return p1.times > p2.times;
}

int sortM(vector<pData> queue){
    int index = 0 ;
    vector<pData> temp = queue;
    sort(temp.begin(), temp.end(), biggerM);
    findP(queue, temp[0].num, index);
    return index;
}

bool findV(vector<int> queue, int num){
    vector<int>::iterator it = std::find(queue.begin(), queue.end(), num); // find 10
    if (it != queue.end()) return true;
    else return false;
}
bool findLRU(vector<int> queue, int num, int &index){
    for(int i = 0 ; i < queue.size() ; i++){
        if(queue[i] == num){
            index = i;
            return true;
        }
        
    }
    return false;
}

void FIFO(){
    int Page_Fault = 0,  Page_Replaces = 0;
    bool found = false;
    outFile << "--------------FIFO-----------------------\n";
    vector<int> queue ;
    string str;
    for(int i = 0 ; i < page_Reference.size() ; i++){
        outFile << page_Reference[i] << "\t";
        if(queue.size()== 0){
            queue.push_back(page_Reference[i]);
            outFile  << queue[0] << "\t" << "F" << "\r\n";
            //outFile  << left << setw(page_frame) << queue[0] << "\t" << "F" << "\r\n";
            Page_Fault++;
        }
        else{
            found = findV(queue, page_Reference[i]);
            if(queue.size() < page_frame){
                if(!found){
                    Page_Fault++;
                    queue.push_back(page_Reference[i]);
                }
            }
            else{
                if(!found){
                    Page_Fault++;
                    Page_Replaces++;
                    queue.erase(queue.begin());
                    queue.push_back(page_Reference[i]);
                }
                
            }
            for(int j = queue.size() -1 ; j >= 0; j--){
                str += to_string(queue[j]);
            }
            outFile  << str ;
            //outFile  << left << setw(page_frame) << str << "\t" ;
            if(!found)outFile<<"\t"<< "F" ;
            outFile <<"\r\n";
            str = "";
            
        }
    }
    outFile << "Page Fault = " << Page_Fault << "  Page Replaces = "<< Page_Replaces << "  Page Frames = " << page_frame <<"\r\n" <<"\r\n" ;
    
}

void LRU(){
    int Page_Fault = 0,  Page_Replaces = 0;
    bool found = false;
    outFile << "--------------LRU-----------------------\n";
    vector<int> queue ;
    pData dataItem;
    int index =0, temp = 0 ;
    string str;
    
    for(int i = 0 ; i < page_Reference.size() ; i++){
        outFile << page_Reference[i] << "\t";
        if(queue.size()== 0){
            queue.push_back(page_Reference[i] );
            outFile  << queue[0] << "\t" << "F" << "\r\n";
//            outFile  << left << setw(page_frame) << queue[0] << "\t" << "F" << "\r\n";
            Page_Fault++;
        }
        else{
            //found = findP(queue,page_Reference[i], index);
            found = findLRU(queue, page_Reference[i], index);
            
            if(queue.size() < page_frame){
                if(!found){
                    Page_Fault++;
                    queue.push_back(page_Reference[i]);
                }
                else{
                    queue.erase(queue.begin()+index);
                    queue.push_back(page_Reference[i]);
                }
            }
            else{
                if(!found){
                    Page_Fault++;
                    Page_Replaces++;
                    queue.erase(queue.begin());
                    queue.push_back(page_Reference[i]);
                }
                else{
                    queue.erase(queue.begin()+index);
                    queue.push_back(page_Reference[i]);
                }
                
            }
            for(int j = queue.size() -1 ; j >= 0; j--){
                str += to_string(queue[j]);
            }
//            outFile  << left << setw(page_frame) << str << "\t" ;
            outFile  << str ;
            if(!found)outFile<<"\t"<< "F" ;
            outFile <<"\r\n";
            str = "";
        }
    }
    outFile << "Page Fault = " << Page_Fault << "  Page Replaces = "<< Page_Replaces << "  Page Frames = " << page_frame <<"\r\n" <<"\r\n" ;
}

void LFU_LRU(){
    int Page_Fault = 0,  Page_Replaces = 0;
    bool found = false;
    outFile << "--------------Least Frequently Used LRU Page Replacement-----------------------\n";
    vector<pData> queue, existRef ;
    int index =0 ;
    string str;
    
    for(int i = 0 ; i < page_Reference.size() ; i++){
        pData dataItem, tempItem;
        outFile << page_Reference[i] << "\t";
        if(queue.size()== 0){
            dataItem.num = page_Reference[i];
            dataItem.times ++;
            queue.push_back(dataItem);
//            outFile  << left << setw(page_frame) << queue[0].num << "\t" << "F" << "\r\n";
            outFile  << queue[0].num << "\t" << "F" << "\r\n";
            Page_Fault++;
        }
        else{
            found = findP(queue,page_Reference[i], index);
            if(queue.size() < page_frame){ //未超過page_frame
                if(!found){ //沒有在queue中
                    Page_Fault++;
                    dataItem.num = page_Reference[i];
                    dataItem.times ++;
                    queue.push_back(dataItem);
                    
                }
                else{ //在queue中 更新在queue中的順序
                    dataItem = queue[index];
                    dataItem.times++;
                    queue.erase(queue.begin()+index);
                    queue.push_back(dataItem);
                }
            }
            else{
                if(!found){ //沒有在queue中且超過page_frame
                    Page_Fault++;
                    Page_Replaces++;
                    dataItem.num = page_Reference[i];
                    dataItem.times ++;
                    index = sortT(queue);
                    queue.erase(queue.begin()+ index);
                    queue.push_back(dataItem);
                    
                }
                else{
                    dataItem = queue[index];
                    dataItem.times ++;
                    queue.erase(queue.begin()+index);
                    queue.push_back(dataItem);
                }
                
            }
            for(int j = queue.size() -1 ; j >= 0; j--){
                str += to_string(queue[j].num);
            }
//            outFile  << left << setw(page_frame) << str << "\t" ;
//            if(!found)outFile<< "F" ;
            outFile  << str ;
            if(!found)outFile<<"\t"<< "F" ;
            outFile <<"\r\n";
            str = "";
            
        }
    }
    outFile << "Page Fault = " << Page_Fault << "  Page Replaces = "<< Page_Replaces << "  Page Frames = " << page_frame <<"\r\n" <<"\r\n" ;
}


void MFU_FIFO(){
    int Page_Fault = 0,  Page_Replaces = 0;
    bool found = false, exist = false;
    outFile << "--------------Most Frequently Used Page Replacement -----------------------\n";
    vector<pData> queue, existRef ;
    int index =0, existIndex = 0 ;
    string str;
    
    for(int i = 0 ; i < page_Reference.size() ; i++){
        pData dataItem, tempItem;
        outFile << page_Reference[i] << "\t";
        if(queue.size()== 0){
            dataItem.num = page_Reference[i];
            dataItem.times ++;
            queue.push_back(dataItem);
//            outFile  << left << setw(page_frame) << queue[0].num << "\t" << "F" << "\r\n";
            outFile  << queue[0].num << "\t" << "F" << "\r\n";
            Page_Fault++;
        }
        else{
            found = findP(queue,page_Reference[i], index);
            if(queue.size() < page_frame){ //未超過page_frame
                if(!found){ //沒有在queue中
                    Page_Fault++;
                    dataItem.num = page_Reference[i];
                    dataItem.times ++;
                    queue.push_back(dataItem);
                    
                }
                else{ //在queue中 "不“更新在queue中的順序
                    queue[index].times++;
                }
            }
            else{
                if(!found){ //沒有在queue中且超過page_frame
                    Page_Fault++;
                    Page_Replaces++;
                    dataItem.num = page_Reference[i];
                    dataItem.times ++;
                    index = sortM(queue);
                    queue.erase(queue.begin()+ index);
                    queue.push_back(dataItem);
                    
                }
                else{
                    queue[index].times++;
                }
                
            }
            for(int j = queue.size() -1 ; j >= 0; j--){
                str += to_string(queue[j].num);
            }
//            outFile  << left << setw(page_frame) << str << "\t" ;
//            if(!found)outFile<< "F" ;
            outFile  << str ;
            if(!found)outFile<<"\t"<< "F" ;
            outFile <<"\r\n";
            str = "";
            
        }
        //outFile << endl;
    }
    outFile << "Page Fault = " << Page_Fault << "  Page Replaces = "<< Page_Replaces << "  Page Frames = " << page_frame <<"\r\n" <<"\r\n" ;
}

void MFU_LRU(){
    int Page_Fault = 0,  Page_Replaces = 0;
    bool found = false, exist = false;
    outFile << "--------------Most Frequently Used LRU Page Replacement -----------------------\n";
    vector<pData> queue, existRef ;
    int index =0, existIndex = 0 ;
    string str;
    
    for(int i = 0 ; i < page_Reference.size() ; i++){
        pData dataItem, tempItem;
        outFile << page_Reference[i] << "\t";
        if(queue.size()== 0){
            dataItem.num = page_Reference[i];
            dataItem.times ++;
            queue.push_back(dataItem);
//            outFile  << left << setw(page_frame) << queue[0].num << "\t" << "F" << "\r\n";
            outFile  << queue[0].num << "\t" << "F" << "\r\n";
            Page_Fault++;
        }
        else{
            found = findP(queue,page_Reference[i], index);
            if(queue.size() < page_frame){ //未超過page_frame
                if(!found){ //沒有在queue中
                    Page_Fault++;
                    dataItem.num = page_Reference[i];
                    dataItem.times ++;
                    queue.push_back(dataItem);
                    
                }
                else{ //在queue中 更新在queue中的順序
                    dataItem = queue[index];
                    dataItem.times++;
                    queue.erase(queue.begin()+index);
                    queue.push_back(dataItem);
                }
            }
            else{
                if(!found){ //沒有在queue中且超過page_frame
                    Page_Fault++;
                    Page_Replaces++;
                    dataItem.num = page_Reference[i];
                    dataItem.times ++;
                    index = sortM(queue);
                    queue.erase(queue.begin()+ index);
                    queue.push_back(dataItem);
                    
                }
                else{
                    dataItem = queue[index];
                    dataItem.times ++;
                    queue.erase(queue.begin()+index);
                    queue.push_back(dataItem);
                }
                
            }
            for(int j = queue.size() -1 ; j >= 0; j--){
                str += to_string(queue[j].num);
            }
//            outFile  << left << setw(page_frame) << str << "\t" ;
//            if(!found)outFile<< "F" ;
            outFile  << str ;
            if(!found)outFile<<"\t"<< "F" ;
            outFile <<"\r\n";
            str = "";
            
        }
        //outFile << endl;
    }
    outFile << "Page Fault = " << Page_Fault << "  Page Replaces = "<< Page_Replaces << "  Page Frames = " << page_frame <<"\r\n" <<"\r\n" ;
}




 int main(int argc, const char * argv[]) {
    // insert code here...
    cout<<"**********************\n";
    cout<<"*1. FIFO             *\n";
    cout<<"*2. LRU              *\n";
    cout<<"*3. LFU_LRU          *\n";
    cout<<"*4. MFU_FIFO         *\n";
    cout<<"*5. MFU_LRU          *\n";
    cout<<"*6. ALL              *\n";
    cout<<"**********************\n";
    readFile();
    
    outFile.open("out_"+filename+".txt");
    if(method == 1){ // FCFS
        FIFO();
        //writeFile("FCFS");
    }
    else if(method == 2){
        LRU();
        //writeFile("RR");
    }
    else if(method == 3){
        LFU_LRU();
        //writeFile("SRTF");
    }
    else if(method == 4){
        MFU_FIFO();
        //writeFile("Priority RR");
    }
    else if(method == 5){
        MFU_LRU();
        //writeFile("HRRN");
    }
    else if(method == 6){
        //string m_name = All;
        FIFO();
        LRU();
        LFU_LRU();
        MFU_FIFO();
        MFU_LRU();
        //writeFile("All");
    }
    else {
        cout << "Method ERROR !\n";
    }
   
}

