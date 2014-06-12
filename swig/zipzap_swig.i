/* -*- c++ -*- */

#define ZIPZAP_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "zipzap_swig_doc.i"

%{
#include "zipzap/zipzap_sink.h"
%}


%include "zipzap/zipzap_sink.h"
GR_SWIG_BLOCK_MAGIC2(zipzap, zipzap_sink);
