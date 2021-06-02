#include "ReadConf.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "CAENVMElib.h"
#include <time.h>
#include <iostream>
#include <fstream>

void StartGGMPulser(long BHandle, ReadConf myconfiguration) {

    CVTimeUnits SelectedTime;
       CVIOSources SelectedStart;
       CVOutputSelect SelectedOutput;
       CVIOPolarity OutPol;
       if (myconfiguration.Polarity == 0) OutPol = cvDirect;
       if (myconfiguration.Polarity == 1) OutPol = cvInverted;

       if (myconfiguration.Output==0) SelectedOutput = cvOutput0 ;  /* Identifies the output line 0 */
       if (myconfiguration.Output==1) SelectedOutput = cvOutput1 ;  /* Identifies the output line 1 */
       if (myconfiguration.Output==2) SelectedOutput = cvOutput2 ;  /* Identifies the output line 2 */
       if (myconfiguration.Output==3) SelectedOutput = cvOutput3 ;  /* Identifies the output line 3 */
       if (myconfiguration.Output==4) SelectedOutput = cvOutput4 ;  /* Identifies the output line 4 */

       if (myconfiguration.TimeUnit==0) SelectedTime = cvUnit25ns;
       if (myconfiguration.TimeUnit==1) SelectedTime = cvUnit1600ns;
       if (myconfiguration.TimeUnit==2) SelectedTime = cvUnit410us;
       if (myconfiguration.TimeUnit==3) SelectedTime = cvUnit104ms;

       if (myconfiguration.Start == 0) SelectedStart = cvManualSW;    /* Manual (button) or software */
       if (myconfiguration.Start == 1) SelectedStart = cvInputSrc0;   /* Input line 0 */
       if (myconfiguration.Start == 2) SelectedStart = cvInputSrc1;   /* Input line 1 */
       if (myconfiguration.Start == 3) SelectedStart = cvCoincidence; /* Inputs coincidence */
       if (myconfiguration.Start == 4) SelectedStart = cvVMESignals;  /* Signals from VME bus */
       if (myconfiguration.Start == 6) SelectedStart = cvMiscSignals; /* Various internal signals */

       CVErrorCodes PulserStatus = CAENVME_SetPulserConf( BHandle,
                                   CVPulserSelect( myconfiguration.Pulser ),         // 0 or 1 for PulserA or Pulser B
                                   myconfiguration.Period,
                                   myconfiguration.Width,
                                   SelectedTime,
                                   myconfiguration.NumberOfPulses,
                                   SelectedStart,                  // CVIOSources Start,
                                   SelectedStart                   // CVIOSources Reset
                                 );
        if( PulserStatus != cvSuccess ){
            std::cerr <<" [ERR:CONTROLLER] Cannot initialize the pulser.\n " << std::endl << CAENVME_DecodeError( PulserStatus) << std::endl;
            exit( -1 );
        }

        CVErrorCodes PulserStatusOutput = CAENVME_SetOutputConf( BHandle,
                                  CVOutputSelect( SelectedOutput ),            //CVOutputSelect OutSel,
                                  OutPol,
                                  cvActiveHigh,                         //CVLEDPolarity LEDPol,
                                  cvMiscSignals
                                  );
        if ( PulserStatusOutput != cvSuccess ){
	    std::cerr <<" [ERR:CONTROLLER] Cannot initialize the output to use pulser.\n " << std::endl << CAENVME_DecodeError( PulserStatusOutput) << std::endl;
            exit( -1 );
        }    
}
void StopGGMPulser(long BHandle, ReadConf myconfiguration) {

        CVErrorCodes PulserStatus = CAENVME_StopPulser( BHandle, CVPulserSelect( myconfiguration.Pulser ) );
        if ( PulserStatus != cvSuccess ) {
            std::cerr <<" [ERR:CONTROLLER] Cannot stop the pulser.\n " << std::endl << CAENVME_DecodeError( PulserStatus) << std::endl;
            exit( -1 );
        }    
}
