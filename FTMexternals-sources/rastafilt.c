//  This file is part of the SFA-MaxLib.
//
//  The SFA-MaxLib is a collection of Max/MSP objects including FTM externals,
//  FTM abstractions and MAX abstractions.
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
//  rastafilt.c
//  ftmext-examples
//
//  Created by Stefano Fasciani on 13/7/12.
//  Copyright (c) 2012 stefanofasciani@stefanofasciani.com. All rights reserved.
//
//  This file is part of the porting to Max of the function rastaplp.m from
//  Daniel P. W. Ellis "PLP and RASTA (and MFCC, and inversion) in Matlab
//  http://www.ee.columbia.edu/~dpwe/resources/matlab/rastamat/
//  which is based on:
//  H. Hermansky, "Perceptual linear predictive (PLP) analysis of speech", J. Acoust. Soc. Am., vol. 87, no. 4, pp. 1738-1752, Apr. 1990.
//  H. Hermansky and N. Morgan, "RASTA processing of speech", IEEE Trans. on Speech and Audio Proc., vol. 2, no. 4, pp. 578-589, Oct. 1994.



#include "ftmlib.h"


struct fhistory{
    float **filtIN;
    float **filtOUT;
    int eos;
};

typedef struct
{
    ftmext_t o; /* mandatory FTM external "super class" */
    int nfilt;
    float numer[5];
    float denom;
    float **filtIN;
    float **filtOUT;
    float *output;
    int eos;
    int i_call;
    fts_object_t *obj;
    fts_atom_t val;
} rastafilt_t;

int rasta_filt(rastafilt_t *self, float *nl_audspec);

/****************************************************
 *
 *  set methods
 *
 */


FTMEXT_METHOD_NUMBER(rastafilt, num)
{
    rastafilt_t *self = (rastafilt_t *)FTMEXT_GET_EXT();
    int nfilt = FTMEXT_GET_INT();
    int i;
    
    if(nfilt <= 1){
        ftmext_error((ftmext_t *)self,"NFILT %d MUST BE BIGGER THAN 1",nfilt);
    }
    else{
        
        /*clean using old filt number*/
        if(self->nfilt != -1){
            for(i=0;i<self->nfilt;i++){
                free(self->filtIN[i]);
                free(self->filtOUT[i]);
            }
            free(self->filtIN);
            free(self->filtOUT);
            free(self->output);
        }
        
        self->nfilt = nfilt;
        
        self->filtIN=(float **)malloc(self->nfilt*sizeof(float *));
        self->filtOUT=(float **)malloc(self->nfilt*sizeof(float *));
        
        for(i=0;i<self->nfilt;i++){
            self->filtIN[i]=(float *)malloc(5*sizeof(float));
            memset(self->filtIN[i],0,5*sizeof(float));
            self->filtOUT[i]=(float *)malloc(5*sizeof(float));
            memset(self->filtOUT[i],0,5*sizeof(float));
        }
        
        self->output=(float *)malloc(self->nfilt*sizeof(float));
        
        self->eos = 1;
        rasta_filt(self,(float *)NULL);
        self->eos = 0;
    }
    
    
    FTMEXT_METHOD_RETURN;
}

FTMEXT_METHOD_OBJECT(rastafilt, NULL, obj)
{
    rastafilt_t *self = (rastafilt_t *)FTMEXT_GET_EXT();
    fts_object_t *obj = FTMEXT_GET_OBJECT();
    int type;
    float * data;
    int size;
    int stride;
    
    if(obj != NULL){
        if(self->nfilt != -1){
            type=fmat_or_fvec_vector_lock((fts_object_t *)obj,&data,&size,&stride);
            if((type==0)||(type==1)){
                ftmext_error((ftmext_t *)self,"UNSUPPORTED TYPE %d",type);
            }
            else if(type==2){
                //ftmext_post((ftmext_t *)self,"FMAT TYPE - size %d - stride %d - 1st-data %f",size,stride,data[0]);
                if(((fmat_get_m((fmat_t *)obj))==1)||((fmat_get_n((fmat_t *)obj))==1)){
                    //ftmext_post((ftmext_t *)self,"GOOD FMAT #row %d #col %d",fmat_get_m((fmat_t *)obj),fmat_get_n((fmat_t *)obj));
                    if(size==(self->nfilt)){
                        memset(self->output,0,(self->nfilt)*sizeof(float));
                        rasta_filt(self,data);
                        memcpy(data,self->output,(self->nfilt)*sizeof(float));
                        
                        fmat_or_fvec_unlock((fts_object_t *)obj);
                        
                        if(self->obj != NULL) fts_object_release(self->obj);
                        
                        self->obj = obj;
                        fts_object_refer(obj);
                        
                        ftmext_outlet_object((ftmext_t *)self, 0, self->obj);
                        
                    }
                    else {
                        ftmext_error((ftmext_t *)self,"INPUT SIZE %d IS DIFFERENT THAN NFILT=%d",size,(self->nfilt));
                    }
                }
                else{
                    ftmext_error((ftmext_t *)self,"ONLY FMAT WITH 1 COL OR 1 ROW SUPPORTED");
                }
            }
            else {
                ftmext_error((ftmext_t *)self,"UNKNOWN TYPE %d",type);
            }
            

        }
        else{
            ftmext_error((ftmext_t *)self,"NFILT WAS NOT SET");
        }
    }
    
    FTMEXT_METHOD_RETURN;
}



/****************************************************
 *
 *  get methods
 *
 */
FTMEXT_METHOD_VOID(rastafilt, getnfilt)
{
    rastafilt_t *self = (rastafilt_t *)FTMEXT_GET_EXT();
    
    ftmext_outlet_number((ftmext_t *)self, 0, self->nfilt);
    
    FTMEXT_METHOD_RETURN;
}



/****************************************************
 *
 *  additional set methods
 *
 */
FTMEXT_METHOD_NUMBER(rastafilt, setnfilt)
{
    rastafilt_t *self = (rastafilt_t *)FTMEXT_GET_EXT();
    rastafilt_num((ftmext_t *)self, FTMEXT_GET_NUMBER());
	
    FTMEXT_METHOD_RETURN;
}


/****************************************************
 *
 *  inlet method
 *
 */
FTMEXT_INLET(rastafilt)
{
    rastafilt_t *self = (rastafilt_t *)FTMEXT_GET_EXT();
    rastafilt_obj((ftmext_t *)self, FTMEXT_GET_OBJECT());
    
    FTMEXT_INLET_RETURN;  
}

FTMEXT_METHOD_VOID(rastafilt, reset)
{
    rastafilt_t *self = (rastafilt_t *)FTMEXT_GET_EXT();
    
    int i;
    
    /*clean using old filt number*/
    if(self->nfilt != -1){
        for(i=0;i<self->nfilt;i++){
            free(self->filtIN[i]);
            free(self->filtOUT[i]);
        }
        free(self->filtIN);
        free(self->filtOUT);
        free(self->output);
    
    
    self->filtIN=(float **)malloc(self->nfilt*sizeof(float *));
    self->filtOUT=(float **)malloc(self->nfilt*sizeof(float *));
    
    for(i=0;i<self->nfilt;i++){
        self->filtIN[i]=(float *)malloc(5*sizeof(float));
        memset(self->filtIN[i],0,5*sizeof(float));
        self->filtOUT[i]=(float *)malloc(5*sizeof(float));
        memset(self->filtOUT[i],0,5*sizeof(float));
    }
    
    self->output=(float *)malloc(self->nfilt*sizeof(float));    
    
    
    self->eos = 1;
    rasta_filt(self,(float *)NULL);
    self->eos = 0;
    }
    
    FTMEXT_METHOD_RETURN;
}

/****************************************************
 *
 *  class
 *
 */

/* constructor */
FTMEXT_INIT(rastafilt)
{
    rastafilt_t *self = (rastafilt_t *)FTMEXT_GET_EXT();
    
    self->nfilt = -1;
    self->eos = 1;
    self->i_call = 0;
    self->denom = 0.94f;
    self->numer[0] = 0.2f;
    self->numer[1] = 0.1f;
    self->numer[2] = 0.0f;
    self->numer[3] = -0.1f;
    self->numer[4] = -0.2f;
    self->output=NULL;
    
    fts_set_int(&self->val, 0);
    
    self->obj = fts_object_create(fvec_class, 1, &self->val);
    fts_object_refer(self->obj);
    
    FTMEXT_INIT_RETURN;
}

/* deconstructor */
FTMEXT_DELETE(rastafilt)
{
    rastafilt_t *self = (rastafilt_t *)FTMEXT_GET_EXT();
    int i;
    
    if(self->nfilt != -1){
        for(i=0;i<self->nfilt;i++){
            free(self->filtIN[i]);
            free(self->filtOUT[i]);
        }
        free(self->filtIN);
        free(self->filtOUT);
        free(self->output);
    }
    
    fts_object_release(self->obj);
    fts_atom_release(&self->val);
    
    FTMEXT_DELETE_RETURN;
}

/* class definition */
FTMEXT_CLASS(sfa, rastafilt, rastafilt_t)
{
    FTMEXT_CLASS_DESCR(_prefix_ extclass, "rastafilt", "rastafilt - compute rasta filtering");
    
    FTMEXT_CLASS_MESSAGE_VOID(rastafilt, getnfilt, "output the number of filter bands");
    
    FTMEXT_CLASS_MESSAGE_NUMBER(rastafilt, setnfilt, "<nfilt: int number> - set the number of filter bands");
    
    FTMEXT_CLASS_INLET_OBJECT(rastafilt, NULL, 0, "<obj: a single row/column fmat> - input vector");
    
    FTMEXT_CLASS_MESSAGE_BANG(rastafilt, reset, "bang to reset filter");

    FTMEXT_CLASS_OUTLET_OBJECT(rastafilt, NULL, 0, "<obj: a single column fmat> - output vector");
    
    FTMEXT_CLASS_RETURN;
}



int rasta_filt(rastafilt_t *self, float *nl_audspec){
    
    int j,nfilt;
    float sum;
    int init;
    
    
    if (self->eos == 1)
    {
        self->i_call = 0;       
        return 0;
    }
    else
    {
        self->i_call=(self->i_call)+1;
        
        if (self->i_call > 5) self->i_call = 5; 
        
        for(nfilt = 0; nfilt < self->nfilt; nfilt++)
        {

            if ((self->i_call > (4))) init = 0;
            
            else init = 1;
            
            self->filtIN[nfilt][0] = nl_audspec[nfilt];
            
            sum = 0.0;
            
            if(init == 0)
            {

                for(j=0; j<5; j++)
                {
                    sum += self->numer[j] * self->filtIN[nfilt][j];
                }
                
                sum += self->denom * self->filtOUT[nfilt][0];
                
            }

            for(j=4; j>0; j--)
            {
                self->filtIN[nfilt][j] = self->filtIN[nfilt][j-1];
            }

            self->filtOUT[nfilt][0] = sum;
            
            self->output[nfilt] = sum;          

        }
        
        return 0;
    }
}

