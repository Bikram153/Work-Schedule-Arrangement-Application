#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

// Class to represent a worker
class Worker {
public:
    string name;
    vector<string> offDays;

    Worker(string n, vector<string> off) : name(n), offDays(off) {}
};

// Class to manage workers and generate schedules
class WorkerManager {
private:
    vector<Worker> workers;
    unordered_map<string, vector<string>> workerSchedules;

public:
    // Load worker data from file
    void loadWorkerData(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Cannot open file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, offDay1, offDay2;
            getline(ss, name, ':');
            getline(ss, offDay1, ',');
            getline(ss, offDay2, ',');

            vector<string> offDays = { offDay1, offDay2 };
            workers.push_back(Worker(name, offDays));
        }
        file.close();
    }

    // Generate and print the weekly schedule
    void generateSchedule() {
        vector<string> daysOfWeek = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
        unordered_map<string, int> dayWorkerCount = { {"Monday", 2}, {"Tuesday", 2}, {"Wednesday", 2}, {"Thursday", 2}, {"Friday", 2}, {"Saturday", 3}, {"Sunday", 3} };
        
        srand(time(0)); // Seed for random shuffling
        for (const auto& day : daysOfWeek) {
            cout << "Schedule for " << day << ":" << endl;
            vector<string> scheduledWorkers;
            vector<Worker> availableWorkers;
            for (const auto& worker : workers) {
                if (find(worker.offDays.begin(), worker.offDays.end(), day) == worker.offDays.end()) {
                    availableWorkers.push_back(worker);
                }
            }
            random_shuffle(availableWorkers.begin(), availableWorkers.end());
            for (int i = 0; i < dayWorkerCount[day]; ++i) {
                if (i < availableWorkers.size()) {
                    workerSchedules[availableWorkers[i].name].push_back(day);
                    scheduledWorkers.push_back(availableWorkers[i].name);
                }
            }
            if (scheduledWorkers.empty()) {
                cout << "- No scheduled worker" << endl;
            }
            else {
                for (const auto& worker : scheduledWorkers) {
                    cout << "- " << worker << endl;
                }
            }
            cout << endl;
        }
    }

    // Print individual schedule for each worker
    void printIndividualSchedules() {
        cout << "Individual Schedules:-" << endl;
        for (const auto& worker : workers) {
            cout << worker.name << ": ";
            for (const auto& day : workerSchedules[worker.name]) {
                cout << day << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    WorkerManager manager;
    manager.loadWorkerData("WorkerData.txt");
    std::cout << "Weekly Schedule:-\n";
    manager.generateSchedule();
    manager.printIndividualSchedules();

    return 0;
}
