// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq4
%include "../upm.i"

%feature("autodoc", "3");

%include "mq4.hpp"
%{
    #include "mq4.hpp"
%}
