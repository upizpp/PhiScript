#pragma once

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#define IMPORT_API __declspec(dllimport)
#else
#define EXPORT_API
#define IMPORT_API
#endif

#ifdef BUILD_DLL
#define PHIAPI EXTERN_C EXPORT_API
#else
#define PHIAPI EXTERN_C IMPORT_API
#endif

struct Phi;
struct PhiError {
    char *msg;
    char *type;
};

PHIAPI const char *phiVersion();

PHIAPI struct Phi *phiCreate();
PHIAPI void phiDestroy(struct Phi *phi);
PHIAPI struct PhiError *phiDoString(struct Phi *phi, const char *chunk);
PHIAPI struct PhiError *phiDoStringWithName(struct Phi *phi, const char *chunk,
                                            const char *chunk_name);
PHIAPI struct PhiError *phiDoFile(struct Phi *phi, const char *filename);
