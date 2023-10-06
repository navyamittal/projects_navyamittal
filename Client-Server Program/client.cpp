/*
	Original author of the starter code
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date: 2/8/20
	
	Please include your Name, UIN, and the date below
	Name: Navya Mittal
	UIN: 831007872
	Date: 9/9/2023
*/
#include "common.h"
#include "FIFORequestChannel.h"
#include<sys/wait.h>
#include<unistd.h>
#include<iostream>
#include<vector>

using namespace std;


void requestDataPoint(int patientID, double time, int ecgRecord, FIFORequestChannel &chan) {
    
		datamsg request(patientID, time, ecgRecord);
		// Send the datamsg object to the server.
		//memcpy(buf, &x, sizeof(datamsg));
		//chan.cwrite(buf, sizeof(datamsg)); // question
	// double reply;
	// chan.cread(&reply, sizeof(double)); //answer
		//char buf[MAX_MESSAGE]; 
		//memcpy(buf, &request, sizeof(datamsg));

		chan.cwrite(&request, sizeof(datamsg));
		double response;
		chan.cread(&response, sizeof(double));

		// Print the result o the console.
		cout << "For person " << patientID << ", at time " << time << ", the value of ecg " << ecgRecord << " is " << response << endl;
	}

void multiplepoints(int patientID, FIFORequestChannel &chan) {
	string filename = "received/x1.csv";
    std::ofstream outputFile;
	outputFile.open(filename);
	

    for ( double i = 0.0; i < 4; i+=0.004) {
        // Request ECG1 data point.
		outputFile << i << ",";
        datamsg requestECG1(patientID, i, 1);
        chan.cwrite(&requestECG1, sizeof(datamsg));

        // Receive and save the response (ECG1 value) to the file.
        double responseECG1;
        chan.cread(&responseECG1, sizeof(double));
        outputFile << responseECG1 << ",";

        // Request ECG2 data point.
        datamsg requestECG2(patientID, i, 2);
        chan.cwrite(&requestECG2, sizeof(datamsg));

        // Receive and save the response (ECG2 value) to the file.
        double responseECG2;
        chan.cread(&responseECG2, sizeof(double));
        outputFile << responseECG2 << std::endl;
    }

    outputFile.close();
}

int main (int argc, char *argv[]) {
	int opt;
	int p = 1;
	double t = 0.0;
	int e = 1;
	int m = MAX_MESSAGE;
	bool new_chan = false;
	vector<FIFORequestChannel*> channels;
	
	string filename = "";
	while ((opt = getopt(argc, argv, "p:t:e:f:m:c")) != -1) {
		switch (opt) {
			case 'p':
				p = atoi (optarg);

				break;
			case 't':
				t = atof (optarg);
				break;
			case 'e':
				e = atoi (optarg);
				break;
			case 'f':
				filename = optarg;
				break;

			case 'm':
				m = atoi (optarg);
				break;

			case 'c':
				new_chan = true;
				break;
		}
	}


	//give arguments for the server
	//server needs './server', '-m' and <val for mm arg>, 'NULL' (to let exec vp kno we're done giving arguments)
	//fork
	//in the child, run execvp using server arguments

	
	//FIFORequestChannel* control_channel = new 
	pid_t child = fork();
	
	if (child==0){
		char val_m[16]; // Make sure val_m has enough space to hold the integer as a string
    sprintf(val_m, "%d", m);

    char* args[] = {(char *)"./server", (char *)"-m", val_m, NULL};
    execvp(args[0], args);
	}else{

	

	FIFORequestChannel cont_chan("control", FIFORequestChannel::CLIENT_SIDE);
	channels.push_back(&cont_chan);

   if (new_chan){
	//send new channel request to the server. 
	MESSAGE_TYPE n = NEWCHANNEL_MSG;
    cont_chan.cwrite(&n, sizeof(MESSAGE_TYPE));
	//create a variable to hold the name 
	//cread the response form the server
	char new_channel_name [30];
	cont_chan.cread(new_channel_name, sizeof(new_channel_name));
	//call the FIFORequestChannel construtor with the name of the server. create dunamically so we can call it outside the if statement.
	FIFORequestChannel* new_channel = new FIFORequestChannel(new_channel_name, FIFORequestChannel::CLIENT_SIDE);
	// you want all the requets to go through the new channel
	//push the new channel into the vector
	channels.push_back(new_channel);
   }
	
	FIFORequestChannel chan = *(channels.back());

	//step 1: create server process (fork + exec the server program)

	// example data point request
    // char buf[MAX_MESSAGE]; // 256
    // datamsg x(1, 0.0, 1); //change from hardcoding to users value
	
	// memcpy(buf, &x, sizeof(datamsg));
	// chan.cwrite(buf, sizeof(datamsg)); // question
	// double reply;
	// chan.cread(&reply, sizeof(double)); //answer
	// cout << "For person " << p << ", at time " << t << ", the value of ecg " << e << " is " << reply << endl;
	
	//Step 2: requesting data points sending a file request to the server

	if(t){
		requestDataPoint(p, t, e, chan);
	}
	else if(p!=-1){
		multiplepoints(p, chan);
	}
	

    // sending a non-sense message, you need to change this timeout
	filemsg fm(0, 0);
	string fname = filename;
	
	int len = sizeof(filemsg) + (fname.size() + 1);
	char* buf2 = new char[len];
	memcpy(buf2, &fm, sizeof(filemsg));
	strcpy(buf2 + sizeof(filemsg), fname.c_str());
	chan.cwrite(buf2, len);  // I want the file length;

	int64_t filesize = 0;
	chan.cread(&filesize, sizeof(int64_t));
	//response buffer

	//char* buf3 = m;

	int sections= ceil(double(filesize)/m); //number of sections

	ofstream file_output;
	file_output.open("received/" + fname);


	for (int i = 0; i < sections; i++) {
        filemsg* file_req = (filemsg*)buf2;
        
        if (sections == 1) {
            file_req->offset = 0;
            file_req->length = filesize;
        } else {
        // Calculate the number of bytes to request in this section
			int bytes_to_request;
			if (i == sections - 1) {
				bytes_to_request = filesize - i * m; // Last section may have smaller size
			} else {
				bytes_to_request = m;
			}
			file_req->length = bytes_to_request;
			file_req->offset = i * m;
			
		}

		//cout << file_req->offset << "   " << file_req->length << endl;

        // Send the file request message
        chan.cwrite(buf2, len);

        // Receive and write the file data to the output file
        char* file_data = new char[file_req->length];;
        chan.cread(file_data, file_req->length);
        file_output.write(file_data, file_req->length);
		
        delete[] file_data;
    }
	delete[] buf2;

    // Close the output file
    file_output.close();
	

	//loop over the segments in the file filesize/ buff capacity(m)
	//create your filemsg instance reuse buf 2 
	//receive the response
	//cread into buf3 length  file_red->len
	//write buf3 into a file: received/filename
	//update the remaining bytes and loop
	
	//delete[] buf3;

	//Step 3: requesting files
	//if a file is requested from a client:
	//query the server for the file size
	//request the server for the file size hint in "i want the fil e length block"
	//request the file
	//start a timer
	//some variables:
	//	file_length: size of file 
	//  remaining bytes
	//	offset:
	//	buffersize:

	//create a while loop
	
	//perform some operations it cant be a single file transfer it has to be a many file transfer 
	/*
	while (remaining bytes to transfer)

		length = remaining bytes:
		offest = file_length - remaining bytes

		filemsg(offest, length)
		make file transfers()
	
	*/ 


	if(new_chan){
		MESSAGE_TYPE q = QUIT_MSG;
    	chan.cwrite(&q, sizeof(MESSAGE_TYPE)); 
		//wait(nullptr);
	}
	// closing the channel    
    MESSAGE_TYPE m = QUIT_MSG;
    chan.cwrite(&m, sizeof(MESSAGE_TYPE)); 
	//pass a valid int pointer. set the int var to what child exited as
	//int stat;
	wait(nullptr);
}
}
