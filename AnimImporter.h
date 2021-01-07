#pragma once

#ifndef _ANIM_IMPORTER_H_
#define _ANIM_IMPORTER_H_

#include "AnimBone.h"
#include "Vector3f.h"
#include "Quaternion.h"

#include <map>
#include <string>
#include <vector>

using namespace std;

class AnimImporter
{
private:
	const map<unsigned int, char*>& boneHashes;

private:
	unsigned int StringToHash(string s);

	Quaternion DecompressQuaternion(
		const unsigned short& flag,
		const unsigned short& sx,
		const unsigned short& sy,
		const unsigned short& sz
	);

	Vector3f DecompressVector(
		const Vector3f& min,
		const Vector3f& max,
		const unsigned short& sx,
		const unsigned short& sy,
		const unsigned short& sz
	);

	float DecompressTime(
		const unsigned short& ct,
		const float& animationLength
	);

public:
	int numBones;
	int numFrames;
	float frameDelay;
	vector<AnimBone> bones;

public:
	void ReadFile(const wchar_t* path);

	AnimImporter(map<unsigned int, char*>& boneHashes) : boneHashes(boneHashes)
	{
	}

	~AnimImporter();
};

#endif
