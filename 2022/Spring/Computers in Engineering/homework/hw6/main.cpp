//
// Note: Code was modified from Figure 11.3.1: Overriding member function.
// in the CIE ZyBook class textbook.

///////////////////////////////////////////////////////////////////////////////
//
//  Matt Kaufman
//  04/06/2022
//  Practice with copy constructors, copy assignment, destructors, and inheritance.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

class Computer {
public:
    explicit Computer(const string usage = "0%", const string status = "not connected", const float size = 0) {
        cout << "> In the Computer constructor." << endl;
        SetCpuUsage(usage);
        SetInternetStatus(status);
        if (GetMemorySize() == 0) {
            SetMemorySize(0);
        }
        else {
            SetMemorySize(size);
        }
    }

    ~Computer() {                                                        // Computer Destructor
        cout << "> In the Computer destructor." << endl;
        print();
        delete memorySize;
        cout << endl << "[memorySize deleted by Computer destructor.]" << endl << endl;
    }

    Computer(const Computer& originalObject) {                           // Computer Copy Constructor
        cout << "> In the Computer copy constructor." << endl;
        SetCpuUsage(originalObject.GetCpuUsage());
        SetInternetStatus(originalObject.GetInternetStatus());
        SetMemorySize(originalObject.GetMemorySize());
    }

    Computer& operator=(const Computer& objToCopy) {                     // Computer Copy Assignment Operator
        if (this != &objToCopy) {
            delete memorySize;
            cout << "[memorySize deleted by Computer copy assignment operator]" << endl;
            memorySize = new float;
            cout << "[new memorySize allocated by Computer copy assignment operator]" << endl;
            *memorySize = *objToCopy.memorySize;
            SetCpuUsage(objToCopy.GetCpuUsage());
            SetInternetStatus(objToCopy.GetInternetStatus());
            SetMemorySize(objToCopy.GetMemorySize());
        }
        return *this;
    }

    string GetCpuUsage() const {                            // cpuUsage getter
        return cpuUsage;
    }
    void SetCpuUsage(const string usage) {                  // cpuUsage setter
        this->cpuUsage = usage;
    }

    string GetInternetStatus() const {                      // internetStatus getter
        return internetStatus;
    }
    void SetInternetStatus(const string status) {           // internetStatus setter
        this->internetStatus = status;
    }

    float GetMemorySize() const {                           // memorySize getter
        if (memorySize == nullptr)
            return 0;
        else return *memorySize;
    }
    void SetMemorySize(const float size) {                  // memorySize setter
        if (memorySize == nullptr) {
            memorySize = new float;
            cout << "[new memorySize allocated by Computer::SetMemorySize]" << endl << endl;
            return;
        }
        *memorySize = size;
    }

    void setComputerStatus(const string usage, const string status) {
        cpuUsage = usage;
        internetStatus = status;
    }

    void print() {
        cout << "Computer status: " << endl;
        cout << "\tCPU usage: " << cpuUsage << endl;
        cout << "\tInternet status: " << internetStatus << endl;
        cout << "\tMemory size: " << GetMemorySize() << endl;
    }

private:
    string cpuUsage;
    string internetStatus;
    float* memorySize = nullptr;        // in GB
};



class Laptop : public Computer {
public:
    explicit Laptop(const string usage = "0%", const string status = "not connected", const float size = 0) : Computer(usage, status, size) {
        cout << "> In the Laptop constructor." << endl;
        SetMemorySize(size);
    }

    void setComputerStatus(const string usage, const string status, const string quality) {
        SetCpuUsage(usage);
        SetInternetStatus(status);
        wifiQuality = quality;
    }

    void print() {
        cout << "Laptop status:" << endl;
        cout << "\tCPU usage: " << GetCpuUsage() << endl;
        cout << "\tInternet status: " << GetInternetStatus() << endl;
        cout << "\tWiFi quality: " << wifiQuality << endl;
        Computer::print();
    }

    ~Laptop() {                                                            // Laptop Destructor
        cout << endl << "> In the Laptop destructor." << endl;
        print();
        cout << endl;
    }

private:
    string wifiQuality;
};




int main() {
    cout << endl;
    Laptop myLaptop;
    myLaptop.setComputerStatus("25%", "connected", "good");
    myLaptop.print();

    cout << endl << endl;

    Laptop myLaptop2("45%", "not connected", 4);        // testing a second Laptop object
    myLaptop2.setComputerStatus("50%", "connected", "average");

    cout << endl << endl;

    myLaptop = myLaptop2;                               // testing the copy assignment operator

    cout << endl << endl;

    Laptop myLaptop3(myLaptop2);                        // testing the copy constructor
    myLaptop3.SetMemorySize(8);                         // testing the memorySize setter

    return 0;
}