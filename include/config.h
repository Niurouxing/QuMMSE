#pragma once
#include "QuBLAS.h"



using RxSymbol_t = apFixed<intBits<9>,fracBits<9>>;
using Nv_t = apFixed<intBits<11>,fracBits<10>>;
using H_t = apFixed<intBits<6>,fracBits<9>>;
using HtH_t = apFixed<intBits<8>,fracBits<11>>;
using HtY_t = apFixed<intBits<10>,fracBits<9>>;


using QgemulAdd_t = apFixed<intBits<12>,fracBits<11>>;
using QgemulMul_t = apFixed<intBits<9>,fracBits<9>>;


using QgemvAdd_t = apFixed<intBits<8>,fracBits<9>>;
using QgemvMul_t = apFixed<intBits<9>,fracBits<9>>;

using QgetrfDiv_t = apFixed<intBits<9>,fracBits<10>>;
using QgetrfSub_t = apFixed<intBits<10>,fracBits<9>>;
using QgetrfMul_t = apFixed<intBits<9>,fracBits<9>>;

using QgetrsDiv_t = apFixed<intBits<9>,fracBits<9>>;
using QgetrsSub_t = apFixed<intBits<9>,fracBits<9>>;
using QgetrsMul_t = apFixed<intBits<11>,fracBits<10>>;