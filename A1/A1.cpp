#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;
void read_file(const string filename,const bool isBinary,unsigned int *n,vector<double> *data){
    ifstream inputFile;
    if (isBinary) {
        inputFile.open(filename,ios::binary);
    }else{
        inputFile.open(filename);
    }
    if (!inputFile.is_open()) {
        cout << "[read_file]Can not open file: "<<filename<<endl;
        exit(1);
    }
    if (isBinary) {
        inputFile.read((char *)n,sizeof(*n));
    }else{
        inputFile >> *n;
    }
    if (*n == 0) {
        cout << "[read_file]Empty data set: "<<filename<<endl;
        exit(2);
    }
    for (int i = 0; i < *n; ++i) {
        double temp = 0.;
        if (inputFile.eof()) {
            break; //already at the end of dataset
        }
        if (isBinary) {
            inputFile.read((char *)&temp, sizeof(double));
        }else{
            inputFile >> temp;
        }
        data->push_back(temp);
    }
    inputFile.close();
}
void show_data(const vector<double> *data){
    if (data->empty()) {
        cout << "[show_data]empty data set"<<endl;
        exit(3);
    }
    cout <<scientific;
    for (auto i: *data) {
        cout << i<<endl;
    }
}
void write_data(const string filename, const vector<double> *data){
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cout << "[write_data]can not open file: "<<filename<<endl;
        exit(1);
    }
    outputFile << scientific;
    for (auto i: *data) {
        outputFile << i <<endl;
    }
    outputFile.close();
}
void write_data(const string filename, const vector<vector<double>> *data){
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cout << "[write_data]can not open file: "<<filename<<endl;
        exit(1);
    }
    outputFile << scientific;
    for (int i = 0; i<data->size(); i++) {
        for (int j = 0; j<data->at(i).size(); j++) {
            outputFile << data->at(i)[j]<<"\t";
        }
        outputFile <<"\n";
    }
    outputFile.close();
}
void cum_sq_sum(const vector<double> *A, vector<double> *sigma){
    if (A->empty()) {
        cout << "[cum_sq_sum] empty dataset" <<endl;
    }
    double sum = 0;
    for (auto i: *A) {
        sum += i;
        sigma->push_back(sum);
    }
}
int main(int argc, const char * argv[]) {
    unsigned int n = 0;
    vector<double> data1;
    read_file("/Users/wenjie/Library/Mobile Documents/com~apple~CloudDocs/Study/Bechlor/SS20/CP-Tutorial/U1/a1/dataset1", false, &n, &data1);
    show_data(&data1);
    vector<double> sigma;
    cum_sq_sum(&data1, &sigma);
    write_data("/Users/wenjie/Library/Mobile Documents/com~apple~CloudDocs/Study/Bechlor/SS20/CP-Tutorial/U1/a1/dataset1_css", &sigma);
    vector<double>data2;
    read_file("/Users/wenjie/Library/Mobile Documents/com~apple~CloudDocs/Study/Bechlor/SS20/CP-Tutorial/U1/a1/dataset2", true, &n, &data2);
    vector<vector<double>> data2_sin;
    vector<double> temp;
    for (int i = 0; i<data2.size(); i++) {
        temp.push_back(data2[i]);
        temp.push_back(sin(data2[i]));
        if (temp.size() == 2) {
            data2_sin.push_back(temp);
        }
        temp.clear();
    }
    write_data("/Users/wenjie/Library/Mobile Documents/com~apple~CloudDocs/Study/Bechlor/SS20/CP-Tutorial/U1/a1/dataset2_sin", &data2_sin);
    return 0;
}
