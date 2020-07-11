A CryEngine Sandbox plugin that allows changing the Stylesheet as well as the Icons used.

## Style
The Stylesheet needs to be a valid .qss file. See the default [Sandbox stylesheet.qss](https://github.com/CRYTEK/CRYENGINE/blob/release/Editor/Styles/stylesheet.qss) for reference.
## Icons
The Icon folder structured used needs to align with the aliases defined in the [Editors .qrc](https://github.com/CRYTEK/CRYENGINE/blob/release/Code/Sandbox/Plugins/EditorCommon/EditorCommon.qrc) files.
The default icons can be found [here](https://github.com/CRYTEK/CRYENGINE/tree/release/Code/Sandbox/Plugins/EditorCommon/icons)
#### Note:
Icon names inside filesystem folders need to be named after their qrc alias, not the actual file name! Otherwise the icon path used in code won't match up with the correct file.
