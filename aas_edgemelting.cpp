/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "qbsp.h"
#include "botlib/aasfile.h"
#include "aas_create.h"

//===========================================================================
// try to melt the windings of the two faces
// FIXME: this is buggy
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
int AAS_MeltFaceWinding(tmp_face_t *face1, tmp_face_t *face2)
{
	int i, n;
	int splits = 0;
	winding_t *w2, *neww;
	plane_s *plane1;

#ifdef DEBUG
	if (!face1->winding) Error("face1 %d without winding", face1->num);
	if (!face2->winding) Error("face2 %d without winding", face2->num);
#endif //DEBUG
	w2 = face2->winding;
	plane1 = &mapplanes[face1->planenum];
	for (i = 0; i < w2->numpoints; i++)
	{
		if (PointOnWinding(face1->winding, plane1->normal, plane1->dist, w2->p[i], &n))
		{
			neww = AddWindingPoint(face1->winding, w2->p[i], n);
			FreeWinding(face1->winding);
			face1->winding = neww;

			splits++;
		} //end if
	} //end for
	return splits;
} //end of the function AAS_MeltFaceWinding
//===========================================================================
// melt the windings of the area faces
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
int AAS_MeltFaceWindingsOfArea(tmp_area_t *tmparea)
{
	int side1, side2, num_windingsplits = 0;
	tmp_face_t *face1, *face2;

	for (face1 = tmparea->tmpfaces; face1; face1 = face1->next[side1])
	{
		side1 = face1->frontarea != tmparea;
		for (face2 = tmparea->tmpfaces; face2; face2 = face2->next[side2])
		{
			side2 = face2->frontarea != tmparea;
			if (face1 == face2) continue;
			num_windingsplits += AAS_MeltFaceWinding(face1, face2);
		} //end for
	} //end for
	return num_windingsplits;
} //end of the function AAS_MeltFaceWindingsOfArea
//===========================================================================
// melt the windings of the faces of all areas
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
void AAS_MeltAreaFaceWindings(void)
{
	tmp_area_t *tmparea;
	int num_windingsplits = 0;

	Log_Write("AAS_MeltAreaFaceWindings\r\n");
	qprintf("%6d edges melted", num_windingsplits);
	//NOTE: first convex area (zero) is a dummy
	for (tmparea = tmpaasworld.areas; tmparea; tmparea = tmparea->l_next)
	{
		num_windingsplits += AAS_MeltFaceWindingsOfArea(tmparea);
		qprintf("\r%6d", num_windingsplits);
	} //end for
	qprintf("\n");
	Log_Write("%6d edges melted\r\n", num_windingsplits);
} //end of the function AAS_MeltAreaFaceWindings
//===========================================================================
// Global, plane-bucketed T-junction melt across all tmp_faces. Unlike
// AAS_MeltAreaFaceWindings this does not restrict pairs to the same area,
// so it catches T-junctions between faces that belong to disjoint area pairs
// (e.g. a patch-derived face between areas Y/Z sitting coplanar with a
// brush-derived face between areas A/X).
//
// Faces are bucketed by (planenum & ~1) so the two orientations of the same
// plane land in the same bucket. PointOnWinding internally rejects off-plane
// points, so faces on distinct parallel planes that happen to share a plane
// hash are a safe no-op.
//===========================================================================
#define MELT_BBOX_EPSILON 0.5

void AAS_MeltAllFaceWindings(void)
{
	tmp_face_t *f, *f1, *f2;
	int i, j, b, idx;
	int numfaces = 0;
	int maxbucketkey = 0;
	int numbuckets = 0;
	int nonempty_buckets = 0;
	int total_splits = 0;
	int *bucket_size;
	int *bucket_offset;
	int *bucket_write;
	tmp_face_t **bucket_faces;
	vec3_t *face_mins;
	vec3_t *face_maxs;
	int bcount, base, splits;

	Log_Write("AAS_MeltAllFaceWindings\r\n");

	//first pass: count live faces and find the largest plane-bucket key
	for (f = tmpaasworld.faces; f; f = f->l_next)
	{
		if (!f->winding) continue;
		if ((f->planenum & ~1) > maxbucketkey) maxbucketkey = f->planenum & ~1;
		numfaces++;
	} //end for
	if (numfaces < 2)
	{
		qprintf("%6d edges melted across %6d plane buckets\n", 0, 0);
		Log_Write("%d edges melted across %d plane buckets\r\n", 0, 0);
		return;
	} //end if
	numbuckets = maxbucketkey + 1;
	bucket_size = (int *) GetClearedMemory(sizeof(int) * numbuckets);
	bucket_offset = (int *) GetClearedMemory(sizeof(int) * numbuckets);
	bucket_write = (int *) GetClearedMemory(sizeof(int) * numbuckets);
	bucket_faces = (tmp_face_t **) GetClearedMemory(sizeof(tmp_face_t *) * numfaces);
	face_mins = (vec3_t *) GetClearedMemory(sizeof(vec3_t) * numfaces);
	face_maxs = (vec3_t *) GetClearedMemory(sizeof(vec3_t) * numfaces);

	//second pass: per-bucket counts
	for (f = tmpaasworld.faces; f; f = f->l_next)
	{
		if (!f->winding) continue;
		bucket_size[f->planenum & ~1]++;
	} //end for
	//build contiguous-array offsets
	{
		int offset = 0;
		for (i = 0; i < numbuckets; i++)
		{
			bucket_offset[i] = offset;
			offset += bucket_size[i];
		} //end for
	}
	//third pass: fill bucket arrays and precompute per-face bounds
	for (f = tmpaasworld.faces; f; f = f->l_next)
	{
		if (!f->winding) continue;
		b = f->planenum & ~1;
		idx = bucket_offset[b] + bucket_write[b]++;
		bucket_faces[idx] = f;
		WindingBounds(f->winding, face_mins[idx], face_maxs[idx]);
	} //end for

	qprintf("%6d edges melted", total_splits);
	//per-bucket pairwise melt
	for (b = 0; b < numbuckets; b++)
	{
		bcount = bucket_size[b];
		if (bcount < 2) continue;
		nonempty_buckets++;
		base = bucket_offset[b];
		for (i = 0; i < bcount; i++)
		{
			f1 = bucket_faces[base + i];
			if (!f1->winding) continue;
			for (j = 0; j < bcount; j++)
			{
				if (i == j) continue;
				f2 = bucket_faces[base + j];
				if (!f2->winding) continue;
				//cheap AABB early-reject (faces are coplanar, so 3D test is enough)
				if (face_mins[base + i][0] > face_maxs[base + j][0] + MELT_BBOX_EPSILON ||
					face_mins[base + i][1] > face_maxs[base + j][1] + MELT_BBOX_EPSILON ||
					face_mins[base + i][2] > face_maxs[base + j][2] + MELT_BBOX_EPSILON ||
					face_mins[base + j][0] > face_maxs[base + i][0] + MELT_BBOX_EPSILON ||
					face_mins[base + j][1] > face_maxs[base + i][1] + MELT_BBOX_EPSILON ||
					face_mins[base + j][2] > face_maxs[base + i][2] + MELT_BBOX_EPSILON)
				{
					continue;
				} //end if
				//sanity guard against pathological winding growth
				if (f1->winding->numpoints >= MAX_POINTS_ON_WINDING - 1) continue;
				splits = AAS_MeltFaceWinding(f1, f2);
				if (splits)
				{
					total_splits += splits;
					qprintf("\r%6d", total_splits);
					//winding grew: refresh bounds so early-reject stays accurate
					WindingBounds(f1->winding, face_mins[base + i], face_maxs[base + i]);
				} //end if
			} //end for
		} //end for
	} //end for
	qprintf("\n");
	Log_Write("%d edges melted across %d plane buckets\r\n", total_splits, nonempty_buckets);

	FreeMemory(bucket_size);
	FreeMemory(bucket_offset);
	FreeMemory(bucket_write);
	FreeMemory(bucket_faces);
	FreeMemory(face_mins);
	FreeMemory(face_maxs);
} //end of the function AAS_MeltAllFaceWindings

