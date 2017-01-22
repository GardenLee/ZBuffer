#pragma once

#include"basicfunctions.h"
#include"Side.h"
#include"Triangle.h"
#include"ActiveSide.h"
#include"ActiveTriangle.h"
#include"Attribution.h"

#include"vmath.h"

#define VERTEX (unsigned short)1
#define NORMAL (unsigned short)2
#define KIND unsigned short

namespace ljd {

	extern Vector lightDirection;//方向光源
	extern Vector ambient;
	extern Vector lightColor;//光照颜色
	extern Vector material;//材质
	extern float shininess;
	extern float strength;

	//初始化分类边表和分类多边形表
	void build_classTriangleList();

	//扫描
	void scan_zBuffer();

	//读取文件
	void read_file(const char* name);

	//读取obj
	bool read_obj(const char* fileName);

	//绘制到bmp
	void draw(const char* name);

	void multi_matrix(KIND k, vmath::mat4 m);
	
	void clearBuffer();

}