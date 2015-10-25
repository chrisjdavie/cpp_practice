//============================================================================
// Name        : main.cpp
// Author      : Chris Davie
// Version     : 1.0
// Description : C/C++ practice
//============================================================================

#include <stdio.h>
#include <vector>
#include <iostream>

#include "read_file.h"
#include "volt_prof.h"
#include "analysis.h"

using namespace std;


int main() {

	// cin filename

	string fname;
	//	fname = "data/voltages.txt";

	cout << "Please input filename" << endl;
	getline(cin,fname);
	cout << endl;

	// voltages
	vector<int> VV_0;
	vector<int> VV_1;

	// Names of sensor (0 and 1)
	vector<string> hheaders;


	// Open file, load in values

	read_in_data( fname, VV_0, VV_1, hheaders );


	// for each jump, analyse data and output results

	vector< vector <int> > VVV; // voltage data for both jumps

	VVV.push_back(VV_0);
	VVV.push_back(VV_1);

	for (size_t o = 0; o < VVV.size(); o++ ){

		vector<int> VV;

		VV = VVV[o];

		// gets the volterature profile for a jump

		vector< vector<size_t> > nnn_jump; // start and end indices for each jump

		int V_bg; // ambient volterature

		get_volt_prof_each_jump( VV, nnn_jump, V_bg );

		vector<int> VV_jump_max;
		vector<int> VV_jump_av;

		analysis( VV, nnn_jump, VV_jump_max, VV_jump_av );
		// from the profiles gets the ambient volt, the peak volt and
		// the median volt for each jump

		cout << hheaders[o] << " sensor" << endl;
		cout << endl;
		cout << "Ambient voltage:      " << V_bg << endl << endl;

		for ( size_t i = 0; i < VV_jump_max.size(); i++ ){
			cout << "jump " << i << endl;
			cout << "Maximum jump voltage: " << VV_jump_max[i] << endl;
			cout << "Average jump voltage: " << VV_jump_av[i] << endl;
			cout << endl;
		}

		cout << endl;
	}

	return 0;
}
