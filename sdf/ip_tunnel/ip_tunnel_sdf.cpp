# include "../../include/netxpto_20180815.h"

# include "../../include/binary_source_20180815.h"
# include "../../include/sink_20180815.h"


// #####################################################################################################
// ################################### System Input Parameters #########################################
// #####################################################################################################

int numberOfBits{ 1000 };	

int main() {

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	Binary BinarySourceOut_0{ "S0_BinarySourceOut_0.sgn" };

	Binary BinarySourceOut_1{ "S1_BinarySourceOut_0.sgn" };
	
	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################


	BinarySource BinarySource_{ {}, { &BinarySourceOut_0, &BinarySourceOut_1 } };
	BinarySource_.setBitPeriod(1e-6);
	BinarySource_.setNumberOfBits(numberOfBits);
	
	Sink Sink_0{ {&BinarySourceOut_0},{} };
	
	Sink Sink_1{ {&BinarySourceOut_1},{} };

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ 
		{ 
			&BinarySource_,
			&Sink_0,
			&Sink_1
		}
	};

	MainSystem.setLogValue(true);
	MainSystem.setSaveSignals(true);

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();
	MainSystem.terminate();

	system("pause");
	return 0;

}