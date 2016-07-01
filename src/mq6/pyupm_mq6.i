// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq6
%include "../upm.i"

%feature("autodoc", "3");

%include "mq6.hpp"
%{
    #include "mq6.hpp"
%}
