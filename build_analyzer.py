#!/usr/bin/env python
import os, glob, platform

#find out if we're running on mac or linux and set the dynamic library extension
dylib_ext = ""
if platform.system().lower() == "darwin":
    dylib_ext = ".dylib"
else:
    dylib_ext = ".so"
    
print "Running on " + platform.system()

#make sure the release folder exists, and clean out any .o/.so file if there are any
if not os.path.exists( "release" ):
    os.makedirs( "release" )

os.chdir( "release" )
o_files = glob.glob( "*.o" )
o_files.extend( glob.glob( "*" + dylib_ext ) )
for o_file in o_files:
    os.remove( o_file )
os.chdir( ".." )


#make sure the debug folder exists, and clean out any .o/.so files if there are any
if not os.path.exists( "debug" ):
    os.makedirs( "debug" )

os.chdir( "debug" )
o_files = glob.glob( "*.o" );
o_files.extend( glob.glob( "*" + dylib_ext ) )
for o_file in o_files:
    os.remove( o_file )
os.chdir( ".." )

#find all the cpp files in /src.  We'll compile all of them
os.chdir( "src" )
cpp_files = glob.glob( "*.cpp" );
os.chdir( ".." )

#specify the search paths/dependencies/options for gcc
include_paths = [ "../SaleaeAnalyzerSdk-1.1.9/include" ]
link_paths = [ "../SaleaeAnalyzerSdk-1.1.9/lib" ]
link_dependencies = [ "-lAnalyzer" ] #refers to libAnalyzer.dylib or libAnalyzer.so

debug_compile_flags = "-O0 -w -c -fpic -g3"
release_compile_flags = "-O3 -w -c -fpic"

#loop through all the cpp files, build up the gcc command line, and attempt to compile each cpp file
for cpp_file in cpp_files:

    #g++
    command = "g++ "

    #include paths
    for path in include_paths: 
        command += "-I\"" + path + "\" "

    release_command = command
    release_command  += release_compile_flags
    release_command += " -o\"release/" + cpp_file.replace( ".cpp", ".o" ) + "\" " #the output file
    release_command += "\"" + "src/" + cpp_file + "\"" #the cpp file to compile

    debug_command = command
    debug_command  += debug_compile_flags
    debug_command += " -o\"debug/" + cpp_file.replace( ".cpp", ".o" ) + "\" " #the output file
    debug_command += "\"" + "src/" + cpp_file + "\"" #the cpp file to compile

    #run the commands from the command line
    print release_command
    os.system( release_command )
    print debug_command
    os.system( debug_command )
    
#lastly, link
#g++
command = "g++ "

#add the library search paths
for link_path in link_paths:
    command += "-L\"" + link_path + "\" "

#add libraries to link against
for link_dependency in link_dependencies:
    command += link_dependency + " "

#make a dynamic (shared) library (.so/.dylib)

if dylib_ext == ".dylib":
    command += "-dynamiclib "
else:
    command += "-shared "

#figgure out what the name of this analyzer is
analyzer_name = ""
for cpp_file in cpp_files:
    if cpp_file.endswith( "Analyzer.cpp" ):
        analyzer_name = cpp_file.replace( "Analyzer.cpp", "" )
        break

#the files to create (.so/.dylib files)
if dylib_ext == ".dylib":
    release_command = command + "-o release/lib" + analyzer_name + "Analyzer.dylib "
    debug_command = command + "-o debug/lib" + analyzer_name + "Analyzer.dylib "
else:
    release_command = command + "-o\"release/lib" + analyzer_name + "Analyzer.so\" "
    debug_command = command + "-o\"debug/lib" + analyzer_name + "Analyzer.so\" "

#add all the object files to link
for cpp_file in cpp_files:
    release_command += "release/" + cpp_file.replace( ".cpp", ".o" ) + " "
    debug_command += "debug/" + cpp_file.replace( ".cpp", ".o" ) + " "
    
#run the commands from the command line
print release_command
os.system( release_command )
print debug_command
os.system( debug_command )

        
