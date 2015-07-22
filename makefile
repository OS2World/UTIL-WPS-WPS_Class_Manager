# To add new extensions to the inference rules uncomment and redefine this:
#.SUFFIXES:
#
#.SUFFIXES: \
#    .C .obj .rc .res

# compiler, linker, resource compiler, resource binder MACRO

CC = icc.exe
CL = ilink.exe
RC = rc.exe
RB = rc.exe

# compiler and linker flags

# Debug version
!ifdef DEBUG

# CFLAGS = /I..\..\headers /I..\..\lvm_tk /Ss /Ti /Rn /G5 /C
CFLAGS = /Ss /Ti /Rn /G5 /C
LFLAGS = /DE /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!else
# RELEASE version

CFLAGS = /Ss /O /Oc /Ol /Rn /G5 /C
LFLAGS = /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!endif

# RCFLAGS = -r -i ..\..\headers
RCFLAGS = -r
RBFLAGS = -x2

.rc.res:
   $(RC) $(RCFLAGS) $<

.C.obj:
   $(CC) $(CFLAGS) $<

all: classmgr.exe

classmgr.exe: \
   classmgr.obj \
   classmgr.res
   $(CL) @<<
      $(LFLAGS)
      /O:classmgr.exe
      classmgr.obj
    <<
    $(RB) $(RBFLAGS) classmgr.res classmgr.exe

classmgr.res: \
   classmgr.rc \
   classmgr.h
