/*
 * read_file.cpp
 *
 *  Created on: 18 Oct 2015
 *      Author: chris
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int read_in_data( string fname, vector<int> &VV_0, vector<int> &VV_1, vector<string> &hheaders ){

	// Open file, load in values
	// Checks for file existence and valid file format

	ifstream dat_file (fname.c_str());

	if ( dat_file )
	{
		try{
			string line;
			getline( dat_file,line );  // headers

			split( hheaders, line, is_any_of(" ")  );
			if ( hheaders.size() != 2 )
				throw 2; // empty file or incorrect first line

			hheaders[1] = hheaders[1].substr(0, hheaders[1].size()-1);

			while ( getline(dat_file,line) )
			{
				vector <string> fields;
				split( fields, line, is_any_of("\t")  );
				if ( fields.size() != 2 )
					throw 3; // incorrect column layout

				long V;
				char *p;
				V = strtol( fields[ 0 ].c_str(), &p, 10 );
				if(*p)
					throw 4; // not numeric
				VV_0.push_back( int(V) );

				fields[1] = fields[1].substr(0, fields[1].size()-1);
				V = strtol( fields[ 1 ].c_str(), &p, 10 );
				if(*p)
					throw 5; // not numeric

				VV_1.push_back( int(V) );
			}

			if ( VV_0.size() == 0 )
				throw 6; // header only

			dat_file.close();

		} catch( int e ) {
			cout << "Invalid input file." << endl;
			cout << e << endl;
			exit(e);
		}


	} else {
		cout << "Error in file handling." << endl;
		cout << "Might be an invalid filename, please try again." << endl;
		exit(1);
	}

	return 0;
}


