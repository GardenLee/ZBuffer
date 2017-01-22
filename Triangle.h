#pragma once
#include"basicfunctions.h"
#include"Side.h"

namespace ljd {

	extern std::list<Side> classSideList[];

	class Triangle {
	
	public:
		static int idGen;
		unsigned int index1;//三个顶点的索引
		unsigned int index2;
		unsigned int index3;
		int id;
		Color color;
		int rangeY;
		int maxY;
		Side s[3];//三条边
		Triangle(int i1, int i2, int i3);
		~Triangle() {}
		bool beLine;

	private:
		Triangle() {}
				
		//material...
	};

}