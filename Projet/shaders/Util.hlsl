#ifndef UTIL_H
#define UTIL_H

// Macro used to fill a struct to match 16 byte stride
#define FILL(size) float##size __FILL__

// Colors used for some default values
#define BLACK float3(0.f, 0.f, 0.f)
#define WHITE float3(1.f, 1.f, 1.f)

#endif // UTIL_H