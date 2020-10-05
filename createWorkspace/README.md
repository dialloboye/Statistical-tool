

1. Setup root (your normal version)
2. To compile: make
3. To run: ./main.exe config/runAS.cfg 
you can pick up a config file inside that folder and modify it 
4. Pay attention: have to change the path written in listGaussianSignal, it's set now to my local address
5. Once finish running, have to make workspace and run asymptotic limits by: 
source runThis/runMakeAllWorkspace_SR2_Zd_XSRatio.sh
then:
source runThis/runAsymptoticsLimit_SR2_Zd_XSRatio.sh



















*********************************************************
*Don't care about the below, I have to rewrite them soon*



TODO: make xml files for total XS with all channels included

The package now can:
1. read in a list of signal root file.
2. extract channel, type of mediator particle, mass
3. Based on the root file and those information, xml files are created!
4. Should provide the list of signal root files with full path


Need to do more:
1. make workspace files ->done
2. run asymptotics ->done


Extra: integrate the process of making the signal root files from efficiency and acceptances
1. Fiducial cross section interpretation: 
-This needs efficiency and luminosity only (DONE)

2. Total cross section interpretation:
-This needs: acceptance x efficiency x luminosity
-Acceptance file needs to be interpolated, split.   
-Then it should be multiplied (at the same mass point) with the corresponding efficiency.
-The efficiency follows the same trend as has been done.
-Should change the ifstream and ofstream to read dynamic files instead of fixed files.


3. Branching ratio H->ZdZd interpretation: (need assumption on the Zd->ll branching ratio)
-This needs the total cross section limit.
-This need BR(Zd->ll) assumed values.

****************************************************************
THIS IS STILL WORK IN PROGRESS 
RANGE FROM 15->60 GeV FOR SIGNAL MASS POINTS
Luminosity = 140 fb-1
(All are defined in file include/prepareSignal.h)

MUST HAVE TWO FILE listEfficiency and listAcceptance
CAN MAKE A TEST RUN:
./main.exe 

Remember to change the path in the file listEfficiency to your own path
RUN two created bash scripts files:
1. Make workspaces by:
source runMakeAllWorkspace.sh

2. Get asymptotic limits:
source runAsymptoticsLimit.sh
********************************************************************

