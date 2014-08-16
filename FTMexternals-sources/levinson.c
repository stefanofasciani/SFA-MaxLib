// This file is part of the Voice-Controlled Interface for Digital Musical Instruments (VCI4DMI).
//
// The Voice-Controlled Interface for Digital Musical Instruments (VCI4DMI) 
// is a collection of MAX patches, MAX externals and MATLAB functions 
// implementing ad-hoc mappings to control an arbitrary number of real-valued
// instrument parameters by variation of the voice timbre.
// The VCI4DMI can be obtained at http://stefanofasciani.com/vci4dmi.html
// VCI4DMI Copyright (C) 2014 Stefano Fasciani, National University of Singapore
// Inquiries: stefanofasciani@stefanofasciani.com
//
// The VCI4DMI is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The VCI4DMI is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Less General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//
// If you use the VCI4DMI or any part of it in any program or publication,
// please acknowledge its authors by adding a reference any of these pubblications:
//
// Fasciani, S. and Wyse, L. 2012. "Adapting general purpose interfaces to synthesis
// engines using unsupervised dimensionality reduction techniques and inverse mapping
// from features to parameters". In Proceedings of the 2012 International Computer
// Music Conference. Ljubljana, Slovenia.
//
// Fasciani, S. and Wyse, L. 2013. "A Self-Organizing Gesture Map for a
// Voice-Controlled Instrument Interface. In Proceedings of the 13th conference
// on New Interfaces for Musical Expression". Daejeon, Korea.
//
// Fasciani, S. 2014. "Voice-Controlled Interface for Digital Musical Instruments".
// PhD Thesis, National University of Singapore.

//
//  levinson.c
//  ftmext-examples
//
//  Created by Stefano Fasciani on 18/6/12.
//  Copyright (c) 2012 stefanofasciani@stefanofasciani.com. All rights reserved.
//


#include "ftmlib.h"

typedef struct
{
    ftmext_t o; /* mandatory FTM external "super class" */
    int order;
    float err;
    fts_object_t *obj;
    fts_atom_t val;
} levinson_t;

int levinson_recursion( int order, float * r, float * a, float *e );

/****************************************************
 *
 *  set methods
 *
 */


FTMEXT_METHOD_NUMBER(levinson, num)
{
    levinson_t *self = (levinson_t *)FTMEXT_GET_EXT();
    int order = FTMEXT_GET_INT();
    
    self->order = order;
    
    FTMEXT_METHOD_RETURN;
}

FTMEXT_METHOD_OBJECT(levinson, NULL, obj)
{
    levinson_t *self = (levinson_t *)FTMEXT_GET_EXT();
    fts_object_t *obj = FTMEXT_GET_OBJECT();
    int type;
    float * data;
    float error;
    int size;
    int stride;
    float * output;
    
    
    if(obj != NULL)
    {
        
        type=fmat_or_fvec_vector_lock((fts_object_t *)obj,&data,&size,&stride);
        if((type==0)||(type==1)){
            ftmext_error((ftmext_t *)self,"UNSUPPORTED TYPE %d",type);
        }
        else if(type==2){
            //ftmext_post((ftmext_t *)self,"FMAT TYPE - size %d - stride %d - 1st-data %f",size,stride,data[0]);
            if(((fmat_get_m((fmat_t *)obj))==1)||((fmat_get_n((fmat_t *)obj))==1)){
                //ftmext_post((ftmext_t *)self,"GOOD FMAT #row %d #col %d",fmat_get_m((fmat_t *)obj),fmat_get_n((fmat_t *)obj));
                if(size>=(self->order+1)){
                    
                    output=(float *)malloc((self->order+1)*sizeof(float));
                    levinson_recursion(self->order,data,output,&error);
                    fmat_reshape_nogrow((fmat_t *)obj,(self->order+1),1);
                    memcpy (data,output,(self->order+1)*sizeof(float));
                    free(output);                    
                    
                }
                else {
                    ftmext_error((ftmext_t *)self,"INPUT SIZE %d IS SMALLER THAN ORDER+1=%d",size,(self->order+1));
                }
            }
            else{
                ftmext_error((ftmext_t *)self,"ONLY FMAT WITH 1 COL OR 1 ROW SUPPORTED");
            }
        }
        else {
            ftmext_error((ftmext_t *)self,"UNKNOWN TYPE %d",type);
        }
        fmat_or_fvec_unlock((fts_object_t *)obj);
        
        
        if(self->obj != NULL)
            fts_object_release(self->obj);
        
        self->obj = obj;
        fts_object_refer(obj);
        
        ftmext_outlet_number((ftmext_t *)self, 1, (double)error);    
        
        ftmext_outlet_object((ftmext_t *)self, 0, self->obj);
        
    }
    
    FTMEXT_METHOD_RETURN;
}



/****************************************************
 *
 *  get methods
 *
 */
FTMEXT_METHOD_VOID(levinson, getorder)
{
    levinson_t *self = (levinson_t *)FTMEXT_GET_EXT();
    
    ftmext_outlet_number((ftmext_t *)self, 0, self->order);
    
    FTMEXT_METHOD_RETURN;
}



/****************************************************
 *
 *  additional set methods
 *
 */
FTMEXT_METHOD_NUMBER(levinson, setorder)
{
    levinson_t *self = (levinson_t *)FTMEXT_GET_EXT();
    levinson_num((ftmext_t *)self, FTMEXT_GET_NUMBER());
	
    FTMEXT_METHOD_RETURN;
}


/****************************************************
 *
 *  inlet method
 *
 */
FTMEXT_INLET(levinson)
{
    levinson_t *self = (levinson_t *)FTMEXT_GET_EXT();
    levinson_obj((ftmext_t *)self, FTMEXT_GET_OBJECT());
    
    FTMEXT_INLET_RETURN;  
}

/****************************************************
 *
 *  class
 *
 */

/* constructor */
FTMEXT_INIT(levinson)
{
    levinson_t *self = (levinson_t *)FTMEXT_GET_EXT();
    
    self->order = 0;
    
    fts_set_int(&self->val, 0);
    
    self->obj = fts_object_create(fvec_class, 1, &self->val);
    fts_object_refer(self->obj);
    
    FTMEXT_INIT_RETURN;
}

/* deconstructor */
FTMEXT_DELETE(levinson)
{
    levinson_t *self = (levinson_t *)FTMEXT_GET_EXT();
    
    fts_object_release(self->obj);
    fts_atom_release(&self->val);
    
    FTMEXT_DELETE_RETURN;
}

/* class definition */
FTMEXT_CLASS(sfa, levinson, levinson_t)
{
    FTMEXT_CLASS_DESCR(_prefix_ extclass, "levinson", "Levinson-Durbin recursion");
    
    FTMEXT_CLASS_MESSAGE_VOID(levinson, getorder, "output the order");
    
    FTMEXT_CLASS_MESSAGE_NUMBER(levinson, setorder, "<order: int number> - set the order");
    
    FTMEXT_CLASS_INLET_OBJECT(levinson, NULL, 0, "<obj: a single row/column fmat> - input vector");
    
    FTMEXT_CLASS_OUTLET_NUMBER(levinson, 1, "<number> prediction error");
    FTMEXT_CLASS_OUTLET_OBJECT(levinson, NULL, 0, "<obj: a single column fmat> - output vector");
    
    FTMEXT_CLASS_RETURN;
}

int levinson_recursion( int order, float * r, float * a, float *e ){
    
	float s;
	float *alp = NULL;
	float *rc;
	float alpmin, rcmct, aib, aip;
	
	int idx, mct, mct2, ib, ip, i_1, i_2, mh;
    
	alp = (float *)malloc((order + 1) * sizeof(float));
	if(alp == (float *)NULL) return -1;
	rc = (float *)malloc((order ) * sizeof(float));
	if(rc == (float *)NULL) return -1;
    
    /*     solution for autoregressive model */
    
    a[0] = 1.;
    alp[0] = r[0];
    rc[0] = -(double)r[1] / r[0];
    a[1] = rc[0];
    alp[1] = r[0] + r[1] * rc[0];
    i_2 = order;
    for (mct = 2; mct <= i_2; ++mct) 
	{
        s = 0.;
        mct2 = mct + 2;
        alpmin = alp[mct - 1];
        i_1 = mct;
        for (ip = 1; ip <= i_1; ++ip) 
		{
            idx = mct2 - ip;
            s += r[idx - 1] * a[ip-1];
        }
        rcmct = -(double)s / alpmin;
        mh = mct / 2 + 1;
        i_1 = mh;
        for (ip = 2; ip <= i_1; ++ip) 
		{
            ib = mct2 - ip;
            aip = a[ip-1];
            aib = a[ib-1];
            a[ip-1] = aip + rcmct * aib;
            a[ib-1] = aib + rcmct * aip;
        }
        a[mct] = rcmct;
        alp[mct] = alpmin - alpmin * rcmct * rcmct;
        rc[mct-1] = rcmct;
    }
    
    *e = alp[order];
    free(alp);
    free(rc);
    return 0;
}

