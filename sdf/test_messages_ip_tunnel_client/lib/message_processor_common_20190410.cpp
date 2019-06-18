#include "../include/message_processor_common_20190410.h"

namespace MessageProcessors {

	t_message_type getMessageType(const t_message& msg) {
		if (msg.messageType[0] == t_message_type::BasisReconciliationMsg) return t_message_type::BasisReconciliationMsg;
		if (msg.messageType[0] == t_message_type::ErrorCorrectionMsg) return t_message_type::ErrorCorrectionMsg;
		if (msg.messageType[0] == t_message_type::ErrorCorrectionPermutationsMsg) return t_message_type::ErrorCorrectionPermutationsMsg;
		if (msg.messageType[0] == t_message_type::PrivacyAmplificationSeedsMsg) return t_message_type::PrivacyAmplificationSeedsMsg;
		if (msg.messageType[0] == t_message_type::ParameterEstimationSeedAndBitsMsg) return t_message_type::ParameterEstimationSeedAndBitsMsg;
		if (msg.messageType[0] == t_message_type::ToggleBERChangeMsg) return t_message_type::ToggleBERChangeMsg;
		return (t_message_type)0;
	}


	t_message_data getMessageData(const t_message & msg, t_message_data_length dataLength) {
		t_message_type mType = getMessageType(msg);
		string mDataString = msg.messageData;
		vector <int> mDataVector;
		string num{ "" };

		if (t_message_type::ErrorCorrectionPermutationsMsg == mType || t_message_type::PrivacyAmplificationSeedsMsg == mType) {
			for (int k = 0; k < dataLength; k++) {
				string data;
				data = mDataString.at(k);
				if (data == "#") {
					mDataVector.push_back(stoi(num));
					num = "";
				}
				else {
					num.append(data);
				}
			}
		}
		else if (t_message_type::ParameterEstimationSeedAndBitsMsg == mType) {
			bool gotSeed = false;

			for (int k = 0; k < dataLength; k++) {
				string data;
				data = mDataString.at(k);
				if (!gotSeed) {
					if (data == "#") {
						mDataVector.push_back(stoi(num));
						num = "";
						gotSeed = true;
					}
					else {
						num.append(data);
					}
				}
				else {
					char datac = mDataString.at(k);
					if (datac == '1') {
						mDataVector.push_back(1);
					}
					else if (datac == '0') {
						mDataVector.push_back(0);
					}
				}
			}
		}
		else {
			for (int k = 0; k < dataLength; k++) {
				char data = mDataString.at(k);

				if (data == '1') {
					mDataVector.push_back(1);
				}
				else if (data == '0') {
					mDataVector.push_back(0);
				}
				else if (data == '2') {
					mDataVector.push_back(2);
				}
				else if (data == '3') {
					mDataVector.push_back(3);
				}
				else if (data == '4') {
					mDataVector.push_back(3);
				}
				else if (data == '5') {
					mDataVector.push_back(5);
				}
				else if (data == '-') {
					char aux_data = mDataString.at((unsigned _int64)k + 1);
					if (aux_data == '2') {
						mDataVector.push_back(-2);
						k++;
					}
					else if (aux_data == '1') {
						mDataVector.push_back(-1);
						k++;
					}
				}

			}
		}
		return mDataVector;
	}

	t_message_data_length getMessageDataLength(const t_message & msg) {

		if ((msg.messageDataLength).size() > 0)
			return stoi(msg.messageDataLength);
		else
			return 0;
	}

	bool sendToMsgProc(vector <t_integer>& data, Signal& signalToMsg, bool& started) {
		int space = signalToMsg.space();
		if (space <= 0) return false;

		if (!started) signalToMsg.bufferPut((t_real)data.size());

		if (space <= 0) return false;
		started = true;

		int ready = (int)data.size();
		space = signalToMsg.space();
		int process = min(ready, space);

		for (auto k = 0; k < process; ++k) {
			signalToMsg.bufferPut((t_real)data[k]);
		}
		data.erase(data.begin(), data.begin() + process);

		if (data.empty()) {
			started = false;
			return true;
		}
		else {
			return false;
		}
	}

	bool recvFromMsgProc(vector <t_integer>& data, Signal& signalToMsg, bool& started, int &numberofValuesToRecv) {
		t_integer ready = signalToMsg.ready();

		t_integer startIdx{ 0 };
		t_real dataIn;
		bool finishedReceiving{ false };
		t_real nValues;
		if (!started) {
			signalToMsg.bufferGet(&nValues);
			numberofValuesToRecv = (int)nValues;
			started = true;
			finishedReceiving = false;
			startIdx = 1;
		}

		for (auto k = startIdx; k < ready; ++k) {
			signalToMsg.bufferGet(&dataIn);
			data.push_back((int)dataIn);
		}
		
		if ((int)data.size() == numberofValuesToRecv) {
			finishedReceiving = true;
			started = false;
		}
		
		return finishedReceiving;
	}

}