#include "Side.h"

using namespace ljd;

//在这里根据索引获取顶点属性
Side::Side(int i1, int i2, int i) :id(i) 
{
	Vertex a = vertexArray[i1];
	Vertex b = vertexArray[i2];
	p1 = vertexArrayWorldCoords[i1];
	p2 = vertexArrayWorldCoords[i2];
	n1 = normalArray[i1];
	n2 = normalArray[i2];
	z1 = a.z;
	z2 = b.z;

	v1 = Point2D(a.x, a.y);
	v2 = Point2D(b.x, b.y);
	if (v1.y < v2.y) 
	{
		exchange(v1, v2);//交换v1 v2，让v1始终是y值大的那个点
		exchange(p1, p2);
		exchange(n1, n2);
		exchange(z1, z2);
	}

	maxY = pixelCoordsY(v1.y);
	rangeY = maxY - pixelCoordsY(v2.y) + 1;//>0
	rangeX = pixelCoordsX(v1.x) - pixelCoordsX(v2.x);
	if (rangeX < 0) rangeX = -rangeX;
	dx = -(v1.x - v2.x) / (v1.y - v2.y);//斜率

	//TODO: dXInvZ,dYInvZ,d1InvZ,dNormalInvZ...
}


