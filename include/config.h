#pragma once
#include "QuBLAS.h"



using RxSymbol_t = Qu<intBits<9>,fracBits<9>>;
using Nv_t = Qu<intBits<11>,fracBits<10>>;
using H_t = Qu<intBits<6>,fracBits<9>>;
using HtH_t = Qu<intBits<8>,fracBits<11>>;
using HtY_t = Qu<intBits<10>,fracBits<9>>;


using QgemulAdd_1_t = Qu<intBits<12>,fracBits<11>>;
using QgemulAdd_2_t = Qu<intBits<12>,fracBits<11>>;
using QgemulAdd_3_t = Qu<intBits<12>,fracBits<11>>;
using QgemulAdd_4_t = Qu<intBits<12>,fracBits<11>>;
using QgemulAdd_5_t = Qu<intBits<12>,fracBits<11>>;
using QgemulAdd_6_t = Qu<intBits<12>,fracBits<11>>;

using QgemulAddList = TypeList<QgemulAdd_1_t, QgemulAdd_2_t, QgemulAdd_3_t, QgemulAdd_4_t, QgemulAdd_5_t, QgemulAdd_6_t>;
using QgemulMul_t = Qu<intBits<9>,fracBits<9>>;


using QgemvAdd_1_t = Qu<intBits<8>,fracBits<9>>;
using QgemvAdd_2_t = Qu<intBits<8>,fracBits<9>>;
using QgemvAdd_3_t = Qu<intBits<8>,fracBits<9>>;
using QgemvAdd_4_t = Qu<intBits<8>,fracBits<9>>;
using QgemvAdd_5_t = Qu<intBits<8>,fracBits<9>>;
using QgemvAdd_6_t = Qu<intBits<8>,fracBits<9>>;

using QgemvAddList = TypeList<QgemvAdd_1_t, QgemvAdd_2_t, QgemvAdd_3_t, QgemvAdd_4_t, QgemvAdd_5_t, QgemvAdd_6_t>;
using QgemvMul_t = Qu<intBits<9>,fracBits<9>>;


using QpotrfMul_t = Qu<intBits<9>,fracBits<9>>;
using QpotrfSub_t = Qu<intBits<9>,fracBits<9>>;
using QpotrfDiv_t = Qu<intBits<9>,fracBits<9>>;

using QpotrsForwardMul_t = Qu<intBits<9>,fracBits<9>>;
using QpotrsForwardSub_t = Qu<intBits<9>,fracBits<9>>;
using QpotrsForwardDiv_t = Qu<intBits<9>,fracBits<9>>;

using QpotrsBackwardMul_t = Qu<intBits<9>,fracBits<9>>;
using QpotrsBackwardSub_t = Qu<intBits<9>,fracBits<9>>;
using QpotrsBackwardDiv_t = Qu<intBits<9>,fracBits<9>>;