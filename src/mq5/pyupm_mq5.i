// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq5
%include "../upm.i"

%feature("autodoc", "3");

%include "mq5.hpp"
%{
    #include "mq5.hpp"
%}
