
==========
SFA-MaxLib
==========


Overview

The SFA-MaxLib is a collection of Max/MSP objects developed in the context of the VCI4DMI. It includes functions and utilities in the form of FTM externals, FTM abstractions and Max abstractions. FTM is a shared library for Max/MSP developed by IRCAM, which provides a small and simple real-time object system and a set of optimized services to be used within Max/MSP externals. FTM extends the data types exchanged between Max/MSP objects by complex data structures such as sequences, matrices, dictionaries, break point functions, tuples and whatever might seem helpful for the processing of music, sound and motion capture data. FTM facilitates the porting to Max/MSP of MATLAB like code.

Installation

The SFA-MaxLib requires FTM to be installed in Max/MSP. The abstractions works on both OSX and Windows, while the externals are built for OSX only. The source code of the externals is provided in the folder "FTMexternals-sources" and the FTM SDK and Max SDK are required to build the externals.

Copy the "SFA-MaxLib" folder and subfolders within your Max/MSP search path (/Applications/Max 6.1/Cycling '74/ default for OSX).
Move the .mxo files from the folder "FTMexernals" to the FTM externals folder (/Applications/Max 6.1/Cycling '74/ftm-and-co/externals/ for standard OSX installation).
The SFA-MaxLib is published under LGPL3 License.

List of FTM Externals

sfa.eig - eigenvalues
sfa.inputcombinations - combination generator
sfa.levinson - levinson-durbin recursion
sfa.lpc2cep - lpc to cepstra conversion
sfa.rastafilt - rasta filter
sfa.rmd - relative mean difference
sfa.roots - polynomial roots
List of Abstractions

sfa.bark.maxpat - energy of the Bark bands from time domain frame
sfa.bark2hz_vect.maxpat - Herts to Bark conversion
sfa.barkspect.maxpat - energy of the Bark bands from spectrum
sfa.ceil.maxpat - ceil function
sfa.featfluxgate.maxpat - gated distance on stream of feature vectors
sfa.fft2barkmx.maxpat - utility sub-abstraction of sfa.bark
sfa.fft2barkmxN.maxpat - utility sub-abstraction of sfa.barkspect
sfa.hynek_eq_coeff.maxpat -  hynek equalization coefficients
sfa.hz2bark.maxpat - Hertz to Bark conversion
sfa.hz2bark_vect.maxpat - Hertz to Bark conversion for vectors
sfa.hz2mel.maxpat - Hertz to Mel conversion
sfa.idft_real_coeff.maxpat - utility sub-abstraction of sfa.rasta-plp
sfa.maxminmem.maxpat - minimum and maximum of a stream of data
sfa.mfcc.maxpat - MFCC coefficients
sfa.modalphafilter.maxpat - 1st order IIR lowpass on a stream of vectors
sfa.nonlinfeqscale.maxpat - linear spectrum to Bark or Mel scale conversion
sfa.rasta-plp.maxpat - PLP and RASTA-PLP coefficients
sfa.spectmoments.maxpat - 4 spectral moments (centroid, deviation, skewness, kurtosis)
sfa.3spectmoments+flatness.maxpat - 3 spectral moments (centroid, deviation, skewness) and the spectral flatness
sfa.spectralflux.maxpat - spectral flux on stream of spectrum vectors
sfa.spectralfluxgate.maxpat - gated spectral flux on stream of spectrum vectors
sfa.std.maxpat - standard deviation
sfa.win_to_fft_size.maxpat - smaller FFT size given frame size
sfa.GCemulator.maxpat - 3D gestural controller emulator
