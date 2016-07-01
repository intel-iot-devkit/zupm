%module javaupm_mq3
%include "../upm.i"

%{
    #include "mq3.hpp"
%}

%include "mq3.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq3");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}