#pragma once

#ifndef _SKELETON_BONE_H
#define _SKELETON_BONE_H

#include "Vector3f.h"
#include "Matrix4x4.h"

using namespace std;

struct SkeletonBone
{
	char name[32];
	unsigned int hash;

	int parent;

	Vector3f scale;
	Matrix4x4 localMatrix;
	Matrix4x4 globalMatrix;
};

#endif
