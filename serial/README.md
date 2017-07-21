# Serial Einfache Wärmeleitungsgleichung

The structure is as follows:
 - In core.c we implement the core functionality (e.g., algorithms).
 - In core_test.c we implement the unit tests as own executable programs without
   using a fancy unit testing framework 
 - In head_conduction.c we implement the main driver of the
   program. This is the program that shall be invoked by the user.
 - The makefile contains all flags and rules to build the program
   and tests and rules how to run the test. Important targets are
   "all", "build", "test", and "clean".

## Documentation
 - html-Folder (doxygen) 