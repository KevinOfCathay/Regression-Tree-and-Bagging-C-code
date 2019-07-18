#pragma once
#ifndef TREE_METHOD_H
#define TREE_METHOD_H

#include "point.h"
#include <string>
#include <list>

/*
HOW TO USE THIS CLASS
	1. Create an instance
		Regression_Tree Tree;

	2. Apply regression tree on your data,
		Tree.Regression(DATA, DIM, L, U);
	    DATA is your data, DIM is the dimention the input space,
	    L is the lower bound of the data, U is the upper bound of the data

	3. The splitting criteria will be printed to the console
*/

struct Boundary {
	int dimention; // The dimention (feature index) of the boundary
	int index;  // The index where the splitting point located
	float location; // The position of the splitting point (splitting criteria)

	// Convenient print out
	friend ostream& operator << (ostream& os, Boundary& b) {
		os << "Dimention: " << b.dimention
			<< " |  Splitting Line:" << b.location
			<< " |  Index:" << b.index << endl;
		return os;
	}
};
struct Block {
	Block(Boundary b, vector<Point> P, bool d, Block* bb) :boundary(b), data(P), direction(d), Base_Block(bb) {
		double sum = 0;
		for ( auto x = P.begin(); x != P.end(); x++ ) {
			sum += x->y;
		}
		average = sum / P.size();
		is_terminal = false;
	};
	Block* Base_Block;
	Boundary boundary;
	vector<Point> data;
	bool direction;
	double average;  // The average y value in this block
	bool is_terminal; // is this block a terminal node
};

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

	Boundary Split(vector<Point>& dataset, int total_dimention, int lower, int upper) {
		Boundary bry; float loss; bool Start = false;

		// The index "a" is the split position
		for ( int dim = 0; dim < total_dimention; dim += 1 ) {

			// Sort the data according to the d-th dimention of x
			Point_Utility::QuickPSort(dataset, dim, lower, upper - 1);

			for ( int a = lower + 1; a < upper - 1; a += 1 ) {
				// Calculate the average
				float avg_L = Average(dataset, lower, a), avg_R = Average(dataset, a, upper);

				// Calculate the total loss (using the MSE loss)
				float Total_Loss = Loss(dataset, lower, a, avg_L) + Loss(dataset, a, upper, avg_R);
				if ( !Start ) { loss = Total_Loss; bry.index = a; bry.dimention = dim; Start = true; }
				else {
					if ( Total_Loss < loss ) {
						loss = Total_Loss;
						bry.index = a; bry.dimention = dim; bry.location = (dataset[a - 1].x[dim] + dataset[a].x[dim]) / 2;
					}
				}
			}
		}
		return bry;
	}

	// Given an optimal index-dimention pair, split the tree into two parts
	pair<vector<Point>, vector<Point>> Partition(vector<Point>& dataset, Boundary splitting_point) {
		// Sort the data according to the d-th dimention of x
		Point_Utility::QuickPSort(dataset, splitting_point.dimention, 0, dataset.size() - 1);
		vector<Point> Left_Half(dataset.begin(), dataset.begin() + splitting_point.index),
			Right_Half(dataset.begin() + splitting_point.index, dataset.end());
		return make_pair(Left_Half, Right_Half);
	}

	void Regression(vector<Point>& dataset, int total_dimention, int lower, int upper, Block* block) {
		// If a node has fewer than x elements, then stop splitting (return)
		// x is defined by the stopping_condition
		if ( upper - lower <= stopping_condition ) {
			block->is_terminal = true;
			return;
		}
		Boundary s = Split(dataset, total_dimention, lower, upper);
		pair<vector<Point>, vector<Point>> split_pair = Partition(dataset, s);

		// Create the left and right block, and push them into the block list
		Block* left_block = new Block(s, split_pair.first, false, block); Block* right_block = new Block(s, split_pair.second, true, block);
		block_list.push_back(left_block), block_list.push_back(right_block);

		// Split the left side
		Regression(split_pair.first, total_dimention, 0, split_pair.first.size(), left_block);

		// Split the right side
		Regression(split_pair.second, total_dimention, 0, split_pair.second.size(), right_block);
	}

	vector<list<Block*>> Batch_Regression(vector<vector<Point>> data_batch, int total_dimention, int lower, int upper) {
		int sz = data_batch.size();
		vector<list<Block*>> temp = {};
		for ( int a = 0; a < sz; a += 1 ) {
			Regression(data_batch[a], total_dimention, lower, upper, nullptr);
			temp.push_back(this->block_list);
			cout << "===Batch " << a << " Completed ===" << endl;
			Reset();
		}
		return temp;
	}
	inline void Set_Stopping_Condition(int x) { stopping_condition = x; }
	void Reset() {
		while ( !block_list.empty() ) {
			Block* k = block_list.front();
			block_list.pop_front();
			delete k;
		}
	}

	// Print out all blocks in the tree, including non-terminal block
	friend ostream& operator << (ostream& os, Regression_Tree& t) {
		os << "=== Regression Tree ===" << endl;
		int sz = t.block_list.size();
		for ( auto b = t.block_list.begin(); b != t.block_list.end(); b++ ) {
			Block* blc = *b;
			os << "Block Information: " << endl <<
				"Block base address: " << blc->Base_Block <<
				"  Block average:" << blc->average <<
				"  Terminal: " << blc->is_terminal << endl;
			Block* t_blc = blc;
			while ( t_blc != nullptr ) {
				string dirc = (t_blc->direction == false) ? "Left" : "Right";
				os << t_blc->boundary << "Direction: " << dirc << endl;
				t_blc = t_blc->Base_Block;
			}

			Point_Utility::PrintVPoint(blc->data);
		}
		return os;
	}
private:
	int stopping_condition = 4;

	// The block list does not support the batch regression
	list<Block*> block_list = {};
};

#endif