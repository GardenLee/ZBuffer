#pragma once

#include"vmath.h"
#include<vector>
#include<list>

#define FLOAT_MAX float(10000000.0f)
#define FRAMEWIDTH int(512)
#define FRAMEHEIGHT int(512)
#define BUFLEN 1000

namespace ljd {


	struct Point2D {
		float x;
		float y;
		Point2D(float _x, float _y) :x(_x), y(_y) {}
		Point2D() :x(0.0f), y(0.0f) {}
	};

	struct Vertex {
		float x;
		float y;
		float z;
		Vertex(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
		Vertex() :x(0.0f), y(0.0f), z(0.0f) {}

		Vertex multi_matrix(vmath::mat4 m) {
			vmath::vec4 res(this->x, this->y, this->z, 1.0f);
			vmath::vec4 ans = m * res;
			ans[0] /= ans[3];
			ans[1] /= ans[3];
			ans[2] /= ans[3];
			Vertex v(ans[0], ans[1], ans[2]);
			return v;
		}
		Vertex operator / (float rhs) const{
			return Vertex(x / rhs, y / rhs, z / rhs);
		}
		friend Vertex operator / (float lhs, Vertex& rhs);
		Vertex operator * (float rhs) const{
			return Vertex(x * rhs, y * rhs, z * rhs);
		}
		Vertex operator * (const Vertex& v) {
			return Vertex(this->x*v.x, this->y*v.y, this->z*v.z);
		}
		Vertex operator - (const Vertex& rhs) const {
			Vertex res(this->x - rhs.x,
				this->y - rhs.y,
				this->z - rhs.z
			);
			return res;
		}
		Vertex operator + (const Vertex& rhs) {
			return Vertex(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
		}
		Vertex operator += (const Vertex& rhs){
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}
		void normalize() {
			float d = sqrt(x*x + y*y + z*z);
			x /= d;
			y /= d;
			z /= d;
		}
		float dot(const Vertex& v) {
			return this->x * v.x + this->y * v.y + this->z * v.z;
		}
		void align() {
			if (this->x >= 1.0f) x = 1.0f;
			if (this->x <= 0.0f) x = 0.0f;
			if (this->y >= 1.0f) y = 1.0f;
			if (this->y <= 0.0f) y = 0.0f;
			if (this->z >= 1.0f) z = 1.0f;
			if (this->z <= 0.0f) z = 0.0f;
		}
	};

	typedef Vertex Vector;


	struct Color {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		//unsigned char a;
		Color(unsigned char _r, unsigned char _g, unsigned char _b) :r(_r), g(_g), b(_b) {}
		Color(const Vector& v) {
			Vector c = v;
			c.align();
			unsigned short _r = c.x / 1.0f * 255;
			r = (unsigned char)_r;
			unsigned short _g = c.y / 1.0f * 255;
			g = (unsigned char)_g;
			unsigned short _b = c.z / 1.0f * 255;
			b = (unsigned char)_b;
		}
		Color() :r(0x00), g(0x00), b(0x00) {}
		void clearColor() {
			r = g = b = 0x00;
		}
	};
	typedef Color Pixel;

	inline int pixelCoordsX(float x) {
		x = x + 1.0f;
		x /= 2.0f;
		return int(x * FRAMEWIDTH);
	}
	inline int pixelCoordsY(float x) {
		x = x + 1.0f;
		x /= 2.0f;
		//x = 1.0f - x;
		return int(x * FRAMEHEIGHT);
	}

	inline void exchange(Point2D& v1, Point2D& v2) {
		Point2D v3 = v2;
		v2 = v1;
		v1 = v3;
	}
	inline void exchange(Vertex& v1, Vertex& v2) {
		Vertex v3 = v2;
		v2 = v1;
		v1 = v3;
	}
	inline void exchange(float& a, float& b) {
		float c = a;
		a = b;
		b = c;
	}

	inline int max(int a, int b, int c) {
		int res = a;
		if (b > res) res = b;
		if (c > res) res = c;
		return res;
	}

	inline float max(float a, float b, float c) {
		float res = a;
		if (b > res) res = b;
		if (c > res) res = c;
		return res;
	}

	inline int min(int a, int b, int c) {
		int res = a;
		if (b < res) res = b;
		if (c < res) res = c;
		return res;
	}

	inline float min(float a, float b, float c) {
		float res = a;
		if (b < res) res = b;
		if (c < res) res = c;
		return res;
	}



	extern std::vector<unsigned int> indexArray;
	extern std::vector<Vertex> vertexArrayWorldCoords;//世界坐标系下的顶点坐标
	extern std::vector<Vector> normalArrayWorldCoords;
	extern std::vector<Vector> readingNormalArrayWorldCoords;//temporary
	extern std::vector<Vertex> vertexArray;//做过矩阵乘法后的坐标，范围[-1.0,1.0]
	extern std::vector<Vector> normalArray;
	extern std::vector<Color> colorArray;//

}


