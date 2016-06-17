#ifndef EXPORT_H_INCLUDED
#define EXPORT_H_INCLUDED

#ifdef WIN32
#   ifdef BUILDING_STATIC_API
#       define PP_API
#       define PP_CALL
#   else
#       ifdef BUILDING_API
#           define PP_API __declspec(dllexport)
#       else
#           define PP_API __declspec(dllimport)
#       endif
#       define PP_CALL __stdcall
#   endif
#else
#   ifdef BUILDING_API
#       define PP_API __attribute__((__visibility__("default")))
#   else
#       define PP_API
#   endif
#   define PP_CALL
#endif

#ifdef WIN32
#   define PP_EXPORTED_TYPE PP_API
#else
#   define PP_EXPORTED_TYPE
#endif

#endif
