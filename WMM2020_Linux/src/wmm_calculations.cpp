/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>

//#include <QFile>
//#include <QFileInfo>

#include "GeomagnetismHeader.h"
#include "GeomagnetismLibrary.c"
#include "EGM9615.h"

/*---------------------------------------------------------------------------*/

/*
class wmmCalculations {

    private:
        MAGtype_MagneticModel * MagneticModels[1];
        char coefficientsFile;

    public:
        wmmCalculations(char filename[]){
            //QFileInfo modelPathInfo(QCoreApplication::applicationDirPath() + "../src/thirdParty/WorldMagneticModel/WMM2020_Linux/src/WMM.COF"); //will only work if compile with cmake for linux, or if installed with windows installer, in other words while developping and testing via qtcreator import the model manually
            //char WMMcoeffFilePath = (modelPathInfo.absoluteFilePath()).toStdString();
            //coefficientsFile = *WMMcoeffFilePath[];
            coefficientsFile = *filename;
        }

        void getWMMCoefficients() {
            int epochs = 1;
            if(!MAG_robustReadMagModels(&coefficientsFile, MagneticModels, epochs)) {
                std::cerr << coefficientsFile << std::endl;
                printf("\n WMM.COF not found.  Press enter to exit... \n ");
            }
        }

        MAGtype_GeoMagneticElements getMagneticModel(double lon, // longitude
                                                     double lat, // latitude
                                                     double h, // ellipsoidal height
                                                     double tYear) // decimal year
        {
            MAGtype_MagneticModel * MagneticModels[1], *TimedMagneticModel;
            MAGtype_Ellipsoid Ellip;
            MAGtype_CoordSpherical CoordSpherical;
            MAGtype_CoordGeodetic CoordGeodetic;
            MAGtype_Date UserDate;
            MAGtype_GeoMagneticElements GeoMagneticElements, Errors;
            MAGtype_Geoid Geoid;
            char ans[20], b;
            int NumTerms, Flag = 1, nMax = 0;
            // Memory allocation

            if(nMax < MagneticModels[0]->nMax) nMax = MagneticModels[0]->nMax;
            NumTerms = ((nMax + 1) * (nMax + 2) / 2);
            TimedMagneticModel = MAG_AllocateModelMemory(NumTerms); // For storing the time modified WMM Model parameters
            if(MagneticModels[0] == NULL || TimedMagneticModel == NULL)
            {
                MAG_Error(2);
            }
            MAG_SetDefaults(&Ellip, &Geoid); // Set default values and constants
            // Check for Geographic Poles

            // Set EGM96 Geoid parameters
            Geoid.GeoidHeightBuffer = GeoidHeightBuffer;
            Geoid.Geoid_Initialized = 1;
            // Set EGM96 Geoid parameters END

            // BEGINNING OF EQUIVALENT OF MAG_GetUserInput
            CoordGeodetic.phi = lat;
            CoordGeodetic.lambda = lon;
            CoordGeodetic.HeightAboveGeoid = h;
            UserDate.DecimalYear = tYear;
            // END OF EQUIVALENT OF MAG_GetUserInput

            MAG_GeodeticToSpherical(Ellip, CoordGeodetic, &CoordSpherical); // Convert from geodetic to Spherical Equations: 17-18, WMM Technical report
            MAG_TimelyModifyMagneticModel(UserDate, MagneticModels[0], TimedMagneticModel); // Time adjust the coefficients, Equation 19, WMM Technical report
            MAG_Geomag(Ellip, CoordSpherical, CoordGeodetic, TimedMagneticModel, &GeoMagneticElements); // Computes the geoMagnetic field elements and their time change
            MAG_CalculateGridVariation(CoordGeodetic, &GeoMagneticElements);
            MAG_WMMErrorCalc(GeoMagneticElements.H, &Errors);

            MAG_FreeMagneticModelMemory(TimedMagneticModel);
            MAG_FreeMagneticModelMemory(MagneticModels[0]);

            return GeoMagneticElements;
        }

};

*/

MAGtype_GeoMagneticElements getMagneticModel(double lon, // longitude
                                             double lat, // latitude
                                             double h, // ellipsoidal height
                                             double tYear, // decimal year
                                             char filename[])
            {
                MAGtype_MagneticModel * MagneticModels[1], *TimedMagneticModel;
                MAGtype_Ellipsoid Ellip;
                MAGtype_CoordSpherical CoordSpherical;
                MAGtype_CoordGeodetic CoordGeodetic;
                MAGtype_Date UserDate;
                MAGtype_GeoMagneticElements GeoMagneticElements, Errors;
                MAGtype_Geoid Geoid;
                char ans[20], b;
                int NumTerms, Flag = 1, nMax = 0;
                /* Memory allocation */

                int epochs = 1;
                if(!MAG_robustReadMagModels(filename, MagneticModels, epochs)) {
                    printf("\n WMM.COF not found.  Press enter to exit... \n ");
                }
                if(nMax < MagneticModels[0]->nMax) nMax = MagneticModels[0]->nMax;
                NumTerms = ((nMax + 1) * (nMax + 2) / 2);
                TimedMagneticModel = MAG_AllocateModelMemory(NumTerms); /* For storing the time modified WMM Model parameters */
                if(MagneticModels[0] == NULL || TimedMagneticModel == NULL)
                {
                    MAG_Error(2);
                }
                MAG_SetDefaults(&Ellip, &Geoid); /* Set default values and constants */
                /* Check for Geographic Poles */

                /* Set EGM96 Geoid parameters */
                Geoid.GeoidHeightBuffer = GeoidHeightBuffer;
                Geoid.Geoid_Initialized = 1;
                /* Set EGM96 Geoid parameters END */

                // BEGINNING OF EQUIVALENT OF MAG_GetUserInput
                CoordGeodetic.phi = lat;
                CoordGeodetic.lambda = lon;
                CoordGeodetic.HeightAboveGeoid = h;
                UserDate.DecimalYear = tYear;
                // END OF EQUIVALENT OF MAG_GetUserInput

                MAG_GeodeticToSpherical(Ellip, CoordGeodetic, &CoordSpherical); /*Convert from geodetic to Spherical Equations: 17-18, WMM Technical report*/
                MAG_TimelyModifyMagneticModel(UserDate, MagneticModels[0], TimedMagneticModel); /* Time adjust the coefficients, Equation 19, WMM Technical report */
                MAG_Geomag(Ellip, CoordSpherical, CoordGeodetic, TimedMagneticModel, &GeoMagneticElements); /* Computes the geoMagnetic field elements and their time change*/
                MAG_CalculateGridVariation(CoordGeodetic, &GeoMagneticElements);
                MAG_WMMErrorCalc(GeoMagneticElements.H, &Errors);

                MAG_FreeMagneticModelMemory(TimedMagneticModel);
                MAG_FreeMagneticModelMemory(MagneticModels[0]);

                return GeoMagneticElements;
            }
