/* $Id$
   $Log$
   Revision 1.5  1993/07/02 07:21:41  mjl
   All PDF routines now start with "pdf_".

 * Revision 1.4  1993/01/23  05:31:46  mjl
 * Added prototypes for functions that support polyline read/writes.
 *
 * Revision 1.3  1992/09/30  18:25:33  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:30  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:33:40  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*--------------------------------------------------------------------------*\

    pdf.h

    Copyright (C) 1992 by Maurice J. LeBrun

    Macros and prototypes for the PDF package.

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies. 
 
    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems. 

\*--------------------------------------------------------------------------*/

#ifndef INCLUDED_PDF
#define INCLUDED_PDF

/* Error numbers */

#define PDF_ERROR		1	/* Unknown error	*/
#define PDF_FNOPEN		2	/* File not open	*/
#define PDF_FAOPEN		3	/* File already open	*/
#define PDF_BADUN		4	/* Bad unit number	*/
#define PDF_BADNBITS		5	/* Invalid # of bits	*/
#define PDF_RDERR		6	/* Read error		*/
#define PDF_WRERR		7	/* Write error		*/
#define PDF_NOTPDF		8	/* Not a valid PDF file */

int  pdf_rd_header	(FILE *file, char *header);
int  pdf_wr_header	(FILE *file, char *header);
int  pdf_wr_1byte	(FILE *file, U_CHAR s);
int  pdf_rd_1byte	(FILE *file, U_CHAR *ps);
int  pdf_wr_2bytes	(FILE *file, U_SHORT s);
int  pdf_rd_2bytes 	(FILE *file, U_SHORT *ps);
int  pdf_wr_2nbytes	(FILE *file, U_SHORT *s, PLINT n);
int  pdf_rd_2nbytes 	(FILE *file, U_SHORT *s, PLINT n);
int  pdf_wr_4bytes	(FILE *file, U_LONG s);
int  pdf_rd_4bytes 	(FILE *file, U_LONG *ps);
int  pdf_wr_ieeef	(FILE *file, float f);
int  pdf_rd_ieeef	(FILE *file, float *pf);

#endif	/* INCLUDED_PDF */
