#pragma once

#ifndef _ANIM_BONE_H_
#define _ANIM_BONE_H_

#include "Vector3f.h"
#include "Quaternion.h"

using namespace std;

struct AnimBone
{
	char name[32];

	vector<pair<float, Vector3f>> translation;
	vector<pair<float, Quaternion>> quaternion;
	vector<pair<float, Vector3f>> scale;
};

#endif
