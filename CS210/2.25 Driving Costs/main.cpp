#include <iostream>
#include <iomanip>               //For setprecision
using namespace std;

int main() {

 double milesPerGallon;
 double dollarsPerGallon;
 double totalCost;
 double totalCost2;
 double totalCost3;
 double milesDriven1;
 double milesDriven2;
 double milesDriven3;
 
   milesPerGallon = 0;
    cin >> milesPerGallon;
   dollarsPerGallon = 0;
    cin >> dollarsPerGallon;
    
   milesDriven1 = 20;
   milesDriven2 = 75;
   milesDriven3 = 500;
   
   totalCost = (milesDriven1 * dollarsPerGallon) / milesPerGallon;
   totalCost2 = (milesDriven2 * dollarsPerGallon) / milesPerGallon;
   totalCost3 = (milesDriven3 *dollarsPerGallon ) / milesPerGallon;
   
   cout << fixed << setprecision(2) << totalCost << " ";
   cout << totalCost2 << " ";
   cout << totalCost3 << endl;
   return 0;
}
