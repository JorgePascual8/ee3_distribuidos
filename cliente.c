// cliente.c
#include "claves.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s server_host\n", argv[0]);
        exit(1);
    }
    CLIENT *cl = client_init(argv[1]);

    init_service(cl);
    double vals[] = {1.2, 2.3, 3.4};
    set_value(cl, 1, "test", 3, vals);
    get_value(cl, 1);

    cleanup(cl);
    return 0;
}
