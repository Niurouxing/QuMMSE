#pragma once
#include "QuBLAS.h"



using TxSymbol_t = apFixed<intBits<11>,fracBits<11>>;
using RxSymbol_t = apFixed<intBits<11>,fracBits<11>>;
using Nv_t = apFixed<intBits<11>,fracBits<11>>;
using H_t = apFixed<intBits<11>,fracBits<11>>;
using HtH_t = apFixed<intBits<11>,fracBits<11>>;
using HtY_t = apFixed<intBits<11>,fracBits<11>>;


using QgemulAdd_t = apFixed<intBits<11>,fracBits<11>>;
using QgemulMul_t = apFixed<intBits<11>,fracBits<11>>;


using QgemvAdd_t = apFixed<intBits<11>,fracBits<11>>;
using QgemvMul_t = apFixed<intBits<11>,fracBits<11>>;

using QgetrfDiv_t = apFixed<intBits<11>,fracBits<11>>;
using QgetrfSub_t = apFixed<intBits<11>,fracBits<11>>;
using QgetrfMul_t = apFixed<intBits<11>,fracBits<11>>;

using QgetrsDiv_t = apFixed<intBits<11>,fracBits<11>>;
using QgetrsSub_t = apFixed<intBits<11>,fracBits<11>>;
using QgetrsMul_t = apFixed<intBits<11>,fracBits<11>>;