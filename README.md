# lol2collada

## Usage

1. Open command prompt.
2. cd to root folder that lol2collada.exe exists.
3. Run program with arguments.

```batch
lol2collada [skn file path] [skl file path] [animation folder root]
```

4. `.dae` files will generated at animations folder.

### Example

```batch
lol2collada "c:/extracted/character/garen/garen.skn" "c:/extracted/character/garen/garen.skl" "c:/extracted/character/garen/animations"
```



## Thanks to

- This library is based on lol2dae GUI.
- lol2dae uses [ImageMagic/exr](https://github.com/ImageMagick/exr).