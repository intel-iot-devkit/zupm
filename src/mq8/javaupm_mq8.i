%module javaupm_mq8
%include "../upm.i"

%{
    #include "mq8.hpp"
%}

%include "mq8.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq8");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}