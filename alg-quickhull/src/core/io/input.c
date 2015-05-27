/*------------------------------------------------------------------------------
 * File: input.c
 * Created: May 15, 2015
 * Last changed: May 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Input-funktioner för att läsa in data från användaren.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "input.h"

#include "core/common.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: GetBoolFromUser()
 * Parameters:
 *   defaultVal  Värdet som ska returneras om användaren inte svarar.
 *
 * Description:
 *   Låter användaren skriva in ja eller nej.
 *------------------------------------*/
bool GetBoolFromUser(bool defaultVal) {
    string s = GetStringFromUser();
    char   c = s[0];

    free(s);

    if (c=='\0')
        return defaultVal;

    if (c=='Y' || c=='y')
        return TRUE;

    return FALSE;
}

/*--------------------------------------
 * Function: GetIntFromUser()
 * Parameters:
 *
 * Description:
 *   Låter användaren skriva in ett heltal.
 *------------------------------------*/
int GetIntFromUser() {
    char buf[16];
    bool validInt = FALSE;

    while (!validInt) {
        validInt = TRUE;

        fgets(buf, sizeof(buf), stdin);

        int len = strlen(buf);
        for (int i = 0; i < len; i++) {
            char c = buf[i];

            if (c=='\r' || c=='\n') {
                buf[i] = '\0';
                break;
            }

            if (!isdigit(c)) {
                validInt = FALSE;
                break;
            }
        }

        if (strlen(buf) == 0)
            validInt = FALSE;

        if (!validInt)
            printf("Invalid integer. Try again: ");
    }

    return atoi(buf);
}

/*--------------------------------------
 * Function: GetStringFromUser()
 * Parameters:
 *
 * Description:
 *   Låter användaren skriva in en sträng. Glöm inte anropa free() efteråt för
 *   att förhindra minnesläckage.
 *------------------------------------*/
string GetStringFromUser() {
    char buf[1024];

    fgets(buf, sizeof(buf), stdin);

    int len = strlen(buf);
    for (int i = 0; i < len; i++) {
        char c = buf[i];
        if (c == '\r' || c == '\n') {
            buf[i] = '\0';
            break;
        }
    }

    return strdup(buf);
}
