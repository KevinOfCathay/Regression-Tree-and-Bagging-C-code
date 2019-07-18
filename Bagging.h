#pragma once
#ifndef BAGGING_HEADER
#define BAGGING_HEADER
#include "point.h"

// Split the whole data set into n batches
class Bagging {
public:
	Bagging() {};
	~Bagging() {};

	// Divide the whole data into X parts, X is any integer greater or equal to 1.
	// Return X batches of data points
	static vector<vector<Point>> Batch(vector<Point> original_data, int batch_size) {
		int sz = original_data.size();
		int *order = new int[sz];
		vector<vector<Point>> Batched;
		Batched.reserve(batch_size);

		for ( int a = 0; a < sz; a += 1 ) { order[a] = a; }
		for ( int a = 0; a < sz; a += 1 ) {
			int b = rand() % sz;
			int temp = order[a];
			order[a] = order[b];
			order[b] = temp;
		}
		for ( int b = 0; b < batch_size; b += 1 ) {
			vector<Point> P;
			P.reserve(sz / batch_size);
			for ( int k = b * (sz / batch_size); k < (b + 1) * (sz / batch_size); k += 1 ) {
				P.push_back(original_data[order[k]]);
			}
			Batched.push_back(P);
		}
		delete[] order;
		return Batched;
	}

	// Use the original data to create X batches, X is any integer greater or equal to 1.
	// There are N datas in each batch. N should less or equal to the size of the original data.
	// Return X batches of data points
	static vector<vector<Point>> BootstapBatch(vector<Point> original_data, int bootstap_size, int batch_size) {
		vector<vector<Point>> Batched;
		Batched.reserve(batch_size);

		for ( int a = 0; a < batch_size; a += 1 ) {
			vector<Point> vp;
			vp.reserve(bootstap_size);
			for ( int b = 0; b < bootstap_size; b += 1 ) {
				int r = rand() % original_data.size();
				vp.push_back(original_data[r]);
			}
			Batched.push_back(vp);
		}
		return Batched;
	}
};

#endif // !BAGGING_HEADER

