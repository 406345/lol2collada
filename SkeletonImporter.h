#pragma once

#ifndef _SKELETON_IMPORTER_H_
#define _SKELETON_IMPORTER_H_

#include "SkeletonBone.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

class SkeletonImporter
{
private:
	int fileVersion;
	const short& skinVersion;

private:
	unsigned int StringToHash(string s);

public:
	int numBones;
	int numBoneIndices;

	vector<SkeletonBone> bones;
	vector<int> boneIndices;

	map<unsigned int, char*> boneHashes;

public:
	void ReadFile(char* path);

	SkeletonImporter(short& skinVersion) : skinVersion(skinVersion)
	{
	};
	~SkeletonImporter();
};

#endif
