//  This file is part of the SFA-MaxLib.
//
//  The SFA-MaxLib is a collection of Max/MSP objects including FTM externals,
//  FTM abstractions and MAX abstractionss.
//  SFA-MaxLib Copyright (C) 2014 Stefano Fasciani, National University of Singapore
//  Inquiries: stefanofasciani@stefanofasciani.com
//  Website: www.stefanofasciani.com
//
//  The SFA-MaxLib is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  The SFA-MaxLib is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Less General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with SFA-MaxLib.  If not, see <http://www.gnu.org/licenses/>.
//
//  lpc2cep.c
//  ftmext-examples
//
//  Created by Stefano Fasciani on 5/7/12.
//  Copyright (c) 2012 stefanofasciani@stefanofasciani.com. All rights reserved.
//
//  This file is part of the porting to Max of the function rastaplp.m from
//  Daniel P. W. Ellis "PLP and RASTA (and MFCC, and inversion) in Matlab
//  http://www.ee.columbia.edu/~dpwe/resources/matlab/rastamat/
//  which is based on:
//  H. Hermansky, "Perceptual linear predictive (PLP) analysis of speech", J. Acoust. Soc. Am., vol. 87, no. 4, pp. 1738-1752, Apr. 1990.
//  H. Hermansky and N. Morgan, "RASTA processing of speech", IEEE Trans. on Speech and Audio Proc., vol. 2, no. 4, pp. 578-589, Oct. 1994.


#include "ftmlib.h"

typedef struct
{
    ftmext_t o; /* mandatory FTM external "super class" */
    int order;
    fts_object_t *obj;
    fts_atom_t val;
} lpc2cep_t;

int lpc2cep(float * a, float * c, int size_a, int size_c);

/****************************************************
 *
 *  set methods
 *
 */


FTMEXT_METHOD_NUMBER(lpc2cep, num)
{
    lpc2cep_t *self = (lpc2cep_t *)FTMEXT_GET_EXT();
    int order = FTMEXT_GET_INT();
    
    self->order = order;
    
    FTMEXT_METHOD_RETURN;
}

FTMEXT_METHOD_OBJECT(lpc2cep, NULL, obj)
{
    lpc2cep_t *self = (lpc2cep_t *)FTMEXT_GET_EXT();
    fts_object_t *obj = FTMEXT_GET_OBJECT();
    int type;
    float * data;
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
                if(size>=(self->order)){
                    
                    output=(float *)malloc((self->order)*sizeof(float));
                    memset (output,0,(self->order)*sizeof(float));
                    lpc2cep(data,output,size,self->order);
                    fmat_reshape_nogrow((fmat_t *)obj,(self->order),1);
                    memcpy (data,output,(self->order)*sizeof(float));
                    free(output);                    
                    
                }
                else {
                    ftmext_error((ftmext_t *)self,"INPUT SIZE %d IS BIGGER THAN ORDER=%d",size,(self->order));
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
        
        
        ftmext_outlet_object((ftmext_t *)self, 0, self->obj);
        
    }
    
    FTMEXT_METHOD_RETURN;
}



/****************************************************
 *
 *  get methods
 *
 */
FTMEXT_METHOD_VOID(lpc2cep, getorder)
{
    lpc2cep_t *self = (lpc2cep_t *)FTMEXT_GET_EXT();
    
    ftmext_outlet_number((ftmext_t *)self, 0, self->order);
    
    FTMEXT_METHOD_RETURN;
}



/****************************************************
 *
 *  additional set methods
 *
 */
FTMEXT_METHOD_NUMBER(lpc2cep, setorder)
{
    lpc2cep_t *self = (lpc2cep_t *)FTMEXT_GET_EXT();
    lpc2cep_num((ftmext_t *)self, FTMEXT_GET_NUMBER());
	
    FTMEXT_METHOD_RETURN;
}


/****************************************************
 *
 *  inlet method
 *
 */
FTMEXT_INLET(lpc2cep)
{
    lpc2cep_t *self = (lpc2cep_t *)FTMEXT_GET_EXT();
    lpc2cep_obj((ftmext_t *)self, FTMEXT_GET_OBJECT());
    
    FTMEXT_INLET_RETURN;  
}

/****************************************************
 *
 *  class
 *
 */

/* constructor */
FTMEXT_INIT(lpc2cep)
{
    lpc2cep_t *self = (lpc2cep_t *)FTMEXT_GET_EXT();
    
    self->order = 0;
    
    fts_set_int(&self->val, 0);
    
    self->obj = fts_object_create(fvec_class, 1, &self->val);
    fts_object_refer(self->obj);
    
    FTMEXT_INIT_RETURN;
}

/* deconstructor */
FTMEXT_DELETE(lpc2cep)
{
    lpc2cep_t *self = (lpc2cep_t *)FTMEXT_GET_EXT();
    
    fts_object_release(self->obj);
    fts_atom_release(&self->val);
    
    FTMEXT_DELETE_RETURN;
}

/* class definition */
FTMEXT_CLASS(sfa, lpc2cep, lpc2cep_t)
{
    FTMEXT_CLASS_DESCR(_prefix_ extclass, "lpc2cep", "lpc2cep - convert the LPC 'a' coefficients in each column of lpcas into frames of cepstra");
    
    FTMEXT_CLASS_MESSAGE_VOID(lpc2cep, getorder, "output the order");
    
    FTMEXT_CLASS_MESSAGE_NUMBER(lpc2cep, setorder, "<order: int number> - set the order");
    
    FTMEXT_CLASS_INLET_OBJECT(lpc2cep, NULL, 0, "<obj: a single row/column fmat> - input vector");
    
    FTMEXT_CLASS_OUTLET_OBJECT(lpc2cep, NULL, 0, "<obj: a single column fmat> - output vector");
    
    FTMEXT_CLASS_RETURN;
}



int lpc2cep(float * a, float * c, int size_a, int size_c){
    
    int i,n,p;  
    float sum;  
    
    p=size_c;
  
    c[0]=(-1.0f)*log(a[0]);
    
    for(n=1;n<=size_a;n++)a[n]=a[n]/a[0];
    
    for (n=1;n<=p;n++){ 
        sum = 0.0;  
        for(i=1;i<n;i++)sum=sum+(n-i)*a[i]*c[n-i];  
        c[n]=(-1.0f)*(a[n]+(sum/(float)n));        
    }
    return 0;
} 
