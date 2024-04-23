#pragma once
#include "QuBLAS.h"

using RxSymbol_t = Qu<intBits<3>, fracBits<3>>;
using H_t = Qu<intBits<0>, fracBits<8>>;
using L_t = Qu<intBits<5>, fracBits<8>>;
using b_t = Qu<intBits<6>, fracBits<6>>;

using QgemulAdd_1_t = Qu<intBits<1>, fracBits<9>>;
using QgemulAdd_2_t = Qu<intBits<2>, fracBits<5>>;
using QgemulAdd_3_t = Qu<intBits<3>, fracBits<4>>;
using QgemulAdd_4_t = Qu<intBits<4>, fracBits<4>>;
using QgemulAdd_5_t = Qu<intBits<5>, fracBits<3>>;
using HtH_t =         Qu<intBits<6>, fracBits<1>>;

using QgemulAddList = TypeList<QgemulAdd_1_t, QgemulAdd_2_t, QgemulAdd_3_t, QgemulAdd_4_t, QgemulAdd_5_t, HtH_t>;
using QgemulMul_t = Qu<intBits<0>, fracBits<9>>;

using QgemvAdd_1_t = Qu<intBits<4>, fracBits<5>>;
using QgemvAdd_2_t = Qu<intBits<4>, fracBits<3>>;
using QgemvAdd_3_t = Qu<intBits<5>, fracBits<3>>;
using QgemvAdd_4_t = Qu<intBits<5>, fracBits<2>>;
using QgemvAdd_5_t = Qu<intBits<6>, fracBits<1>>;
using HtY_t =        Qu<intBits<6>, fracBits<1>>;

using QgemvAddList = TypeList<QgemvAdd_1_t, QgemvAdd_2_t, QgemvAdd_3_t, QgemvAdd_4_t, QgemvAdd_5_t, HtY_t>;
using QgemvMul_t = Qu<intBits<4>, fracBits<4>>;

using QpotrfMul_t = Qu<intBits<-2>, fracBits<2>>;
using QpotrfSub_t = Qu<intBits<5>, fracBits<2>>;
using QpotrfDiv_t = Qu<intBits<3>, fracBits<4>>;

using QpotrsForwardMul_t = Qu<intBits<2>, fracBits<1>>;
using QpotrsForwardSub_t = Qu<intBits<6>, fracBits<2>>;
using QpotrsForwardDiv_t = Qu<intBits<4>, fracBits<3>>;

using QpotrsBackwardMul_t = Qu<intBits<1>, fracBits<5>>;
using QpotrsBackwardSub_t = Qu<intBits<4>, fracBits<5>>;
using QpotrsBackwardDiv_t = Qu<intBits<0>, fracBits<5>>;