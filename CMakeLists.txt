start_sources()

sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Root"
		"StylePlugin.cpp"
		"StylePlugin.h"
		"IconPack.h"
		"IconPack.cpp"
		"IconPackManger.h"
		"IconPackManger.cpp"
		"StdAfx.h"
		"StdAfx.cpp"
)

end_sources()


CryEditorPlugin(StyleChanger PCH "StdAfx.cpp" SOLUTION_FOLDER "Sandbox/Plugins")

target_include_directories(${THIS_PROJECT} PRIVATE ../../EditorQt)	
target_include_directories(${THIS_PROJECT} PRIVATE ../../EditorQt/Include)
target_include_directories(${THIS_PROJECT} PRIVATE "${SDK_DIR}/Qt/5.12.3_python_37/msvc2017_64/Qt/include/QtGui/5.12.3")
target_include_directories(${THIS_PROJECT} PRIVATE "${SDK_DIR}/Qt/5.12.3_python_37/msvc2017_64/Qt/include/QtCore/5.12.3")