//
//  Metodos Cuantitativos Proyecto Final
//
//  Mónica Lozano
//  Andrés López De León
//  Eliézer Galván
//

#include <iostream>
#include <fstream>
#include <math.h>

#define TOTAL_ITERATIONS  10000000
#define MAX_CLIENTS 500

int clients = 0;
int iterations = 0;
double time_passed = 0.0;
int frames[2000], delays[2000];

struct Client{
	int arrival_time, current_frame, dropped_packets, complete_frames, finish_time;
};

Client client_list[MAX_CLIENTS];

using namespace std;

void loadInitialData()
{
	int lines = 2000;
	
	//Load frames from file
	ifstream infile;
	infile.open("frames.txt");
	for(int i=0; i<lines; i++){
		infile >> frames[i];
	}
	infile.close();
	
	//Load delays from file
	infile.open("delays.txt");
	for(int i=0; i<lines; i++){
		infile >> delays[i];
	}
	infile.close();
}

void generateCSV(string filename, string contents)
{
	//Generate report as CSV
	ofstream outfile;
	outfile.open(filename);
	outfile << contents;
	outfile.close();
}

int main(int argc, const char * argv[]) {
	
	//Load frames and delays from txt files
	loadInitialData();
	cout << "Initial data loaded..." << endl;
	
	//Start simulation
    while (iterations <= TOTAL_ITERATIONS) {
        if (iterations % 222 == 0) {
            cout << "Departure time: " << iterations << " microseconds" << endl;
        }
        
        if (iterations % 41000 == 0) {
            cout << "Frame processing time: " << iterations << " microseconds" << endl;
        }
        
        if (iterations % 2000000 == 0) {
            cout << "New client arrives time: " << iterations << " microseconds" << endl;
        }
        
        iterations++;
    }
	
	//Generate CSV files from contents
	string contents = "hello,there\nlitte,world";
	generateCSV("results.csv", contents);
	cout << "Report generated..." << endl;
	
    return 0;
}
