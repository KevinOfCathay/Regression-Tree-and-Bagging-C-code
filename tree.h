#pragma once
#ifndef TREE_METHOD_H
#define TREE_METHOD_H

#include "point.h"

class Regression_Tree {
public:
	float Loss(vector<Point>& dataset, int lower, int upper, float average) {
		float loss = 0;
		for ( int p = lower; p < upper; p += 1 ) {
			loss += pow(dataset[p].y - average, 2);
		}
		return loss;
	}

	float Average(vector<Point>& dataset, int lower, int upper) {
		float sum = 0;
		for ( int p = lower; p < upper; p += 1 ) {
			sum += dataset[p].y;
		}
		return sum / (upper - lower);
	}

	pair<int, int> Split(vector<Point>& dataset, int total_dimention, int lower, int upper) {
		float loss; int idx, dim; bool Start = false;

		// The index "a" is the split position
		for ( int d = 0; d < total_dimention; d += 1 ) {

			// Sort the data according to the d-th dimention of x
			Point_Utility::QuickPSort(dataset, d, lower, upper - 1);

			for ( int a = lower + 1; a < upper - 1; a += 1 ) {
				// Calculate the average
				float avg_L = Average(dataset, lower, a), avg_R = Average(dataset, a, upper);

				// Calculate the total loss (using the MSE loss)
				float Total_Loss = Loss(dataset, lower, a, avg_L) + Loss(dataset, a, upper, avg_R);
				if ( !Start ) { loss = Total_Loss; idx = a; dim = d; Start = true; }
				else {
					if ( Total_Loss < loss ) {
						loss = Total_Loss; idx = a; dim = d;
					}
				}
			}
		}
		return make_pair(idx, dim);
	}

	pair<vector<Point>, vector<Point>> Partition(vector<Point>& dataset, pair<int, int> splitting_point) {
		// Sort the data according to the d-th dimention of x
		Point_Utility::QuickPSort(dataset, splitting_point.second, 0, dataset.size() - 1);
		vector<Point> Left_Half(dataset.begin(), dataset.begin() + splitting_point.first),
			Right_Half(dataset.begin() + splitting_point.first, dataset.end());
		return make_pair(Left_Half, Right_Half);
	}

	void Regression(vector<Point>& dataset, int total_dimention, int lower, int upper) {
		// If a node has fewer than 4 elements, then stop splitting (return)
		if ( upper - lower <= 4 ) {
			Point_Utility::PrintVPoint(dataset);
			cout << "======" << endl;
			return;
		}
		pair<int, int> s = Split(dataset, total_dimention, lower, upper);
		//cout << s.first << " , " << s.second << endl;
		pair<vector<Point>, vector<Point>> split_pair = Partition(dataset, s);

		// Split the left side
		Regression(split_pair.first, total_dimention, 0, split_pair.first.size());

		// Split the right side
		Regression(split_pair.second, total_dimention, 0, split_pair.second.size());
	}

	void Batch_Regression(vector<vector<Point>> data_batch, int total_dimention, int lower, int upper) {
		int sz = data_batch.size();
		for ( int a = 0; a < sz; a += 1 ) {
			Regression(data_batch[a], total_dimention, lower, upper);
			cout << "===Batch " << a << " Completed ===" << endl;
		}
	}
};

#endif