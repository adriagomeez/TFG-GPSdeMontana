#include "utm.h"

//coordinates to utm
void LLtoUTM(const double Lat, const double Long,
                               double &UTMNorthing, double &UTMEasting)
    {
        double a = WGS84_A;
        double eccSquared = UTM_E2;
        double k0 = UTM_K0;

        double LongOrigin;
        double eccPrimeSquared;
        double N, T, C, A, M;

        //Make sure the longitude is between -180.00 .. 179.9
        double LongTemp = (Long+180)-int((Long+180)/360)*360-180;

        double LatRad = Lat* pi / 180;
        double LongRad = LongTemp* pi / 180;
        double LongOriginRad;
        int    ZoneNumber;

        ZoneNumber = int((LongTemp + 180)/6) + 1;

        if( Lat >= 56.0 && Lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0 )
            ZoneNumber = 32;

        // Special zones for Svalbard
        if( Lat >= 72.0 && Lat < 84.0 )
        {
            if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
            else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
            else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
            else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
        }
        // +3 puts origin in middle of zone
        LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;
        LongOriginRad = LongOrigin * pi / 180;

        eccPrimeSquared = (eccSquared)/(1-eccSquared);

        N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
        T = tan(LatRad)*tan(LatRad);
        C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
        A = cos(LatRad)*(LongRad-LongOriginRad);

        M = a*((1 - eccSquared/4 - 3*eccSquared*eccSquared/64
                - 5*eccSquared*eccSquared*eccSquared/256) * LatRad
               - (3*eccSquared/8 + 3*eccSquared*eccSquared/32
                  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
               + (15*eccSquared*eccSquared/256
                  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad)
               - (35*eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));

        UTMEasting = (double)
        (k0*N*(A+(1-T+C)*A*A*A/6
               + (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
         + 500000.0);

        UTMNorthing = (double)
        (k0*(M+N*tan(LatRad)
             *(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
               + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));

        if(Lat < 0)
        {
            //10000000 meter offset for southern hemisphere
            UTMNorthing += 10000000.0;
        }
    }

//utm to coordinates
void UTMtoLL(const double UTMNorthing, const double UTMEasting,
                               const int UTMZone, double& Lat,  double& Long )
    {
        double k0 = UTM_K0;
        double a = WGS84_A;
        double eccSquared = UTM_E2;
        double eccPrimeSquared;
        double e1 = (1-sqrt(1-eccSquared))/(1+sqrt(1-eccSquared));
        double N1, T1, C1, R1, D, M;
        double LongOrigin;
        double mu, phi1Rad;
        double x, y;
        int ZoneNumber;

        x = UTMEasting - 500000.0; //remove 500,000 meter offset for longitude
        y = UTMNorthing;

        ZoneNumber = UTMZone;


        //+3 puts origin in middle of zone
        LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;
        eccPrimeSquared = (eccSquared)/(1-eccSquared);

        M = y / k0;
        mu = M/(a*(1-eccSquared/4-3*eccSquared*eccSquared/64
                   -5*eccSquared*eccSquared*eccSquared/256));

        phi1Rad = mu + ((3*e1/2-27*e1*e1*e1/32)*sin(2*mu)
                        + (21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(4*mu)
                        + (151*e1*e1*e1/96)*sin(6*mu));

        N1 = a/sqrt(1-eccSquared*sin(phi1Rad)*sin(phi1Rad));
        T1 = tan(phi1Rad)*tan(phi1Rad);
        C1 = eccPrimeSquared*cos(phi1Rad)*cos(phi1Rad);
        R1 = a*(1-eccSquared)/pow(1-eccSquared*sin(phi1Rad)*sin(phi1Rad), 1.5);
        D = x/(N1*k0);

        Lat = phi1Rad - ((N1*tan(phi1Rad)/R1)
                         *(D*D/2
                           -(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
                           +(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared
                             -3*C1*C1)*D*D*D*D*D*D/720));

        Lat = Lat * 180 / pi;

        Long = ((D-(1+2*T1+C1)*D*D*D/6
                 +(5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)
                 *D*D*D*D*D/120)
                / cos(phi1Rad));
        Long = LongOrigin + Long * 180 / pi;

    }
