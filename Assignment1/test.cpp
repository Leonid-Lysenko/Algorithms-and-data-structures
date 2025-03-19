// test.cpp
#include "encoder.h"
#include "decoder.h"
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>
#include <ctime>

std::string generate_random_string(size_t length) {
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789!@#$%^&*()-_=+[]{}|;:'\",.<>?/~` ";
    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result += charset[rand() % (sizeof(charset) - 1)];
    }
    return result;
}

TEST(ASCII85Test, EncodeMultipleBytes) {
    std::string original = "Hello";
    std::istringstream input(original);
    std::ostringstream encoded_output;
    ascii85Encoder(input, encoded_output);

    std::istringstream encoded_input(encoded_output.str());
    std::ostringstream decoded_output;
    ascii85Decoder(encoded_input, decoded_output);

    EXPECT_EQ(decoded_output.str(), original);
}

TEST(ASCII85Test, RandomEncodeDecode) {
    srand(time(nullptr));
    for (int i = 0; i < 10; ++i) {  
        std::string original = generate_random_string(50 + rand() % 50);
        std::istringstream input(original);
        std::ostringstream encoded_output;
        ascii85Encoder(input, encoded_output);

        std::istringstream encoded_input(encoded_output.str());
        std::ostringstream decoded_output;
        ascii85Decoder(encoded_input, decoded_output);

        EXPECT_EQ(decoded_output.str(), original);
    }
}


TEST(ASCII85Test, InvalidDecoding) {
    srand(time(nullptr));
    for (int i = 0; i < 5; ++i) {  
        std::string invalid_data = generate_random_string(20 + rand() % 30);
        invalid_data += '@';

        std::istringstream input(invalid_data);
        std::ostringstream output;

        std::cerr << "Testing invalid decoding with input: " << invalid_data << std::endl;

        EXPECT_THROW(ascii85Decoder(input, output), std::runtime_error);
    }
}

TEST(ASCII85Test, EncodeLargeData) {
    std::string large_data(1000, 'A');
    std::istringstream input(large_data);
    std::ostringstream encoded_output;
    ascii85Encoder(input, encoded_output);

    std::istringstream encoded_input(encoded_output.str());
    std::ostringstream decoded_output;
    ascii85Decoder(encoded_input, decoded_output);

    EXPECT_EQ(decoded_output.str(), large_data);
}

TEST(ASCII85Test, DecodeInvalidCharacter) {
    std::istringstream input("hello$~>");
    std::ostringstream output;
    EXPECT_THROW(ascii85Decoder(input, output), std::runtime_error);
}

TEST(ASCII85Test, DecodeLargeData) {
    std::string large_data(1000, 'A');
    std::istringstream input(large_data);
    std::ostringstream encoded_output;
    ascii85Encoder(input, encoded_output);

    std::istringstream encoded_input(encoded_output.str());
    std::ostringstream decoded_output;
    ascii85Decoder(encoded_input, decoded_output);

    EXPECT_EQ(decoded_output.str(), large_data);
}

TEST(ASCII85Test, TruncatedInput) {
    std::istringstream input("9ar~");  
    std::ostringstream output;
    EXPECT_THROW(ascii85Decoder(input, output), std::runtime_error);
}

TEST(ASCII85Test, EncodeEmptyStringWithMultipleCalls) {
    std::istringstream input("");
    std::ostringstream output;

    ascii85Encoder(input, output);
    EXPECT_EQ(output.str(), "~>");

    input.str("");
    output.str("");
    ascii85Encoder(input, output);
    EXPECT_EQ(output.str(), "~>");
}

TEST(ASCII85Test, EncodeEmptyInput) {
    std::istringstream input("");
    std::ostringstream output;
    ascii85Encoder(input, output);
    EXPECT_EQ(output.str(), "~>");
}

TEST(ASCII85Test, EncodeSingleByte) {
    std::istringstream input("A");
    std::ostringstream output;
    ascii85Encoder(input, output);
    EXPECT_EQ(output.str().substr(0, 2), "5l");
}

TEST(ASCII85Test, DecodeSingleByte) {
    std::istringstream input("5l~>");
    std::ostringstream output;
    ascii85Decoder(input, output);
    EXPECT_EQ(output.str(), "A");
}

TEST(ASCII85Test, EncodeDecodeRoundTrip) {
    std::string original = "Hello, World!";
    std::istringstream input(original);
    std::ostringstream encoded_output;
    ascii85Encoder(input, encoded_output);

    std::istringstream encoded_input(encoded_output.str());
    std::ostringstream decoded_output;
    ascii85Decoder(encoded_input, decoded_output);

    EXPECT_EQ(decoded_output.str(), original);
}

TEST(ASCII85Test, DecodeEmptyString) {
    std::istringstream input("~>");
    std::ostringstream output;
    ascii85Decoder(input, output);
    EXPECT_EQ(output.str(), "");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

