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

	extern Vector lightDirection;//�����Դ
	extern Vector ambient;
	extern Vector lightColor;//������ɫ
	extern Vector material;//����
	extern float shininess;
	extern float strength;

	//��ʼ������߱�ͷ������α�
	void build_classTriangleList();

	//ɨ��
	void scan_zBuffer();

	//��ȡ�ļ�
	void read_file(const char* name);

	//��ȡobj
	bool read_obj(const char* fileName);

	//���Ƶ�bmp
	void draw(const char* name);

	void multi_matrix(KIND k, vmath::mat4 m);
	
	void clearBuffer();

}