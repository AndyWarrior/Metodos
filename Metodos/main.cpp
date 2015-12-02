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
#define k_CLIENTS 60
#define ERROR_CONSTANT 0.001
#define BUFFER_MAX 4500

int clients = 0, current_clients = 0;
int iterations = 0;
double time_passed = 0.0;
int frames[2000], delays[2000];

int buffer[MAX_CLIENTS];

struct Client{
	int arrival_time = 0, current_frame = 0, dropped_packets = 0, successful_packets = 0, complete_frames = 0, finish_time = -1;
};

Client client_list[MAX_CLIENTS];

using namespace std;

//File handling

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

//Helper functions

int getPacketsForFrameSize(int size){
	return ceil(size/1500);
}

void cleanBuffer(){
	for(int i=0; i<MAX_CLIENTS; i++){
		buffer[i] = 0;
	}
}

//Main

int main(int argc, const char * argv[]) {
	
	//Load frames and delays from txt files
	loadInitialData();
	cout << "Initial data loaded..." << endl;
	
	cleanBuffer();
	
	//Start simulation
    while (iterations <= TOTAL_ITERATIONS) {
        if (iterations % 222 == 0) {
            cout << "Departure time: " << iterations << " microseconds" << endl;
        }
        
        if (iterations % 41000 == 0) {
            cout << "Frame processing time: " << iterations << " microseconds" << endl;
			
			double prob_error = current_clients*ERROR_CONSTANT;
			
			for(int i=0; i<clients; i++){
				if(client_list[i].finish_time == -1){ //client is in system
					int packets = getPacketsForFrameSize(frames[client_list[i].current_frame]);
					int error_flag = false;
					for(int j=0; j<packets; j++){
						if(rand() <= prob_error){
							//packet error
							client_list[i].dropped_packets++;
							error_flag = true;
						}else{
							//packet success
							client_list[i].successful_packets++;
							buffer[i]++;
						}
					}
					if(!error_flag){
						client_list[i].complete_frames++;
					}
					
					client_list[i].current_frame++;
				}
			}
        }
        
        if (iterations % 2000000 == 0) {
            cout << "New client arrives time: " << iterations << " microseconds" << endl;
			
			Client new_client = *new Client;
			new_client.arrival_time = iterations;
			client_list[clients++] = new_client;
			current_clients++;
        }
        
        iterations++;
    }
	
	//Generate CSV files from contents
	string contents = "hello,there\nlitte,world";
	generateCSV("results.csv", contents);
	cout << "Report generated..." << endl;
	
    return 0;
}
