#pragma once
#include"basicfunctions.h"

namespace ljd {


	class Side {

	public:
		Point2D v1;//[-1.0,1.0]
		Point2D v2;

		//Attribution:
		Vector n1;//����
		Vector n2;
		Vertex p1;//�ռ�����
		Vertex p2;
		float z1;//�ռ�z����
		float z2;

		int maxY;//in pixel coords,���뱣֤��triangle��maxY���
		int rangeY;
		int rangeX;
		int id;//���������ε�id
		float dx;//б��

		Side(int i1, int i2, int i);
		Side() {}

		//TODO: dXInvZ,dYInvZ,d1InvZ,dNormalInvZ...

	};

}