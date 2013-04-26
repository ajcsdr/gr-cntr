# Copyright 2011,2012 Free Software Foundation, Inc.
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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX


##############I ADDED STUFF IN HERE################################
include_directories(${Boost_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
    ${GR_UHD_INCLUDE_DIRS}
    ${GNURADIO_CORE_INCLUDE_DIRS}
    ${GRUEL_INCLUDE_DIRS}
    ${UHD_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS}
				${UHD_LIBRARY_DIRS}
				)
########################################################################


list(APPEND MyModules3_sources
    magic_impl.cc
)

add_library(gnuradio-MyModules3 SHARED ${MyModules3_sources})
target_link_libraries(gnuradio-MyModules3 ${Boost_LIBRARIES} ${GRUEL_LIBRARIES} ${GNURADIO_CORE_LIBRARIES} ${UHD_LIBRARIES})
set_target_properties(gnuradio-MyModules3 PROPERTIES DEFINE_SYMBOL "gnuradio_MyModules3_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-MyModules3
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_MyModules3_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_MyModules3.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_MyModules3.cc
)

add_executable(test-MyModules3 ${test_MyModules3_sources})

target_link_libraries(
  test-MyModules3
  ${GNURADIO_CORE_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-MyModules3
)

GR_ADD_TEST(test_MyModules3 test-MyModules3)
