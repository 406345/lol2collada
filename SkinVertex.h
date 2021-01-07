#pragma once

#ifndef _SKIN_VERTEX_H_
#define _SKIN_VERTEX_H_

#include "Vector3f.h"

using namespace std;

struct SkinVertex
{
	Vector3f position;
	Vector3f normal;

	char boneIndices[4];
	float boneWeights[4];

	float u, v;
};

#endif
