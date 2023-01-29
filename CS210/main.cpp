#include <Python.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}


int main()
{
	ifstream inFS;     // Input file stream
	vector<string> item;
	vector<int> itemNum;
	string itemName;
	int numBought;
	int i;

	//Show menu
	while (true) {
		cout << "*******************************************************" << endl;
		cout << "*    1 - Display all items and # times purchased      *" << endl;
		cout << "*    2 - Display specific item and # times purchased  *" << endl;
		cout << "*    3 - Display graph of all items purchased         *" << endl;
		cout << "*    4 - Exit Program                                 *" << endl;
		cout << "*******************************************************" << endl;

		int userInput;  //obtain user input
		string userString; //obtain user input 
		cout << "Please enter a selection. 4 will exit program. " << endl;
		cin >> userInput;
		switch (userInput) {
		case 1: //call to Python function to output entire list and number of occurrences
			CallProcedure("wholeList");
			break;
		case 2: //call to python function to output specific item and number of times it occurred. 
			cout << "Enter the name of the item to see how many times it was purchased: " << endl;
			CallProcedure("specificItem");

			break;
		case 3: //call to Python function to output a graph of all items and number of occurrences
			CallProcedure("graphItems");
			inFS.open("frequency.dat"); //function to open file to read
			if (!inFS.is_open()) { //loop to validate opening of frequency.dat, created in Python
				cout << "Could not open file" << endl;
				return 1; // 1 indicates error
			}
			while (inFS.is_open()) {
				while (inFS >> itemName >> numBought) { //while loop to add itemName and numBought into their respective vectors, item, itemNum
					item.push_back(itemName);
					itemNum.push_back(numBought);
				}
				for (i = 0; i < item.size(); i++) { //for loop to print graph
					for (i = 0; i < itemNum.size(); i++) {
						cout << item.at(i)  << string(itemNum.at(i), '*') << endl;
					}
				}
				inFS.close();

				break;
		case 4:
			cout << "Goodbye.";
			return 0;
			break;
		default:
			cout << "Invalid input. Please try again" << endl;
			break;
			}
		}
	}
		return 0;
	}
