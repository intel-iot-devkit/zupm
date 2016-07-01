%module javaupm_mq2
%include "../upm.i"

%{
    #include "mq2.hpp"
%}

%include "mq2.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq2");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}