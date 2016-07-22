%module javaupm_mq4
%include "../upm.i"

%{
    #include "mq4.hpp"
%}

%include "mq4.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq4");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
