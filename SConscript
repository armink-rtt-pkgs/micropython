from building import *

# get current directory
cwd     = GetCurrentDir()
# The set of source files associated with this SConscript file.
src     = Glob('py/*.c')
src    += Glob('lib/mp-readline/*.c')
src    += Glob('lib/utils/*.c')
src    += Glob('extmod/*.c')
src    += Glob('port/*.c')

path    = [cwd + '/']
path   += [cwd + '/port']

LOCAL_CCFLAGS = ' -include "port/mpy_project_cfg.h"'

group = DefineGroup('MicroPython', src, depend = ['PKG_USING_WEBTERMINAL'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
