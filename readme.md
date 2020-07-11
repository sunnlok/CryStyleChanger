A CryEngine Sandbox plugin that allows changing the Stylesheet as well as the Icons used.

To use this plugin, place the **StyleChanger.dll** into:
> {Your Engine Folder}/bin/win_x64/EditorPlugins

## Style
The Stylesheet needs to be a valid .qss file. See the default [Sandbox stylesheet.qss](https://github.com/CRYTEK/CRYENGINE/blob/release/Editor/Styles/stylesheet.qss) for reference.
## Icons
The Icon folder structured used needs to align with the aliases defined in the [Editors .qrc](https://github.com/CRYTEK/CRYENGINE/blob/release/Code/Sandbox/Plugins/EditorCommon/EditorCommon.qrc) files.
Default icons can be found [here](https://github.com/CRYTEK/CRYENGINE/tree/release/Code/Sandbox/Plugins/EditorCommon/icons).
Icons need to be standard multi resolution .ico files. Specifcation for this can be found in the [CryEngine Documentation](https://docs.cryengine.com/display/CEPROG/Theme%2C+Styling+and+Colors).

A simple powershell script to convert PNGs to icons with ImageMagick can look like this:
```powershell
$icons = Get-ChildItem -Filter "*.png" -Recurse -File
foreach($icon in $icons){
$iconPath = Join-Path $icon.DirectoryName $icon.BaseName

$png = $iconPath + ".png"
$ico = $iconPath + ".ico"

magick.exe convert -background transparent $png -define icon:auto-resize=16,24,32,48,64,72,96,128,256 $ico
}
```

#### Note:
Icon names inside filesystem folders need to be named after their qrc alias, not the actual file name! Otherwise the icon path used in code won't match up with the correct file.
