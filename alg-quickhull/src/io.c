//------------------------------------------------
// INCLUDES
//------------------------------------------------

#include "common.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------
// FUNCTIONS
//------------------------------------------------

/*--------------------------------------
 * Function: GetIntFromUser()
 * Parameters:
 *
 * Description:
 *   Låter användaren skriva in ett heltal.
 *------------------------------------*/
int GetIntFromUser() {
    char buf[16];
    bool isValidInt = FALSE;

    while (!isValidInt) {
        fgets(buf, sizeof(buf), stdin);

        int len = strlen(buf);

        isValidInt = TRUE;
        for (int i = 0; i < len; i++) {
            char c = buf[i];

            if (c=='\r' || c=='\n') {
                buf[i] = '\0';
                break;
            }

            if (!isdigit(c)) {
                isValidInt = FALSE;
                break;
            }
        }

        if (strlen(buf) == 0)
            isValidInt = FALSE;

        if (!isValidInt)
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
            buf[i] = 0;
            break;
        }
    }

    return strdup(buf);
}