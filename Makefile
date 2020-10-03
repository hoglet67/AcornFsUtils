CC       = gcc
CXXFLAGS = -g -Wall
CFLAGS	= -O2 -Wall

LIB_MODULES = acorn-fs.o acorn-adfs.o acorn-dfs.o

all: afsls afstree afscp afschk interleave deinterleave title

afsls: afsls.o $(LIB_MODULES)

afstree: afstree.o $(LIB_MODULES)

afscp: afscp.o $(LIB_MODULES)

afschk: afschk.o  $(LIB_MODULES)

interleave: interleave.o adfs_utils.o

deinterleave: deinterleave.o  adfs_utils.o

title: title.o adfs_utils.o

*.o: acorn-fs.h

clean:
	rm -f *.o afsls afstree afscp afschk interleave deinterleave title

