
#ifndef Q1_DCT_H
#define Q1_DCT_H

#include <cmath>
#include <string>
#include <vector>

using std::vector;

static vector<double> dct_1d(const std::vector<double> &data);

static vector<vector<double>> dct_2d(const vector<vector<double>> &image);

struct DCT {
	bool is_parsed = false;
	size_t size;
	vector<vector<double>> input;
	vector<vector<double>> row_major;
	vector<vector<double>> col_major;


	void set_row_major();
	void set_col_major();
};

// Function to perform 1D DCT along a vector
std::vector<double> dct_1d(const std::vector<double> &data) {
	int N = data.size();
	std::vector<double> result(N, 0.0);

	for (int u = 0; u < N; ++u) {
		double sum = 0.0;
		double Cu = (u == 0) ? 1.0 / std::sqrt(2.0) : 1.0;

		for (int x = 0; x < N; ++x) {
			sum += data[x] * std::cos((2.0 * x + 1.0) * u * M_PI / (2.0 * N));
		}

		result[u] = Cu * std::sqrt(2.0 / N) * sum;
	}

	return result;
}

// Function to perform 2D DCT on an image represented as a 2D vector
std::vector<std::vector<double>> dct_2d(const std::vector<std::vector<double>> &image) {
	auto rows = image.size();
	auto cols = image[0].size();

	// DCT along rows
	std::vector<std::vector<double>> dct_rows(rows, std::vector<double>(cols, 0.0));
	for (int i = 0; i < rows; ++i) {
		dct_rows[i] = dct_1d(image[i]);
	}

	// DCT along columns
	std::vector<std::vector<double>> dct_result(rows, std::vector<double>(cols, 0.0));
	for (int j = 0; j < cols; ++j) {
		std::vector<double> column_data;
		for (int i = 0; i < rows; ++i) {
			column_data.push_back(dct_rows[i][j]);
		}

		std::vector<double> column_result = dct_1d(column_data);

		for (int i = 0; i < rows; ++i) {
			dct_result[i][j] = column_result[i];
		}
	}

	return dct_result;
}
std::vector<std::vector<double>> dct_2d_col(const std::vector<std::vector<double>> &image) {
        int rows = image.size();
        int cols = image[0].size();

        std::vector<std::vector<double>> dct_cols(rows, std::vector<double>(cols, 0.0));
        for (int j = 0; j < cols; ++j) {
                std::vector<double> column_data;
                for (int i = 0; i < rows; ++i) {
                        column_data.push_back(image[i][j]);
                }

                std::vector<double> column_result = dct_1d(column_data);

                for (int i = 0; i < rows; ++i) {
                        dct_cols[i][j] = column_result[i];
                }
        }

        std::vector<std::vector<double>> dct_result(rows, std::vector<double>(cols, 0.0));
        for (int i = 0; i < rows; ++i) {
                dct_result[i] = dct_1d(dct_cols[i]);
        }

        return dct_result;
}


void DCT::set_row_major() {
    row_major = dct_2d(input);

}
void DCT::set_col_major() {
    col_major = dct_2d_col(input);
}
#endif  // Q1_DCT_H
