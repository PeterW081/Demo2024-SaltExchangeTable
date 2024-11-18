#pragma
#include <gtest/gtest.h>
#include <filesystem>
#include "def_project/SaltExchangeTable.h"

TEST(Example02, main)
{
    std::cout << "┌──────────────────────────────────────────────────────────────┐" << std::endl << std::endl;

    using def_project::EnumOrderSide;
    using def_project::EnumOrderTimeInForce;
    using def_project::SaltExchangeTable;
    using def_project::TypeId;

    auto func = []<typename T>(T &t) -> std::map<TypeId, TypeId> {
        auto suborder_id = std::map<TypeId, TypeId>();
        for (auto &var : t)
        {
            suborder_id[var.m_user_id.value_or(0)] = var.m_suborder_id.value_or(0);
        }
        return suborder_id;
    };
    struct
    {
        TypeId Seller01 = 83001;
        TypeId Seller02 = 83002;
        TypeId Seller03 = 83003;
        TypeId Buyer001 = 66001;
    } user_id;
    struct
    {
        TypeId Seller01;
        TypeId Seller02;
        TypeId Seller03;
        TypeId Buyer001;
    } order_id;
    auto suborder_id = std::map<TypeId, TypeId>();
    auto stub_tmp = std::invoke_result<decltype(&SaltExchangeTable::fx_fgApi_suborder_match_request_remake_by_oca_by_auto_redo), SaltExchangeTable, TypeId, TypeId, std::initializer_list<TypeId>>::type();

    // init
    auto table = SaltExchangeTable();
    order_id.Seller01 = table.fx_fgApi_order_add(user_id.Seller01, EnumOrderSide::SELLER, EnumOrderTimeInForce::NORMAL, 999, 10).value_or(0);  // Seller01
    order_id.Seller02 = table.fx_fgApi_order_add(user_id.Seller02, EnumOrderSide::SELLER, EnumOrderTimeInForce::NORMAL, 999, 30).value_or(0);  // Seller02
    order_id.Seller03 = table.fx_fgApi_order_add(user_id.Seller03, EnumOrderSide::SELLER, EnumOrderTimeInForce::NORMAL, 999, 100).value_or(0); // Seller03
    order_id.Buyer001 = table.fx_fgApi_order_add(user_id.Buyer001, EnumOrderSide::BUYER, EnumOrderTimeInForce::OCA, 1000, 1).value_or(0);      // Buyer01
    stub_tmp = table.fx_fgApi_suborder_match_request_remake_by_oca_by_auto_redo(                                                               //
        user_id.Buyer001, order_id.Buyer001, {order_id.Seller01, order_id.Seller02, order_id.Seller03}                                         //
    );

    // step01
    table.fx_fgApi_print_user_realtime_match_request_as_target(std::cout, std::string(" -- " + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__)));
    if (not stub_tmp.lock()->empty())
    {
        suborder_id = func(stub_tmp.lock().operator*());
        table.fx_fgApi_suborder_match_request_response(         //
            user_id.Seller02, suborder_id[user_id.Seller02], 33 //
        );
    }

    // step02
    table.fx_fgApi_print_user_realtime_match_request_as_target(std::cout, std::string(" -- " + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__)));
    if (not stub_tmp.lock()->empty())
    {
        suborder_id = func(stub_tmp.lock().operator*());
        table.fx_fgApi_suborder_match_request_response(        //
            user_id.Seller01, suborder_id[user_id.Seller01], 1 //
        );
    }

    // finish
    table.fx_fgApi_print_user_realtime_match_request_as_target(std::cout, std::string(" -- " + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__)));
    table.fx_fgApi_print_user_realtime_position(std::cout);

    //
    EXPECT_EQ(stub_tmp.lock().get(), nullptr);
    std::cout << "└──────────────────────────────────────────────────────────────┘" << std::endl;
}
