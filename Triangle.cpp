#include "Triangle.h"
#include "zbuffer.h"
#include<stdio.h>
#include<stdlib.h>
#include<random>
#include<algorithm>

using namespace std;
using namespace ljd;

int Triangle::idGen = 0;


bool cmp_by_y(const int& v1, const int& v2) {
	return vertexArray[v1].y > vertexArray[v2].y;
}

Triangle::Triangle(int i0, int i1, int i2) :index1(i0), index2(i1), index3(i2){
	id = idGen++;
	beLine = false;

	int indices[3] = { i0,i1,i2 };
	sort(indices, indices + 3, cmp_by_y);//对三个索引对应的点的y值进行排序

	Vertex vertices[3] = { vertexArray[indices[0]],vertexArray[indices[1]] ,vertexArray[indices[2]] };

	if (pixelCoordsY(vertices[0].y) == pixelCoordsY(vertices[1].y) == pixelCoordsY(vertices[2].y)) {
		beLine = true;
	}

	//在这里考虑三条边的顺序，为后面构造活化边对提供方便
	else if (pixelCoordsY(vertices[0].y) == pixelCoordsY(vertices[1].y)) {
		if (vertices[0].x < vertices[1].x) {
			s[0] = Side(indices[0], indices[2], id);
			s[1] = Side(indices[1], indices[2], id);
			s[2] = Side(indices[0], indices[1], id);
		}
		else {
			s[0] = Side(indices[1], indices[2], id);
			s[1] = Side(indices[0], indices[2], id);
			s[2] = Side(indices[1], indices[0], id);
		}
	}
	else {
		if (vertices[1].x < vertices[2].x) {
			s[0] = Side(indices[0], indices[1], id);
			s[1] = Side(indices[0], indices[2], id);
			s[2] = Side(indices[1], indices[2], id);
		}
		else {
			s[0] = Side(indices[0], indices[2], id);
			s[1] = Side(indices[0], indices[1], id);
			s[2] = Side(indices[1], indices[2], id);
		}
	}

	//加入分类边表
	if (!beLine) {
		classSideList[s[0].maxY].push_back(s[0]);
		classSideList[s[1].maxY].push_back(s[1]);
		classSideList[s[2].maxY].push_back(s[2]);

		maxY = max(s[0].maxY, s[1].maxY, s[2].maxY);
		rangeY = max(s[0].rangeY, s[1].rangeY, s[2].rangeY);

		float rr = rand() / (double)(RAND_MAX);
		float gg = rand() / (double)(RAND_MAX);
		float bb = rand() / (double)(RAND_MAX);
		unsigned short r = 0xff * rr;
		unsigned short g = 0xff * gg;
		unsigned short b = 0xff * bb;
		color = Color(r, g, b);
	}
	
}

