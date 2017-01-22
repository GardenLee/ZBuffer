#include "Attribution.h"

namespace ljd {

	Attribution::Attribution(const Triangle& t, int i):sidei(i)
	{
		x = t.s[i].v1.x;							//x1
		iz = 1 / t.s[i].z1;							//1/z1,空间中的z坐标
		piz = t.s[i].p1 * iz;						//p1/z1
		niz = t.s[i].n1 * iz;						//n1/z1
		dy = t.s[i].rangeY;							//rangeY
		dx = (t.s[i].v2.x - t.s[i].v1.x) / dy;		//dx
		float izd = 1 / t.s[i].z2;					//1/z2
		diz = (izd - iz) / dy;						//(1/z2 - 1/z1)/rangeY
		Vertex p2iz2 = t.s[i].p2 * izd;
		dpiz = (p2iz2 - piz) / float(dy);			//(p2/z2 - p1/z1)/rangeY
		Vertex n2iz2 = t.s[i].n2 * izd;
		dniz = (n2iz2 - niz) / float(dy);			//(n2/z2 - n1/z1)/rangeY
	}


	Attribution::~Attribution()
	{
	}


}
