#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>

using namespace std;

double getCpuUsage() {
    ifstream statFile("/proc/stat");
    string line;

    if (statFile.is_open()) {
        getline(statFile, line);
        statFile.close();

        istringstream iss(line);
        string cpuLabel;
        iss >> cpuLabel;

        if (cpuLabel == "cpu") {
            double user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
            iss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

            double totalCpuTime = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
            double cpuUsage = (1.0 - (idle / totalCpuTime)) * 100.0;
            return cpuUsage;
        }
    }
}

void displayMemoryInfo() {
    ifstream meminfoFile("/proc/meminfo");
    string line;

    while (getline(meminfoFile, line)) {
        cout << line << endl;
    }

    meminfoFile.close();
}

int main() {
    while (true) {
        char command[200];
        cout << ":) ";
        cin.getline(command, sizeof(command));

        if (strcmp(command, "help") == 0) {
            cout<< "mem  show memory usage, cpu show cpu usage , exit is exit" << endl;
        }

        else if (strcmp(command, "exit") == 0) {
            break; 
        } else if (strcmp(command, "cpu") == 0) {
            double cpuUsage = getCpuUsage();
            if (cpuUsage != -1.0) {
                cout << "CPU Usage: " << cpuUsage << "%" << endl;
            } else {
                cerr << "Error while reading CPU usage." << endl;
            }
        } else if (strcmp(command, "mem") == 0) {
            ifstream meminfoFile("/proc/meminfo");
            string line;

            while (getline(meminfoFile, line)) {
                cout << line << endl;
            }
            meminfoFile.close();
        }
        else{
            cout<<"unknown command"<<endl;
        }
    }

    return 0;
}
