#include <iostream>
#include <iomanip>               // For setprecision
using namespace std;

double DrivingCost(double drivenMiles, double milesPerGallon, double dollarsPerGallon){ //function declaration with parameters
   double totalCost;
   totalCost = (drivenMiles * dollarsPerGallon) / milesPerGallon; //definition of function -- what is the function doing?
   return totalCost;                                             //return totalCost to output the total cost of user input
}
int main() {
   double userMPG = 0.0; //declaration of userMPG for user input
   double userDollarsPerGallon = 0.0; //declaration of userDollarsPerGallon for user input
   
   
   cin >> userMPG >> userDollarsPerGallon; //where user input is obtained
   
   cout << fixed << setprecision(2); //formatting future print statements
   // Below is three print statements, each calling for DrivingCost function, where 1 parameter is input in program, rest is
   cout << DrivingCost(10.0, userMPG, userDollarsPerGallon) << " ";                          // user dependent
   cout << DrivingCost(50.0, userMPG, userDollarsPerGallon) << " ";
   cout << DrivingCost(400.00, userMPG, userDollarsPerGallon) << endl;


   return 0;
}
