#pragma once

#ifndef _SKIN_IMPORTER_H_
#define _SKIN_IMPORTER_H_

#include "SkinVertex.h"
#include <vector>

using namespace std;

class SkinImporter
{
private:
	int magicNumber;
	int numMaterials;
	short matHeader;

public:
	int numIndices;
	int numVertices;

	vector<short> indices;
	vector<SkinVertex> vertices;

	short fileVersion;

	void ReadFile(char* path);

	SkinImporter();
	~SkinImporter();
};

#endif
