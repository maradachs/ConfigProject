#include "Base64Helper.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

    std::string Base64Helper::Encode(const std::string& data) {
        BIO* bio, * b64;
        BUF_MEM* bufferPtr;

        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Без переноса строки
        bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);

        BIO_write(bio, data.c_str(), data.length());
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);

        std::string encoded(bufferPtr->data, bufferPtr->length);
        BIO_free_all(bio);

        return encoded;
    }

    std::string Base64Helper::Decode(const std::string& encoded) {
        BIO* bio, * b64;
        char* buffer = new char[encoded.length()];
        memset(buffer, 0, encoded.length());

        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Без переноса строки
        bio = BIO_new_mem_buf(encoded.c_str(), -1);
        bio = BIO_push(b64, bio);

        int decodedLength = BIO_read(bio, buffer, encoded.length());
        BIO_free_all(bio);

        std::string decoded(buffer, decodedLength);
        delete[] buffer;

        return decoded;
    }
