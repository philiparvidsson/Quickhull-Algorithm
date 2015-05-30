/*------------------------------------------------------------------------------
 * File: common.h
 * Created: May 15, 2015
 * Last changed: May 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Blandade saker som beh�vs h�r och d�r.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _common_h_
#define _common_h_

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: Authors
 *
 * Description:
 *   Programmets programmerare.
 *------------------------------------*/
#define ProgramAuthors "Philip Arvidsson (philip@philiparvidsson.com)"

/*--------------------------------------
 * Constant: Version
 *
 * Description:
 *   Programmets version.
 *------------------------------------*/
#define ProgramVersion "0.97"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: actionT
 *
 * Description:
 *   Representerar en callback-funktion med ett argument.
 *------------------------------------*/
typedef void (*actionT)(void *);

/*--------------------------------------
 * Type: bool
 *
 * Description:
 *   Boolesk typ.
 *------------------------------------*/
typedef enum {
    FALSE, TRUE
} bool;

/*--------------------------------------
 * Type: string
 *
 * Description:
 *   Str�ngtyp.
 *------------------------------------*/
typedef char *string;

#endif // _common_h_
