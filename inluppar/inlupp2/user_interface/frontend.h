/**
 * @file frontend.h
 * @author Karl Widborg Kortelainen and Mauritz Hamrin Sverredal
 * @date 2023-11-06
 * @brief Operations related the user interface of the database
 */

#include "../business_logic/database.h"

/// @brief When called with a databse_t* it will start
/// the user interface.
/// @param db database_t*. Callee has responsibility to free.
void event_loop(database_t *db);
