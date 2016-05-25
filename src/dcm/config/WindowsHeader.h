// WindowsHeader.h

#ifndef WINDOWS_HEADER_H
#define WINDOWS_HEADER_H

#ifdef UNIX
#ifndef NODLLEXPORT
#define NODLLEXPORT
#endif
#endif

#ifndef DLLEXPORT

#ifndef NODLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#endif

#ifndef DLLIMPORT

#ifndef NODLLEXPORT
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLIMPORT
#endif

#endif


#ifdef XMLCONFIGPARSER_EXPORTS
#    define XmlConfigParser_DLLEXPIMPDECL DLLEXPORT
#else
#    define XmlConfigParser_DLLEXPIMPDECL DLLIMPORT
#endif

#ifdef DRDATAFORMAT_EXPORTS
#    define DrDataFormat_DLLEXPIMPDECL DLLEXPORT
#else
#    define DrDataFormat_DLLEXPIMPDECL DLLIMPORT
#endif

#ifdef DRDATATAGDB_EXPORTS
#    define DrDataTagDb_DLLEXPIMPDECL DLLEXPORT
#else
#    define DrDataTagDb_DLLEXPIMPDECL DLLIMPORT
#endif

#ifdef DRDATAFACTORY_EXPORTS
#    define DrDataFactory_DLLEXPIMPDECL DLLEXPORT
#else
#    define DrDataFactory_DLLEXPIMPDECL DLLIMPORT
#endif

#ifdef SIMPLEDRDATA_EXPORTS
#	 define SimpleDrData_DLLEXPIMPDECL DLLEXPORT
#else
#	 define SimpleDrData_DLLEXPIMPDECL DLLIMPORT
#endif

#define DRDATADEF_DLLEXPIMPDECL SimpleDrData_DLLEXPIMPDECL

#ifdef TREIPC_EXPORTS
#	  define TreIpc_DLLEXPIMPDECL DLLEXPORT
#else
#	  define TreIpc_DLLEXPIMPDECL DLLIMPORT
#endif

#ifdef WIN32

#pragma warning ( disable : 4251 )
#pragma warning ( disable : 4273 )

#endif // WIN32

#ifdef HPUX
#include <ostream>
#endif

#ifdef LINUX
#include <ostream>
using std::ostream;
#endif

#endif // WINDOWS_HEADER_H
