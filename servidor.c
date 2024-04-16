// servidor.c
#include <stdio.h>
#include "keyvalue.h"

#define MAX_STORE 100

// Define a basic structure for storing key-value pairs
struct keyvalue_store {
    int key;
    char value1[256];
    int N_value2;
    double V_value2[32];
    int used;
};

struct keyvalue_store datastore[MAX_STORE];

// Initialize the datastore
void init_datastore() {
    for (int i = 0; i < MAX_STORE; i++) {
        datastore[i].used = 0;
    }
}

// Function implementations
int *kv_init_1_svc(void *argp, struct svc_req *rqstp) {
    static int result = 0;
    init_datastore();
    result = 0;  // Success
    return &result;
}

int *kv_set_1_svc(keyvalue_set_args *argp, struct svc_req *rqstp) {
    static int result = 0;
    for (int i = 0; i < MAX_STORE; i++) {
        if (!datastore[i].used) {
            datastore[i].used = 1;
            datastore[i].key = argp->key;
            strcpy(datastore[i].value1, argp->value1);
            datastore[i].N_value2 = argp->N_value2;
            memcpy(datastore[i].V_value2, argp->V_value2, sizeof(double) * argp->N_value2);
            result = 0;  // Success
            return &result;
        }
    }
    result = -1; // Fail if no space
    return &result;
}

keyvalue_get_res *kv_get_1_svc(keyvalue_get_args *argp, struct svc_req *rqstp) {
    static keyvalue_get_res result;
    for (int i = 0; i < MAX_STORE; i++) {
        if (datastore[i].used && datastore[i].key == argp->key) {
            result.status = 0; // Success
            strcpy(result.value1, datastore[i].value1);
            result.N_value2 = datastore[i].N_value2;
            memcpy(result.V_value2, datastore[i].V_value2, sizeof(double) * datastore[i].N_value2);
            return &result;
        }
    }
    result.status = -1; // Key not found
    return &result;
}

int *kv_modify_1_svc(keyvalue_set_args *argp, struct svc_req *rqstp) {
    static int result = -1;
    for (int i = 0; i < MAX_STORE; i++) {
        if (datastore[i].used && datastore[i].key == argp->key) {
            strcpy(datastore[i].value1, argp->value1);
            datastore[i].N_value2 = argp->N_value2;
            memcpy(datastore[i].V_value2, argp->V_value2, sizeof(double) * argp->N_value2);
            result = 0; // Success
            return &result;
        }
    }
    return &result; // Key not found or not modified
}

int *kv_delete_1_svc(int *argp, struct svc_req *rqstp) {
    static int result = -1;
    for (int i = 0; i < MAX_STORE; i++) {
        if (datastore[i].used && datastore[i].key == *argp) {
            datastore[i].used = 0; // Mark as unused
            result = 0; // Success
            return &result;
        }
    }
    return &result; // Key not found
}

int *kv_exist_1_svc(int *argp, struct svc_req *rqstp) {
    static int result = 0;
    for (int i = 0; i < MAX_STORE; i++) {
        if (datastore[i].used && datastore[i].key == *argp) {
            result = 1; // Key exists
            return &result;
        }
    }
    result = 0; // Key does not exist
    return &result;
}

int main() {
    register SVCXPRT *transp;

    pmap_unset(KEYVALUE_PROG, KEYVALUE_VERS);

    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
        fprintf(stderr, "Cannot create TCP service.\n");
        exit(1);
    }

    if (!svc_register(transp, KEYVALUE_PROG, KEYVALUE_VERS, keyvalue_prog_1, IPPROTO_TCP)) {
        fprintf(stderr, "Unable to register (KEYVALUE_PROG, KEYVALUE_VERS, tcp).\n");
        exit(1);
    }

    svc_run();
    fprintf(stderr, "svc_run returned\n");
    exit(1);
}
