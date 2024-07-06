#pragma once

#include <vector>

struct vec3d {
	float x, y, z;
};

struct triangle {
	vec3d p[3];
};

struct mesh {
	std::vector<triangle> tris;
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

class TryEngine3D {
public:
	TryEngine3D() {
	}

public:
	mesh meshCube;
	mat4x4 matProj;
	mat4x4 matRotZ, matRotX;

	float fTheta;

	void MultiplyMatrixOnVector(vec3d& i, vec3d& o, mat4x4& m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f) {
			o.x /= w;
			o.y /= w;
			o.z /= w;
		}
	}

public:
	bool OnCreate(float ScreenWidth, float ScreenHeight) {
		meshCube.tris = {
			//north
			{0.0f ,0.0f,0.0f   , 0.0f ,1.0f,0.0f,   1.0f ,1.0f,0.0f },
			{0.0f ,0.0f,0.0f   , 1.0f ,1.0f,0.0f,   1.0f ,0.0f,0.0f },

			//east
			{1.0f ,0.0f,0.0f   , 1.0f ,1.0f,0.0f,   1.0f ,1.0f,1.0f },
			{1.0f ,0.0f,0.0f   , 1.0f ,1.0f,1.0f,   1.0f ,0.0f,1.0f },

			//south
			{1.0f ,0.0f,1.0f   , 1.0f ,1.0f,1.0f,   0.0f ,1.0f,1.0f },
			{1.0f ,0.0f,1.0f   , 0.0f ,1.0f,1.0f,   1.0f ,1.0f,0.0f },

			//west
			{0.0f ,0.0f,1.0f   , 0.0f ,1.0f,1.0f,   0.0f ,1.0f,0.0f },
			{0.0f ,0.0f,1.0f   , 0.0f ,1.0f,0.0f,   0.0f ,0.0f,0.0f },

			//top
			{0.0f ,1.0f,0.0f   , 0.0f ,1.0f,1.0f,   1.0f ,1.0f,1.0f },
			{0.0f ,1.0f,0.0f   , 1.0f ,1.0f,1.0f,   1.0f ,1.0f,0.0f },

			//bottom
			{0.0f ,0.0f,0.0f   , 0.0f ,0.0f,1.0f,   1.0f ,0.0f,1.0f },
			{0.0f ,0.0f,0.0f   , 1.0f ,0.0f,1.0f,   1.0f ,0.0f,0.0f },

		};

		//Projection matrix

		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight / (float)ScreenWidth;
		float fForRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRatio * fForRad;
		matProj.m[1][1] = fForRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
		return true;
	}


public:
	void SetTime(float fElapsedTime) {
		fTheta += 1.0f * fElapsedTime;
		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;
	}
public:
	triangle OnUserUpdate(triangle tri, float ScreenWidth, float ScreenHeight) {
		triangle triProjection;
		triangle triTranslated,triRotatedZ, triRotatedZX;;

		// Rotate in Z - Axis
		MultiplyMatrixOnVector(tri.p[0], triRotatedZ.p[0], matRotZ);
		MultiplyMatrixOnVector(tri.p[1], triRotatedZ.p[1], matRotZ);
		MultiplyMatrixOnVector(tri.p[2], triRotatedZ.p[2], matRotZ);

		// Rotate in X-Axis
		MultiplyMatrixOnVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
		MultiplyMatrixOnVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
		MultiplyMatrixOnVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

		// Offset into the screen
		triTranslated = triRotatedZX;
		triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
		triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
		triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

		// Project triangles from 3D --> 2D
		MultiplyMatrixOnVector(triTranslated.p[0], triProjection.p[0], matProj);
		MultiplyMatrixOnVector(triTranslated.p[1], triProjection.p[1], matProj);
		MultiplyMatrixOnVector(triTranslated.p[2], triProjection.p[2], matProj);

		// Преобразуем координаты в экранные координаты
		triProjection.p[0].x += 1.0f; triProjection.p[0].y += 1.0f;
		triProjection.p[1].x += 1.0f; triProjection.p[1].y += 1.0f;
		triProjection.p[2].x += 1.0f; triProjection.p[2].y += 1.0f;

		triProjection.p[0].x *= ScreenWidth/2; triProjection.p[0].y *= ScreenHeight / 2;
		triProjection.p[1].x *= ScreenWidth / 2; triProjection.p[1].y *= ScreenHeight / 2;
		triProjection.p[2].x *= ScreenWidth / 2; triProjection.p[2].y *= ScreenHeight / 2;

		return triProjection;
	}
	
};
