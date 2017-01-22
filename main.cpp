#include"zbuffer.h"
#include<iostream>
#include<string>

using namespace ljd;
using namespace std;
using namespace vmath;


int main() {

	string fileName;
	string defaultFileName = "1";
	cout << "������obj�ļ�·����" << endl;
	cin >> fileName;
	if (fileName == defaultFileName)
		fileName = "C:\\teapot2.obj";

	if (read_obj(fileName.c_str())) {
		cout << "Obj�ļ���ȡ�ɹ�" << endl;
	}
	else {
		exit(0);
	}

	clearBuffer();

	lightDirection = Vector(-1.0f, 1.0f, 1.0f);//�����Դ
	ambient = Vector(0.3f, 0.3f, 0.3f);
	lightColor = Vector(1.0f, 1.0f, 1.0f);//������ɫ
	material = Vector(0.75f, 0.3f, 0.1f);//����
	shininess = 40.0f;
	strength = 0.5f;
	
	mat4 projectionMatrix(frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f));
	mat4 viewMatrix(lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f)));
	mat4 rotateMatrix = rotate(30.0f, vec3(1.0f, 0.0f, 0.0f));
	mat4 translateMatrix = translate(vec3(10.0f, -30.0f, -100.0f));

	multi_matrix(VERTEX, projectionMatrix * viewMatrix * translateMatrix * rotateMatrix);//ֱ�Ӱ���������ת��ͶӰ����
	multi_matrix(NORMAL, translate(vec3(0.0f, 0.0f, 0.0f)));//���߲���
	cout << "����˷����" << endl;

	build_classTriangleList();
	cout << "�������б����ɹ�" << endl;

	scan_zBuffer();
	cout << "ɨ�������" << endl;

	cout << "������Ŀ���ļ�·����" << endl;
	cin >> fileName;
	if (fileName == defaultFileName)
		fileName = "C:";
	draw(string(fileName+"\\image.bmp").c_str());
	cout << "�ѻ��Ƶ�Ŀ���ļ�" << endl;

}
