#pragma once
#ifndef _COLLADA_WRITER_H_
#define _COLLADA_WRITER_H_

#include <vector>
#include <list>
#include <sstream>

#include "AnimImporter.h"
#include "SkeletonBone.h"
#include "SkinVertex.h"

using namespace std;

class ColladaWriter
{
public:
	enum class Mode
	{
		Mesh = 1,
		Skeleton,
		Animation
	};

private:
	const wchar_t* filePath;
	const vector<short>& indices;
	const vector<SkinVertex>& vertices;

	vector<SkeletonBone>& bones;

	const vector<int>& boneIndices;
	const AnimImporter& animation;


	list<int> nodes;
	ostringstream nodeStream;

private:
	bool emplaceNode(int boneId);

	void createNodeStringStream();
	void writeHeader();
	void writeMaterial();
	void writeMesh();
	void writeSkin();
	void writeScene(Mode& mode);
	void writeAnimation();

public:
	void writeFile(const wchar_t* path, Mode& mode);

	ColladaWriter(
		vector<short>& indices,
		vector<SkinVertex>& vertices,
		vector<SkeletonBone>& bones,
		vector<int>& boneIndices,
		AnimImporter& animation) :
		indices(indices), vertices(vertices), bones(bones), boneIndices(boneIndices), animation(animation)
	{
	};

	~ColladaWriter();
};

#endif
