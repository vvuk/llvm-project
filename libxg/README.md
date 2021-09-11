# libxg

libxg, or libxenograft, is a currently maintained set of function headers targeted for IRIX to help ease porting of more modern software by providing definitions and header files IRIX does not have. 

The original code in the file is released under a 3-clause BSD license due to mostly being derived from BSD and Solaris' BSD code. 

libxg is currently in beta: It should be okay for most basic use-cases

## Usage

See BUILDING for how to build. 

To compile dynamically against it, pass -lxg to the linker. Otherwise, you can take libxg.a and compile it statically. 

The protos should be good enough to drop right into /usr/include. 

## Documentation

See the wiki article here:  https://wiki.irixnet.org/libxg 
