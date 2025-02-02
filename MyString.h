#pragma once

// This is the custom string class.
class MyString {
public:
    static void copy(const char* source, char* destination);
    static int length(const char* str);
    static bool compare(const char* str1, const char* str2);
    static bool contains(const char* str, char ch);
};


