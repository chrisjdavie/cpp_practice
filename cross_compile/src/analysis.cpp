/*
 * analysis.cpp
 *
 *  Created on: 18 Oct 2015
 *      Author: chris
 */
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int analysis(vector<int> VV, vector< vector<size_t> > nnn_jump, vector<int> &VV_jump_max, vector<int> &VV_jump_av){

	// Finds the maximum and average (median) voltages for each jump
	// inputs - VV - voltage profile from the sensors
	//	      - nnn_jump - start and end indices for each jump
	// outputs - VV_jump_max - maximum voltages for each jump
	//		   - VV_jump_av - average (median) voltages for each jump

	for ( size_t i = 0; i < nnn_jump.size(); i++ ){

		vector<size_t> nn;

		nn = nnn_jump[i];

		// find maximum volt for each jump, also extract V data for
		// the jump


		double V_jump = -1;
		double V_max = -1;

		vector<double> VV_jump( nn[1]-nn[0] );

		int j = 0;
		for ( size_t n = nn[0]; n < nn[1]; n++ ){
			int V;
			V = VV[ n ];
			if ( V > V_max )
				V_max = V;

			VV_jump[ j ] = V;
			j++;
		}

		// find the median voltage

		sort( VV_jump.begin(), VV_jump.end() );

		if ( VV_jump.size()%2 == 1 ){
			V_jump = VV_jump [ (VV_jump.size() - 1)/2 ];
		} else {
			int V_l = VV_jump [ (VV_jump.size() - 1)/2 ];
			int V_r = VV_jump [ (VV_jump.size() - 1)/2 + 1 ];
			V_jump = ( double(V_l) + double(V_r) )/2.0;
		}

		VV_jump_av.push_back( V_jump );
		VV_jump_max.push_back( V_max );

	}
	return 0;
}

// Was using the minimum gradient V instead of average V,
// but that's not the specification

//		vector<double> ddV;
//		vector<double> nn_dV;
//		size_t n_Vm1 = nn[0];
//		int V_m1 = VV[ nn[0] ];
//
//		double dV_min = 1.0e15;
//			if ( V_m1 != VV[ n ] ){
//
//				int V;
//				double dV;
//
//				V = VV[ n ];
//				dV = abs(double( V - V_m1 )/double( n - n_Vm1 ));
//
//				if ( dV < dV_min ){
//					V_jump = V_m1;
//					dV_min = dV;
//				}
//				V_m1 = V;
//				n_Vm1 = n;
//
//
//
//
//			}



