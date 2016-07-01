// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq7
%include "../upm.i"

%feature("autodoc", "3");

%include "mq7.hpp"
%{
    #include "mq7.hpp"
%}
