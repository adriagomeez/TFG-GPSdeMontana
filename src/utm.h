#ifndef UTM_H
#define UTM_H

#define FLOAT_32

#ifdef FLOAT_64

#else
#ifdef FLOAT_32
#define FLOAT float
#define SIN sinf
#define COS cosf
#define TAN tanf
#define POW powf
#define SQRT sqrtf
#define FLOOR floorf

#endif
#endif


#include <math.h>

#define pi 3.14159265358979

#define WGS84_A		6378137.0		///< major axis
#define WGS84_B		6356752.31424518	///< minor axis
#define WGS84_F		0.0033528107		///< ellipsoid flattening
#define WGS84_E		0.0818191908		///< first eccentricity
#define WGS84_EP	0.0820944379		///< second eccentricity

    // UTM Parameters
#define UTM_K0		0.9996			///< scale factor
#define UTM_FE		500000.0		///< false easting
#define UTM_FN_N	0.0           ///< false northing, northern hemisphere
#define UTM_FN_S	10000000.0    ///< false northing, southern hemisphere
#define UTM_E2		(WGS84_E*WGS84_E)	///< e^2
#define UTM_E4		(UTM_E2*UTM_E2)		///< e^4
#define UTM_E6		(UTM_E4*UTM_E2)		///< e^6
#define UTM_EP2		(UTM_E2/(1-UTM_E2))

void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);

void UTMtoLL(const double UTMNorthing, const double UTMEasting, const int UTMZone, double& Lat,  double& Long );

#endif
