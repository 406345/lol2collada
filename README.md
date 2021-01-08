# lol2collada

**Current version** : `1.1.0` [CHANGELOG.md](https://github.com/cqtd/lol2collada/blob/main/CHANGELOG.md)



## Usage

1. Open command prompt.
2. cd to root folder that lol2collada.exe exists.
3. Run program with arguments.

```batch
lol2collada [skn file path] [skl file path] [animation folder root] [output folder root]
```

4. `.dae` files will generated at animations folder.

### Example

* cmd

```batch
lol2collada "c:/extracted/character/garen/garen.skn" "c:/extracted/character/garen/garen.skl" "c:/extracted/character/garen/animations" "c:/converted/garen"
```

* .NET (unity)

```csharp
public class LolCharacterUtil
{
	public class Native
	{
		[DllImport("lol2collada")]
		public static extern int Export(string skin, string skel, string anims, string output);
	}

	public static void Import(string champion, string folder, string output, int skin = 0)
	{
		string skinFolder = skin == 0 ? "base" : $"skin{skin:00}";
		string folderRoot = $"{folder}\\{champion}\\skins\\{skinFolder}";
            
		string skn = folderRoot + $"\\{champion}.skn";
		string skl = folderRoot + $"\\{champion}.skl";
            
		string animationFolderPath = folderRoot + "\\animations";

		if (!new FileInfo(skn).Exists)
		{
			throw new FileNotFoundException($"{skn}");
		}
			
		if (!new FileInfo(skl).Exists)
		{
			throw new FileNotFoundException($"{skl}");
		}
			
		if (!new DirectoryInfo(animationFolderPath).Exists)
		{
			throw new DirectoryNotFoundException($"{animationFolderPath}");
		}
			
		string path = $"output/{champion}";
		path = path.Replace("/", "\\");
			
		DirectoryInfo destination = new DirectoryInfo(path);

		if (!destination.Exists)
		{
			destination.Create();
		}
			
		int result = Native.Export(
			skn,
			skl,
			animationFolderPath,
			path
		);

		if (result == 0)
		{
			Debug.Log("Success");
		}
		else
		{
			Debug.LogError("Failed");
		}
	}
}
```





## Thanks to

- This library is based on lol2dae GUI.
- lol2dae uses [ImageMagic/exr](https://github.com/ImageMagick/exr).