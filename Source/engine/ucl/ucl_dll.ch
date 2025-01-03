/* ucl_dll.c -- DLL initialization of the UCL library

   This file is part of the UCL data compression library.

   Copyright (C) 1996-2004 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   The UCL library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The UCL library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the UCL library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/opensource/ucl/
 */


/***********************************************************************
// Windows 16 bit + Watcom C + DLL
************************************************************************/

#if (ACC_OS_WIN16 && ACC_CC_WATCOMC) && defined(__SW_BD)

/* don't pull in <windows.h> - we don't need it */
#if 0
int FAR PASCAL LibMain ( HANDLE hInstance, unsigned short wDataSegment,
                          unsigned short wHeapSize, char* lpszCmdLine )
#else
int __far __pascal LibMain ( int a, short b, short c, long d )
#endif
{
    ACC_UNUSED(a); ACC_UNUSED(b); ACC_UNUSED(c); ACC_UNUSED(d);
    return 1;
}

#endif


/*
vi:ts=4:et
*/
