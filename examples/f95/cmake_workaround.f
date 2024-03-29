c     This is now simply a file configured by CMake into plplot.mod.proxy
c     in the top-level build tree.  It turns out that any file with any
c     contents with that name is all that is required to work around the
c     CMake 2.4.3 bug described below.  So this file is no longer actually
c     compiled. Alan W. Irwin 2006-08-17.

c     This is a dummy fortran file needed to work around a bug in the
c     current (2.4.2) cmake code when compiling the fortran examples 
c     in the build tree. Once cmake is fixed this file and all
c     references to it should be removed.
c     The dependency checking code does not handle external modules 
c     correctly, even though they are correctly found when compiling.
c     To work round this we need to fool cmake into thinking that there
c     is a plplot module_ generated by the examples, but we don't
c     actually want such a module_ or it would clash with the real plplot
c     module_ generated in bindings/f95. To do this we use_ another
c     bug in cmake, namely that it doesn't handle fortran fixed form 
c     source code correctly. When scanning for included modules it
c     checks comments as well as the code. We can use_ this to fool cmake
c     without affecting compilation at all. This is also why the comments
c     contain a _ after certain fortran keywords - we don't want them to 
c     be picked up by cmake.
c     This is ugly - I know. It _only_ affects the building of the
c     examples in the build tree when testing is enabled. It does not
c     appear in the installed tree at all. Let's hope we can find a 
c     proper fix for cmake.
c     Andrew Ross 30/07/2006.

c     Following two lines are to fool cmake into handling the dependecy 
c     correctly for plplot.
c      module plplot
c      end module plplot
      subroutine dummy
      end subroutine dummy
