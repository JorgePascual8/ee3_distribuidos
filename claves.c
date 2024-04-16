// claves.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keyvalue.h"

CLIENT *client_init(char *host) {
    CLIENT *clnt;
    clnt = clnt_create(host, KEYVALUE_PROG, KEYVALUE_VERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
    return clnt;
}

void init_service(CLIENT *clnt) {
    int *result;
    result = kv_init_1(NULL, clnt);
    if (*result != 0) {
        fprintf(stderr, "Error during initialization\n");
    }
}

void set_value(CLIENT *clnt, int key, const char *value1, int N_value2, double V_value2[]) {
    keyvalue_set_args args;
    args.key = key;
    strcpy(args.value1, value1);
    args.N_value2 = N_value2;
    memcpy(args.V_value2, V_value2, sizeof(double) * N_value2);
    int *result;
    result = kv_set_1(&args, clnt);
    if (*result != 0) {
        fprintf(stderr, "Error setting value\n");
    }
}

void get_value(CLIENT *clnt, int key) {
    keyvalue_get_args args;
    args.key = key;
    keyvalue_get_res *result;
    result = kv_get_1(&args, clnt);
    if (result->status == 0) {
        printf("Value1: %s\n", result->value1);
        printf("N_value2: %d\n", result->N_value2);
        for (int i = 0; i < result->N_value2; i++) {
            printf("V_value2[%d]: %f\n", i, result->V_value2[i]);
        }
    } else {
        fprintf(stderr, "Error retrieving value\n");
    }
}

void modify_value(CLIENT *clnt, int key, const char *value1, int N_value2, double V_value2[]) {
    keyvalue_set_args args;
    args.key = key;
    strcpy(args.value1, value1);
    args.N_value2 = N_value2;
    memcpy(args.V_value2, V_value2, sizeof(double) * N_value2);
    int *result;
    result = kv_modify_1(&args, clnt);
    if (*result != 0) {
        fprintf(stderr, "Error modifying value\n");
    }
}

void delete_key(CLIENT *clnt, int key) {
    int *result;
    result = kv_delete_1(&key, clnt);
    if (*result != 0) {
        fprintf(stderr, "Error deleting key\n");
    }
}

void check_existence(CLIENT *clnt, int key) {
    int *result;
    result = kv_exist_1(&key, clnt);
    if (*result == 1) {
        printf("Key %d exists.\n", key);
    } else if (*result == 0) {
        printf("Key %d does not exist.\n", key);
    } else {
        fprintf(stderr, "Error checking existence of key\n");
    }
}

void cleanup(CLIENT *clnt) {
    clnt_destroy(clnt);
}
