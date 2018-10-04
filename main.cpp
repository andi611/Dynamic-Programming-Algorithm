/****************************************************************************
  FileName     [ Bezier_Curver_Plotter.cpp ]
  Synopsis     [ Implementation of a bezier curver plotter ]
  Author       [ Ting-Wei (Andy) Liu ]
  Copyright    [ Copyleft(c), NTUEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include <float.h>
#include <utility>
using namespace std;


/**************************************/
/*          Static varaibles          */
/**************************************/
static string str_buf;


/*********************************************/
/*        class Bezier_Curver_Plotter        */
/*********************************************/
class Bezier_Curver_Plotter {
public:
    // utility functions
	bool read_input_table (const string&);
    void display_input (bool display_table = false) const;
    void display_output () const;
    void save_output (const string&) const;

    // conputational functions
    void compute_BC_memorization ();
    void compute_BC_iteration ();

private:
    // class members
    vector< vector<int> > input_table;
    vector< pair<double, double> > output_table;
    int n_control_points;
    int n_sampled_points;

    // private functions
    pair<double, double> compute_BC_xy_m (const int& i, const int& n, const long double& t, vector< vector<double> >& mem_x, vector< vector<double> >& mem_y) const;
    pair<double, double> compute_BC_xy_i (const long double& t, vector<double>& mem_x, vector<double>& mem_y) const;

	// helper functions
    vector< vector<double> > get_mem_matrix_m() const {return vector< vector<double> >(n_control_points, vector<double>(n_control_points+1, FLT_MAX)); }// initialize a N x N matrix
    vector<double> get_mem_matrix_i() const {return vector<double> (n_control_points, 0.0); }// initialize a size N vector
	void str2int (const string& str, int& num);
	bool pack_tokens (const string&, vector<string>&, size_t nOpts = 0) const;
	size_t get_tok_first (const string&, string&, size_t pos = 0) const;
	size_t get_tok (const string&, string&, size_t pos = 0) const;

 };


/***************************************************************/
/*        class Bezier_Curver_Plotter utility functions        */
/***************************************************************/
bool Bezier_Curver_Plotter::read_input_table (const string& input_file)
{
	// opening file
	ifstream ifs(input_file.c_str());
	if (!ifs) {
		cerr << "Cannot open file \"" << input_file << "\"!" << endl;
		return false; }

	// get first line
	vector<string> tokens;
	if (!getline(ifs, str_buf)) { 
		cerr << "Failed to read input file!" << endl; return false; }
	if (!pack_tokens(str_buf, tokens, 1)) { 
		cerr << "Failed when processing input file!" << endl; return false; }
	str2int(tokens.at(0), n_control_points);

	// get middle lines
	for (int i = 0; i < n_control_points; ++i) {
		assert(getline(ifs, str_buf));
		assert(pack_tokens(str_buf, tokens, 2));
		vector<int> line;
		for (int j = 0; j < 2; ++j) {
			int holder = 0;
			str2int(tokens.at(j), holder);
			line.push_back(holder);
		}
		input_table.push_back(line);
	}

    // get last line
    assert(getline(ifs, str_buf));
    assert(pack_tokens(str_buf, tokens, 1));
    str2int(tokens.at(0), n_sampled_points);
	return true;
}

void Bezier_Curver_Plotter::display_input(bool display_table) const {
	if (display_table == true) {
        for (int i = 0; i < input_table.size(); ++i) {
    		for (int j = 0; j < input_table.at(i).size(); ++j) {
    			cout << input_table.at(i).at(j) << ' ';
    		}
    		cout << endl;
    	}
    }
    cout << "Number of control points: " << n_control_points << endl;
    cout << "Number of sampled points: " << n_sampled_points << endl;
    cout << "Number of rows in input table: " << input_table.size() << endl;
}


void Bezier_Curver_Plotter::display_output() const {
    for (int i = 0; i < output_table.size(); ++i) {
        cout << fixed << setprecision(2) << output_table.at(i).first << '\t';
        cout << fixed << setprecision(2) << output_table.at(i).second << endl;
    }
}

void Bezier_Curver_Plotter::save_output(const string& output_file) const {
    ofstream file;
    file.open (output_file.c_str());
    file << fixed << setprecision(2);
    for (int i = 0; i < output_table.size(); ++i) {
            file << output_table.at(i).first << '\t';
            file << output_table.at(i).second << '\n';
    }
    file.close();
    cout << "Result successfully saved to " << output_file << endl;
}


/*********************************************************************/
/*        class Bezier_Curver_Plotter computational functions        */
/*********************************************************************/
void Bezier_Curver_Plotter::compute_BC_memorization () {
    long double t = 0.0;
    long double t_interval = (1.0/(n_sampled_points-1));
    for (int i = 0; i < n_sampled_points; ++i) {
        vector< vector<double> > mem_x = get_mem_matrix_m();
        vector< vector<double> > mem_y = get_mem_matrix_m();
        output_table.push_back(compute_BC_xy_m(0, n_control_points, t, mem_x, mem_y));
        t += t_interval;
    }
}

pair<double, double> Bezier_Curver_Plotter::compute_BC_xy_m (const int& i, const int& n, const long double& t, vector< vector<double> >& mem_x, vector< vector<double> >& mem_y) const {
    if (i == n - 1) return make_pair(input_table.at(i).at(0), input_table.at(i).at(1));
    else if (mem_x.at(i).at(n) != FLT_MAX) return make_pair(mem_x.at(i).at(n), mem_y.at(i).at(n));
    else {
        pair<double, double> xy_left = compute_BC_xy_m(i, n-1, t, mem_x, mem_y);
        pair<double, double> xy_right = compute_BC_xy_m(i+1, n, t, mem_x, mem_y);
        mem_x.at(i).at(n) = ((1-t) * xy_left.first + t * xy_right.first);
        mem_y.at(i).at(n) = ((1-t) * xy_left.second + t * xy_right.second);
        return make_pair(mem_x.at(i).at(n), mem_y.at(i).at(n));
    }
}

void Bezier_Curver_Plotter::compute_BC_iteration () {
    long double t = 0.0;
    long double t_interval = (1.0/(n_sampled_points-1));
    vector<double> mem_x = get_mem_matrix_i();
    vector<double> mem_y = get_mem_matrix_i();
    output_table.resize(n_sampled_points);
    for (int i = 0; i < n_sampled_points; ++i) {
        output_table.at(i) = compute_BC_xy_i(t, mem_x, mem_y);
        t += t_interval;
    }
}

pair<double, double> Bezier_Curver_Plotter::compute_BC_xy_i (const long double& t, vector<double>& mem_x, vector<double>& mem_y) const {
    int n = 1;
    long double one_minus_t = 1-t;
    // compute p0p1, p1p2, p2p3 ...
    for (int i = 0; i+n < n_control_points; ++i){
        mem_x.at(i) = one_minus_t * input_table.at(i).at(0) + t * input_table.at(i+n).at(0); // mem_x.at(i).at(i+n) = one_minus_t * input_table.at(i).at(0) + t * input_table.at(i+n).at(0);
        mem_y.at(i) = one_minus_t * input_table.at(i).at(1) + t * input_table.at(i+n).at(1); // mem_y.at(i).at(i+n) = one_minus_t * input_table.at(i).at(1) + t * input_table.at(i+n).at(1);
    }
    // compute p0p1p2, p1p2p3, p2p3p4 ... p0p1p2p3, p1p2p3p4, p2p3p4p5 ... p0p1p2p3p4, p1p2p3p4p5 ...
    while (n < n_control_points-1) {
        n += 1;
        for (int i = 0; i+n < n_control_points; ++i){
            mem_x.at(i) = one_minus_t * mem_x.at(i) + t * mem_x.at(i+1); // mem_x.at(i).at(i+n) = one_minus_t * mem_x.at(i).at(i+n-1) + t * mem_x.at(i+1).at(i+n);
            mem_y.at(i) = one_minus_t * mem_y.at(i) + t * mem_y.at(i+1); // mem_y.at(i).at(i+n) = one_minus_t * mem_y.at(i).at(i+n-1) + t * mem_y.at(i+1).at(i+n);
        }
    }
    return make_pair(mem_x.at(0), mem_y.at(0)); // mem_x.at(0).at(n), mem_y.at(0).at(n)
}


/**************************************************************/
/*        class Bezier_Curver_Plotter helper functions        */
/**************************************************************/
void Bezier_Curver_Plotter::str2int(const string& str, int& num)
{
   num = 0;
   size_t i = 0;
   int sign = 1;
   if (str[0] == '-') { sign = -1; i = 1; }
   for (; i < str.size(); ++i) {
      if (isdigit(str[i])) {
         num *= 10;
         num += int(str[i] - '0');
      }
   }
   num *= sign;
}

// split a string into tokens and pack them
// if nOpts is specified (!= 0), the number of tokens must be exactly = nOpts
// Otherwise, return false.
//
bool Bezier_Curver_Plotter::pack_tokens (const string& buf, vector<string>& tokens, size_t nOpts) const
{
    string token;
    if(!tokens.empty()) tokens.clear();
    size_t n = get_tok_first(buf, token);
    while (token.size()) {
        tokens.push_back(token);
        n = get_tok(buf, token, n);
    }
    if (nOpts != 0) {
        if (tokens.size() < nOpts) {
        	return false;
        }
        if (tokens.size() > nOpts) {
        	return false; }
    }
    return true;
}

// Parse the string "str" for the token "tok", beginning at position "pos",
// with delimiter "del". The leading "del" will be skipped.
// Return "string::npos" if not found. Return the past to the end of "tok"
// (i.e. "del" or string::npos) if found.
//
size_t Bezier_Curver_Plotter::get_tok_first(const string& str, string& tok, size_t pos) const
{   
	size_t begin = str.find_first_not_of('\t', pos);
	begin = str.find_first_not_of(' ', pos);
	if (begin == string::npos) { tok = ""; return begin; }
	assert(begin - pos == 0); // error handling: begin - pos should be 0 for the first col;
	size_t end = str.find_first_of(' ', begin);
	tok = str.substr(begin, end - begin);
	return end;
}

size_t Bezier_Curver_Plotter::get_tok(const string& str, string& tok, size_t pos) const
{   
    size_t begin = str.find_first_not_of(' ', pos);
    if (begin == string::npos) { tok = ""; return begin; }
    assert (begin - pos == 1); // error handling: begin - pos should be 1
    size_t end = str.find_first_of(' ', begin);
    tok = str.substr(begin, end - begin);
    return end;
}


/**********************************/
/*          Main Function         */
/**********************************/
int main(int argc, char** argv)
{   
    // Settings
    bool show_input = false;
    bool show_output = false;
    bool use_DP_memorization = false;
    bool use_DP_iteration = true;

    if (argc != 3) { // argument count
        cout << "Illegal number of arguments! Terminating program!" << endl; 
        return 0;
    }
    
    Bezier_Curver_Plotter* BC_Plotter = new Bezier_Curver_Plotter();

    BC_Plotter->read_input_table(argv[1]);
	BC_Plotter->display_input(show_input);

    cout << "Computing bezier curve..." << endl;
    if (use_DP_memorization) BC_Plotter->compute_BC_memorization();
    if (use_DP_iteration) BC_Plotter->compute_BC_iteration();

    if (show_output) BC_Plotter->display_output();
    BC_Plotter->save_output(argv[2]);

    return 0;
}

