#include <stdio.h>

#include "user_interface/frontend.h"

int main(int argc, char *argv[])
{
    database_t *db = create_database();
    event_loop(db);
    destroy_database(db);
    return 0;
}
