#include "db.h"
#include <iostream>
#include <request.h>
#include <parser.h>
#include <response.h>
#include <gtest/gtest.h>

TEST(bd_test, allocTest) {
    Database db;
    db.set("key1", "value1");
    db.set("key2", "value1");
    db.set("key3", "value1");
    db.set("key4", "value1");
    db.set("key5", "value1");
    db.set("key6", "value1");
    db.set("key7", "value1");
    db.set("key8", "value1");
    db.set("key9", "value1");
    db.set("key10", "value1");
    db.set("key11", "value1");

    EXPECT_EQ(db.getSize(), 11);
    EXPECT_EQ(db.getCapacity(), 20);
}

TEST(bd_test, simleTestSetGetDel) {
    Database db;
    EXPECT_EQ(db.get("key"), "not found");
    EXPECT_EQ(db.del("key"), false);
    EXPECT_EQ(db.set("key", "value1"), true);
    EXPECT_EQ(db.get("key"), "value1");
    EXPECT_EQ(db.set("key", "value2"), true);
    EXPECT_EQ(db.get("key"), "value2");
    EXPECT_EQ(db.del("key"), true);
    EXPECT_EQ(db.get("key"), "not found");
}

TEST(bd_test, dataErasureDeletion) {
    Database db;
    EXPECT_EQ(db.set("key1", "value1"), true);
    EXPECT_EQ(db.set("key2", "value1"), true);
    EXPECT_EQ(db.set("key3", "value1"), true);
    EXPECT_EQ(db.set("key4", "value1"), true);
    EXPECT_EQ(db.set("key5", "value1"), true);
    EXPECT_EQ(db.set("key6", "value1"), true);
    EXPECT_EQ(db.del("key3"), true);
}

TEST(handlers_test, simpleRequestTest) {
    const std::string input("SET key123 val321");
    Request req = Parser::parse(input);
    EXPECT_EQ(req.getType(), RequestType::SET);
    EXPECT_EQ(req.getKey(), "key123");
    EXPECT_EQ(req.getValue(), "val321");
}

TEST(handlers_test, simpleResponseTest) {
    Response response;
    response.setResponse("asdfgsdfhja");
    EXPECT_EQ(response.getResponse(), "asdfgsdfhja");
}

TEST(handlers_test, simpleParserTest) {
    const std::string inputSet("SET key123 val321");
    Request reqSet = Parser::parse(inputSet);
    EXPECT_EQ(reqSet.getType(), RequestType::SET);

    const std::string inputGet("GET key123 val321");
    Request reqGet = Parser::parse(inputGet);
    EXPECT_EQ(reqGet.getType(), RequestType::GET);

    const std::string inputDel("DEL key123 val321");
    Request reqDel = Parser::parse(inputDel);
    EXPECT_EQ(reqDel.getType(), RequestType::DEL);

    const std::string inputUnknown("BJIKL key123 val321");
    Request reqUnknown = Parser::parse(inputUnknown);
    EXPECT_EQ(reqUnknown.getType(), RequestType::UNKNOWN);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}