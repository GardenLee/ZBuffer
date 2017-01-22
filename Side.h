#pragma once
#include"basicfunctions.h"

namespace ljd {


	class Side {

	public:
		Point2D v1;//[-1.0,1.0]
		Point2D v2;

		//Attribution:
		Vector n1;//法线
		Vector n2;
		Vertex p1;//空间坐标
		Vertex p2;
		float z1;//空间z坐标
		float z2;

		int maxY;//in pixel coords,必须保证和triangle的maxY相符
		int rangeY;
		int rangeX;
		int id;//所属三角形的id
		float dx;//斜率

		Side(int i1, int i2, int i);
		Side() {}

		//TODO: dXInvZ,dYInvZ,d1InvZ,dNormalInvZ...

	};

}