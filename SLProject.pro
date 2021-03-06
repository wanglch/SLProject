requires(qtHaveModule(opengl))

TEMPLATE = subdirs
SUBDIRS += lib-SLExternal
SUBDIRS += lib-SLExternal/assimp
SUBDIRS += lib-SLProject
SUBDIRS += app-Demo-GLFW
SUBDIRS += app-NodeManipulationDemo
SUBDIRS += exercises/ch06_ColorCube
SUBDIRS += exercises/ch07_DiffuseCube
SUBDIRS += exercises/ch09_TextureMapping
SUBDIRS += exercises/cv01_ChangeBrightnessAndContrast
SUBDIRS += exercises/cv13_HeadPoseEstimation
SUBDIRS += exercises/cv13_FaceTracking

lib-SLProject.depends = lib-SLExternal
lib-SLProject.depends = lib-SLExternal/assimp
app-Demo-GLFW.depends = lib-SLProject
app-NodeManipulationDemo.depends = lib-SLProject
ch06_ColorCube.depends = lib-SLExternal
ch07_DiffuseCube.depends = lib-SLExternal
ch09_TextureMapping.depends = lib-SLExternal

CONFIG -= qml_debug

cache()

OTHER_FILES += SLProjectCommon.pro \
               SLProjectCommonLibraries.pro \

