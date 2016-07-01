// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq8
%include "../upm.i"

%feature("autodoc", "3");

%include "mq8.hpp"
%{
    #include "mq8.hpp"
%}
