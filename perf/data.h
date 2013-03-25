#ifndef _DATA_H_
#define _DATA_H_

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

void load_data(const char* filename);

size_t data_size();

const char* data_elem(size_t i);

#ifdef __cplusplus
}
#endif

#endif
