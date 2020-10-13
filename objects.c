#include "objects.h"
#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

int init_mgit_dir(const char* path) {

    char pathbuf[BUFSIZ] = {0};
    int return_code;

    snprintf(pathbuf, BUFSIZ, "%s/%s", path, MGIT_FOLDER);
    return_code = fs_mkdir(pathbuf);
    snprintf(pathbuf, BUFSIZ, "%s/%s", path, MGIT_OBJECTS_FOLDER);
    return_code = fs_mkdir(pathbuf);

    return return_code;
}

int hash_object(const char* path) {
    char hash[HASH_STRING_BYTES] = {0};
    size_t n_read = 0;
    char buff[BUFSIZ] = {0};
    char f_dest_name[BUFSIZ];

    sha_file(hash, path);
    snprintf(f_dest_name, BUFSIZ, "%s/%s", MGIT_OBJECTS_FOLDER, hash);

    FILE* dest_f = fopen(f_dest_name, "w");
    if (!dest_f) {
        return MGIT_FILE_OPEN_ERROR;
    }
    FILE* src_f = fopen(path, "r");
    if (!src_f) {
        return MGIT_FILE_OPEN_ERROR;
    }

    while ((n_read = fread(buff, sizeof(char), BUFSIZ, src_f)) != 0) {
        fwrite(buff, sizeof(char), n_read, dest_f);
    }
    fclose(src_f);
    fclose(dest_f);
    return 0;
}

int sha_file(char dest_hash[HASH_STRING_BYTES], const char* path) {

    unsigned char hash[SHA256_DIGEST_LENGTH];
    char buff[BUFSIZ] = {'\0'};
    size_t read_len;
    SHA256_CTX sha256_ctx;

    FILE* f = fopen(path, "r");
    if (!f) {
        return MGIT_FILE_OPEN_ERROR;
    }
    SHA256_Init(&sha256_ctx);
    while ((read_len = fread(buff, sizeof(char), BUFSIZ, f)) > 0) {
        SHA256_Update(&sha256_ctx, buff, read_len);
    }

    fclose(f);
    SHA256_Final(hash, &sha256_ctx);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&dest_hash[i * 2], "%02x", hash[i]);
    }
    return 0;
}


int cat_object(const char* hash, file_handler* handler) {
    char buff[BUFSIZ];
    snprintf(buff, BUFSIZ, "%s%s", MGIT_OBJECTS_FOLDER, hash);
    FILE* f = fopen(buff, "r");

    if (!f) {
        return MGIT_FILE_OPEN_ERROR;
    }
    handler(f);

    fclose(f);

    return 0;
}
