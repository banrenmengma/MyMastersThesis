#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

struct DataSet
{
	const int TaskNum;
	std::vector<std::vector<int> > prec;
	std::vector<std::vector<int> > succ;
	// std::vector<int> proc_time;
	std::vector<std::vector<int> > opt_srvcs;
	std::vector<std::vector<int> > srvcs_requst;
	std::vector<std::vector<int> > proc_time;
	std::vector<std::vector<int> > proc_cost;
	const int RsrcNum;
	const int SrvcNum;
	std::vector<int> RsrcCpcty;
	std::vector<int> SrvcCpcty;
	std::vector<std::vector<int> > CoeMatrix;
};

class DataInitialization
{
public:
	DataSet Initiate() {
 		ifstream in;
		string filename("d:\\rangen\\Pat1.rcp");
		// getline(cin, filename, '\n');

		in.open(filename);
		if(!in) {
			cerr << "Open Error" << endl;
		}

		char ch;
		int enter_count = 0;
		string str_temp = "";
		std::vector<int> vec;
		std::vector<std::vector<int> > succ;
		while(!in.eof()){
			in.read(&ch, 1);
			if(ch == '\n') enter_count++;
			if(enter_count >= 4) {
				
				// string str_temp = "";
				if(!(ch == ' ' || ch == '\n')){
					str_temp = str_temp + ch;
				}
				if((ch == ' ' || ch == '\n') && (!str_temp.empty())){
					int int_temp = atoi(str_temp.c_str());
					vec.push_back(int_temp);
					str_temp = "";
				}
				if(ch == '\n' && !vec.empty()){
					std::vector<int> vec1(vec.begin() + 6, vec.begin() + vec.size());
					succ.push_back(vec1);
					vec1.clear();
					vec.clear();
				}
			}
		}


	}	
};

int main(int argc, char const *argv[])
{
	ifstream in;
	string filename("d:\\rangen\\Pat1.rcp");
	// getline(cin, filename, '\n');

	in.open(filename);
	if(!in) {
		cerr << "Open Error" << endl;
		return 1;
	}

	char ch;
	int enter_count = 0;
	string str_temp = "";
	std::vector<int> vec;
	std::vector<std::vector<int> > succ;
	while(!in.eof()){
		in.read(&ch, 1);
		if(ch == '\n') enter_count++;
		if(enter_count >= 4) {
			
			// string str_temp = "";
			if(!(ch == ' ' || ch == '\n')){
				str_temp = str_temp + ch;
			}
			if((ch == ' ' || ch == '\n') && (!str_temp.empty())){
				int int_temp = atoi(str_temp.c_str());
				vec.push_back(int_temp - 1);
				str_temp = "";
			}
			if(ch == '\n' && !vec.empty()){
				std::vector<int> vec1(vec.begin() + 6, vec.begin() + vec.size());
				succ.push_back(vec1);
				vec1.clear();
				vec.clear();
			}
		}
	}

	in.close();

	const int TaskNum = succ.size();

	std::vector<std::vector<int> > prec;
	int coe_temp[TaskNum][TaskNum];

	for(size_t i = 0; i < TaskNum; ++i){
		for(size_t j = 0; j < TaskNum; ++j){
			coe_temp[i][j] = 0;	
		}
	} 

	for(size_t i = 0; i < TaskNum; ++i){
		for(size_t j = 0; j < succ[i].size(); ++j){
			coe_temp[i][succ[i][j]] = 1;
		}
	}

	std::vector<int> coe_vec_temp;
	for(size_t j = 0; j < TaskNum; ++j) {
		for (size_t i = 0; i < TaskNum; ++i) {
			if (coe_temp[i][j] == 1) {
				coe_vec_temp.push_back(i);
			}
		}
		prec.push_back(coe_vec_temp);
		coe_vec_temp.clear();
	}

	// size_t rand_seed = unsigned(time(NULL));
	size_t rand_seed = 1474622995;
	srand(rand_seed);
	double rand_0_1 = ((double)rand())/RAND_MAX;
	int SrvcNum = (int)((0.5 * rand_0_1 + 0.3) * TaskNum);
	int RsrcNum = (int)((0.1 + 0.2 * rand_0_1) * TaskNum);

	std::vector<std::vector<int> > opt_srvcs;
	std::vector<std::vector<int> > srvcs_requst;
	std::vector<std::vector<int> > proc_time;
	std::vector<std::vector<int> > proc_cost;
	int opt_set_num;
	int opt_srv_id;
	int rand_srv_req;
	int rand_srv_time;
	int rand_srv_cost;
	std::vector<int> opt_set_elm;
	std::vector<int> opt_set_req;
	std::vector<int> opt_set_time;
	std::vector<int> opt_set_cost;

	for (int i = 0; i < TaskNum; ++i){
		// std::vector<int> opt_set_elm;
		if (i == 0 || i == TaskNum - 1) {
			opt_set_elm.push_back(0);
			opt_set_req.push_back(0);
			opt_set_time.push_back(0);
			opt_set_cost.push_back(0);
			opt_srvcs.push_back(opt_set_elm);
			srvcs_requst.push_back(opt_set_req);
			proc_time.push_back(opt_set_time);
			proc_cost.push_back(opt_set_cost);
			opt_set_elm.clear();
			opt_set_req.clear();
			opt_set_time.clear();
			opt_set_cost.clear();
			continue;
		}
		opt_set_num = rand() % 5 + 1;
		for (int i = 0; i < opt_set_num; ++i) {
			if (i == 0) {
				opt_srv_id = rand() % (SrvcNum - 1) + 1;
				opt_set_elm.push_back(opt_srv_id);
			} else {
				opt_srv_id = rand() % (SrvcNum - 1) + 1;
				while(opt_srv_id == opt_set_elm.back()){
					opt_srv_id = rand() % (SrvcNum - 1) + 1;
				}
				opt_set_elm.push_back(opt_srv_id);
			}

			rand_srv_req = rand() % 3 + 1;
			opt_set_req.push_back(rand_srv_req);

			rand_srv_time = rand() % 10 + 1;
			opt_set_time.push_back(rand_srv_time);

			rand_srv_cost = rand() % 5 + 3;
			opt_set_cost.push_back(rand_srv_cost);
		}
		opt_srvcs.push_back(opt_set_elm);
		opt_set_elm.clear();

		srvcs_requst.push_back(opt_set_req);
		opt_set_req.clear();

		proc_time.push_back(opt_set_time);
		opt_set_time.clear();

		proc_cost.push_back(opt_set_cost);
		opt_set_cost.clear();
	}


	std::vector<std::vector<int> > CoeMatrix_trans;
	std::vector<std::vector<int> > CoeMatrix;
	std::vector<int> coe_par;
	int rand_rsrc_num;

	for (int i = 0; i < SrvcNum; ++i) {
		for (int j = 0; j < RsrcNum; ++j) {
			coe_par.push_back(0);
		}
		CoeMatrix_trans.push_back(coe_par);
		coe_par.clear();
	}

	for (int i = 0; i < SrvcNum; ++i) {
		rand_rsrc_num = rand() % 8 + 1;
		int j = 0;
		while (j < rand_rsrc_num) {
			int pos = rand() % RsrcNum;
			if (CoeMatrix_trans[i][pos] != 0) {
				continue;
			} else {
				int rand_rsrc_coe = rand() % 5 + 1;
				CoeMatrix_trans[i][pos] = rand_rsrc_coe;
				++j;
			}
		}
	}

	for (int i = 0; i < RsrcNum; ++i) {
		for (int j = 0; j < SrvcNum; ++j) {
			coe_par.push_back(0);
		}
		CoeMatrix.push_back(coe_par);
		coe_par.clear();
	}

	for (int i = 0; i < RsrcNum; ++i) {
		for (int j = 0; j < SrvcNum; ++j) {
			CoeMatrix[i][j] = CoeMatrix_trans[j][i];
		}
	}

	std::vector<int> RsrcCpcty;
	

	for(int i = 0; i < RsrcNum; ++i) {
		RsrcCpcty.push_back(rand() % 10 + 15);
	}

	std::vector<int> SrvcCpcty;
	int temp_max[SrvcNum];
	for (int i = 0; i < SrvcNum; ++i) {
		temp_max[i] = (int)(INT_MAX / 2);
		for (int j = 0; j < RsrcNum; ++j) {
			if (CoeMatrix_trans[i][j] > 0) {
				temp_max[i] = min(temp_max[i], RsrcCpcty[j] / CoeMatrix_trans[i][j]);
			}
		}
		SrvcCpcty.push_back(temp_max[i]);
	}

	ofstream outfile;
	string ofname("d:\\rangen\\MyData1.rcp");
	outfile.open(ofname);
	
	if (!outfile) {
		cerr << "Open Error" << endl;
	}

	string divide_line(100, '*');

	outfile << left << setw(15) << "random seed" << ":" << '\t' << rand_seed << endl;

	outfile << divide_line << endl;

	outfile << left << setw(10) << "projects" << ":" << '\t' << 1 << endl;
	outfile << left << setw(10) << "tasks" << ":" << '\t' << TaskNum << endl; 
	outfile << left << setw(10) << "resources" << ":" << '\t' << RsrcNum << endl;
	outfile << left << setw(10) << "services" << ":" << '\t' << SrvcNum << endl;
	
	outfile << divide_line << endl;

	outfile << "PROJECT INFORMATION:" << endl;
	outfile << left << setw(15) << "TaskNo" << left << setw(15) << "Modes" << left << setw(15) << "Demands" << left << setw(15) << "Duration" << left << setw(15) << "Cost" << left << setw(15) << "Successors" << endl;
	for (int i = 0; i < TaskNum; ++i) {
		for (int j = 0; j < opt_srvcs[i].size(); ++j) {
			if (j == 0) {
				outfile << left << setw(15) << i << left << setw(15) << opt_srvcs[i][j] << left << setw(15) << srvcs_requst[i][j] << left << setw(15) << proc_time[i][j] << left << setw(15) << proc_cost[i][j];
				for (int k = 0; k < succ[i].size(); ++k) {
					outfile << left << setw(4) << succ[i][k]; 
				}  
				outfile << endl;
			} else {
				outfile << left << setw(15) << "" << left << setw(15) << opt_srvcs[i][j] << left << setw(15) << srvcs_requst[i][j] << left << setw(15) << proc_time[i][j] << left << setw(15) << proc_cost[i][j] << endl;
			}
		}
	} 

	outfile << divide_line << endl;

	outfile << "SERVICE-RESOURCE COEMATRIX" << endl;

	outfile << left << setw(6) << "";
	for (int i = 0; i < RsrcNum; ++i) {
		outfile << left << setw(1) << 'R' << left << setw(5) << (i + 1); 
	}
	outfile << endl;

	for (int i = 0; i < SrvcNum; ++i) {
		outfile << left << setw(1) << 'S' << left << setw(5) << i;
		for (int j = 0; j < RsrcNum; ++j) {
			outfile << left << setw(6) << CoeMatrix_trans[i][j] ;
		} 
		outfile << endl;
	}

	outfile << divide_line << endl;

	outfile << "RESOURCE CAPACITY" << endl;

	outfile << left << setw(6) << "";

	for (int i = 0; i < RsrcNum; ++i) {
		outfile << left << setw(1) << 'R' << left << setw(5) << (i + 1); 
	}
	outfile << endl;

	outfile << left << setw(6) << "";

	for (int i = 0; i < RsrcNum; ++i) {
		outfile << left << setw(6) << RsrcCpcty[i];
	}
	outfile << endl;

	outfile.close();
}

