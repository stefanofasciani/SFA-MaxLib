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
//  eig.c
//  ftmext-examples
//
//  Created by Stefano Fasciani on 30/11/12.
//  Copyright (c) 2012 stefanofasciani@stefanofasciani.com. All rights reserved.
//

#include<math.h>
#include<complex.h>
#include <Accelerate/Accelerate.h>
#include "ftmlib.h"

typedef struct
{
    ftmext_t o; /* mandatory FTM external "super class" */;
    fts_object_t *obj;
    fts_atom_t val;
} eig_t;

void zgeTranspose( double complex *Transposed, double complex *M ,int n);

/****************************************************
 *
 *  set methods
 *
 */

FTMEXT_METHOD_OBJECT(eig, NULL, obj)
{
    eig_t *self = (eig_t *)FTMEXT_GET_EXT();
    fts_object_t *obj = FTMEXT_GET_OBJECT();
    int type;
    float * data;
    int size;
    int stride;
    int i;
    int cols;
    int rows;
    double complex * eigvalues;
    double complex * A;
    double complex * AT;
    double * auxptr;
    int N;
    char JOBVL='N';   // Do not compute Right eigenvectors
    char JOBVR='N';   // Do not compute Left eigenvectors
    double complex DUMMY[1];
    int LDVL=1;
    int LDVR=1;
    int LWORK;
    int INFO;
    double complex * WORK;
    double complex * RWORK;
    
    
    if(obj != NULL){
        
        type=fmat_or_fvec_vector_lock((fts_object_t *)obj,&data,&size,&stride);
        
        if((type!=2)||(stride!=1)){
            ftmext_error((ftmext_t *)self,"UNSUPPORTED TYPE %d or STRIDE %d",type,stride);
        }
        else{
            
            rows=fmat_get_m((fmat_t *)obj);
            cols=fmat_get_n((fmat_t *)obj);
            
            if(cols != rows){
                ftmext_error((ftmext_t *)self,"MATRIX MUST BE SQUARE - ROWS %d COLS %d",rows,cols);
            }
            else{
                
                N=rows;
                eigvalues=(double complex *)calloc(2*N,sizeof(double complex));
                A=(double complex *)calloc(N*N,sizeof(double complex));
                AT=(double complex *)calloc(N*N,sizeof(double complex));
                auxptr=(double *)A;
                LWORK=4*N;
                WORK =  (double complex *)calloc(LWORK,sizeof(double complex));
                RWORK = (double complex *)calloc(2*N,sizeof(double complex));
            
                for(i=0;i<N*N;i++){
                    auxptr[2*i]=data[i];
                    //maxext_post((ftmext_t *)self,"COPYNG DATA %f",(float)auxptr[2*i]);
                }
            
                zgeTranspose(AT,A,N);
                
                zgeev_( &JOBVL, &JOBVR, &N, AT ,  &N , eigvalues ,
                       DUMMY, &LDVL,
                       DUMMY, &LDVR,
                       WORK,
                       &LWORK, RWORK, &INFO );
                
                
                auxptr=(double *)eigvalues;
                for(i=0;i<2*N;i++){
                    data[i]=(float)auxptr[i];
                }

                
                fmat_reshape_nogrow((fmat_t *)obj,N,2);
                
                free(WORK);
                free(RWORK);
                free(AT);
                free(eigvalues);
                free(A);
            }
            
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
FTMEXT_INLET(eig)
{
    eig_t *self = (eig_t *)FTMEXT_GET_EXT();
    eig_obj((ftmext_t *)self, FTMEXT_GET_OBJECT());
    
    FTMEXT_INLET_RETURN;
}

/****************************************************
 *
 *  class
 *
 */

/* constructor */
FTMEXT_INIT(eig)
{
    eig_t *self = (eig_t *)FTMEXT_GET_EXT();
    
    fts_set_int(&self->val, 0);
    
    self->obj = fts_object_create(fvec_class, 1, &self->val);
    fts_object_refer(self->obj);
    
    FTMEXT_INIT_RETURN;
}

/* deconstructor */
FTMEXT_DELETE(eig)
{
    eig_t *self = (eig_t *)FTMEXT_GET_EXT();
    
    fts_object_release(self->obj);
    fts_atom_release(&self->val);
    
    FTMEXT_DELETE_RETURN;
}

/* class definition */
FTMEXT_CLASS(sfa, eig, eig_t)
{
    FTMEXT_CLASS_DESCR(_prefix_ extclass, "eig", "computes the eigenvalues of the input matrix");
    
    FTMEXT_CLASS_INLET_OBJECT(eig, NULL, 0, "<obj: matrix fmat> - input matrix eig computed on rows");
    
    FTMEXT_CLASS_OUTLET_OBJECT(eig, NULL, 0, "<obj: two column fmat> - output complex eigenvalues");
    
    FTMEXT_CLASS_RETURN;
}

void zgeTranspose( double complex *Transposed, double complex *M ,int n)
{
    
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<n;j++) Transposed[i+n*j] = M[i*n+j];
}


