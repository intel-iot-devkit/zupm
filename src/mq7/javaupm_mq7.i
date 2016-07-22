%module javaupm_mq7
%include "../upm.i"

%{
    #include "mq7.hpp"
%}

%include "mq7.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq7");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
