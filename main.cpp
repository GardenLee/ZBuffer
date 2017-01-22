#include"zbuffer.h"
#include<iostream>
#include<string>

using namespace ljd;
using namespace std;
using namespace vmath;


int main() {

	string fileName;
	string defaultFileName = "1";
	cout << "请输入obj文件路径：" << endl;
	cin >> fileName;
	if (fileName == defaultFileName)
		fileName = "C:\\teapot2.obj";

	if (read_obj(fileName.c_str())) {
		cout << "Obj文件读取成功" << endl;
	}
	else {
		exit(0);
	}

	clearBuffer();

	lightDirection = Vector(-1.0f, 1.0f, 1.0f);//方向光源
	ambient = Vector(0.3f, 0.3f, 0.3f);
	lightColor = Vector(1.0f, 1.0f, 1.0f);//光照颜色
	material = Vector(0.75f, 0.3f, 0.1f);//材质
	shininess = 40.0f;
	strength = 0.5f;
	
	mat4 projectionMatrix(frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f));
	mat4 viewMatrix(lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f)));
	mat4 rotateMatrix = rotate(30.0f, vec3(1.0f, 0.0f, 0.0f));
	mat4 translateMatrix = translate(vec3(10.0f, -30.0f, -100.0f));

	multi_matrix(VERTEX, projectionMatrix * viewMatrix * translateMatrix * rotateMatrix);//直接把世界坐标转到投影坐标
	multi_matrix(NORMAL, translate(vec3(0.0f, 0.0f, 0.0f)));//法线不动
	cout << "矩阵乘法完成" << endl;

	build_classTriangleList();
	cout << "分类多边行表建立成功" << endl;

	scan_zBuffer();
	cout << "扫描线完成" << endl;

	cout << "请输入目标文件路径：" << endl;
	cin >> fileName;
	if (fileName == defaultFileName)
		fileName = "C:";
	draw(string(fileName+"\\image.bmp").c_str());
	cout << "已绘制到目标文件" << endl;

}
