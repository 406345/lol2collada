#include "pch.h"

#include "SkinImporter.h"
#include "SkeletonImporter.h"
#include "AnimImporter.h"
#include "ColladaWriter.h"

#include <iostream>
#include <filesystem>

extern "C" {
DLL_API void Export()
{
}
}

using std::filesystem::directory_iterator;

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		cout << "usage:" << endl;
		cout << "  lol2collada skinFile skeletonFile animationFolder" << endl << endl;
		cout << "  ex) lol2collada base/garen.skn base/garen.skl base/animations" << endl;

		return 0;
	}

	auto skinFilePath = argv[1];
	auto skeletonFilePath = argv[2];
	auto animationFolderRoot = argv[3];

	cout << "Skin : " << skinFilePath << endl;
	cout << "Skeleton : " << skeletonFilePath << endl;

	vector<wstring> animations;

	for (const auto& file : directory_iterator(animationFolderRoot))
	{
		if (file.path().extension() == ".anm")
		{
			cout << "Animation : " << file.path() << endl;

			auto name = file.path().wstring();
			animations.push_back(name);
		}
	}

	auto mode = ColladaWriter::Mode::Animation;

	if (mode >= ColladaWriter::Mode::Skeleton)
	{
		if (mode == ColladaWriter::Mode::Animation)
		{
			for (auto animFile : animations)
			{
				try
				{
					SkinImporter skin;
					skin.ReadFile(skinFilePath);

					SkeletonImporter skeleton(skin.fileVersion);
					AnimImporter anim(skeleton.boneHashes);

					skeleton.ReadFile(skeletonFilePath);

					auto path = animFile.c_str();
					anim.ReadFile(path);

					std::wstring str(animFile);

					string daeFile;

					daeFile.assign(str.begin(), str.end());
					daeFile.replace(daeFile.end() - 3, daeFile.end(), "dae");
					wstring wdae = wstring(daeFile.begin(), daeFile.end());

					cout << "Generate... : " << daeFile << endl;

					ColladaWriter outputCollada(skin.indices, skin.vertices, skeleton.bones, skeleton.boneIndices,
					                            anim);
					outputCollada.writeFile(wdae.c_str(), mode);
				}
				catch (int ex)
				{
					cout << "\t\t [Exception] : " << ex << endl;
				}
			}
		}
	}

	cout << "Done! " << endl;

	return 0;
}
