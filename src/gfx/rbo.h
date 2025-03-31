#ifndef RBO_H
#define RBO_H

#include "../types.h"
#include "glad/glad.h"

typedef u32 rbo_t;

void rbo_create(rbo_t* rbo);
void rbo_destroy(rbo_t* rbo);
void rbo_bind(rbo_t rbo);
void rbo_storage(rbo_t rbo, u32 format, u64 width, u64 height);

#endif // RBO_H