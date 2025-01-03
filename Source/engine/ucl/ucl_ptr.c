/* ucl_ptr.c -- low-level pointer constructs

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


#include "ucl_conf.h"


/***********************************************************************
//
************************************************************************/

UCL_PUBLIC(ucl_uintptr_t)
__ucl_ptr_linear(const ucl_voidp ptr)
{
    ucl_uintptr_t p;

#if (ACC_OS_DOS16 || ACC_OS_OS216 || ACC_OS_WIN16)
    p = (((ucl_uintptr_t)(ACC_FP_SEG(ptr))) << (16 - ACC_MM_AHSHIFT)) + (ACC_FP_OFF(ptr));
#else
    p = PTR_LINEAR(ptr);
#endif

    return p;
}


/***********************************************************************
//
************************************************************************/

UCL_PUBLIC(unsigned)
__ucl_align_gap(const ucl_voidp ptr, ucl_uint size)
{
    ucl_uintptr_t p, s, n;

    assert(size > 0);

    p = __ucl_ptr_linear(ptr);
    s = (ucl_uintptr_t) (size - 1);
#if 0
    assert((size & (size - 1)) == 0);
    n = ((p + s) & ~s) - p;
#else
    n = (((p + s) / size) * size) - p;
#endif

    assert((int)n >= 0);
    assert(n <= s);

    return (unsigned)n;
}



/*
vi:ts=4:et
*/
