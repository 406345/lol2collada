#include "pch.h"
#include "SkinImporter.h"
#include <fstream>

void SkinImporter::ReadFile(char* path)
{
	ifstream inFile(path, ios::in | ios::binary);
	if (!inFile.is_open())
	{
		throw ImporterException("Unable to open input file.");
	}


	inFile.read(reinterpret_cast<char*>(&magicNumber), 4);

	if (magicNumber != 1122867)
	{
		throw ImporterException("Corrupted or invalid header in file.");
	}


	inFile.read(reinterpret_cast<char*>(&fileVersion), 2);
	inFile.read(reinterpret_cast<char*>(&matHeader), 2);

	if (fileVersion > 4)
	{
		throw ImporterException("File version is not supported.");
	}

	if (!matHeader == 1)
	{
		throw ImporterException("The file could not be parsed.");
	}

	if (fileVersion > 0)
	{
		inFile.read(reinterpret_cast<char*>(&numMaterials), 4);


		for (int i = 0; i < numMaterials; ++i)
		{
			inFile.seekg(80, ios_base::cur);
		}
	}

	if (fileVersion == 4)
	{
		inFile.seekg(4, ios_base::cur);
	}


	inFile.read(reinterpret_cast<char*>(&numIndices), 4);
	inFile.read(reinterpret_cast<char*>(&numVertices), 4);

	if (fileVersion == 4)
	{
		inFile.seekg(48, ios_base::cur);
	}


	indices.resize(numIndices);
	for (int i = 0; i < numIndices; ++i)
	{
		inFile.read(reinterpret_cast<char*>(&indices.at(i)), 2);
	}


	vertices.resize(numVertices);

	for (int i = 0; i < numVertices; ++i)
	{
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).position), 12);


		inFile.read(reinterpret_cast<char*>(&vertices.at(i).boneIndices), 4);


		inFile.read(reinterpret_cast<char*>(&vertices.at(i).boneWeights), 16);


		inFile.read(reinterpret_cast<char*>(&vertices.at(i).normal), 12);


		inFile.read(reinterpret_cast<char*>(&vertices.at(i).u), 4);
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).v), 4);

		vertices.at(i).v = 1.0f - vertices.at(i).v;
	}

	for (auto& i : vertices)
	{
		const float totalWeight = i.boneWeights[0] + i.boneWeights[1] + i.boneWeights[2] + i.boneWeights[3];
		const float weightError = 1.0f - totalWeight;

		if (weightError != 0.0f)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i.boneWeights[j] > 0.0f)
				{
					i.boneWeights[j] += (i.boneWeights[j] / totalWeight) * weightError;
				}
			}
		}
	}

	inFile.close();
}

SkinImporter::SkinImporter()
{
}

SkinImporter::~SkinImporter()
{
}
