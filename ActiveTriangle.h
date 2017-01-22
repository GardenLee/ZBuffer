#pragma once
#include"basicfunctions.h"
#include"Triangle.h"

namespace ljd {

	class ActiveTriangle {

	public:
		int id;
		int dy;
		ActiveTriangle(const Triangle& t) {
			id = t.id;
			dy = t.rangeY;
		}
	};
	

}

