%module javaupm_mq5
%include "../upm.i"

%{
    #include "mq5.hpp"
%}

%include "mq5.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq5");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}