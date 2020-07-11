start_sources()

sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Root"
		"StylePlugin.cpp"
		"StylePlugin.h"
		"StdAfx.h"
		"StdAfx.cpp"
)

end_sources()


CryEditorPlugin(StyleChanger PCH "StdAfx.cpp" SOLUTION_FOLDER "Sandbox/Plugins")

target_include_directories(${THIS_PROJECT} PRIVATE ../../EditorQt)	
target_include_directories(${THIS_PROJECT} PRIVATE ../../EditorQt/Include)
target_include_directories(${THIS_PROJECT} PRIVATE "${CRYENGINE_DIR}/Code/SDKs/Qt/5.6/msvc2015_64/Qt/include/QtGui/5.6.0/")