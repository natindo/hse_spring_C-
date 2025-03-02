#include "db.h"
#include <iostream>
#include <request.h>
#include <parser.h>
#include <response.h>
#include <gtest/gtest.h>
#include <app.h>

TEST(bd_test, simleTestSetGetDel) {
    Database db;
    EXPECT_EQ(db.get("key"), "");
    EXPECT_EQ(db.del("key"), false);
    EXPECT_EQ(db.set("key", "value1"), true);
    EXPECT_EQ(db.get("key"), "value1");
    EXPECT_EQ(db.set("key", "value2"), true);
    EXPECT_EQ(db.get("key"), "value2");
    EXPECT_EQ(db.del("key"), true);
    EXPECT_EQ(db.get("key"), "");
}

TEST(bd_test, simleCorrectlyDeleteAndInsertDataInOneBacket) {
    Database db;
    EXPECT_EQ(db.set("key", "value1"), true);
    EXPECT_EQ(db.set("mdx", "value2"), true);
    EXPECT_EQ(db.set("ocw", "value3"), true);
    EXPECT_EQ(db.set("qbv", "value4"), true);
    EXPECT_EQ(db.get("ocw"), "value3");
    EXPECT_EQ(db.del("ocw"), true);
    EXPECT_EQ(db.get("ocw"), "");
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

TEST(app_test, simpleAppTest) {
    std::string inputData =
        "ABC key1\n"
        "GET key1\n"
        "DEL key1\n"
        "SET key1 value1\n"
        "GET key1\n"
        "DEL key1\n";

    std::string expectedOutput =
        "\n"
        "not found\n"
        "not found\n"
        "ok\n"
        "value1\n"
        "ok\n";

    // Подменим потоки ввода-вывода, так как "run" ничего не возвращает
    std::istringstream inputStream(inputData);
    std::ostringstream outputStream;

    std::cin.rdbuf(inputStream.rdbuf());
    std::cout.rdbuf(outputStream.rdbuf());

    App app;
    app.run();

    EXPECT_EQ(outputStream.str(), expectedOutput);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}