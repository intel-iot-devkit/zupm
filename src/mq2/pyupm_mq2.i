// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq2
%include "../upm.i"

%feature("autodoc", "3");

%include "mq2.hpp"
%{
    #include "mq2.hpp"
%}
