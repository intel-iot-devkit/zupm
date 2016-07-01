// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq3
%include "../upm.i"

%feature("autodoc", "3");

%include "mq3.hpp"
%{
    #include "mq3.hpp"
%}
