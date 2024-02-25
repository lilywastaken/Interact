#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

#define WINDOW_NAME "Chess"

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    size_t start = 0, end = 0;
    while ((end = str.find(delimiter, start)) != string::npos) {
        token = str.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

string get_window_name(string window_id) {
    // Open a pipe to the xprop command to get the window name
    string cmd = "xprop -id " + window_id + " WM_NAME";
    FILE* fp = popen(cmd.c_str(), "r");
    if (!fp) {
        cerr << "Failed to run xprop" << endl;
        return "";
    }

    // Parse the output of xprop to extract the window name
    string window_name;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strlen(line) - 1] = '\0';
        if (strstr(line, "WM_NAME") != NULL) {
            auto tokens = split(line, '=');
            if (tokens.size() >= 2) {
                window_name = tokens[1];
                window_name.erase(0, 1); // Remove the leading double quote
                window_name.erase(window_name.length() - 1); // Remove the trailing double quote
            }
        }
    }

    // Close the pipe to the xprop command
    pclose(fp);

    return window_name;
}

int main() {


    // Open a pipe to the wmctrl command
    FILE* fp = popen("wmctrl -l", "r");
    if (!fp) {
        cerr << "Failed to run wmctrl" << endl;
        return 1;
    }

    // Parse the output of wmctrl to get a list of window IDs and names
    vector<pair<string, string>> windows;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strlen(line) - 1] = '\0';
        auto tokens = split(line, ' ');
        if (tokens.size() >= 3) {
            string window_id = tokens[0];
            string window_name = get_window_name(window_id);
            windows.push_back({window_id, window_name});
        }
    }

    // Close the pipe to the wmctrl command
    pclose(fp);
    
    string window_id;
    
    for(auto window : windows){
      if(window.second.find(WINDOW_NAME) != string::npos){
      
      	// I mean if you forget to close my git you download chess game from it'll find chess in it
      	if(window.second.find("lilywastaken") != string::npos) continue;
      	
        window_id = window.first;
        break;
      }
    }
    
    //sleep(1);
    //system(click_command.c_str());
    //exit(0);

    // Print the IDs and names of the open windows
    for (auto window : windows) cout << "Window ID: " << window.first << " Name: " << window.second << "\"" << endl;
	
    VideoCapture cap("ximagesrc xid="+window_id+" ! video/x-raw,framerate=30/1 ! videoconvert ! appsink", CAP_GSTREAMER);
    if (!cap.isOpened()) {
        cerr << "Failed to open video capture" << endl;
        return 1;
    }
    
    int waiting = 0;

    while (waitKey(1) < 0) {
    waiting+=1;
        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            cerr << "Failed to capture frame" << endl;
            break;
        }

        //imshow("Window", frame);
        
		 // Split the image into its component channels
		vector<Mat> channels;
		split(frame, channels);
		
		Mat r = channels[0];
		Mat g = channels[1];
		Mat b = channels[2];
		
		cout << endl << "Window size: " << frame.cols << " | " << frame.rows << endl;

		// Process the channels one row at a time
		/*for (int i = 0; i < frame.rows; i+=400) {
		    // Process the pixel values in the row
		    for (int j = 0; j < frame.cols; j+=600) {
		        int r_val = (int)r.at<uint8_t>(i, j);
		        int g_val = (int)g.at<uint8_t>(i, j);
		        int b_val = (int)b.at<uint8_t>(i, j);
		        cout << "Pixel (" << i << ", " << j << "): " << r_val << ", " << g_val << ", " << b_val << endl;
		    }
		}*/
		
		string click_command_1 = "xdotool mousemove --window "+window_id+" 200 820 click 1";
		string click_command_2 = "xdotool mousemove --window "+window_id+" 80 600 click 1";
		
		// Is knight in or out?
		int r_val = (int)r.at<uint8_t>(800, 180);
		
		cout << "Red channel value at 180, 800: " << r_val << endl;
		
		if(r_val == 255){
			// Knight out!
			cout << endl << "Knight is in, pushing him out" << endl;
			system(click_command_1.c_str());
			cout << click_command_1 << endl;
   			this_thread::sleep_for(chrono::milliseconds(50));
			system(click_command_2.c_str());
			cout << click_command_2 << endl;
		}
		else{
			// Knight in!
			cout << endl << "Knight is out, pulling him in" << endl;
			system(click_command_2.c_str());
			cout << click_command_2 << endl;
   			this_thread::sleep_for(chrono::milliseconds(50));
			system(click_command_1.c_str());
			cout << click_command_1 << endl;
		}
		    
		exit(0);
    }

    cap.release();
    destroyAllWindows();
    
    /*string command = "xdotool windowactivate --sync " + window_id;
    system(command.c_str());
    
    sleep(1);
    
    auto start_time = chrono::high_resolution_clock::now();
    for(int i=0; i<10; i++){
    	command = "xdotool mousemove --window " + window_id + " " + to_string(200+8*i) + " 550 click 1";
    	//cout << command << endl;
    	system(command.c_str());
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    cout << "Elapsed time: " << duration.count() << " microseconds" << endl;*/
    
    
    //system("xdotool mousemove --window "+window_id+" 400 500 click 1");
    return 0;
}



// Drag and drop using delays :
// xdotool mousemove --window 0xc00010 200 300 mousedown 1 mousemove_relative 150 150 sleep 1s mousemove_relative 100 100 mouseup 1










