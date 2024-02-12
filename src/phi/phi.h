#pragma once

#ifdef __cplusplus
#   define EXTERN_C extern "C"
#else
#   define EXTERN_C
#endif

#ifdef BUILD_DLL
#	define PHIAPI EXTERN_C __declspec(dllexport)
#else
#	define PHIAPI EXTERN_C __declspec(dllimport)
#endif

struct Phi;
struct PhiError
{
    char* msg;
    char* type;
};


PHIAPI const char* phiVersion();

PHIAPI struct Phi* phiCreate();
PHIAPI void phiDestroy(struct Phi* phi);
PHIAPI struct PhiError* phiDoString(struct Phi* phi, const char* chunk);
PHIAPI struct PhiError* phiDoStringWithName(struct Phi* phi, const char* chunk, const char* chunk_name);
PHIAPI struct PhiError* phiDoFile(struct Phi* phi, const char* filename);

