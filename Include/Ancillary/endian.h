/*
 *  Copyright 2010 Duane R. Bailey
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*/

#ifndef INCLUDE_UTIL_ENDIAN_H_
#define INCLUDE_UTIL_ENDIAN_H_
#include <stdint.h>
// Beware, these macros are Darwin-dependent.
#ifdef __LITTLE_ENDIAN__

static inline uint16_t BigToHost16(uint16_t v) {
    return (v << 8) | (v >> 8);
}

static inline uint32_t BigToHost32(uint32_t v) {
    return (v << 24) | (v >> 24) |  ((v << 8) & 0x00FF0000) |  ((v >> 8) & 0x0000FF00);
}

#else

static inline uint16_t BigToHost16(uint16_t v) {
    return v;
}

static inline uint32_t BigToHost32(uint32_t v) {
    return v;
}

#endif  // __LITTLE_ENDIAN__
#endif  // INCLUDE_UTIL_ENDIAN_H_
