#include "pcap_handler.h"
#include "http.h"
#include <radiotap_iter.h>
#include <string>

using namespace std;

string mac2string(unsigned char mac[6]) {
  char mac_c_str[18];
  sprintf(mac_c_str, "%02X:%02X:%02X:%02X:%02X:%02X\0", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return string{mac_c_str};
}

/*
 * \function process_pkts PCAP callback function
 * \param user a pointer (to be cast) to a RSSILog variable
 * \param pkt a pointer to a rtap header
 * \param bytes a pointer to the captured packet, starting by the radiotap header
 */
void process_pkts(u_char* user, const struct pcap_pkthdr *pkt, const u_char *bytes) {
 
  //change the captured packet into the radio tap header structure
  auto radiotap_head = (struct ieee80211_radiotap_header *) bytes;
  auto user_data = ( pcap_handler_user_data *) user;
  
  //check if it is a radiotap head
  if (radiotap_head -> it_version == 0){
  	struct ieee80211_radiotap_iterator iter;
  	//it turns to an int if there is error in initialization
  	int error = ieee80211_radiotap_iterator_init(&iter, radiotap_head, pkt->caplen, nullptr);
		if(!error){
			//the bytes packet after the radioTap Header
			auto wifi_head = (struct ieee80211_header *) (bytes + iter._max_length);
			if (wifi_head->frame_control == 0x0080){
				RSSISample sample1;
				while(ieee80211_radiotap_iterator\next( &iter ) == 0){
					//the antenna signal
					if(iter.this_arg_index == IEEE80211_RADIOTAP_DBM_ANTSIGNAL){
						if(sample.mac_address !=""){
							user_data->sample1.push_back(sample);
						}
						sample = RSSISample();
						sample.antenna = -1;
						sample.mac_address = mac2string(wifi_head->address2);
						sample.rssi =((int) *(iter.this_arg) - 256);
						sample.ts = (struct timeval) pkts->ts;
					}
					if (iter.this_arg_index == IEE80211_RADIOTAP_ANTENNA)sample.antenna = ((int) *(iter.this_arg));
					else sample.antenna = -1;				
				}
				// for get current time
				struct timeval c_t;
				gettimeofday(&c_t,NULL);
				if((c_t.tv_sec - user_data -> sample[0].ts.tv_sec) <= 1){
					send_samples(user_data -> sample1, user_data -> ap_mac_addr);
				}
				user_data -> sample1 = vector<RSSISample>();
			}
		} 
  }
}
