/*
 * volt_prof.cpp
 *
 *  Created on: 18 Oct 2015
 *      Author: chris
 */

#include <algorithm>
#include <vector>

using namespace std;

int get_background_volt( vector<int> VV, int &V_bg );
int remove_background( vector<int> VV, int V_bg, vector<int> &VV_clean );
int split_into_jumps( vector<int> VV, int V_bg, vector< vector<size_t> > &nnn_jump );

int get_volt_prof_each_jump(vector<int> VV, vector< vector<size_t> > &nnn_jump, int &V_bg){

	// gets the voltage profile for each jump
	// input : VV - voltage data from sensors
	// output: nnn_jump - start and end indices for each jump
	//		   V_bg - background voltage

	get_background_volt( VV, V_bg );

	// Remove the background fluctuations
	vector<int> VV_clean;
	remove_background( VV, V_bg, VV_clean );

	// Split the data into the left and right jumps
	split_into_jumps( VV_clean, V_bg, nnn_jump );

	return 0;
}

int get_background_volt( vector<int> VV, int &V_bg ){

	// Assuming background volt is the most common voltage
	//
	// input : VV - voltage data from sensors
	// output: V_bg - background (modal) voltage

	//// Finding unique volt and how many occurrences

	sort( VV.begin(), VV.end() );

	vector<int> VV_un;
	vector<int> cts_un;

	VV_un.push_back( VV[0] );
	int i_curr = 1;

	for (size_t n = 1; n < VV.size(); n++ ){
		if ( VV[ n ] != VV[ n-1 ] ){
			cts_un.push_back( i_curr );

			VV_un.push_back( VV[ n ] );
			i_curr = 1;
		}
		i_curr++;
	}

	cts_un.push_back( i_curr );


	//// Finding the unique volt with the most occurrences

	int V_mode = -1;
	int cts_un_max = -1;

	for (size_t n = 0; n < VV_un.size(); n++ ){
		if ( cts_un[ n ] > cts_un_max ){
			V_mode = VV_un[ n ];
			cts_un_max = cts_un[ n ];
		}
	}

	V_bg = V_mode;

	return 0;
}

int remove_background( vector<int> VV, int V_bg, vector<int> &VV_clean ){

	// remove background variation from baseline (+- 1)
	// input - VV - voltage data from sensors
	//		 - V_bg - background (modal) voltage
	// output - VV_clean - voltage data with background removed

	//// find smallest fluctuation

	int DV_min = 1e7;

	for (size_t n = 0; n < VV.size() - 1; n++ ){
		int DV = abs(VV[ n ] - VV[ n + 1 ]);
		if ( DV > 0 && DV < DV_min )
			DV_min = DV;
	}

	//// if fluctuation is DV from the background, replace
	//// with background

	VV_clean = VV;

	for (size_t n = 0; n < VV.size() - 1; n++ ){
		if ( DV_min == abs(V_bg - VV[ n ])
				&& DV_min >= abs(V_bg - VV[ n+1 ]) ){
			VV_clean[ n ] = V_bg;
		}
	}

	return 0;
}

int split_into_jumps( vector<int> VV, int V_bg, vector< vector<size_t> > &nnn_jump ){

	// Split the data into jumps
	// input : VV - voltage profile from the sensors (with the noise removed)
	//		   V_bg - background voltage
	// output: nnn_jump - start and end indices for each jump

	//// 2 criteria - above background voltage
	////            - the voltage increase lasts for a time
	////			- than a time period n

	size_t n_spike = 6;

	bool b_chunk = false;
	size_t n_chunk_s = VV.size() + 1;
	vector<size_t> n_se(2);
	int V;
	int k = 0;

	for (size_t n = 0; n < VV.size() - 1; n++ ){

		V = VV[ n ];

		// chunk start

		if (V > V_bg && !b_chunk){
			k++;
			b_chunk = true;
			n_chunk_s = n;
		}

		// chunk end

		if ( V == V_bg && b_chunk ){
			if ( n - n_chunk_s > n_spike ){
				n_se[0] = n_chunk_s;
				n_se[1] = n - 1;

				nnn_jump.push_back(n_se);
			}
			b_chunk = false;
			n_chunk_s = -1;
		}

	}

	return 0;
}
