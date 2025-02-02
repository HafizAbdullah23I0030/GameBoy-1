#include "MyString.h"
// This is the custom string class.
void MyString::copy(const char* source, char* destination) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        ++i;
    }
    destination[i] = '\0';
}

int MyString::length(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        ++length;
    }
    return length;
}

bool MyString::compare(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        ++i;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

bool MyString::contains(const char* str, char ch) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ch) {
            return true;
        }
        ++i;
    }
    return false;
}
