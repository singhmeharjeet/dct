
#ifndef Q1_DCT_H
#define Q1_DCT_H

#include <cmath>
#include <string>
#include <vector>

using std::vector;

struct DCT {
	bool is_parsed = false;
	size_t size;
	vector<vector<int>> input;
	vector<vector<int>> row_major;
	vector<vector<int>> col_major;

	const double c = 1 / sqrt(2);

	int FDCT(int u, int v) {
		const double Cu = (u == 0) ? c : 1.0;
		const double Cv = (v == 0) ? c : 1.0;
		const double sol = (Cu * Cv) / 4;
		double sum = 0.0;
		int _j;

		for (int j = 0; j < size; ++j) {
			_j = j << 1;
			for (int k = 0; k < size; ++k) {
				sum += input[j][k] * cos(((_j + 1) * u * M_PI) / 16) * cos((((k << 1) + 1) * v * M_PI) / 16);
			}
		}
		return (int) round(sol * sum);
	}

	void set_row_major() {
		row_major.resize(size);
		for (int i = 0; i < size; i++) {
			row_major[i].resize(size);
			for (int j = 0; j < size; j++) {
				row_major[i][j] = FDCT(i, j);
			}
		}
	}

	void set_col_major() {
		col_major.resize(size);
		for (int i = 0; i < size; i++) {
			col_major[i].resize(size);
			for (int j = 0; j < size; j++) {
				col_major[i][j] = FDCT(j, i);
			}
		}
	}


};

#endif  // Q1_DCT_H
