#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <algorithm>
#include "LinkedPtrSkipList.cpp"

class MyObj
{
	public:
	int ID;
};

MyObj* CreateTestObj() {
	static int counter = 0;

	MyObj* obj = new MyObj();
	obj->ID = counter++;
	return obj;
}

string randString() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    int size = (rand() % 10 ) + 1;
    string value = "";
    for (int i = 0; i < size; ++i) {
        value += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return value;
}

void printpair(LinkedPtrSkipList<MyObj>::skipListPair p) {
	cout << endl << p.Key() << ":" << p.Object()->ID;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	LinkedPtrSkipList<MyObj>* list = new LinkedPtrSkipList<MyObj>();
	list->insert("A",CreateTestObj());
	list->insert("B",CreateTestObj());
	list->insert("C",CreateTestObj());
	list->insert("E",CreateTestObj());
	list->insert("D",CreateTestObj());
	list->insert("Bag",CreateTestObj());
	list->insert("Aag",CreateTestObj());
	list->insert("Ded",CreateTestObj());
	list->insert("Debil",CreateTestObj());
	list->insert("Hello",CreateTestObj());
	list->insert("Gambit",CreateTestObj());
	list->insert("Zong$",CreateTestObj());
	list->insert("zed",CreateTestObj());
/*  
	for(int n = 0; n < 10000; n++) {
		list->insert(randString(),CreateTestObj());
	}
*/
	list->debugPrint();

	LinkedPtrSkipList<MyObj>::iterator itr;
	for ( itr = list->begin() ; itr != list->end( ) ; itr++ ){
		LinkedPtrSkipList<MyObj>::skipListPair p = *itr;
		cout << endl << p.Key() << "," << p.Object()->ID;
	}
	cout << endl << endl;

	for_each ( list->begin(), list->end(), printpair);
	cout << endl;

	MyObj* bag = list->find("Bag");
	cout << endl << "Found the ID under the key Bag is:" << bag->ID;

	MyObj* gidi = list->find("Gidi");
	if(gidi == NULL) {

		cout << endl << "Couldn't find anything under the Key Gidi." << endl;
	} else {
		cout << endl << "Found the ID under the key Gid is:" << gidi->ID << endl;
	}

	cout << endl << "Remove Debil" << endl;

	MyObj* ded = list->remove("Debil");
	if(ded != NULL) {
		cout << endl << "Remove returned node " << ded->ID << endl;
	} else {
		cout << endl << "Ded wan't found!" << endl;
	}
	list->debugPrint();

	if(list->exists("Aag")) {
		cout << endl << "Aag exists!" << endl;
	} else {
		cout << endl << "Aag doesn't exist..." << endl;
	}
	
	if(list->exists("Debil")) {
		cout << endl << "Debil exists!" << endl;
	} else {
		cout << endl << "Debil doesn't exist..." << endl;
	}

	cout << endl;
	LinkedPtrSkipList<MyObj>* listcopy = new LinkedPtrSkipList<MyObj>(list);
	listcopy->debugPrint();

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */