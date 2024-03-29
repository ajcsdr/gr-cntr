# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Include swig generation macros
########################################################################
find_package(SWIG)
find_package(PythonLibs)
if(NOT SWIG_FOUND OR NOT PYTHONLIBS_FOUND)
    return()
endif()
include(GrSwig)
include(GrPython)

########################################################################
# Setup swig generation
########################################################################
foreach(incdir ${GNURADIO_CORE_INCLUDE_DIRS})
    list(APPEND GR_SWIG_INCLUDE_DIRS ${incdir}/swig)
endforeach(incdir)

foreach(incdir ${GRUEL_INCLUDE_DIRS})
    list(APPEND GR_SWIG_INCLUDE_DIRS ${incdir}/gruel/swig)
endforeach(incdir)



set(GR_SWIG_LIBRARIES gnuradio-MyModules3)
set(APPEND GR_SWIG_LIBRARIES gnuradio-uhd ${UHD_LIBRARIES})
set(GR_SWIG_DOC_FILE ${CMAKE_CURRENT_BINARY_DIR}/MyModules3_swig_doc.i)
set(GR_SWIG_DOC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include)


######################I ADDED THIS######################################
set(APPEND GR_SWIG_INCLUDE_DIRS
    ${GR_UHD_INCLUDE_DIRS}
    ${GNURADIO_CORE_SWIG_INCLUDE_DIRS}
    ${GRUEL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${UHD_INCLUDE_DIRS}
)
########################################################################
GR_SWIG_MAKE(MyModules3_swig MyModules3_swig.i)

########################################################################
# Install the build swig module
########################################################################
GR_SWIG_INSTALL(TARGETS MyModules3_swig DESTINATION ${GR_PYTHON_DIR}/MyModules3)

########################################################################
# Install swig .i files for development
########################################################################
install(
    FILES
    MyModules3_swig.i
    ${CMAKE_CURRENT_BINARY_DIR}/MyModules3_swig_doc.i
    DESTINATION ${GR_INCLUDE_DIR}/MyModules3/swig
)
