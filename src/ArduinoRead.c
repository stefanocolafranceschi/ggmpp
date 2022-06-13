#include "ArduinoRead.h"

char serialPortFilename1[] = "/dev/ttyACM0";
char serialPortFilename2[] = "/dev/ttyACM1";

bool isReadComplete(int input[]);

bool is_number(const std::string& s);

void PrintElements(int input[], int board);

double* ArduinoRead();

//int main() {
//    double* Arduino = 0;
//    Arduino = ArduinoRead();
//}

double* ArduinoRead() {
    bool ArduinoVerbose = false;
    bool Printout = true;

    double myArduino[32];

    //double myArduino = new double[32];
    for (int i=0; i<=31; i++) {
        myArduino[i] = -666;
    }

    for (int i=0; i<=15; i++) {
        arduino1[i]=-666;
        arduino2[i]=-666;
    }

    serPort = fopen(serialPortFilename1, "r");
    if (serPort != NULL) {

	do {
	    memset(readBuffer, 0, 1024);
	    //fread(readBuffer, sizeof(char),1024,serPort);
            fgets (readBuffer , 1024 , serPort);
	    if(sizeof(readBuffer) != 0) {
                std::string myprova;
                myprova = readBuffer;
                usleep(100000);
                char * pch;
//std::cout << readBuffer << std::endl;
                for (int k=0; k<=15; k++) {
                    std::stringstream ss;
                    ss << std::setw(2) << std::setfill('0') << k;
                    s = "1_A" + ss.str() + "=";

                    pch = strstr (readBuffer,s.c_str());
                    if (pch != NULL) {
                        std::string temp = myprova.substr(6,4);
                        if(is_number(temp)) {
                            arduino1[k]=std::stoi(temp);
                            if (ArduinoVerbose) std::cout << "FOUND " << arduino1[k] << std::endl;
                        }
                    }
                    s = "2_A" + ss.str() + "=";

                    pch = strstr (readBuffer,s.c_str());
                    if (pch != NULL) {
                        std::string temp = myprova.substr(6,4);
                        if(is_number(temp)) {
                            arduino2[k]=std::stoi(temp);
                            if (ArduinoVerbose) std::cout << "FOUND " << arduino2[k] << std::endl;
                        }
                    }
                }
	    }
        } while( !isReadComplete(arduino1) );
    }
    if (Printout) PrintElements(arduino1, 1);

    serPort = fopen(serialPortFilename2, "r");
    if (serPort != NULL) {
        do {
            memset(readBuffer, 0, 1024);
            //fread(readBuffer, sizeof(char),1024,serPort);
            fgets (readBuffer , 1024 , serPort);
            if(sizeof(readBuffer) != 0) {

                std::string myprova;
                myprova = readBuffer;
                usleep(100000);
                char * pch;

                for (int k=0; k<=15; k++) {
                    std::stringstream ss;
                    ss << std::setw(2) << std::setfill('0') << k;
                    s = "1_A" + ss.str() + "=";

                    pch = strstr (readBuffer,s.c_str());
                    if (pch != NULL) {
                        std::string temp = myprova.substr(6,4);
                        if(is_number(temp)) {
                            arduino1[k]=std::stoi(temp);
                            if (ArduinoVerbose) std::cout << "FOUND " << arduino1[k] << std::endl;
                        }
                    }
                    s = "2_A" + ss.str() + "=";

                    pch = strstr (readBuffer,s.c_str());
                    if (pch != NULL) {
                        std::string temp = myprova.substr(6,4);
                        if(is_number(temp)) {
                            arduino2[k]=std::stoi(temp);
                            if (ArduinoVerbose) std::cout << "FOUND " << arduino2[k] << std::endl;
                        }
                    }
		}
            }
	} while( !isReadComplete(arduino2) );
    }

    if (Printout) PrintElements(arduino2, 2);

    for (int i=0; i<=15; i++) {
        myArduino[i] = arduino1[i];
        myArduino[i] = myArduino[i]*5/1024*1000;
    }
    for (int i=0; i<=15; i++) {
        myArduino[i+16] = arduino2[i];
        myArduino[i+16] = myArduino[i+16]*5/1024*1000;
    }
//std::cout << ".............XXXX " << myArduino[16] << std::endl;
    return myArduino;
}

bool is_number(const std::string& s){
    return s.find_first_not_of("0123456789") == std::string::npos;
}

bool isReadComplete(int input[]) {
    bool completed = true;
    for (int i=0; i<16; i++) {
        if ( input[i] == -666 ) completed = false;
    }
    return completed;
}

void PrintElements(int input[], int board) {
    //std::cout << sizeof(input)/sizeof(input[0]) << std::endl;
    for (int i=0; i<16; ++i) {
        std::cout << board << "_Analog[" << i << "] = " << input[i] << " counts, equal to " << input[i]*4.8828125 << " mV" <<  std::endl;
    }         
}
