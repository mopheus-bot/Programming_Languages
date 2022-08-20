#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;


// calls a python function with no return value
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


// calls a python function and passes a string as a parameter for that function
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


// prints menu and gets menu choice from user, returning the menu choice
// includes input validation
int PrintMenuOptions() {
	int userChoice = 0;

	cout << "------------------ MAIN MENU ------------------" << endl;
	cout << "1: View all items with purchase frequencies" << endl;
	cout << "2: View specific item with purchase frequency" << endl;
	cout << "3: View histogram of all products sold" << endl;
	cout << "4: Exit program" << endl;
	cout << "Please select a menu option: ";

	cin.exceptions(istream::failbit);
	try {
		cin >> userChoice;
	}

	catch (ios_base::failure& fail) {
		cin.clear();
		string tmp;
		getline(cin, tmp);
		throw runtime_error("\nPlease enter an integer: 1, 2, 3, or 4.");
	}

	cout << endl;

	return userChoice;
}


// Open file created by Python to read products that have sold and their frequencies
// close file when completed
void ReadOutputFile() {
	ifstream inFS;						// input file stream
	string fileProduct;					// to store current product being read
	string fileFreq;					// to store frequency for current product
	int freq;							// for type conversion

	// Open file
	inFS.open("frequency.dat");

	if (!inFS.is_open()) {
		cout << "Could not open file frequency.dat" << endl;
	}

	// While loop, runs until the end of the file is reached
	while (!inFS.eof()) {
		inFS >> fileProduct;
		inFS >> fileFreq;

		if (!inFS.fail()) {
			cout << setfill(' ') << right << setw(16) << fileProduct;
			// convert string frequency number to int, add one to account for space in output statement
			freq = stoi(fileFreq) + 1;
			cout << setfill('*') << left << setw(freq) << " " << endl;
		}
	}
	// Close file when done reading
	inFS.close();
}


int main() {
	int menuSelection = 0;
	string itemChoice;

	while (menuSelection != 4) {
		try {
			menuSelection = PrintMenuOptions();

			switch (menuSelection) {
			case 1:
				cout << "\n" << endl;
				CallProcedure("PrintAllItems");
				break;

			case 2:
				cout << "\nPlease enter an item to search for: ";
				cin >> itemChoice;
				cout << "\nYou sold " << callIntFunc("ReturnSpecificItem", itemChoice) << " ";
				itemChoice[0] = tolower(itemChoice[0]);
				cout << itemChoice << ".\n\n\n" << endl;
				break;

			case 3:
				cout << "\n" << endl;
				CallProcedure("CreateOutputFile");
				ReadOutputFile();
				cout << "\n\n" << endl;
				break;

			case 4:
				break;

			default:
				throw runtime_error("\nPlease enter an integer: 1, 2, 3, or 4.");
			}
		}

		catch (runtime_error& excpt) {
			cout << excpt.what() << "\n\n" << endl;
			continue;
		}
	}
	return 0;
}