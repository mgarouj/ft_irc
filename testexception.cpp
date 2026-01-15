#include <iostream>
#include <exception>
#include <stdexcept>

void func() {
    // هنا كنلوحو runtime_error (الابن) وفيه ميساج
    throw std::runtime_error("This is a specific error!");
}

int main() {
    try {
        func();
    }
    // ❌ طريقة خاطئة (بدون &)
    catch (std::exception ob) { 
        // هنا وقع Slicing. 'ob' ولات مجرد exception عادية
        // غالباً غتطبع ليك "std::exception" وتنسى الميساج "This is a specific error!"
        std::cout << "By Value: " << ob.what() << std::endl; 
    }
    
    try {
        func();
    }
    // ✅ طريقة صحيحة (مع &)
    catch (std::exception& ob) {
        // هنا 'ob' كتشير للكائن الأصلي كامل
        // غتطبع ليك "This is a specific error!"
        std::cout << "By Reference: " << ob.what() << std::endl;
    }
}