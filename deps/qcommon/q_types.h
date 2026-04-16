/*
===========================================================================
Copyright (C) 2017 Poul Sander

This file is part of bspc source code.

bspc source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

bspc source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#ifndef Q_MERGE_H
#define Q_MERGE_H


typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

//map plane
struct plane_s
{
	vec3_t normal;
	vec_t dist;
	int type;
	int signbits;
	struct plane_s	*hash_chain;
};

#endif /* Q_MERGE_H */

