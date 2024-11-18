#pragma once
#include <cstdint>

namespace def_project
{
using TypeId = std::uint64_t;
using TypeQty = std::uint16_t;
using TypeQtyModify = std::int32_t;
enum class EnumOrderSide : std::uint8_t;
enum class EnumOrderTimeInForce : std::uint8_t;
enum class EnumOrderCycleHook : std::uint8_t;
enum class EnumSubOrderRequestSide : std::uint8_t;
}

enum class        //
    def_project:: //
    EnumOrderSide : std::uint8_t
{
    NULL0 = 0,
    BUYER,
    SELLER,
};

enum class        //
    def_project:: //
    EnumOrderTimeInForce : std::uint8_t
{
    NULL0 = 0,
    NORMAL,
    OCA, // OCA: One-Cancel-All
};

enum class        //
    def_project:: //
    EnumOrderCycleHook : std::uint8_t
{
    NULL0 = 0,
    POST_SUBORDER_MATCH_REQUEST_RESPONSE,
};

enum class        //
    def_project:: //
    EnumSubOrderRequestSide : std::uint8_t
{
    NULL0 = 0,
    SOURCE,
    TARGET,
};
