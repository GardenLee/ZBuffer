#pragma once

#include"basicfunctions.h"
#include"Triangle.h"

namespace ljd {

	class Attribution
	{
	public:

		float x;
		Vertex piz;//p/z
		Vector niz;//n/z
		float iz;//1/z

		float dx;
		int dy;
		float diz;//d 1/z
		Vertex dpiz;//d p/z
		Vector dniz;//d n/z

		//for debug:
		int sidei;


		Attribution(const Triangle& t, int i);
		Attribution() :x(-9999.0f) {}
		void update() {
			x += dx;
			dy--;
			piz += dpiz;
			niz += dniz;
			iz += diz;
		}
		~Attribution();
	};

	
	inline void exchange(Attribution& l, Attribution& r) {
		Attribution temp = l;
		l = r;
		r = temp;
	}


}

