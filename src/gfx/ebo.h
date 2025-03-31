#ifndef EBO_H
#define EBO_H

#include "../types.h"

typedef u32 ebo_t;

void ebo_create(ebo_t* ebo);
void ebo_destroy(ebo_t* ebo);
void ebo_bind(ebo_t ebo);
void ebo_buffer(ebo_t ebo, u64 size, const void* indices, u32 usage);

#endif // EBO_H