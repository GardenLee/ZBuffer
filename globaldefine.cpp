#include"zbuffer.h"
#include<windows.h>
#include<fstream>
#include<string>
#include<sstream>
#include<iostream>

using namespace std;
using namespace vmath;

namespace ljd{

	//֡����
	Pixel frameBuffer[FRAMEHEIGHT][FRAMEWIDTH];
	//ɨ������zbuffer
	float scanZBuf[FRAMEWIDTH] = { FLOAT_MAX };


	//�������顢����������������飬��obj�ж���:
	vector<unsigned int> indexArray;
	vector<Vertex> vertexArrayWorldCoords;//��������ϵ�µĶ�������
	vector<Vector> normalArrayWorldCoords;
	vector<Vector> readingNormalArrayWorldCoords;//temporary
	vector<Vertex> vertexArray;//�˹�ͶӰ���������꣬��Χ[-1.0,1.0]
	vector<Vector> normalArray;
	vector<Color> colorArray;


	//�������α�
	list<Triangle> classTriangleList[FRAMEHEIGHT];//0~511
	typedef list<Triangle>::iterator classTriangleIterator;
	//����߱�
	list<Side> classSideList[FRAMEHEIGHT];
	typedef list<Side>::iterator classSideIterator;

	//��߱�
	list<ActiveSide> activeSideList;
	typedef list<ActiveSide>::iterator activeSideIterator;

	//ȫ�����ԣ�
	Vector lightDirection;//�����Դ
	Vector ambient;
	Vector lightColor;//������ɫ
	Vector material;//����
	float shininess;
	float strength;

	Vertex operator / (float lhs, Vertex & rhs)
	{
		return Vertex(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
	}

	//functions---------------------------------------------------------------------

	void transpose(unsigned int resIndex, unsigned int desIndex) 
	{
		normalArrayWorldCoords[desIndex] = readingNormalArrayWorldCoords[resIndex];
	}

	bool read_obj(const char* fileName) 
	{
		char ch;
		ifstream objFile;
		objFile.open(fileName);

		if (objFile.fail()) 
		{
			cout << "�ļ���ʧ��" << endl;
			return false;
		}

		char* buf = new char[BUFLEN];
		while (objFile.getline(buf, BUFLEN)) 
		{
			istringstream iss(buf);

			if (buf[0] == 'v'&&buf[1] != 'n') 
			{
				float x, y, z;
				iss >> ch >> x >> y >> z;
				vertexArrayWorldCoords.push_back(Vertex(x, y, z));
			}

			else if (buf[0] == 'v'&&buf[1] == 'n') 
			{
				float x, y, z;
				iss >> ch >> ch >> x >> y >> z;
				readingNormalArrayWorldCoords.push_back(Vector(x, y, z));
			}

			else if (buf[0] == 'g') break;

			else continue;
		}

		normalArrayWorldCoords.resize(readingNormalArrayWorldCoords.size());

		if (vertexArrayWorldCoords.size() != readingNormalArrayWorldCoords.size()) 
		{
			cout << "�����뷨������ƥ��" << endl;
			return false;
		}

		while (objFile.getline(buf, BUFLEN)) 
		{
			istringstream iss(buf);

			if (buf[0] == 'f') 
			{
				unsigned int va, vb, vc, na, nb, nc;
				iss >> ch >> va >> ch >> ch >> na >> vb >>
					ch >> ch >> nb >> vc >> ch >> ch >> nc;
				indexArray.push_back(va - 1);
				indexArray.push_back(vb - 1);
				indexArray.push_back(vc - 1);

				transpose(na - 1, va - 1);
				transpose(nb - 1, vb - 1);
				transpose(nc - 1, vc - 1);
			}

			else continue;
		}
		//now vertices and normals are coordinate
		readingNormalArrayWorldCoords.clear();//no more use
	
		return true;
	}

	void read_file(const char* name) 
	{
		char ch;
		ifstream file;
		file.open(name);
		if (file.fail()) 
		{
			cout << "file open fail" << endl;
			exit(0);
		}

		char* buf = new char[200];
		while (file.getline(buf, 200)) 
		{
			istringstream iss(buf);
			if (buf[0] == 'v') 
			{
				float x, y, z;
				iss >> ch >> x >> y >> z;
				vertexArray.push_back(Vertex(x, y, z));//ʵ���ϲ�Ӧ�ü��������

			}

			else if (buf[0] == 't') {
				int x, y, z;
				unsigned char r, g, b;
				int rr, gg, bb;
				iss >> ch >> x >> y >> z >> rr >> gg >> bb;
				r = (unsigned char)rr;
				g = (unsigned char)gg;
				b = (unsigned char)bb;
				indexArray.push_back(x);
				indexArray.push_back(y);
				indexArray.push_back(z);
				colorArray.push_back(Color(r, g, b));
			}
		}
	}

	void build_classTriangleList(){
		for (int i = 0; i < FRAMEHEIGHT; i++)
		{
			classSideList[i].clear();
			classTriangleList[i].clear();
		}

		//pick 3 vertices (depending on 3 indices) to construct a triangle
		for (unsigned int i = 0; i < indexArray.size(); i += 3) 
		{
			//if (indexArray[i] != indexArray[i + 1] && indexArray[i + 1] != indexArray[i + 2] && indexArray[i] != indexArray[i + 2])
			//{
				Triangle t(indexArray[i], indexArray[i + 1], indexArray[i + 2]);

				//problem may occur: t.maxY is not in [0,511]
				if (!t.beLine)
					classTriangleList[t.maxY].push_back(t);
			//}

		}
	}
	
	void clearBuffer() {
		for (int i = 0; i < FRAMEWIDTH; i++)
		{
			scanZBuf[i] = FLOAT_MAX;
		}
		for (int j = 0; j < FRAMEHEIGHT; j++) {
			for (int i = 0; i < FRAMEWIDTH; i++) {
				frameBuffer[j][i].clearColor();
			}
		}
	}

	//core work:
	void scan_zBuffer() 
	{
		DWORD startTime, endTime;//���ڼ�ʱ
		startTime = GetTickCount();

		activeSideList.clear();

		int currentScanY = FRAMEHEIGHT - 1;
		while (currentScanY >= 0) 
		{
			for (int i = 0; i < FRAMEWIDTH; i++)
			{
				scanZBuf[i] = FLOAT_MAX;
			}

			//��ǰɨ����ɨ�������Σ��������
			if (!classTriangleList[currentScanY].empty()) 
			{
				for (classTriangleIterator ite = classTriangleList[currentScanY].begin();
					ite != classTriangleList[currentScanY].end();
					ite++
					) 
				{
					//initialize activeSidePair with Triangle
					activeSideList.push_back(*ite);
				}
			}

			//��ÿһ�Ի��
			for (activeSideIterator ite = activeSideList.begin();
				ite != activeSideList.end();
				//ite++
				)
			{
				if (ite->attribLeft.dy <= 0 || ite->attribRight.dy <= 0) 
				{
					ite = activeSideList.erase(ite);
					continue;
				}
				//������ҷ���
				if (pixelCoordsX(ite->attribRight.x) < pixelCoordsX(ite->attribLeft.x))
				{
					//ite = activeSideList.erase(ite);
					//continue;
					exchange(ite->attribLeft, ite->attribRight);//problem may be here
				}
		
				int xleft = pixelCoordsX(ite->attribLeft.x);
				int xright = pixelCoordsX(ite->attribRight.x);

				float diz = 0.0f;
				if(xright != xleft)
					diz = (ite->attribRight.iz - ite->attribLeft.iz) / (xright - xleft);//����1/z����

				Vector dpiz = Vector(0.0f, 0.0f, 0.0f);
				if (xright != xleft)
					dpiz = (ite->attribRight.piz - ite->attribLeft.piz) / (xright - xleft);//����p/z����

				Vector dniz = Vector(0.0f, 0.0f, 0.0f);
				if (xright != xleft)
					dniz = (ite->attribRight.niz - ite->attribLeft.niz) / (xright - xleft);//����n/z����

				float iz = ite->attribLeft.iz;
				Vector piz = ite->attribLeft.piz;
				Vector niz = ite->attribLeft.niz;

				//�Ըñ߶�֮���ÿ������
				for (int x = xleft; x <= xright; x++)
				{
					float z = 1 / iz;
					Vector pos = piz * z;
					Vector normal = niz * z;
					
					if (z < scanZBuf[x]) //��ȸ�С
					{
						scanZBuf[x] = z;
						//shading:

						//using Blinn-Phong model:
						//Vector lightDirect = lightDirection;
						//Vector eyeDirect = Vector(0.0f, 0.0f, 0.0f) - pos;
						//Vector h = lightDirect + eyeDirect;
						//lightDirect.normalize();
						//h.normalize();
						//normal.normalize();
						//float diffuse = max(0.0f, normal.dot(lightDirect));
						//float specular = max(0.0f, h.dot(normal));
						//if (diffuse < 0.000001f)
						//	specular = 0.0f;
						//else if(specular > 0.0f)
						//	specular = pow(specular, shininess) * strength;
						//Vector scatteredLight = ambient + lightColor * diffuse;
						//Vector reflectedLight = lightColor * specular;
						//Vector c = material * scatteredLight + reflectedLight;
						//c.align();
						//Color color(c);

						//using Phong model:
						Vector lightDirect = lightDirection;
						Vector eyeDirect = Vector() - pos;
						Vector h = normal - lightDirect;
						h.normalize();
						lightDirect.normalize();
						normal.normalize();
						eyeDirect.normalize();
						float diffuse = max(0.0f, normal.dot(lightDirect));
						float specular = max(0.0f, h.dot(eyeDirect));
						if (diffuse < 0.000001f)
							specular = 0.0f;
						else if (specular > 0.0f)
							specular = pow(specular, shininess)*strength;
						Vector scatteredLight = ambient + lightColor * diffuse;
						Vector reflectedLight = lightColor * specular;
						Vector c = material * scatteredLight + reflectedLight;
						c.align();
						Color color(c);

						frameBuffer[FRAMEHEIGHT - 1 - currentScanY][x] = color;
						//frameBuffer[FRAMEHEIGHT - 1 - currentScanY][x] = ite->color;
					}
					iz += diz;
					piz += dpiz;
					niz += dniz;
				}

				//���¸û�߶�
				ite->attribLeft.update();
				ite->attribRight.update();

				//�����һ����dy����0���£���һ����û�У�����Ҫ�ñ�ѡ�ߴ���֮
				if (ite->attribLeft.dy <= 1 && ite->attribRight.dy > 1)
				{
					exchange(ite->attribLeft, ite->attribwait);
				}
				else if (ite->attribRight.dy <= 1 && ite->attribLeft.dy > 1)
				{
					exchange(ite->attribRight, ite->attribwait);
				}

				ite++;
			}

			currentScanY--;
		}

		endTime = GetTickCount();
		cout << "ɨ������Ⱦ��ʱ" << endTime - startTime << "����" << endl;

	}

	//----------------------

	void multi_matrix(KIND k, vmath::mat4 m) 
	{
		if (k == VERTEX) {
			vertexArray.resize(vertexArrayWorldCoords.size());
			for (int i = 0; i < vertexArrayWorldCoords.size(); i++) {
				vertexArray[i] = vertexArrayWorldCoords[i].multi_matrix(m);
			}
		}
		//ע�⣬ƽ�ƾ�������ž���Ҫ����normal��
		else if (k == NORMAL) {
			normalArray.resize(normalArrayWorldCoords.size());
			for (int i = 0; i < normalArrayWorldCoords.size(); i++) {
				normalArray[i] = normalArrayWorldCoords[i].multi_matrix(m);
				normalArray[i].normalize();
			}
		}
	}


	//----------------------bmp
	struct BMPHeader 
	{
		WORD    type;
		DWORD   size;
		WORD    reserved1;
		WORD    reserved2;
		DWORD   offset;
	};


	struct BMPInfoHeader 
	{
		DWORD      size;
		LONG       width;
		LONG       height;
		WORD       planes;
		WORD       bitsPerPixel;
		DWORD      compression;
		DWORD      imageSize;
		LONG       xPelsPerMeter;
		LONG       yPelsPerMeter;
		DWORD      clrUsed;
		DWORD      clrImportant;
	};

	void save_bmp(const char* name, int width, int height, unsigned char* data)
	{
		BMPHeader hdr;
		BMPInfoHeader infoHdr;

		infoHdr.size = 40;
		infoHdr.width = width;
		infoHdr.height = height;
		infoHdr.planes = 1;
		infoHdr.bitsPerPixel = 24;
		infoHdr.compression = 0;
		infoHdr.imageSize = width* height * 3;
		infoHdr.xPelsPerMeter = 0;
		infoHdr.yPelsPerMeter = 0;
		infoHdr.clrUsed = 0;
		infoHdr.clrImportant = 0;

		hdr.type = 0x4d42;
		hdr.reserved1 = 0;
		hdr.reserved2 = 0;
		hdr.offset = 54;
		hdr.size = (DWORD)(sizeof(BMPHeader) + sizeof(BMPInfoHeader) + width * height * 3);

		FILE *fd;

		if (!(fd = fopen(name, "wb+")))
		{
			printf("���Ƶ�Ŀ���ļ�ʧ��\n");
			exit(0);
		}
		fwrite(&hdr, 1, sizeof(BMPHeader), fd);
		fwrite(&infoHdr, 1, sizeof(BMPInfoHeader), fd);
		fwrite(data, 1, width* height * 3, fd);
		fclose(fd);
	}

	//���Ƶ�bmp��ȥ
	void draw(const char* name)
	{
		unsigned char data[FRAMEWIDTH * FRAMEHEIGHT * 3];
		int ii = 0;
		for (int i = FRAMEHEIGHT - 1; i >= 0; i--) 
		{
			for (int j = 0; j < FRAMEWIDTH; j++) 
			{
				data[ii++] = frameBuffer[i][j].b;
				data[ii++] = frameBuffer[i][j].g;
				data[ii++] = frameBuffer[i][j].r;
			}
		}

		save_bmp(name, FRAMEWIDTH, FRAMEHEIGHT, data);
	}


}