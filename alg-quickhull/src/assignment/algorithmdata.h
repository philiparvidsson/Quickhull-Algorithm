/*------------------------------------------------------------------------------
 * File: algorithmdata.h
 * Created: May 21, 2015
 * Last changed: May 21, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Datatyp för att utvärdera algoritmer.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _algorithmdata_h_
#define _algorithmdata_h_

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*
 * Type: algorithmDataT
 *
 * Description:
 *   Innehåller data om en algoritms arbete.
 */
typedef struct {
    int numOps;
    int numAllocs;
    int numBytes;
} algorithmDataT;

#endif // _algorithmdata_h_
