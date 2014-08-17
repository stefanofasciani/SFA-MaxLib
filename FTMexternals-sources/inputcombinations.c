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
//  inputcombinations.c
//  ftmext-examples
//
//  Created by Stefano Fasciani on 5/2/12.
//  Copyright (c) 2012 stefanofasciani@stefanofasciani.com. All rights reserved.
//

#include "ftmlib.h"

typedef struct
{
    ftmext_t o; /* mandatory FTM external "super class" */;
    fts_object_t *obj;
    fts_atom_t val;
} inputcombinations_t;

int numberofcombinations(inputcombinations_t *self,float *data, int nparam);
int computecombinationsmat(inputcombinations_t *self,float *dataout,float * datain,int nparams, int ncombs);

#define EPSILON 0.000001

/****************************************************
 *
 *  set methods
 *
 */

FTMEXT_METHOD_OBJECT(inputcombinations, NULL, obj)
{
    inputcombinations_t *self = (inputcombinations_t *)FTMEXT_GET_EXT();
    fts_object_t *obj = FTMEXT_GET_OBJECT();
    int type;
    float * data1;
    float * data2;
    int size;
    int stride;
    int cols;
    int rows;
    int ncombs;
    fmat_t * outmat;
    
    
    if(obj != NULL){
        
        type=fmat_or_fvec_vector_lock((fts_object_t *)obj,&data1,&size,&stride);
        
        if((type!=2)||(stride!=1)){
            ftmext_error((ftmext_t *)self,"UNSUPPORTED TYPE %d or STRIDE %d",type,stride);
            fmat_or_fvec_unlock((fts_object_t *)obj);
            
            if(self->obj != NULL)
                fts_object_release(self->obj);
            
            self->obj = obj;
            fts_object_refer(obj);
        }
        else{
            
            rows=fmat_get_m((fmat_t *)obj);
            cols=fmat_get_n((fmat_t *)obj);
            
            if(cols==4){
                
                
                ncombs=numberofcombinations(self,data1,rows);
                
                //maxext_post((ftmext_t *)self,"NUMBER COMBINATIONS %d",ncombs);
                
                if(ncombs>0){
                    

                    outmat=fmat_create(rows,ncombs);
                    
                    fmat_or_fvec_vector_lock((fts_object_t *)outmat,&data2,&size,&stride);
                    
                    computecombinationsmat(self,data2,data1,rows,ncombs);
                    
                    fmat_or_fvec_unlock((fts_object_t *)obj);
                    fmat_or_fvec_unlock((fts_object_t *)outmat);
                
                
                    if(self->obj != NULL)
                        fts_object_release(self->obj);
                    
                    self->obj = obj;
                    fts_object_refer(obj);
                    
                    ftmext_outlet_object((ftmext_t *)self, 0, (fts_object_t *)outmat);
            
                }
                else{
                    
                    ftmext_error((ftmext_t *)self,"NEGATIVE OR NULL NUMBER OF COMBINATIONS");
                    
                    fmat_or_fvec_unlock((fts_object_t *)obj);
                    
                    if(self->obj != NULL)
                        fts_object_release(self->obj);
                    
                    self->obj = obj;
                    fts_object_refer(obj);
                
                }
                
            }
            else{
                ftmext_error((ftmext_t *)self,"NUMBER OF COLUMNS %d MUST BE 4",cols);
                
                fmat_or_fvec_unlock((fts_object_t *)obj);
                
                if(self->obj != NULL)
                    fts_object_release(self->obj);
                
                self->obj = obj;
                fts_object_refer(obj);
                
            
            }  
        }
  
    }
    FTMEXT_METHOD_RETURN;
}



/****************************************************
 *
 *  inlet method
 *
 */
FTMEXT_INLET(inputcombinations)
{
    inputcombinations_t *self = (inputcombinations_t *)FTMEXT_GET_EXT();
    inputcombinations_obj((ftmext_t *)self, FTMEXT_GET_OBJECT());
    
    FTMEXT_INLET_RETURN;
}

/****************************************************
 *
 *  class
 *
 */

/* constructor */
FTMEXT_INIT(inputcombinations)
{
    inputcombinations_t *self = (inputcombinations_t *)FTMEXT_GET_EXT();
    
    fts_set_int(&self->val, 0);
    
    self->obj = fts_object_create(fvec_class, 1, &self->val);
    fts_object_refer(self->obj);
    
    FTMEXT_INIT_RETURN;
}

/* deconstructor */
FTMEXT_DELETE(inputcombinations)
{
    inputcombinations_t *self = (inputcombinations_t *)FTMEXT_GET_EXT();
    
    fts_object_release(self->obj);
    fts_atom_release(&self->val);
    
    FTMEXT_DELETE_RETURN;
}

/* class definition */
FTMEXT_CLASS(sfa, inputcombinations, inputcombinations_t)
{
    FTMEXT_CLASS_DESCR(_prefix_ extclass, "inputcombinations", "compute all combination if DMI input parameters");
    
    FTMEXT_CLASS_INLET_OBJECT(inputcombinations, NULL, 0, "<obj: matrix fmat> - input matrix with by row: enabled; max; min; resolution");
    
    FTMEXT_CLASS_OUTLET_OBJECT(inputcombinations, NULL, 0, "<obj: single row fmat> - fmat with all possible combinations by column");
    
    FTMEXT_CLASS_RETURN;
}

int numberofcombinations(inputcombinations_t *self ,float *data, int nparam){

    int ncombs=1;
    int i;
    float temp,ena,max,min,res;
    
    //enabled; max; min; resolution;
    for(i=0;i<nparam;i++){
        
        ena=data[(4*i)];
        max=data[(4*i)+1];
        min=data[(4*i)+2];
        res=data[(4*i)+3];
        
        if (ena==1){
            if(max<=min){
                ftmext_error((ftmext_t *)self,"MAX MUST BE BIGGEE THAN MIN");
                return -1;
            }
            if((res<=0)||(res>=1)){
                ftmext_error((ftmext_t *)self,"RES MUST BE BETWEEN 0 AND 1");
                return -1;
            }
            temp=(max-min)/res;
            temp=floor(temp+1+EPSILON);
            ncombs=ncombs*temp;
        }
    }

    return ncombs;

}


int computecombinationsmat(inputcombinations_t *self,float *dataout,float * datain,int nparams, int ncombs){
    
    int i,j,k,z,thisncomb,acccomb;
    float ena,max,min,res,temp;
    
    acccomb=1;
    
    for(i=0;i<nparams;i++){
        
        ena=datain[(4*i)];
        max=datain[(4*i)+1];
        min=datain[(4*i)+2];
        res=datain[(4*i)+3];
        
        //maxext_post((ftmext_t *)self,"ena %f ; max %f ; min %f ; res %f",ena,max,min,res);
        
        if(ena==1){
            temp=(max-min)/res;
            thisncomb=floor(temp+1+EPSILON);
            
            //maxext_post((ftmext_t *)self,"I %d ; THISCOMB %d",i,thisncomb);
            
            j=0;
            while(j<(ncombs/acccomb)){
                for(k=0;k<thisncomb;k++){ 
                    for(z=0;z<acccomb;z++){ 
                        dataout[(ncombs*i)+(j*acccomb)+(k*acccomb)+z]=min+(res*(float)k);
                        //maxext_post((ftmext_t *)self,"WRITING [%d]=%f with k %d",((ncombs*i)+(j*acccomb)+(k*acccomb)+z),min+(res*(float)k),k);
                    }
                }
                j=j+(thisncomb);
                //maxext_post((ftmext_t *)self,"UPDATE J %d",j);
            }
            acccomb=acccomb*thisncomb;
            //maxext_post((ftmext_t *)self,"UPDATE ACCCOMB %d",acccomb);
        }
        else{
            for(j=0;j<ncombs;j++){
                dataout[(ncombs*i)+j]=0.0;
            }
        }
    
    }
    return 0;
}

