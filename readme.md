A CryEngine Sandbox plugin that allows that allows to completely change the visual appearance of the editor, be it the stylesheet or icons.

To use this plugin, place the **StyleChanger.dll** into:
> {Your Engine Folder}/bin/win_x64/EditorPlugins

From then on you can change the visual appearance of the editor via the **Style** entry inside the preferences menu.

## Style
---
Allows loading of a custom stylesheet via the stylesheet option. The loaded stylesheet is refreshed via a filewatcher so changes are instantly updated.

The Stylesheet needs to be a valid .qss file. See the default [Sandbox stylesheet.qss](https://github.com/CRYTEK/CRYENGINE/blob/release/Editor/Styles/stylesheet.qss) for reference.
## Icons
---
Custom icons can be loaded via one or more **Icon Packs**.

 Icons need to be standard multi resolution .ico files. Specifcation for this can be found in the [CryEngine Documentation](https://docs.cryengine.com/display/CEPROG/Theme%2C+Styling+and+Colors).

 The Default icons can be found [here](https://github.com/CRYTEK/CRYENGINE/tree/release/Code/Sandbox/Plugins/EditorCommon/icons).

### Icon Pack:

An Icon Pack is defined via a **.crysip** file, which is a simple json file used to load the actual icon resources.

It is structured like this:
```json
{
	"isIconFolder" : false,
	"resources" : [
		{
			"prefix" : "/myIcons",
			"file"	 : "./myIcons.rcc"
		}
	]
}
```

The seperate parameters do the following:

- `isIconFolder`
    
    Signifies if icons are stored as plain files relative to the **.crysip** file.
    The folder structure needs to match the aliases used inside the [Editors .qrc files](https://github.com/CRYTEK/CRYENGINE/blob/release/Code/Sandbox/Plugins/EditorCommon/EditorCommon.qrc), not the filenames.
- `resources`
        
    A list of descriptors for binary **.rcc** files that can be compiled from a **.qrc** file with the qt resource compiler.
    
    Otherwise the icon path used in code won't match up with the correct file.
    
    - `prefix`

        The prefix used inside the qresource definition of the **.qrc** file.
        If this does not match the pack won't load properly.
        See example for details.
    - `file`

        The relative path to a **.rcc** file. 

### Resource files:

Binary resource files **(.rcc)** can be generated from a **.qrc** QT resource file. This file contains aliases that map to actual icon resources. This gets then compiled to a binary via the QT resource compiler with a simple command:
>rcc -binary myIcons.qrc -o myIcons.rcc

The aliases used need to match the ones used in the default [Editor .qrc files](https://github.com/CRYTEK/CRYENGINE/blob/release/Code/Sandbox/Plugins/EditorCommon/EditorCommon.qrc) to properly override the desired icons.

An example **.qrc** could look like this:
```xml
<RCC>
	<qresource prefix="/myIcons">
		<file alias="General/Folder.ico">General/Folder.ico</file>
		<file alias="General/Folder_Open.ico">General/Folder.ico</file>
		<file alias="General/History_Redo.ico">General/History_Redo.ico</file>
		<file alias="General/History_Undo.ico">General/History_Undo.ico</file>
	</qresource>
</RCC>
```
The specified icon files are relative to the **.qrc** location.


---
#### Converting Images

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