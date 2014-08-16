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
//
//  rmd.c
//  ftmext-examples
//
//  Created by Stefano Fasciani on 24/7/12.
//  Copyright (c) 2012 stefanofasciani@stefanofasciani.com. All rights reserved.
//

#include "ftmlib.h"

typedef struct
{
    ftmext_t o; /* mandatory FTM external "super class" */;
    fts_object_t *obj;
    fts_atom_t val;
} rmd_t;

float relative_mean_diference(float *in,int size);

/****************************************************
 *
 *  set methods
 *
 */

FTMEXT_METHOD_OBJECT(rmd, NULL, obj)
{
    rmd_t *self = (rmd_t *)FTMEXT_GET_EXT();
    fts_object_t *obj = FTMEXT_GET_OBJECT();
    int type;
    float * data;
    int size;
    int stride;
    int i;
    int cols;
    int rows;
    float * output;
    
    
    if(obj != NULL){
        
        type=fmat_or_fvec_vector_lock((fts_object_t *)obj,&data,&size,&stride);
        
        if((type!=2)||(stride!=1)){
            ftmext_error((ftmext_t *)self,"UNSUPPORTED TYPE %d or STRIDE %d",type,stride);
        }
        else{

            rows=fmat_get_m((fmat_t *)obj);
            cols=fmat_get_n((fmat_t *)obj);
            
            output=(float *)malloc(rows*sizeof(float));
            
            for(i=0;i<rows;i++){
                output[i]=relative_mean_diference(&data[i*cols],cols);
            }
            
            fmat_reshape_nogrow((fmat_t *)obj,1,rows);
            memcpy (data,output,rows*sizeof(float));
            free(output);
                    
        }
        
        fmat_or_fvec_unlock((fts_object_t *)obj);
        
        if(self->obj != NULL)
            fts_object_release(self->obj);
        
        self->obj = obj;
        fts_object_refer(obj);
        
        ftmext_outlet_object((ftmext_t *)self, 0, self->obj);
        
    }
    
    FTMEXT_METHOD_RETURN;
}



/****************************************************
 *
 *  inlet method
 *
 */
FTMEXT_INLET(rmd)
{
    rmd_t *self = (rmd_t *)FTMEXT_GET_EXT();
    rmd_obj((ftmext_t *)self, FTMEXT_GET_OBJECT());
    
    FTMEXT_INLET_RETURN;  
}

/****************************************************
 *
 *  class
 *
 */

/* constructor */
FTMEXT_INIT(rmd)
{
    rmd_t *self = (rmd_t *)FTMEXT_GET_EXT();
    
    fts_set_int(&self->val, 0);
    
    self->obj = fts_object_create(fvec_class, 1, &self->val);
    fts_object_refer(self->obj);
    
    FTMEXT_INIT_RETURN;
}

/* deconstructor */
FTMEXT_DELETE(rmd)
{
    rmd_t *self = (rmd_t *)FTMEXT_GET_EXT();
    
    fts_object_release(self->obj);
    fts_atom_release(&self->val);
    
    FTMEXT_DELETE_RETURN;
}

/* class definition */
FTMEXT_CLASS(sfa, rmd, rmd_t)
{
    FTMEXT_CLASS_DESCR(_prefix_ extclass, "rmd", "relative mean difference");
    
    FTMEXT_CLASS_INLET_OBJECT(rmd, NULL, 0, "<obj: matrix fmat> - input matrix rmd computed on rows");
    
    FTMEXT_CLASS_OUTLET_OBJECT(rmd, NULL, 0, "<obj: single row fmat> - output rmd vector");
    
    FTMEXT_CLASS_RETURN;
}

float relative_mean_diference(float *in,int size){
    
    int i,j;
    float absm=0.0;
    float rmd=0.0;
    
    for(i=0;i<size;i++){
        absm=absm+in[i];
        for(j=0;j<size;j++){
            rmd=rmd+fabs(in[i]-in[j]);
        }
    }
    absm=fabs(absm/(float)size);
    rmd=(rmd/((float)size*(float)(size-1)*absm));
    
    return rmd;
}
