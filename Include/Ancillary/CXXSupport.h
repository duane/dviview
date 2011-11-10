#ifndef __INCLUDE_ANCILLARY_CXXSUPPORT_H__
#define __INCLUDE_ANCILLARY_CXXSUPPORT_H__

// The following macro is stolen from Google's C++ style guide.
// see http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#Copy_Constructors
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif  // __INCLUDE_ANCILLARY_CXXSUPPORT_H__
