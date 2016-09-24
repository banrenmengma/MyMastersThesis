#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
using namespace std;

struct DataSet
{
	const int TaskNum;
	std::vector<std::vector<int> > prec;
	std::vector<int> proc_time;
	std::vector<std::vector<int> > opt_srvcs;
	std::vector<std::vector<int> > srvcs_requst;
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
		const int TaskNum = rand() % 10 + 10;
		const int GroupNum = rand() % 4 + 4;
		int temp = (int)(TaskNum - 3)/2;
		if (temp == 0) temp++;
		int loop_num = (int)(1.5 * TaskNum - 1) - (int)(rand() % temp) + (int)(rand() % temp);

		std::vector<int> TaskAllocate;
		for (int i = 0; i < TaskNum; ++i) {
			TaskAllocate.push_back(i);
		}
		random_shuffle(TaskAllocate.begin() + 1, TaskAllocate.end() - 1);
		std::vector<int> vec_split;
		vec_split.push_back(1);
		vec_split.push_back(TaskNum - 1);
		for (int i = 0; i < GroupNum - 1; ++i) {
			int r_n = rand() % (TaskNum - 1) + 1;
			while (isIn(vec_split, r_n)) {
				r_n = rand() % (TaskNum - 1) + 1;
			}
			vec_split.push_back(r_n);
		}
		sort(vec_split.begin(), vec_split.end());
		std::vector<std::vector<int> > vec_allocate;
		for (int i = 0; i < GroupNum; ++i) {
			std::vector<int> vec(TaskAllocate.begin() + vec_split[i], TaskAllocate.begin() + vec_split[i + 1]);
			vec_allocate.push_back(vec);
			vec.clear();
		}


		// int bottom = TaskNum + GroupNum - 2;
		// int top = 

		std::vector<std::vector<int> > matrix;
		std::vector<int> vec(TaskNum, 0);
		for (int i = 0; i < TaskNum; ++i) {
			matrix.push_back(vec);
		}

		for (size_t i = 0; i < vec_allocate.size(); ++i) {
			if (vec_allocate[i].size() != 2) {
				for (size_t j = 0; j < vec_allocate[i].size(); ++j) {
					if (j == 0) {
						matrix[0][vec_allocate[i][j]] = 1;
					}
					if (j == vec_allocate[i].size() - 1) {
						matrix[vec_allocate[i][j]][TaskNum - 1] = 1;
					}
					if (j != 0 && j != vec_allocate[i].size() - 1) {
						matrix[vec_allocate[i][j - 1]][vec_allocate[i][j]] = 1;
						matrix[vec_allocate[i][j]][vec_allocate[i][j + 1]] = 1;
					}
				}
			} else {
				matrix[0][vec_allocate[i][0]] = 1;
				matrix[vec_allocate[i][0]][vec_allocate[i][1]] = 1;
				matrix[vec_allocate[i][1]][TaskNum - 1] = 1;
			}		
		}

		std::vector<std::vector<int> > matrix_init = matrix;
		int loop_count = 0;
		while (noZeroNum(matrix) < loop_num) {
			int index1 = rand() % vec_allocate.size();
			int index2 = rand() % vec_allocate.size();
			while (index2 == index1) index2 = rand() % vec_allocate.size();
			int index3 = rand() % vec_allocate[index1].size();
			int index4 = rand() % vec_allocate[index2].size();
			// while (index4 == index3) index4 = rand() % vec_allocate[index2].size();
			if (matrix[vec_allocate[index1][index3]][vec_allocate[index2][index4]] == 0) {
				matrix[vec_allocate[index1][index3]][vec_allocate[index2][index4]] = 1;
			} else {
				continue;
			}
			// if (existLoop(matrix)) {
			// 	matrix[vec_allocate[index1][index3]][vec_allocate[index2][index4]] = -1;
			// }
			ValidConvert2(matrix, matrix_init, vec_allocate[index1][index3], vec_allocate[index2][index4]);
			loop_count++;
			if (loop_count > 100 * TaskNum) break;
			// cout << noZeroNum(matrix) << endl;
		}

		// cout << noZeroNum(matrix) << '\t' << loop_num << endl;
		// for (size_t i = 0; i < matrix.size(); ++i) {
		// 	for (size_t j = 0; j < matrix[i].size(); ++j) {
		// 		cout << matrix[i][j] << '\t';
		// 	}
		// 	cout << endl;
		// }
		// cout << endl;

		std::vector<std::vector<int> > prec;
		for (int i = 0; i < TaskNum; ++i) {
			std::vector<int> vec;
			prec.push_back(vec);
		}

		for (size_t i = 0; i < matrix[0].size(); ++i) {
			for (size_t j = 0; j < matrix.size(); ++j) {
				if (matrix[j][i] == 1) {
					prec[i].push_back(j);
				}
			}
		}


		std::vector<int> proc_time;
		for (int i = 0; i < TaskNum; ++i) {
			proc_time.push_back((rand() % 6 + 2));
		}

		int SrvcNum = rand() % 10 + (int)(TaskNum / 10);
		int RsrcNum = SrvcNum + (rand() % 3) + 2;

		std::vector<std::vector<int> > opt_srvcs;
		for (int i = 0; i < TaskNum; ++i) {
			std::vector<int> vec;
			vec.push_back(rand() % SrvcNum);
			opt_srvcs.push_back(vec);
			vec.clear();
		}

		std::vector<int> RsrcCpcty;
		for (int i = 0; i < RsrcNum; ++i) {
			RsrcCpcty.push_back(rand() % 10 + 2);
		}

		std::vector<std::vector<int> > CoeMatrix;
		for (int i = 0; i < SrvcNum; ++i) {
			std::vector<int> vec(RsrcNum, 0);
			CoeMatrix.push_back(vec);
			vec.clear();
		}
		for (int i = 0; i < SrvcNum; ++i) {
			int temp = max(1, (int)(RsrcNum / 6));
			int CoeSrvcNum = rand() % temp + (int)(RsrcNum / 3);
			int CoeSrvcLoopCount = 0;
			while (CoeSrvcLoopCount < CoeSrvcNum) {
				int j = rand() % RsrcNum;
				if (CoeMatrix[i][j] != 0) {
					continue;
				} else {
					CoeMatrix[i][j] = rand() % (int)((RsrcCpcty[j] + 1) / 2) + rand() % (int)((RsrcCpcty[j] + 1) / 2);
					CoeSrvcLoopCount++;
				}
			} 
		}
		// for (int i = 0; i < SrvcNum; ++i) {
		// 	std::vector<int> vec;
		// 	for (int j = 0; j < RsrcNum; ++j) {
		// 		vec.push_back(rand() % (RsrcCpcty[j] + 1));
		// 	}
		// 	CoeMatrix.push_back(vec);
		// 	vec.clear();
		// }

		std::vector<int> SrvcCpcty;
		int temp_max[SrvcNum];
		for (int i = 0; i < SrvcNum; ++i) {
			temp_max[i] = (int)(INT_MAX / 2);
			for (int j = 0; j < RsrcNum; ++j) {
				if (CoeMatrix[i][j] > 0) {
					temp_max[i] = min(temp_max[i], RsrcCpcty[j] / CoeMatrix[i][j]);
				}
			}
			SrvcCpcty.push_back(temp_max[i]);
		}

		std::vector<std::vector<int> > srvcs_requst;
		for (int i = 0; i < TaskNum; ++i) {
			std::vector<int> vec;
			int SrvcNo = opt_srvcs[i][0];
			vec.push_back(rand() % (SrvcCpcty[SrvcNo] + 1));
			srvcs_requst.push_back(vec);
			vec.clear();
		}

		DataSet AllData = {TaskNum, prec, proc_time, opt_srvcs, srvcs_requst, RsrcNum, SrvcNum, RsrcCpcty, SrvcCpcty, CoeMatrix};
		return AllData;

		// ==========================================

		// std::vector<std::vector<int> > matrix;
		// std::vector<int> vec(TaskNum, 0);
		// for (int i = 0; i < TaskNum; ++i) {
		// 	matrix.push_back(vec);
		// }
		// int temp = (int)(TaskNum - 3)/2;
		// if (temp == 0) temp++;
		// int loop_num = (int)(1.5 * TaskNum - 1) - (int)(rand() % temp) + (int)(rand() % temp);

		// bool flag;
		// std::vector<int> row_col;
		// int maxloop = 10;
		// int count = 0;
		// std::vector<int> v;
		// while (noZeroNum(matrix) < loop_num) {
		// 	row_col = r_c(TaskNum);
		// 	while (matrix[row_col[0]][row_col[1]] != 0) {
		// 		row_col = r_c(TaskNum);
		// 	}
		// 	matrix[row_col[0]][row_col[1]] = 1;
		// 	flag = existLoop(matrix);
		// 	while (flag) {
		// 		matrix[row_col[0]][row_col[1]] = 0;
		// 		row_col = r_c(TaskNum);

		// 		matrix[row_col[0]][row_col[1]] = 1;
		// 		flag = existLoop(matrix);
		// 	}
		// 	// cout << row_col[0] << '\t' << row_col[1] << '\t';
		// 	ValidConvert(matrix);

		// 	v.push_back(noZeroNum(matrix));
		// 	if (count > 10) {
		// 		if (v[count] == v[count - maxloop])
		// 			break;
		// 	}
		// 	count++;
		// }

		// ============================================



		// for (int i = 0; i < TaskNum; ++i) {
		// 	for (int j = 0; j < TaskNum; ++j) {
		// 		if (matrix[i][j] != 0) {
		// 			matrix[i][j] = 0;
		// 			matrix[j][i] = 1;
		// 			bool flag_temp = existLoop(matrix);
		// 			if (!flag_temp) {
		// 				matrix[i][j] = 1;
		// 			} 
		// 			matrix[j][i] = 0;
		// 		}
		// 	}
		// }

		// cout << noZeroNum(matrix) << '\t' << loop_num << endl;
		// return vec_allocate;
	}
private:
	std::vector<int> r_c(int TaskNum) {
		std::vector<int> vec;
		int row;
		int col;
		row = rand() % TaskNum;
		col = rand() % TaskNum;
		while (col == row) {
			col = rand() % TaskNum;
		}
		vec.clear();
		vec.push_back(row);
		vec.push_back(col);
		return vec;
	}

	bool isIn(std::vector<int> &vec, int n) {
		if (vec.empty()) return false;
		for (size_t i = 0; i < vec.size(); ++i) {
			if (vec[i] == n) return true;
		}
		return false;
	}

	int zero_index(std::vector<std::vector<int> > &vec) {
		if (vec.empty()) return -1;
		for (size_t i = 0; i < vec[0].size(); ++i) {
			size_t count = 0;
			for (size_t j = 0; j < vec.size(); ++j) {
				if (vec[i][j] != 1) {
					count++;
				} else {
					break;
				}
			}
			if (count == vec.size()) {
				return i;
			}
		}
		return -1;
	}

	bool existLoop(std::vector<std::vector<int> > &vec_const) {
		std::vector<std::vector<int> > vec = vec_const;
		int num = vec.size();
		int index;
		index = zero_index(vec);
		while (index >= 0) {
			for (size_t i = 0; i < vec.size(); ++i) {
				vec[i].erase(vec[i].begin() + index);
			}
			vec.erase(vec.begin() + index);
			index = zero_index(vec);
			num--;
		}
		if (num > 0) {
			return true;
		} else {
			return false;
		}
	}

	void ValidConvert(std::vector<std::vector<int> > &matrix) {
		for (size_t i = 0; i < matrix.size(); ++i) {
			for (size_t j = 0; j < matrix.size(); ++j) {
				if (matrix[i][j] != 0) {
					matrix[i][j] = 0;
					matrix[j][i] = 1;
					bool flag_temp = existLoop(matrix);
					if (!flag_temp) {
						matrix[i][j] = 1;
					}	
					matrix[j][i] = 0;
				}
			}
		}
	}

	void ValidConvert2(std::vector<std::vector<int> > &matrix, std::vector<std::vector<int> > &matrix_init, int x, int y) {
		for (size_t i = 0; i < matrix.size(); ++i) {
			for (size_t j = 0; j < matrix.size(); ++j) {
				if (existLoop(matrix)) {
					matrix[x][y] = -1;
				} else {
					if (matrix_init[i][j] == 1) {
						matrix[i][j] = 0;
						matrix[j][i] = 1;
						bool flag_temp = existLoop(matrix);
						if (flag_temp) {
							matrix[x][y] = -1;	
						}
						matrix[i][j] = 1;
						matrix[j][i] = 0;
					} else {
						if (matrix[i][j] == 1) {
							matrix[i][j] = 0;
							matrix[j][i] = 1;
							bool flag_temp = existLoop(matrix);
							if (!flag_temp) {
								matrix[i][j] = 1;
								matrix[j][i] = 0;
							} else {
								matrix[i][j] = -1;
								matrix[j][i] = -1;
							}								
						}
					}			
				}				
			}
		}
	}


	int noZeroNum(std::vector<std::vector<int> > &vec) {
		if (vec.empty()) return 0;
		int count = 0;
		for (size_t i = 0; i < vec.size(); ++i) {
			for (size_t j = 0; j < vec[i].size(); ++j) {
				if (vec[i][j] > 0) {
					count++;
				}
			}
		}
		return count;
	}
};

int main(int argc, char const *argv[])
{
	srand((unsigned)time(NULL));
	DataInitialization di;
	DataSet data = di.Initiate();
	// cout << vec.size();
	// for (size_t i = 0; i < vec.size(); ++i) {
	// 	for (size_t j = 0; j < vec[i].size(); ++j)
	// 		cout << vec[i][j] << '\t';
	// 	cout << endl;
	// }

	int TaskNum = data.TaskNum;
	std::vector<std::vector<int> > prec = data.prec;
	std::vector<int> proc_time = data.proc_time;
	std::vector<std::vector<int> > opt_srvcs = data.opt_srvcs;
	std::vector<std::vector<int> > srvcs_requst = data.srvcs_requst;
	int RsrcNum = data.RsrcNum;
	int SrvcNum = data.SrvcNum;
	std::vector<int> RsrcCpcty = data.RsrcCpcty;
	std::vector<int> SrvcCpcty = data.SrvcCpcty;
	std::vector<std::vector<int> > CoeMatrix = data.CoeMatrix;

	cout << "TaskNum = " << TaskNum << endl;
	cout << "prec = " << endl;
	for (size_t i = 0; i < prec.size(); ++i) {
		for (size_t j = 0; j < prec[i].size(); ++j) {
			cout << prec[i][j] << '\t'; 
		}
		cout << endl;
	}
	cout << "proc_time = " << endl;
	for (size_t i = 0; i < proc_time.size(); ++i) {
		cout << proc_time[i] << '\t';
	}
	cout << endl;
	cout << "opt_srvcs = " << endl;
	for (size_t i = 0; i < opt_srvcs.size(); ++i) {
		for (size_t j = 0; j < opt_srvcs[i].size(); ++j) {
			cout << opt_srvcs[i][j] << '\t';
		}
		cout << endl;
	}
	cout << "srvcs_requst = " << endl;
	for (size_t i = 0; i < srvcs_requst.size(); ++i) {
		for (size_t j = 0; j < srvcs_requst[i].size(); ++j) {
			cout << srvcs_requst[i][j] << '\t';
		}
		cout << endl;
	}
	cout << "RsrcNum = " << RsrcNum << endl;
	cout << "SrvcNum = " << SrvcNum << endl;
	cout << "RsrcCpcty = " << endl;
	for (size_t i = 0; i < RsrcCpcty.size(); ++i) {
		cout << RsrcCpcty[i] << '\t';
	}
	cout << endl;
	cout << "SrvcCpcty = " << endl;
	for (size_t i = 0; i < SrvcCpcty.size(); ++i) {
		cout << SrvcCpcty[i] << '\t';
	}
	cout << endl;
	cout << "CoeMatrix = " << endl;
	for (size_t i = 0; i < CoeMatrix.size(); ++i) {
		for (size_t j = 0; j < CoeMatrix[i].size(); ++j) {
			cout << CoeMatrix[i][j] << '\t';
		}
		cout << endl;
	}
	return 0;
}
