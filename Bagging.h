#pragma once
#ifndef BAGGING_HEADER
#define BAGGING_HEADER
#include "point.h"

// Split the whole data set into n batches
class Bagging {
public:
	Bagging() {};
	~Bagging() {};

	// Return a batched data points
	static vector<vector<Point>> Batch(vector<Point> original_data, int batch_size) {
		int sz = original_data.size();
		int *order = new int[sz];
		vector<vector<Point>> Batched = {};
		for ( int a = 0; a < sz; a += 1 ) {
			order[a] = a;
		}
		for ( int a = 0; a < sz; a += 1 ) {
			int b = rand() % sz;
			int temp = order[a];
			order[a] = order[b];
			order[b] = temp;
		}

		for ( int b = 0; b < batch_size; b += 1 ) {
			vector<Point> P = {};
			for ( int k = b * (sz / batch_size); k < (b + 1) * (sz / batch_size); k += 1 ) {
				P.push_back(original_data[order[k]]);
			}
			Batched.push_back(P);
		}
		return Batched;
	}
private:

};

#endif // !BAGGING_HEADER

