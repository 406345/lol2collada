#include "pch.h"
#include "ColladaWriter.h"
#include "Euler.h"
#include "EulerUtilities.h"

#include <ctime>
#include <fstream>
#include <deque>

using namespace std;

bool ColladaWriter::emplaceNode(int boneId)
{
	int parent = bones.at(boneId).parent;

	if (parent == -1)
	{
		nodes.push_back(boneId);
		nodes.push_back(bones.size() + boneId);
		return true;
	}

	int parentEnd = bones.size() + parent;
	auto it = find(nodes.begin(), nodes.end(), parentEnd);

	if (it == nodes.end())
	{
		return false;
	}

	nodes.emplace(it, boneId);
	nodes.emplace(it, boneId + bones.size());

	return true;
}

void ColladaWriter::createNodeStringStream()
{
	int numBones = bones.size();

	for (auto& i : nodes)
	{
		if (i >= numBones)
		{
			nodeStream << "\t\t\t" << "</node>" << endl;
		}

		else
		{
			string name = bones.at(i).name;
			Euler rotations;

			rotations.extract(bones.at(i).localMatrix);
			rotations.negate();
			EulerUtilities::radiansToDegrees(rotations);

			float tx = bones.at(i).localMatrix[0][3];
			float ty = bones.at(i).localMatrix[1][3];
			float tz = bones.at(i).localMatrix[2][3];

			nodeStream << "\t\t\t" << R"(<node id=")" << name << R"(" name=")" << name << R"(" sid=")" << name <<
				R"(" type="JOINT">)" << endl;
			nodeStream << "\t\t\t\t" << R"(<translate sid="translate">)" << tx << " " << ty << " " << tz <<
				"</translate>" << endl;
			nodeStream << "\t\t\t\t" << R"(<rotate sid ="rotateX">1 0 0 )" << rotations.x << "</rotate>" << endl;
			nodeStream << "\t\t\t\t" << R"(<rotate sid="rotateY">0 1 0 )" << rotations.y << "</rotate>" << endl;
			nodeStream << "\t\t\t\t" << R"(<rotate sid ="rotateZ">0 0 1 )" << rotations.z << "</rotate>" << endl;
			nodeStream << "\t\t\t\t" << R"(<scale sid="scale">)" << bones.at(i).scale.x << " " << bones.at(i).scale.y <<
				" " << bones.at(i).scale.z << R"(</scale>)" << endl;
		}
	}
}

void ColladaWriter::writeHeader()
{
	ofstream outFile(filePath, ios::out | ios::binary | ios::trunc);
	if (!outFile.is_open())
	{
		throw ImporterException("Unable to create output file.");
	}

	time_t rawtime = time(nullptr);
	struct tm timeInfo;
	gmtime_s(&timeInfo, &rawtime);
	int timeSize = sizeof("0000-00-00T00:00:00Z");
	char formattedTime[sizeof("0000-00-00T00:00:00Z")];
	strftime(formattedTime, timeSize, "%Y-%m-%dT%H:%M:%SZ", &timeInfo);

	outFile << R"(<?xml version="1.0" encoding="utf-8"?>)" << endl;
	outFile << R"(<COLLADA xmlns="http://www.collada.org/2005/11/COLLADASchema" version="1.4.1">)" << endl;
	outFile << "\t" << "<asset>" << endl;
	outFile << "\t\t" << "<contributor>" << endl;
	outFile << "\t\t\t" << "<authoring_tool>lol2dae v1.3</authoring_tool>" << endl;
	outFile << "\t\t" << "</contributor>" << endl;
	outFile << "\t\t" << "<created>" << formattedTime << "</created>" << endl;
	outFile << "\t\t" << "<modified>" << formattedTime << "</modified>" << endl;
	outFile << "\t\t" << R"(<unit meter="0.01" name="centimeter"/>)" << endl;
	outFile << "\t\t" << "<up_axis>Y_UP</up_axis>" << endl;
	outFile << "\t" << "</asset>" << endl;

	outFile.close();
}

void ColladaWriter::writeMaterial()
{
	ofstream outFile(filePath, ios::out | ios::binary | ios::app);
	if (!outFile.is_open())
	{
		throw ImporterException("Unable to open output file.");
	}

	outFile << endl;
	outFile << "\t" << "<library_effects>" << endl;
	outFile << "\t\t" << R"(<effect id="DefaultLambert">)" << endl;
	outFile << "\t\t\t" << "<profile_COMMON>" << endl;
	outFile << "\t\t\t\t" << R"(<technique sid="Base">)" << endl;
	outFile << "\t\t\t\t\t" << "<lambert>" << endl;
	outFile << "\t\t\t\t\t\t" << "<emission>" << endl;
	outFile << "\t\t\t\t\t\t\t" << "<color>0.0 0.0 0.0 1.0</color>" << endl;
	outFile << "\t\t\t\t\t\t" << "</emission>" << endl;
	outFile << "\t\t\t\t\t\t" << "<ambient>" << endl;
	outFile << "\t\t\t\t\t\t\t" << "<color>0.0 0.0 0.0 1.0</color>" << endl;
	outFile << "\t\t\t\t\t\t" << "</ambient>" << endl;
	outFile << "\t\t\t\t\t\t" << "<diffuse>" << endl;
	outFile << "\t\t\t\t\t\t\t" << "<color>0.5 0.5 0.5 1.0</color>" << endl;
	outFile << "\t\t\t\t\t\t" << "</diffuse>" << endl;
	outFile << "\t\t\t\t\t" << "</lambert>" << endl;
	outFile << "\t\t\t\t" << "</technique>" << endl;
	outFile << "\t\t\t" << "</profile_COMMON>" << endl;
	outFile << "\t\t" << "</effect>" << endl;
	outFile << "\t" << "</library_effects>" << endl;
	outFile << "\t" << "<library_materials>" << endl;
	outFile << "\t\t" << R"(<material id="DefaultMaterial">)" << endl;
	outFile << "\t\t\t" << R"(<instance_effect url="#DefaultLambert"/>)" << endl;
	outFile << "\t\t" << "</material>" << endl;
	outFile << "\t" << "</library_materials>" << endl;
	outFile << endl;

	outFile.close();
}

void ColladaWriter::writeMesh()
{
	ofstream outFile(filePath, ios::out | ios::binary | ios::app);
	if (!outFile.is_open())
	{
		throw ImporterException("Unable to open output file.");
	}

	ostringstream positionString, normalString, textureString, indiceString;

	for (auto& i : vertices)
	{
		positionString << i.position.x << " ";
		positionString << i.position.y << " ";
		positionString << i.position.z << " ";
		normalString << i.normal.x << " ";
		normalString << i.normal.y << " ";
		normalString << i.normal.z << " ";
		textureString << i.u << " ";
		textureString << i.v << " ";
	}

	for (auto& i : indices)
	{
		indiceString << i << " ";
	}

	outFile << "\t" << "<library_geometries>" << endl;
	outFile << "\t\t" << R"(<geometry id="Mesh" name="Mesh">)" << endl;
	outFile << "\t\t\t" << "<mesh>" << endl;
	outFile << "\t\t\t\t" << R"(<source id="MeshPosition">)" << endl;
	outFile << "\t\t\t\t\t" << R"(<float_array id="MeshPositionArray" count=")" << vertices.size() * 3 << R"(">)" <<
		endl;
	outFile << "\t\t\t\t\t" << positionString.str().c_str() << endl;
	outFile << "\t\t\t\t\t" << "</float_array>" << endl;
	outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<accessor source="#MeshPositionArray" count=")" << vertices.size() <<
		R"(" stride="3">)" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<param name="X" type="float"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<param name="Y" type="float"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<param name="Z" type="float"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << "</accessor>" << endl;
	outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
	outFile << "\t\t\t\t" << "</source>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << R"(<source id="MeshNormal">)" << endl;
	outFile << "\t\t\t\t\t" << R"(<float_array id="MeshNormalArray" count=")" << vertices.size() * 3 << R"(">)" << endl;
	outFile << "\t\t\t\t\t" << normalString.str().c_str() << endl;
	outFile << "\t\t\t\t\t" << "</float_array>" << endl;
	outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<accessor source="#MeshNormalArray" count=")" << vertices.size() <<
		R"(" stride="3">)" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<param name="X" type="float"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<param name="Y" type="float"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<param name="Z" type="float"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << "</accessor>" << endl;
	outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
	outFile << "\t\t\t\t" << "</source>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << R"(<source id="MeshTexture">)" << endl;
	outFile << "\t\t\t\t\t" << R"(<float_array id="MeshTextureArray" count=")" << vertices.size() * 2 << R"(">)" <<
		endl;
	outFile << "\t\t\t\t\t" << textureString.str().c_str() << endl;
	outFile << "\t\t\t\t\t" << "</float_array>" << endl;
	outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<accessor source="#MeshTextureArray" count=")" << vertices.size() <<
		R"(" stride="2">)" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<param name="S" type="float"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<param name="T" type="float"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << "</accessor>" << endl;
	outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
	outFile << "\t\t\t\t" << "</source>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << R"(<vertices id="MeshVertices">)" << endl;
	outFile << "\t\t\t\t\t" << R"(<input semantic="POSITION" source="#MeshPosition"/>)" << endl;
	outFile << "\t\t\t\t" << "</vertices>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << R"(<triangles count=")" << static_cast<int>(indices.size()) / 3 <<
		R"(" material="DefaultMaterial">)" <<
		endl;
	outFile << "\t\t\t\t" << R"(<input semantic="VERTEX" offset="0" source="#MeshVertices"/>)" << endl;
	outFile << "\t\t\t\t" << R"(<input semantic="NORMAL" offset="0" source="#MeshNormal"/>)" << endl;
	outFile << "\t\t\t\t" << R"(<input semantic="TEXCOORD" offset="0" set="0" source="#MeshTexture"/>)" << endl;
	outFile << "\t\t\t\t" << "<p>" << endl;
	outFile << "\t\t\t\t" << indiceString.str().c_str() << endl;
	outFile << "\t\t\t\t" << "</p>" << endl;
	outFile << "\t\t\t\t" << "</triangles>" << endl;
	outFile << "\t\t\t" << "</mesh>" << endl;
	outFile << "\t\t" << "</geometry>" << endl;
	outFile << "\t" << "</library_geometries>" << endl;
	outFile << endl;

	outFile.close();
}

void ColladaWriter::writeSkin()
{
	ofstream outFile(filePath, ios::out | ios::binary | ios::app);
	if (!outFile.is_open())
	{
		throw ImporterException("Unable to open output file.");
	}

	ostringstream jointString;
	ostringstream inverseMatrixString;

	for (auto& i : boneIndices)
	{
		jointString << bones.at(i).name << " ";

		Matrix4x4 inverseMatrix;
		inverseMatrix = bones.at(i).globalMatrix.gjInverse();

		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				inverseMatrixString << inverseMatrix[r][c] << " ";
			}
		}
	}

	ostringstream weightString;
	ostringstream vertexCountString;
	ostringstream jointIndiceString;
	int weightIndexCount = 0;

	weightString << "1 ";
	++weightIndexCount;

	for (auto& i : vertices)
	{
		int numWeights = 0;

		for (int j = 0; j < 4; ++j)
		{
			int weightIndex;

			if (i.boneWeights[j] <= 0.0f)
			{
				continue;
			}
			if (i.boneWeights[j] >= 1.0f)
			{
				weightIndex = 0;
			}

			else
			{
				weightString << i.boneWeights[j] << " ";
				weightIndex = weightIndexCount;
				++weightIndexCount;
			}

			jointIndiceString << static_cast<int>(i.boneIndices[j]) << " " << weightIndex << " ";
			++numWeights;
		}

		vertexCountString << numWeights << " ";
	}

	outFile << "\t" << "<library_controllers>" << endl;
	outFile << "\t\t" << R"(<controller id="MeshSkin" name="MeshSkin">)" << endl;
	outFile << "\t\t\t" << R"(<skin source="#Mesh">)" << endl;
	outFile << "\t\t\t\t" << "<bind_shape_matrix>" << endl;
	outFile << "\t\t\t\t" << "1 0 0 0" << endl;
	outFile << "\t\t\t\t" << "0 1 0 0" << endl;
	outFile << "\t\t\t\t" << "0 0 1 0" << endl;
	outFile << "\t\t\t\t" << "0 0 0 1" << endl;
	outFile << "\t\t\t\t" << "</bind_shape_matrix>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << R"(<source id="MeshSkinJoints">)" << endl;
	outFile << "\t\t\t\t\t" << R"(<Name_array id="MeshSkinJointsArray" count=")" << boneIndices.size() << R"(">)" <<
		endl;
	outFile << "\t\t\t\t\t" << jointString.str().c_str() << endl;
	outFile << "\t\t\t\t\t" << "</Name_array>" << endl;
	outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<accessor source="#MeshSkinJointsArray" count=")" << boneIndices.size() <<
		R"(" stride="1">)" << endl;
	outFile << "\t\t\t\t\t\t\t" << R"(<param name="JOINT" type="Name"></param>)" << endl;
	outFile << "\t\t\t\t\t\t" << "</accessor>" << endl;
	outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
	outFile << "\t\t\t\t" << "</source>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << R"(<source id="MeshSkinBindPoses">)" << endl;
	outFile << "\t\t\t\t\t" << R"(<float_array id="MeshSkinBindPosesArray" count=")" << boneIndices.size() * 16 <<
		R"(">)" << endl;
	outFile << "\t\t\t\t\t" << inverseMatrixString.str().c_str() << endl;
	outFile << "\t\t\t\t\t" << "</float_array>" << endl;
	outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<accessor source="#MeshSkinBindPosesArray" count=")" << boneIndices.size() <<
		R"(" stride="16">)" << endl;
	outFile << "\t\t\t\t\t\t\t" << R"(<param name="TRANSFORM" type="float4x4"></param>)" << endl;
	outFile << "\t\t\t\t\t\t" << "</accessor>" << endl;
	outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
	outFile << "\t\t\t\t" << "</source>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << R"(<source id="MeshSkinWeights">)" << endl;
	outFile << "\t\t\t\t\t" << R"(<float_array id="MeshSkinWeightsArray" count=")" << weightIndexCount << R"(">)" <<
		endl;
	outFile << "\t\t\t\t\t" << weightString.str().c_str() << endl;
	outFile << "\t\t\t\t\t" << "</float_array>" << endl;
	outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
	outFile << "\t\t\t\t\t\t" << R"(<accessor source="#MeshSkinWeightsArray" count=")" << weightIndexCount <<
		R"(" stride="1">)" << endl;
	outFile << "\t\t\t\t\t\t\t" << R"(<param name="WEIGHT" type="float"></param>)" << endl;
	outFile << "\t\t\t\t\t\t" << "</accessor>" << endl;
	outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
	outFile << "\t\t\t\t" << "</source>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << "<joints>" << endl;
	outFile << "\t\t\t\t\t" << R"(<input semantic="JOINT" source="#MeshSkinJoints"></input>)" << endl;
	outFile << "\t\t\t\t\t" << R"(<input semantic="INV_BIND_MATRIX" source="#MeshSkinBindPoses"></input>)" << endl;
	outFile << "\t\t\t\t" << "</joints>" << endl;
	outFile << endl;

	outFile << "\t\t\t\t" << R"(<vertex_weights count=")" << vertices.size() << R"(">)" << endl;
	outFile << "\t\t\t\t\t" << R"(<input semantic="JOINT" source="#MeshSkinJoints" offset="0"></input>)" << endl;
	outFile << "\t\t\t\t\t" << R"(<input semantic="WEIGHT" source="#MeshSkinWeights" offset="1"></input>)" << endl;
	outFile << "\t\t\t\t\t" << "<vcount>" << endl;
	outFile << "\t\t\t\t\t" << vertexCountString.str().c_str() << endl;
	outFile << "\t\t\t\t\t" << "</vcount>" << endl;
	outFile << "\t\t\t\t\t" << "<v>" << endl;
	outFile << "\t\t\t\t\t" << jointIndiceString.str().c_str() << endl;
	outFile << "\t\t\t\t\t" << "</v>" << endl;
	outFile << "\t\t\t\t" << "</vertex_weights>" << endl;
	outFile << "\t\t\t" << "</skin>" << endl;
	outFile << "\t\t" << "</controller>" << endl;
	outFile << "\t" << "</library_controllers>" << endl;
	outFile << endl;

	outFile.close();
}

void ColladaWriter::writeScene(Mode& mode)
{
	ofstream outFile(filePath, ios::out | ios::binary | ios::app);
	if (!outFile.is_open())
	{
		throw ImporterException("Unable to open output file.");
	}

	if (mode == Mode::Mesh)
	{
		outFile << "\t" << "<library_visual_scenes>" << endl;
		outFile << "\t\t" << R"(<visual_scene id="DefaultScene">)" << endl;
		outFile << "\t\t\t" << R"(<node id="Model" name="Model">)" << endl;
		outFile << "\t\t\t\t" << "<translate>0 0 0</translate>" << endl;
		outFile << "\t\t\t\t" << "<rotate>0 0 1 0</rotate>" << endl;
		outFile << "\t\t\t\t" << "<rotate>0 1 0 0</rotate>" << endl;
		outFile << "\t\t\t\t" << "<rotate>1 0 0 0</rotate>" << endl;
		outFile << "\t\t\t\t" << "<scale>1 1 1</scale>" << endl;
		outFile << "\t\t\t\t" << R"(<instance_geometry url="#Mesh">)" << endl;
		outFile << "\t\t\t\t\t" << "<bind_material>" << endl;
		outFile << "\t\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t\t\t" << R"(<instance_material symbol="DefaultMaterial" target="#DefaultMaterial"/>)" <<
			endl;
		outFile << "\t\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t\t" << "</bind_material>" << endl;
		outFile << "\t\t\t\t" << "</instance_geometry>" << endl;
		outFile << "\t\t\t" << "</node>" << endl;
		outFile << "\t\t" << "</visual_scene>" << endl;
		outFile << "\t" << "</library_visual_scenes>" << endl;
		outFile << "\t" << "<scene>" << endl;
		outFile << "\t\t" << R"(<instance_visual_scene url="#DefaultScene"/>)" << endl;
		outFile << "\t" << "</scene>" << endl;
		outFile << "</COLLADA>" << endl;

		outFile.close();

		return;
	}

	deque<int> boneIdDeque;

	for (size_t i = 0; i < bones.size(); ++i)
	{
		boneIdDeque.push_back(i);
	}

	while (!boneIdDeque.empty())
	{
		bool res = emplaceNode(boneIdDeque.front());

		if (!res)
		{
			boneIdDeque.push_back(boneIdDeque.front());
		}

		boneIdDeque.pop_front();
	}

	nodeStream.clear();
	createNodeStringStream();
	nodes.clear();

	outFile << "\t" << "<library_visual_scenes>" << endl;
	outFile << "\t\t" << R"(<visual_scene id="DefaultScene">)" << endl;
	outFile << nodeStream.str().c_str() << endl;
	outFile << "\t\t\t" << R"(<node id="Model" name="Model">)" << endl;
	outFile << "\t\t\t\t" << R"(<instance_controller url="#MeshSkin">)" << endl;
	outFile << "\t\t\t\t\t" << "<skeleton>#DefaultScene</skeleton>" << endl;
	outFile << "\t\t\t\t\t" << "<bind_material>" << endl;
	outFile << "\t\t\t\t\t\t" << "<technique_common>" << endl;
	outFile << "\t\t\t\t\t\t\t" << R"(<instance_material symbol="DefaultMaterial" target="#DefaultMaterial"/>)" << endl;
	outFile << "\t\t\t\t\t\t" << "</technique_common>" << endl;
	outFile << "\t\t\t\t\t" << "</bind_material>" << endl;
	outFile << "\t\t\t\t" << "</instance_controller>" << endl;
	outFile << "\t\t\t" << "</node>" << endl;
	outFile << "\t\t" << "</visual_scene>" << endl;
	outFile << "\t" << "</library_visual_scenes>" << endl;
	outFile << "\t" << "<scene>" << endl;
	outFile << "\t\t" << R"(<instance_visual_scene url="#DefaultScene"/>)" << endl;
	outFile << "\t" << "</scene>" << endl;
	outFile << "</COLLADA>" << endl;

	outFile.close();
}

void ColladaWriter::writeAnimation()
{
	ofstream outFile(filePath, ios::out | ios::binary | ios::app);
	if (!outFile.is_open())
	{
		throw ImporterException("Unable to open output file.");
	}

	outFile << "\t" << "<library_animations>" << endl;
	outFile << "\t\t" << "<animation>" << endl;

	for (auto& i : animation.bones)
	{
		ostringstream translationInputStream;
		ostringstream translationInterpolationStream;

		for (auto& k : i.translation)
		{
			translationInputStream << k.first << " ";
			translationInterpolationStream << "LINEAR ";
		}


		outFile << "\t\t\t" << "<animation>" << endl;
		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(TranslationInput">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(TranslationInputArray" count=")" << i
		                                                                                                      .
		                                                                                                      translation
		                                                                                                      .size() <<
			R"(">)" << endl;
		outFile << "\t\t\t\t\t" << translationInputStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(TranslationInputArray" count=")" << i
		                                                                                                        .
		                                                                                                        translation
		                                                                                                        .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="TIME" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(TranslationInterpolation">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<Name_array id=")" << i.name << R"(TranslationInterpolationArray" count=")" << i
		                                                                                                             .
		                                                                                                             translation
		                                                                                                             .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << translationInterpolationStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</Name_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(TranslationInterpolationArray" count=")" <<
			i.translation.size() << R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="INTERPOLATION" type="Name"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(Translation">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(TranslationArray" count=")" << i
		                                                                                                 .translation.
		                                                                                                 size() * 3 <<
			R"(">)" << endl;

		outFile << "\t\t\t\t\t";

		for (auto& t : i.translation)
		{
			outFile << t.second.x << " " << t.second.y << " " << t.second.z << " ";
		}

		outFile << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor count=")" << i.translation.size() << R"(" stride="3" source="#)" << i.
			name << R"(TranslationArray">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="X" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="Y" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="Z" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<sampler id=")" << i.name << R"(TranslateSampler">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INPUT" source="#)" << i.name << R"(TranslationInput"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INTERPOLATION" source="#)" << i.name <<
			R"(TranslationInterpolation"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="OUTPUT" source="#)" << i.name << R"(Translation"/>)" << endl;
		outFile << "\t\t\t\t" << "</sampler>" << endl;
		outFile << "\t\t\t\t" << R"(<channel source="#)" << i.name << R"(TranslateSampler" target=")" << i.name <<
			R"(/translate"/>)" << endl;
		outFile << "\t\t\t" << "</animation>" << endl;
		outFile << endl;

		ostringstream rotationInputStream;
		ostringstream rotationInterpolationStream;

		for (auto& k : i.quaternion)
		{
			rotationInputStream << k.first << " ";
			rotationInterpolationStream << "LINEAR ";
		}


		outFile << "\t\t\t" << "<animation>" << endl;
		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateXInput">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(RotateXInputArray" count=")" << i
		                                                                                                  .quaternion.
		                                                                                                  size() <<
			R"(">)" << endl;
		outFile << "\t\t\t\t\t" << rotationInputStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(RotateXInputArray" count=")" << i
		                                                                                                    .quaternion.
		                                                                                                    size() <<
			R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="TIME" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateXInterpolation">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<Name_array id=")" << i.name << R"(RotateXInterpolationArray" count=")" << i
		                                                                                                         .
		                                                                                                         quaternion
		                                                                                                         .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << rotationInterpolationStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</Name_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(RotateXInterpolationArray" count=")" << i
		                                                                                                            .
		                                                                                                            quaternion
		                                                                                                            .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="INTERPOLATION" type="Name"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		vector<Euler> ea;
		ea.resize(i.quaternion.size());

		for (size_t a = 0; a < ea.size(); ++a)
		{
			ea.at(a).extract(i.quaternion.at(a).second.inverse());
			ea.at(a).negate();
		}

		for (size_t a = 1; a < ea.size(); ++a)
		{
			Euler nr;
			nr.nearestRotation(ea.at(a), ea.at(a - 1));
		}

		for (auto& a : ea)
		{
			EulerUtilities::radiansToDegrees(a);
		}

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateX">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(RotateXArray" count=")" << i.quaternion.size()
			<< R"(">)" << endl;

		outFile << "\t\t\t\t\t";

		for (auto& a : ea)
		{
			outFile << a.x << " ";
		}

		outFile << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor count=")" << i.quaternion.size() << R"(" source="#)" << i.name <<
			R"(RotateXArray">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="ANGLE" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<sampler id=")" << i.name << R"(RotateXSampler">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INPUT" source="#)" << i.name << R"(RotateXInput"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INTERPOLATION" source="#)" << i.name <<
			R"(RotateXInterpolation"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="OUTPUT" source="#)" << i.name << R"(RotateX"/>)" << endl;
		outFile << "\t\t\t\t" << "</sampler>" << endl;
		outFile << "\t\t\t\t" << R"(<channel source="#)" << i.name << R"(RotateXSampler" target=")" << i.name <<
			R"(/rotateX.ANGLE"/>)" << endl;
		outFile << "\t\t\t" << "</animation>" << endl;
		outFile << endl;


		outFile << "\t\t\t" << "<animation>" << endl;
		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateYInput">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(RotateYInputArray" count=")" << i
		                                                                                                  .quaternion.
		                                                                                                  size() <<
			R"(">)" << endl;
		outFile << "\t\t\t\t\t" << rotationInputStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(RotateYInputArray" count=")" << i
		                                                                                                    .quaternion.
		                                                                                                    size() <<
			R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="TIME" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateYInterpolation">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<Name_array id=")" << i.name << R"(RotateYInterpolationArray" count=")" << i
		                                                                                                         .
		                                                                                                         quaternion
		                                                                                                         .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << rotationInterpolationStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</Name_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(RotateYInterpolationArray" count=")" << i
		                                                                                                            .
		                                                                                                            quaternion
		                                                                                                            .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="INTERPOLATION" type="Name"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateY">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(RotateYArray" count=")" << i.quaternion.size()
			<< R"(">)" << endl;

		outFile << "\t\t\t\t\t";

		for (auto& a : ea)
		{
			outFile << a.y << " ";
		}

		outFile << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor count=")" << i.quaternion.size() << R"(" source="#)" << i.name <<
			R"(RotateYArray">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="ANGLE" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<sampler id=")" << i.name << R"(RotateYSampler">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INPUT" source="#)" << i.name << R"(RotateYInput"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INTERPOLATION" source="#)" << i.name <<
			R"(RotateYInterpolation"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="OUTPUT" source="#)" << i.name << R"(RotateY"/>)" << endl;
		outFile << "\t\t\t\t" << "</sampler>" << endl;
		outFile << "\t\t\t\t" << R"(<channel source="#)" << i.name << R"(RotateYSampler" target=")" << i.name <<
			R"(/rotateY.ANGLE"/>)" << endl;
		outFile << "\t\t\t" << "</animation>" << endl;
		outFile << endl;


		outFile << "\t\t\t" << "<animation>" << endl;
		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateZInput">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(RotateZInputArray" count=")" << i
		                                                                                                  .quaternion.
		                                                                                                  size() <<
			R"(">)" << endl;
		outFile << "\t\t\t\t\t" << rotationInputStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(RotateZInputArray" count=")" << i
		                                                                                                    .quaternion.
		                                                                                                    size() <<
			R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="TIME" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateZInterpolation">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<Name_array id=")" << i.name << R"(RotateZInterpolationArray" count=")" << i
		                                                                                                         .
		                                                                                                         quaternion
		                                                                                                         .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << rotationInterpolationStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</Name_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(RotateZInterpolationArray" count=")" << i
		                                                                                                            .
		                                                                                                            quaternion
		                                                                                                            .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="INTERPOLATION" type="Name"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(RotateZ">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(RotateZArray" count=")" << i.quaternion.size()
			<< R"(">)" << endl;

		outFile << "\t\t\t\t\t";

		for (auto& a : ea)
		{
			outFile << a.z << " ";
		}

		outFile << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor count=")" << i.quaternion.size() << R"(" source="#)" << i.name <<
			R"(RotateZArray">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="ANGLE" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<sampler id=")" << i.name << R"(RotateZSampler">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INPUT" source="#)" << i.name << R"(RotateZInput"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INTERPOLATION" source="#)" << i.name <<
			R"(RotateZInterpolation"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="OUTPUT" source="#)" << i.name << R"(RotateZ"/>)" << endl;
		outFile << "\t\t\t\t" << "</sampler>" << endl;
		outFile << "\t\t\t\t" << R"(<channel source="#)" << i.name << R"(RotateZSampler" target=")" << i.name <<
			R"(/rotateZ.ANGLE"/>)" << endl;
		outFile << "\t\t\t" << "</animation>" << endl;

		ostringstream scaleInputStream;
		ostringstream scaleInterpolationStream;

		for (auto& k : i.scale)
		{
			scaleInputStream << k.first << " ";
			scaleInterpolationStream << "LINEAR ";
		}


		outFile << "\t\t\t" << "<animation>" << endl;
		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(ScaleInput">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(ScaleInputArray" count=")" << i.scale.size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << scaleInputStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(ScaleInputArray" count=")" << i.scale.size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="TIME" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(ScaleInterpolation">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<Name_array id=")" << i.name << R"(ScaleInterpolationArray" count=")" << i
		                                                                                                       .scale.
		                                                                                                       size() <<
			R"(">)" << endl;
		outFile << "\t\t\t\t\t" << scaleInterpolationStream.str().c_str() << endl;
		outFile << "\t\t\t\t\t" << "</Name_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor source="#)" << i.name << R"(ScaleInterpolationArray" count=")" << i
		                                                                                                          .scale
		                                                                                                          .size()
			<< R"(">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="INTERPOLATION" type="Name"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<source id=")" << i.name << R"(Scale">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<float_array id=")" << i.name << R"(ScaleArray" count=")" << i.scale.size() * 3 <<
			R"(">)" << endl;

		outFile << "\t\t\t\t\t";

		for (auto& s : i.scale)
		{
			outFile << s.second.x << " " << s.second.y << " " << s.second.z << " ";
		}

		outFile << endl;
		outFile << "\t\t\t\t\t" << "</float_array>" << endl;
		outFile << "\t\t\t\t\t" << "<technique_common>" << endl;
		outFile << "\t\t\t\t\t" << R"(<accessor count=")" << i.scale.size() << R"(" stride="3" source="#)" << i.name <<
			R"(ScaleArray">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="X" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="Y" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<param name="Z" type="float"/>)" << endl;
		outFile << "\t\t\t\t\t" << "</accessor>" << endl;
		outFile << "\t\t\t\t\t" << "</technique_common>" << endl;
		outFile << "\t\t\t\t" << "</source>" << endl;
		outFile << endl;

		outFile << "\t\t\t\t" << R"(<sampler id=")" << i.name << R"(ScaleSampler">)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INPUT" source="#)" << i.name << R"(ScaleInput"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="INTERPOLATION" source="#)" << i.name <<
			R"(ScaleInterpolation"/>)" << endl;
		outFile << "\t\t\t\t\t" << R"(<input semantic="OUTPUT" source="#)" << i.name << R"(Scale"/>)" << endl;
		outFile << "\t\t\t\t" << "</sampler>" << endl;
		outFile << "\t\t\t\t" << R"(<channel source="#)" << i.name << R"(ScaleSampler" target=")" << i.name <<
			R"(/scale"/>)" << endl;
		outFile << "\t\t\t" << "</animation>" << endl;
		outFile << endl;
	}

	outFile << "\t\t" << "</animation>" << endl;
	outFile << "\t" << "</library_animations>" << endl;

	outFile.close();
}

void ColladaWriter::writeFile(const wchar_t* path, Mode& mode)
{
	filePath = path;

	writeHeader();
	writeMaterial();
	writeMesh();

	if (mode >= Mode::Skeleton)
	{
		writeSkin();
	}

	if (mode == Mode::Animation)
	{
		writeAnimation();
	}

	writeScene(mode);
}

ColladaWriter::~ColladaWriter()
{
}
