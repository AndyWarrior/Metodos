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
#include <string>

#define TOTAL_ITERATIONS 1000000000
#define MAX_CLIENTS 500
#define ERROR_CONSTANT 0.001
#define BUFFER_MAX 4500
#define FRAMES 2000
#define MTU 1500

int clients = 0, current_clients = 0;
int iterations = 0;
double time_passed = 0.0;
int frames[FRAMES], delays[FRAMES];
int tot_buf_size = 0;
int total_frames = FRAMES * MAX_CLIENTS;

int buffer[MAX_CLIENTS];

struct Client{
	int arrival_time = 0,
		current_frame = 0,
		dropped_packets = 0,
		successful_packets = 0,
		complete_frames = 0,
		frames_delay = 0,
		finish_time = -1;
};

Client client_list[MAX_CLIENTS];

using namespace std;

//File handling

void loadInitialData(){
	
	//Load frames from file
	ifstream infile;
	infile.open("frames.txt");
	for(int i=0; i<FRAMES; i++){
		infile >> frames[i];
	}
	infile.close();
	
	//Load delays from file
	infile.open("delays.txt");
	for(int i=0; i<FRAMES; i++){
		infile >> delays[i];
	}
	infile.close();
}

void generateCSV(string filename, string contents){
	//Generate report as CSV
	ofstream outfile;
	outfile.open(filename);
	outfile << contents;
	outfile.close();
}

//Helper functions

int getPacketsForFrameSize(int size){
	if(size % MTU == 0){
		return ceil(size/MTU);
	}else{
		return ceil(size/MTU) + 1;
	}
}

void cleanBuffer(){
	for(int i=0; i<MAX_CLIENTS; i++){
		buffer[i] = 0;
	}
}

int getBufferSize(){
	int size = 0;
	for(int i=0; i<MAX_CLIENTS; i++){
		size += buffer[i];
	}
	return size;
}

void handleBufferOverload(){
	int size_before = 0, size_after = 0, overflow = -1;
	
	for(int i=0; i<MAX_CLIENTS; i++){
		
		if(overflow == 1){
			buffer[i] = 0;
		}else{
			size_after += buffer[i];
			if(size_after > BUFFER_MAX){
				buffer[i] = BUFFER_MAX - size_before;
				overflow = 1;
			}
		}
		size_before = size_after;
	}
}

double calculateCompletedFrames(){
	int frames_delivered = 0;
	for(int i=0; i<MAX_CLIENTS; i++){
		frames_delivered += client_list[i].complete_frames;
	}
	return (double)frames_delivered / (double)total_frames;
}

void generateReport(){
	
	string contents = "Client #,Arrival Time,Dropped Packets,Successful Packets,Complete Frames,Departure Time\n";
	
	for(int i=0; i<MAX_CLIENTS; i++){
		contents += to_string(i) + "," + to_string(client_list[i].arrival_time) + "," + to_string(client_list[i].dropped_packets) + "," + to_string(client_list[i].successful_packets) + "," + to_string(client_list[i].complete_frames) + "," + to_string(client_list[i].finish_time) + "\n";
	}
	
	generateCSV("results.csv", contents);
	cout << "Report generated..." << endl;
}

int calculateQueueWaitTime(){
	double expected_packets_sent = MAX_CLIENTS * 1.5 * 24;
	return (1 / (BUFFER_MAX - expected_packets_sent)) * 1000000;
}

//Main

int main(int argc, const char * argv[]){
	
	//Load frames and delays from txt files
	loadInitialData();
	cout << "Initial data loaded..." << endl;
	
	cleanBuffer();
	//Start simulation
    while (iterations <= TOTAL_ITERATIONS) {
        
        //This is a departure
        if (iterations % 222 == 0) {
            //cout << "Departure time: " << iterations << " microseconds" << endl;
            if(getBufferSize() > BUFFER_MAX){
                cout << "Buffer is overloaded, dropped " << getBufferSize() - BUFFER_MAX << " packets"<< endl;
                handleBufferOverload();
            }
            
            if (getBufferSize() > 0) {
                for(int i=0; i<MAX_CLIENTS; i++){
                    if (buffer[i] > 0) {
                        buffer[i]--;
                        if (buffer[i] == 0) {
                            if(client_list[i].current_frame >= FRAMES){
                                //this are packets for the last frame for this client
                                client_list[i].finish_time = iterations;
                                current_clients--;
                                if(current_clients == 0){
                                    iterations = TOTAL_ITERATIONS + 1;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
        
        
        //This is the frame processing
        if (iterations % 41000 == 0) {
            //cout << "Frame processing time: " << iterations << " microseconds" << endl;
			
			double prob_error = current_clients*ERROR_CONSTANT;
			for(int i=0; i<clients; i++){
				if(client_list[i].finish_time == -1){ //client is in system
					int packets = getPacketsForFrameSize(frames[client_list[i].current_frame]);
					int error_flag = false;
					for(int j=0; j<packets; j++){
						if(((double) rand() / (RAND_MAX)) <= prob_error){
							//packet error
							client_list[i].dropped_packets++;
							error_flag = true;
						}else{
							//packet success
							client_list[i].successful_packets++;
							buffer[i]++;
                            tot_buf_size++;
						}
					}
					if(!error_flag){
						client_list[i].complete_frames++;
						int frame_delay = calculateQueueWaitTime() + delays[i];
						client_list[i].frames_delay += frame_delay;
					}
					
					client_list[i].current_frame++;
				}
			}
        }
        
        //This is a new client arrival
        if (iterations % 2000000 == 0) {
            //cout << "New client arrives time: " << iterations << " microseconds" << endl;
            if(clients < MAX_CLIENTS){
                Client new_client = *new Client;
                new_client.arrival_time = iterations;
                client_list[clients++] = new_client;
                current_clients++;
            }
        }
        
        iterations++;
    }
	
	//Generate CSV files from contents
	generateReport();
	
	cout << endl << "--- STATS ---" << endl;
    cout << "Percentage of complete delivered frames: " << calculateCompletedFrames() << endl;
	cout << "W = " << calculateQueueWaitTime() << " microseconds" << endl;
	
	cout << client_list[0].frames_delay << endl;
	
	return 0;
}
