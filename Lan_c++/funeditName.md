```cpp



using namespace winrt;
using namespace Windows::Foundation;
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <iostream>

#include "pch.h"
using namespace std;

// อ่านค่า offset จาก Registry และ XOR กับ key
DWORD readXorOffset(const char* regPath, const char* valueName, DWORD xorKey) {
    HKEY hKey;
    DWORD offset = 0;
    DWORD dataSize = sizeof(offset);

    // เปิดคีย์ใน HKEY_CURRENT_USER\regPath
    if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, valueName, NULL, NULL, (LPBYTE)&offset, &dataSize) == ERROR_SUCCESS) {
            offset ^= xorKey;  // XOR ค่า offset กับ key
        }
        RegCloseKey(hKey);
    }

    return offset;
}

// เขียนค่า offset ลง Registry โดย XOR ก่อน (ค่าจะถูกลบเมื่อรีเครื่อง)
void writeXorOffset(const char* regPath, const char* valueName, DWORD offset, DWORD xorKey) {
    HKEY hKey;
    DWORD disposition;

    // สร้าง (หรือเปิด) Key ใน HKEY_CURRENT_USER\regPath (ค่าจะถูกลบเมื่อรีเครื่อง)
    if (RegCreateKeyEx(HKEY_CURRENT_USER, regPath, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition) == ERROR_SUCCESS) {
        DWORD xorOffset = offset ^ xorKey;  // XOR ค่า offset ก่อนเขียนลง Registry
        RegSetValueEx(hKey, valueName, 0, REG_DWORD, (const BYTE*)&xorOffset, sizeof(xorOffset));
        RegCloseKey(hKey);
    }
}

void xor_calculate_key(const char* original, const char* encoded, char* key) {
    size_t len = strlen(original);
    for (size_t i = 0; i < len; i++) {
        key[i] = original[i] ^ encoded[i]; // คำนวณ Key
    }
    key[len] = '\0'; // ปิดท้าย string
}

// ฟังก์ชันถอดรหัส
void xor_decrypt(const char* encoded, const char* key, char* output) {
    size_t len = strlen(encoded);
    for (size_t i = 0; i < len; i++) {
        output[i] = encoded[i] ^ key[i]; // XOR กลับเป็น Original
    }
    output[len] = '\0';
}

void xor_decryptEx(const char* encoded, const char* key, char* output, size_t len) {
    for (size_t i = 0; i < len; i++) {
        output[i] = encoded[i] ^ key[i]; // XOR กลับเป็น Original
    }
    output[len] = '\0';
}


void xor_simd_string(const char* encoded, const unsigned char* key, char* output, size_t len) {
    // เตรียม block 16 bytes (padding ถ้าไม่ครบ)
    char enc_block[16] = { 0 };
    char key_block[16] = { 0 };

    memcpy(enc_block, encoded, len);
    memcpy(key_block, key, len);

    __m128i a = _mm_loadu_si128((const __m128i*)enc_block);
    __m128i b = _mm_loadu_si128((const __m128i*)key_block);
    __m128i r = _mm_xor_si128(a, b);

    _mm_storeu_si128((__m128i*)output, r);
    output[len] = '\0'; // ปิดท้าย string
}


int main() {
    init_apartment();
    Uri uri(L"http://aka.ms/cppwinrt");
    printf("Xor String, %ls!\n", uri.AbsoluteUri().c_str());

    char original[255], encoded[255], key[255], decrypted[255];
    char continueChoice;

    do {
        // รับชื่อที่ต้องการเข้ารหัส
        printf("Enter the original string to encrypt: ");
        cin.getline(original, sizeof(original));

        // รับชื่อที่ต้องการเข้ารหัสเป็นค่า encoded
        printf("Enter the encoded string: ");
        cin.getline(encoded, sizeof(encoded));

        // คำนวณ key จากการ XOR ระหว่าง original และ encoded
        xor_calculate_key(original, encoded, key);
     //   xor_simd_string(encoded, (BYTE)key, decrypted, sizeof(key)); 
        // แสดงค่า key ในรูปแบบ Hex
        printf("Generated Key : "); 
        size_t len = strlen(original);
        for (size_t i = 0; i < len; i++) {
       
            printf("0x%X ", (unsigned char)key[i]);
        }
        printf("\n");
      
        // ถอดรหัส encoded ด้วย key ที่คำนวณได้ เพื่อให้ได้ original กลับมา
        xor_decrypt(encoded, key, decrypted);

        // แสดงผล
        printf("Original text: %s\n", original);
        printf("Encoded text: %s\n", encoded);
        printf("Decrypted text: %s\n", decrypted);

        // ถามผู้ใช้ว่าอยากทำการเข้ารหัสใหม่หรือไม่
        printf("Do you want to encrypt/decrypt another string? (y/n): ");
        cin >> continueChoice;
        cin.ignore(); // ล้าง buffer เพื่อรับค่าจากผู้ใช้ใหม่
    } while (continueChoice == 'y' || continueChoice == 'Y');

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}



```
