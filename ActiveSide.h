#pragma once
#include"basicfunctions.h"
#include"Side.h"
#include"Triangle.h"
#include"Attribution.h"

namespace ljd {
	class Attribution;
	class ActiveSide {

	public:

		Attribution attribLeft;
		Attribution attribRight;
		Attribution attribwait;

		Color color;

		int id;

		ActiveSide(const Triangle& t);
	};


}