/*
Copyright (C) 2019 Apogee Software, Ltd.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __vector_h
#define __vector_h
#ifdef __cplusplus
extern "C" {
#endif


typedef struct vect
   {
   appfloat x,y,z;
   }fpvector_t;

#define EQTOL 0.001
#define ISAPPXEQ(a,b) (fabs((a)-(b)) < EQTOL)

#define FP_VECTORS_EQUAL(v1,v2) (ISAPPXEQ((v1)->x,(v2)->x) && ISAPPXEQ((v1)->y,(v2)->y) && ISAPPXEQ((v1)->z,(v2)->z))

#define ASSIGN_VECTOR(v,nx,ny,nz) \
   {                              \
   (v).x = (nx);                    \
   (v).y = (ny);                    \
   (v).z = (nz);                    \
   }

//#define FP_2x2Det(x1,y1,x2,y2) ((x1)*(y2) - (x2)*(y1))


#define FP_DotProduct(v1,v2) \
   (((v1)->x*(v2)->x) + ((v1)->y*(v2)->y) + ((v1)->z*(v2)->z))


appfloat FP_VectorLength(fpvector_t*v);
appfloat FP_TripleProduct(fpvector_t*vec1,fpvector_t *vec2, fpvector_t *vec3);
appfloat FP_2x2Det(appfloat x1, appfloat y1, appfloat x2, appfloat y2);
void FP_VectorAdd(fpvector_t *result, fpvector_t *v1, fpvector_t*v2);
void FP_VectorSub(fpvector_t *result, fpvector_t *v1, fpvector_t*v2);
void FP_ScaleVector(fpvector_t *result, fpvector_t *v, appfloat scale);
void FP_NormalizeVector(fpvector_t *v);
void FP_DecomposeVector(fpvector_t*,fpvector_t*,appfloat *);
void FP_CrossProduct(fpvector_t *result, fpvector_t *v1, fpvector_t*v2);
fpvector_t FP_RotateVAV(fpvector_t *vector, fpvector_t* rotvector, appfloat cosang,
                        appfloat sinang);

//extern fpvector_t _IVEC,_JVEC,_KVEC;

#ifdef __cplusplus
};
#endif
#endif
