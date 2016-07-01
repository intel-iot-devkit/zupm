// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq9
%include "../upm.i"

%feature("autodoc", "3");

%include "mq9.hpp"
%{
    #include "mq9.hpp"
%}
