/***********************************************************************/
/* Resistance calculation tool                                         */
/*                                                                     */   
/* © Evgeny Sobolev 09.02.1984 y.b Voronezh, Russia                    */
/* International Passport: 76 1375783                                  */
/* Tel.: +79003030374                                                  */
/***********************************************************************/	
	

#include "stdio.h"

typedef unsigned long uint32_t;

double pow10OfVal( const double value ) {
	static const size_t pow10ArraySize = 18;
	static const double pow10[pow10ArraySize] = { 1e-8, 1e-7, 1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9 };
	for( size_t index = 1; index != pow10ArraySize; ++index ) {
		const double curPower = pow10[index];
		if ( curPower >  value ) {
			return pow10[index - 1];
		}
	}
	return 4e9;
}

enum class E24ValueType {
	MaxValue,
	MinValue
};

double e24Value( const double value, const E24ValueType valueType ) {
	static const size_t e24ArraySize = 26;
	static const uint32_t e24xHandreed[e24ArraySize]   = {
													 91,
												  	 100, 110, 120, 130, 140, 150, 160, 180, 
												     200, 220, 240, 270, 
												     300, 360, 390, 
												     430, 470, 
												     510, 560, 
												     620, 680, 
												     750, 
												     820,
												     910,
													 1000 };
	
	const double pow10val = pow10OfVal(value);	
	const uint32_t scaledValue = (value * 100) / pow10val;
	const uint32_t threeDidgitUpValue   = scaledValue;
	const uint32_t threeDidgitDownValue = scaledValue;

	uint32_t maxValue = scaledValue;
	uint32_t minValue = scaledValue;
	
	
	
	if ( E24ValueType::MaxValue == valueType ) {
		// Finc max value
		for( int i = 0; i != e24ArraySize; ++i ) {
			if ( e24xHandreed[i] >= scaledValue ) { maxValue = e24xHandreed[i]; break; };
		}		
		return maxValue * pow10val / 100.0;
	} else if ( E24ValueType::MinValue == valueType ) {
		// Find min value
		for( int i = e24ArraySize; i != 0; --i ) {
			const int index = i-1;
			if ( e24xHandreed[index] <= scaledValue ) { minValue = e24xHandreed[index]; break; };
		}		
		return minValue * pow10val / 100.0;
	} else {
		return 1e10;
	}
	
}

void E24ResByVoltage( const double voltage, const double resistance, double accuracy ) {
	
	const double resMax = e24Value(resistance, E24ValueType::MaxValue );
	const double resMin = e24Value(resistance, E24ValueType::MinValue );
	
	const double resMaxRealMax = resMax * (1.0 + accuracy);
	const double resMaxRealMin = resMax * (1.0 - accuracy);
	
	const double resMinRealMax = resMin * (1.0 + accuracy);
	const double resMinRealMin = resMin * (1.0 - accuracy);
	
	const double IR = voltage / resistance;
	const double PR = voltage * IR;
	
	const double IRmaxMax = voltage / resMaxRealMin;
	const double PRmaxMax = voltage * IRmaxMax;
	
	const double IRmaxMin = voltage / resMaxRealMax;
	const double PRmaxMin = voltage * IRmaxMin;	
	
	const double IRminMax = voltage / resMinRealMin;
	const double PRminMax = voltage * IRminMax;
	
	const double IRminMin = voltage / resMinRealMax;
	const double PRminMin = voltage * IRminMin;

	printf("Resistor:          %0.2lf\n", resistance );
	printf("Voltage:           %0.3lf\n", voltage );
	printf("Current:           %0.6lf\n", IR );
	printf("Power dissipation: %0.3lf\n", PR );
	printf("\n");	
	printf("Resistor(E24):     %0.2lf [%0.2lf .. %0.2lf]\n", resMin, resMinRealMin, resMinRealMax );
	printf("Voltage:           %0.3lf\n", voltage );
	printf("Current:           %0.6lf .. %0.6lf\n", IRminMin, IRminMax );
	printf("Power dissipation: %0.3lf .. %0.3lf\n", PRminMin, PRminMax );
	printf("\n");
	if ( resMax != resMin ) {
		printf("Resistor(E24):     %0.2lf [%0.2lf .. %0.2lf]\n", resMax, resMaxRealMin, resMaxRealMax );
		printf("Voltage:           %0.3lf\n", voltage );
		printf("Current:           %0.6lf .. %0.6lf\n", IRmaxMin, IRmaxMax );
		printf("Power dissipation: %0.3lf .. %0.3lf\n", PRmaxMin, PRmaxMax );
		printf("\n");
	}
}

void E24ResByCurrent( const double current, const double resistance, double accuracy ) {
	
	const double resMax = e24Value(resistance, E24ValueType::MaxValue );
	const double resMin = e24Value(resistance, E24ValueType::MinValue );
	
	const double resMaxRealMax = resMax * (1.0 + accuracy);
	const double resMaxRealMin = resMax * (1.0 - accuracy);
	
	const double resMinRealMax = resMin * (1.0 + accuracy);
	const double resMinRealMin = resMin * (1.0 - accuracy);

	const double voltage = current * resistance;
	const double PR = voltage * current;
	
	
	const double URmaxMax = current * resMaxRealMin;
	const double PRmaxMax = URmaxMax * current;
	
	const double URmaxMin = current * resMaxRealMax;
	const double PRmaxMin = URmaxMin * current;

	
	
	const double URminMax = current * resMinRealMin;
	const double PRminMax = URminMax * current;
	
	const double URminMin = current * resMinRealMax;
	const double PRminMin = URminMin * current;
	

	printf("Resistor:          %0.2lf\n", resistance );
	printf("Voltage:           %0.3lf\n", voltage );
	printf("Current:           %0.6lf\n", current );
	printf("Power dissipation: %0.3lf\n", PR );
	printf("\n");	
	printf("Resistor(E24):     %0.2lf [%0.2lf .. %0.2lf]\n", resMin, resMinRealMin, resMinRealMax );
	printf("Voltage:           %0.3lf .. %0.2lf\n", URminMax, URminMin );
	printf("Current:           %0.6lf\n", current );
	printf("Power dissipation: %0.3lf .. %0.3lf\n", PRminMax, PRminMin );
	printf("\n");
	if ( resMax != resMin ) {	
		printf("Resistor(E24):     %0.2lf [%0.2lf .. %0.2lf]\n", resMax, resMaxRealMin, resMaxRealMax );
		printf("Voltage:           %0.3lf .. %0.2lf\n", URmaxMax, URmaxMin );
		printf("Current:           %0.6lf\n", current );
		printf("Power dissipation: %0.3lf .. %0.3lf\n", PRmaxMax, PRmaxMin );
		printf("\n");
	}
}





int main( const int argc, const char* argv[] ) {
	
	double resistance = 0;
	double voltage = 0;
	double current = 0;
	double power = 0;
	
	int useU = 0;
	int useI = 0;
	int useR = 0;
	int useP = 0;

	
	if ( argc == 3 ) {
	
		useU = sscanf( argv[1], "U=%lf", &voltage );
		useI = sscanf( argv[1], "I=%lf", &current );
		useR = sscanf( argv[1], "R=%lf", &resistance );
		useP = sscanf( argv[1], "P=%lf", &power );

		if ( 0 == useU ) useU = sscanf( argv[2], "U=%lf", &voltage );
		if ( 0 == useI ) useI = sscanf( argv[2], "I=%lf", &current );
		if ( 0 == useR ) useR = sscanf( argv[2], "R=%lf", &resistance );
		if ( 0 == useP ) useP = sscanf( argv[2], "P=%lf", &power );
		
		if ( 0 != useP) {
			if ( 0 != useI ) {			
				useU = 1;
				voltage = power / current;
			} else if ( 0 != useU ) {
				useI = 1;
				current = power / voltage;
			}
		}
		
		// printf("U=%lf, I=%lf, R=%lf\n", voltage, current, resistance );

		if ( ( 0 != useU ) && ( 0 != useR ) ) {
			// Use voltage and resistance
			printf("**************************\n");
			printf("*** Fixed Voltage Mode ***\n");
			printf("**************************\n");
			E24ResByVoltage( voltage, resistance, 0.05 );		
		} else  if ( ( 0 != useI ) && ( 0 != useR ) ) {
			// Use current and resistance
			printf("**************************\n");
			printf("*** Fixed current Mode ***\n");
			printf("**************************\n");
			E24ResByCurrent( current, resistance, 0.05 );
		} else if ( ( 0 != useI ) && ( 0 != useU ) ) {
			// Use voltage and current
			resistance = voltage / current;
			printf("**************************\n");
			printf("*** Fixed Voltage Mode ***\n");
			printf("**************************\n");
			E24ResByVoltage( voltage, resistance, 0.05 );		
			printf("**************************\n");
			printf("*** Fixed Current Mode ***\n");
			printf("**************************\n");
			E24ResByCurrent( current, resistance, 0.05 );
		}
	
	} else {
		printf("E24 Resistance calculation tool\n");
		printf("Usage:\n"); 
		printf("1. ohm U=[Voltage, Volt]  R=[Resistance,  Ohm]\n");
		printf("2. ohm I=[Current, Amper] R=[Resistance, Ohm]\n");
		printf("3. ohm U=[Voltage, Volt]  I=[Current, Amper]\n");
		printf("4. ohm I=[Current, Amper] P=[Power, Watt]\n");
		printf("5. ohm U=[Voltage, Volt]  P=[Power, Watt]\n");
	}

}
