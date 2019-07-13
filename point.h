#pragma once
// A Header that is used to generate and operate n-d points
#ifndef POINT_HEADER
#define POINT_HEADER

#include <iostream>
#include <random>
#include <vector>
#include <ctime>

using namespace std;
struct Point {
	Point(int Xdata[], int Ydata, int Dimention) {
		x = {};
		for ( int a = 0; a < Dimention; a += 1 ) {
			x.push_back(Xdata[a]);
		}
		y = Ydata;
	}
	Point(vector<float> Xdata, int Ydata) : x(Xdata), y(Ydata) {};

	// Each point has a series of x and a output y
	vector<float> x;
	float y;
};

class Point_Utility {
public:
	static void PrintPoint(Point& X) {
		cout << "x: ( ";
		for ( auto i = X.x.begin(); i != X.x.end(); i++ ) {
			cout << *i << " , ";
		}
		cout << " ),  y: " << X.y << endl;
	}
	static void PrintVPoint(vector<Point>& VP) {
		for ( auto i = VP.begin(); i != VP.end(); i++ ) {
			PrintPoint(*i);
		}
		cout << endl;
	}

	static void Sort(vector<Point>& VP, int Dimention) {
		int sz = VP.size();
		for ( int a = 0; a < sz; a += 1 ) {
			for ( int b = a + 1; b < sz; b += 1 ) {
				if ( VP[a].x[Dimention] > VP[b].x[Dimention] ) {
					Point temp = VP[a];
					VP[a] = VP[b];
					VP[b] = temp;
				}
			}
		}
	}

	// Assign a series of value to a specific demention
	static void Assign(vector<Point>& VP, vector<float> Data, int Dimention) {
		int a = 0;
		if ( Dimention >= VP[0].x.size() ) {
			for ( auto i = VP.begin(); i != VP.end(); i++ ) {
				i->y = Data[a];
				a++;
			}
		}
		else {
			for ( auto i = VP.begin(); i != VP.end(); i++ ) {
				i->x[Dimention] = Data[a];
				a++;
			}
		}
	}

	static void BulkInit(vector<Point>& VP, int Dimention, int size) {
		vector<float> zeros = {};
		for ( int p = 0; p < Dimention; p += 1 ) {
			zeros.push_back(0.0);
		}
		for ( int x = 0; x < size; x += 1 ) {
			VP.push_back(Point(zeros, 0));
		}
	}
};

class Random_Utility {
public:
	Random_Utility() {};
	~Random_Utility() {};

	// mean: mean
	// sigma: Standard deviation

	static vector<float> Sequence(int lower, int upper, float mean, float sigma) {
		default_random_engine engine{ static_cast<unsigned int>(rand() % 1000) };
		normal_distribution<float> Noise(mean, sigma);

		vector<float> temp = {};
		for ( int a = lower; a < upper; a += 1 ) {
			temp.push_back(a + Noise(engine));
		}
		return temp;
	}

	// For a given x data, calculate the corresponding y data
	static void Quadratic(vector<Point>& VP, float mean, float sigma, float scaler = 1, float bias = 0) {
		default_random_engine engine{ static_cast<unsigned int>(rand() % 1000) };
		normal_distribution<float> Noise(mean, sigma);

		for ( auto i = VP.begin(); i != VP.end(); i++ ) {
			float sum = 0;
			for ( auto n = i->x.begin(); n != i->x.end(); n++ ) {
				sum += scaler * pow((*n), 2) + bias;
			}
			sum += Noise(engine);
			i->y = sum;
		}
	}
private:

};

#endif // 