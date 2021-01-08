#include "pch.h"

#include "SkinImporter.h"
#include "SkeletonImporter.h"
#include "AnimImporter.h"
#include "ColladaWriter.h"

#include <iostream>
#include <filesystem>

using std::filesystem::directory_iterator;

int process(char* skinPath, char* skeletonPath, char* animationFolder, char* output)
{
	int success = 0;

	vector<filesystem::path> animations;

	for (const auto& file : directory_iterator(animationFolder))
	{
		if (file.path().extension() == ".anm")
		{
			cout << "Animation : " << file.path() << endl;

			auto path = file.path();
			animations.push_back(path);
		}
	}

	auto mode = ColladaWriter::Mode::Animation;
	auto outputRoot = std::string(output);

	if (mode >= ColladaWriter::Mode::Skeleton)
	{
		if (mode == ColladaWriter::Mode::Animation)
		{
			for (const auto& animationPath : animations)
			{
				try
				{
					SkinImporter skin;
					skin.ReadFile(skinPath);

					SkeletonImporter skeleton(skin.fileVersion);
					AnimImporter anim(skeleton.boneHashes);

					skeleton.ReadFile(skeletonPath);

					auto wpath = animationPath.wstring();
					auto path = wpath.c_str();
					anim.ReadFile(path);

					auto animationFileName = animationPath.filename().string();
					auto formatted = animationFileName.replace(animationFileName.end() - 3, animationFileName.end(), "dae");

					auto outputPath = outputRoot + string("/");
					outputPath += formatted;
					
					auto w_outputPath = wstring(outputPath.begin(), outputPath.end());
					
					cout << "Generate... " << outputPath << endl;

					ColladaWriter outputCollada(
						skin.indices,
						skin.vertices,
						skeleton.bones,
						skeleton.boneIndices,
						anim);
					
					outputCollada.writeFile(w_outputPath.c_str(), mode);

					success++;
				}
				catch (int ex)
				{
					cout << "\t\t [Exception] : " << ex << endl;
				}
			}
		}
	}

	return 0;
}

#ifdef DLL_EXPORTS

extern "C" {
	DLL_API int Export(char* skin_path, char* skel_path, char* animation_folder, char* output_folder)
	{
		cout << "Skin : " << skin_path << endl;
		cout << "Skeleton : " << skel_path << endl;

		return process(skin_path, skel_path, animation_folder, output_folder);
	}
}

#else

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		cout << "usage:" << endl;
		cout << "  lol2collada skinFile skeletonFile animationFolder outputRoot" << endl << endl;
		cout << "  ex) lol2collada base/garen.skn base/garen.skl base/animations base/outputs" << endl;

		return 0;
	}

	return process(argv[1], argv[2], argv[3], argv[4]);
}

#endif