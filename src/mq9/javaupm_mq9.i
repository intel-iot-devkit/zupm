%module javaupm_mq9
%include "../upm.i"

%{
    #include "mq9.hpp"
%}

%include "mq9.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq9");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
