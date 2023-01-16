#include <Python.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <cmath>
#include <string>
#include "Validator.h"
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
	PyObject* my_module = PyImport_ImportModule("python_modules.python_code");
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
	pName = PyUnicode_FromString((char*)"python_modules.python_code");
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
	pName = PyUnicode_FromString((char*)"python_modules.python_code");
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

//Description: Outputs a message if the value is not within the range of min - max.
//@param validator - requires a validator object to be created. 
void handleInvalidInput(Validator validator) {

	cout << "Entered value is not valid. Valid range is [" << validator.getMinValue() << " - " << validator.getMaxValue() << "]" << endl;
}

//Description: Displays the main menu.
void displayMenu() {
	cout << "1 - Display total inventory." << endl;
	cout << "2 - Display frequency of a specific item." << endl;
	cout << "3 - Display text-based histogram and create a dat file containing total inventory." << endl;
	cout << "4 - Exit Program" << endl;
	cout << endl;
}

int main() {
	enum menuOptions { TOTAL_INVENTORY = 1, ITEM_FREQUENCY = 2, DISPLAY_HISTOGRAM, EXIT_PROGRAM = 4};
	const int MIN_MENU_OPTION = 1;
	const int MAX_MENU_OPTION = 4;
	int userMenuChoice;
	Validator menuValidator(MIN_MENU_OPTION, MAX_MENU_OPTION);

	do {
		displayMenu();
		cin >> userMenuChoice;

		while (!menuValidator.isValid(userMenuChoice)) {
			handleInvalidInput(menuValidator);
			cin >> userMenuChoice;
		}

		if (userMenuChoice == TOTAL_INVENTORY) {
			cout << endl;
			cout << "TOTAL INVENTORY:" << endl;
			CallProcedure("determineInventory");
			cout << endl;
		}

		else if (userMenuChoice == ITEM_FREQUENCY) {
			string userInput;
			cout << endl;
			cout << "Please enter an item to check: " << endl;
			cin >> userInput;
			int itemInventory = callIntFunc("determineFrequency", userInput);
			cout << endl;
			cout << "There are " << itemInventory << " " << userInput << " in inventory." << endl;
			cout << endl;
		}

		else if (userMenuChoice == DISPLAY_HISTOGRAM) {
			vector<string> items;
			vector<int> quantities;
			cout << endl;
			CallProcedure("output");

			//Reading in the frequency.dat file and populating the items and quantities vectors.
			ifstream inStream;
			inStream.open("frequency.dat");
			if (!inStream.is_open()) {
				cout << "File could not be opened!" << endl;
			}
			else {
				string item;
				int quantity;
				inStream >> item >> quantity;
				while (!inStream.fail()) {
					items.push_back(item);
					quantities.push_back(quantity);
					inStream >> item >> quantity;
				}
			}
			inStream.close();

			//outputing the histogram using the items and quantities vectors.
			cout << "Text-based histogram of total inventory:" << endl;
			for (int i = 0; i < items.size(); i++) {
				cout << left << setw(11) << items.at(i) << " ";
				for (int j = 0; j < quantities.at(i); j++) {
					cout << "*";
				}
				cout << endl;
			}
			cout << endl;
		}
	} while (userMenuChoice != EXIT_PROGRAM);

	return 0;
}