#include "pch.h"
#include "SkeletonImporter.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

#include <fstream>

unsigned SkeletonImporter::StringToHash(string s)
{
	unsigned int hash = 0;
	unsigned int temp = 0;

	for (auto& c : s)
	{
		hash = (hash << 4) + tolower(c);
		temp = hash & 0xf0000000;

		if (temp != 0)
		{
			hash = hash ^ (temp >> 24);
			hash = hash ^ temp;
		}
	}

	return hash;
}

void SkeletonImporter::ReadFile(char* path)
{
	ifstream inFile(path, ios::in | ios::binary);
	if (!inFile.is_open())
	{
		throw ImporterException(
			"Unable to open skeleton file, please place it in the same directory as your mesh file.");
	}

	inFile.seekg(4, ios_base::cur);
	inFile.read(reinterpret_cast<char*>(&fileVersion), 4);

	if (fileVersion != 1953262451 && fileVersion != 587026371)
	{
		throw ImporterException("Skeleton file is invalid or unsupported.");
	}

	if (fileVersion == 1953262451)
	{
		inFile.seekg(8, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&numBones), 4);

		bones.resize(numBones);

		for (int i = 0; i < numBones; ++i)
		{
			inFile.read(reinterpret_cast<char*>(&bones.at(i).name), 32);
			bones.at(i).hash = StringToHash(bones.at(i).name);
			boneHashes[bones.at(i).hash] = bones.at(i).name;
			inFile.read(reinterpret_cast<char*>(&bones.at(i).parent), 4);
			inFile.read(reinterpret_cast<char*>(&bones.at(i).scale.x), 4);
			bones.at(i).scale.x *= 10.0f;
			bones.at(i).scale.y = bones.at(i).scale.z = bones.at(i).scale.x;
			inFile.read(reinterpret_cast<char*>(&bones.at(i).globalMatrix), 48);

			bones.at(i).globalMatrix[3][0] = bones.at(i).globalMatrix[3][1] = bones.at(i).globalMatrix[3][2] = 0.0f;
			bones.at(i).globalMatrix[3][3] = 1.0f;
		}

		for (auto& i : bones)
		{
			Matrix4x4 relativeMatrix, inverseMatrix;

			if (i.parent != -1)
			{
				inverseMatrix = bones.at(i.parent).globalMatrix.gjInverse();
				i.localMatrix = inverseMatrix * i.globalMatrix;
			}

			else
			{
				i.localMatrix = i.globalMatrix;
			}
		}

		if (skinVersion == 0 || skinVersion == 1)
		{
			numBoneIndices = numBones;
			boneIndices.resize(numBoneIndices);

			for (int i = 0; i < numBoneIndices; ++i)
			{
				boneIndices.at(i) = i;
			}
		}

		else if (skinVersion == 2)
		{
			inFile.read(reinterpret_cast<char*>(&numBoneIndices), 4);

			boneIndices.resize(numBoneIndices);

			for (int i = 0; i < numBoneIndices; ++i)
			{
				inFile.read(reinterpret_cast<char*>(&boneIndices.at(i)), 4);
			}
		}
	}

	else if (fileVersion == 587026371)
	{
		inFile.seekg(6, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&numBones), 2);
		numBones = short(numBones);
		bones.resize(numBones);

		inFile.read(reinterpret_cast<char*>(&numBoneIndices), 4);
		boneIndices.resize(numBoneIndices);

		short dataOffset;
		inFile.read(reinterpret_cast<char*>(&dataOffset), 2);
		inFile.seekg(2, ios_base::cur);

		int boneIndicesOffset, boneNamesOffset;
		inFile.seekg(4, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&boneIndicesOffset), 4);
		inFile.seekg(8, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&boneNamesOffset), 4);

		inFile.seekg(dataOffset, ios_base::beg);

		for (int i = 0; i < numBones; ++i)
		{
			inFile.seekg(2, ios_base::cur);
			short boneId;
			inFile.read(reinterpret_cast<char*>(&boneId), 2);

			if (boneId != i)
			{
				throw ImporterException("Read error, unexpected identification value.");
			}

			inFile.read(reinterpret_cast<char*>(&bones.at(i).parent), 2);
			bones[i].parent = static_cast<short>(bones.at(i).parent);

			inFile.seekg(2, ios_base::cur);

			inFile.read(reinterpret_cast<char*>(&bones.at(i).hash), 4);
			boneHashes[bones.at(i).hash] = bones.at(i).name;

			inFile.seekg(4, ios_base::cur);

			float tx, ty, tz;
			inFile.read(reinterpret_cast<char*>(&tx), 4);
			inFile.read(reinterpret_cast<char*>(&ty), 4);
			inFile.read(reinterpret_cast<char*>(&tz), 4);

			inFile.read(reinterpret_cast<char*>(&bones.at(i).scale), 12);

			Quaternion q;

			inFile.read(reinterpret_cast<char*>(&q.v.x), 4);
			inFile.read(reinterpret_cast<char*>(&q.v.y), 4);
			inFile.read(reinterpret_cast<char*>(&q.v.z), 4);
			inFile.read(reinterpret_cast<char*>(&q.r), 4);

			bones.at(i).localMatrix = q.toMatrix44();
			bones.at(i).localMatrix.gjInvert();

			bones.at(i).localMatrix[0][3] = tx;
			bones.at(i).localMatrix[1][3] = ty;
			bones.at(i).localMatrix[2][3] = tz;

			bones.at(i).localMatrix[3][0] = bones.at(i).localMatrix[3][1] = bones.at(i).localMatrix[3][2] = 0.0f;
			bones.at(i).localMatrix[3][3] = 1.0f;

			inFile.seekg(44, ios_base::cur);
		}

		for (auto& i : bones)
		{
			if (i.parent != -1)
			{
				i.globalMatrix = bones.at(i.parent).globalMatrix * i.localMatrix;
			}

			else
			{
				i.globalMatrix = i.localMatrix;
			}
		}

		inFile.seekg(boneIndicesOffset, ios_base::beg);

		for (int i = 0; i < numBoneIndices; ++i)
		{
			inFile.read(reinterpret_cast<char*>(&boneIndices.at(i)), 2);
			boneIndices.at(i) = short(boneIndices.at(i));
		}

		inFile.seekg(boneNamesOffset, ios_base::beg);

		for (int i = 0; i < numBones; ++i)
		{
			unsigned char curChar;
			streamoff filePos;

			do
			{
				filePos = inFile.tellg();
				inFile.read(reinterpret_cast<char*>(&curChar), 1);
			}
			while (filePos % 4 != 0);

			int curPos = 0;

			while (curChar)
			{
				bones.at(i).name[curPos] = curChar;
				++curPos;

				if (curPos >= 31)
				{
					break;
				}

				inFile.read(reinterpret_cast<char*>(&curChar), 1);
			}

			bones.at(i).name[curPos] = '\0';
		}
	}

	inFile.close();
}

SkeletonImporter::~SkeletonImporter()
{
}
