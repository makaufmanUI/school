///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Programmer: Matt Kaufman
//  Date: 2/11/2022
//  Name: hw2.cpp
//  Description: Create a class named Dog with a constructor and several different methods;
//               create 4 separate objects of the Dog class and test each class method a
//               total of 4 times on these objects; output expected vs. actual restults to console
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
using namespace std;


// Create the Dog class
class Dog {
public:
    explicit Dog(int months = 0) {  // specify the class constructor
        if (months < 0) {
            setMonths(0);           // set  months  to zero if class object initiated with negative value
        }
        else {
            setMonths(months);      // set  months  equal to input argument if not a negative number
        }
    }
    void setMonths(int monthsToSet);
    int getMonths() const;
    double getHumanYears() const;
    double getDogYears() const;
    void addBirthday();
    string getStage() const;
private:
    int months;
};


// Declare  setMonths()  setter method with one integer input parameter (what to set months value to)
//   Returns nothing
void Dog::setMonths(int monthsToSet) {
    if (monthsToSet < 0) {
        months = 0;                 // set months value to zero if input argument is negative
    }
    else {
        months = monthsToSet;       // set months value equal to input argument if not negative
    }
}


// Declare  getMonths()  getter method with no input parameters
//   Returns the value of  months  as an integer
int Dog::getMonths() const {            // make impossible for the method to change any values
    return months;
}


// Declare  getHumanYears()  getter method with no input parameters
//   Returns the equivalent dog age in human years as a double
double Dog::getHumanYears() const {     // make impossible for the method to change any values
    return months / 12.0;
}


// Declare  getDogYears()  getter method with no input parameters
//   Returns the equivalent dog age in dog years as a double
double Dog::getDogYears() const {       // make impossible for the method to change any values
    return months / 12.0 * 7.0;
}


// Declare  addBirthday()  method with no input parameters
//   Returns nothing, only updates the value of  months
void Dog::addBirthday() {
    months += 12;
}


// Declare  getStage()  getter method with no input parameters
//   Returns the dog's stage of life as a string
string Dog::getStage() const {          // make impossible for the method to change any values
    string stage;
    if (months < 11) {
        stage = "Puppy";
    }
    else if (months < 25) {
        stage = "Adolescence";
    }
    else if (months < 80) {
        stage = "Adulthood";
    }
    else {
        stage = "Senior";
    }
    return stage;
}




int main()
{
    cout << endl << endl;


    // Create Dog class objects with varying initial ages
    Dog duke;
    Dog buddy(-5);
    Dog simba(10);
    Dog watson(0);

    // Test constructors by checking current value of  months ( also tests  getMonths() )
    cout << "> Constructor tests:" << endl;
    cout << "    Expected  duke.getMonths()  to return 0. Actual value is " << duke.getMonths() << "." << endl;
    cout << "    Expected  buddy.getMonths()  to return 0. Actual value is " << buddy.getMonths() << "." << endl;
    cout << "    Expected  simba.getMonths()  to return 10. Actual value is " << simba.getMonths() << "." << endl;
    cout << "    Expected  watson.getMonths()  to return 0. Actual value is " << watson.getMonths() << "." << endl << endl;


    // Set age of each dog with setMonths
    duke.setMonths(-10);
    buddy.setMonths(12);
    simba.setMonths(18);
    watson.setMonths(6);

    // Test  setMonths()  by checking current value of  months ( also tests  getMonths() )
    cout << "> setMonths tests:" << endl;
    cout << "    Expected  duke.getMonths()  to return 0. Actual value is " << duke.getMonths() << "." << endl;
    cout << "    Expected  buddy.getMonths()  to return 12. Actual value is " << buddy.getMonths() << "." << endl;
    cout << "    Expected  simba.getMonths()  to return 18. Actual value is " << simba.getMonths() << "." << endl;
    cout << "    Expected  watson.getMonths()  to return 6. Actual value is " << watson.getMonths() << "." << endl << endl;


    // Test  getHumanYears()  by calling that method (value of  months  should be the same as above)
    cout << "> getHumanYears() tests:" << endl;
    cout << "    Expected  duke.getHumanYears()  to return 0. Actual value is " << duke.getHumanYears() << "." << endl;
    cout << "    Expected  buddy.getHumanYears()  to return 1. Actual value is " << buddy.getHumanYears() << "." << endl;
    cout << "    Expected  simba.getHumanYears()  to return 1.5. Actual value is " << simba.getHumanYears() << "." << endl;
    cout << "    Expected  watson.getHumanYears()  to return 0.5. Actual value is " << watson.getHumanYears() << "." << endl << endl;


    // Add 1 human year worth of age to each dog
    duke.addBirthday();
    buddy.addBirthday();
    simba.addBirthday();
    watson.addBirthday();


    // Test  getDogYears()  by calling that method (should be 7 more than would otherwise be without the birthday)
    cout << "> getDogYears() tests:" << endl;
    cout << "    Expected  duke.getDogYears()  to return 7. Actual value is " << duke.getDogYears() << "." << endl;
    cout << "    Expected  buddy.getDogYears()  to return 14. Actual value is " << buddy.getDogYears() << "." << endl;
    cout << "    Expected  simba.getDogYears()  to return 17.5. Actual value is " << simba.getDogYears() << "." << endl;
    cout << "    Expected  watson.getDogYears()  to return 10.5. Actual value is " << watson.getDogYears() << "." << endl << endl;


    // Print the stage of life of each dog to the console
    cout << "> Stages of life:" << endl;
    cout << "    Duke's stage of life: " << duke.getStage() << "." << endl;
    cout << "    Buddy's stage of life: " << buddy.getStage() << "." << endl;
    cout << "    Simba's stage of life: " << simba.getStage() << "." << endl;
    cout << "    Watson's stage of life: " << watson.getStage() << "." << endl << endl;


    cout << "> getMonths() method is tested in the constructor tests and the setMonths() tests when checking the value of  months. No separate test needed." << endl << endl;


    return 0;
}