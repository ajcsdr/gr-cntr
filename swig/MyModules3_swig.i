/* -*- c++ -*- */

#define MYMODULES3_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "MyModules3_swig_doc.i"

%{
#include "MyModules3/magic.h"
%}


%include "MyModules3/magic.h"
GR_SWIG_BLOCK_MAGIC2(MyModules3, magic);
