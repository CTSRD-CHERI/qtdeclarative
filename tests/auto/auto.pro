TEMPLATE=subdirs
SUBDIRS=\
    qml \
    quick \
    quicktest \
    qmltest \
    qmldevtools \
    cmake \
    installed_cmake \
    toolsupport

# Takes forever to compile and we're not running them right now
SUBDIRS-= quick

qtHaveModule(gui):qtConfig(opengl(es1|es2)?) {
    SUBDIRS += particles
    qtHaveModule(widgets): SUBDIRS += quickwidgets

}

# console applications not supported
uikit: SUBDIRS -= qmltest

installed_cmake.depends = cmake
