#include "http.h"
#include <Poco/Net/Net.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTMLForm.h>
#include <map>
#include <set>
#include <cmath>

using namespace std;
using namespace Poco::Net;

void send_samples(RSSILog samples, string ap_mac_addr) {
 
	URI uri("http://127.0.0.1:5000/rssi?"); // decoupe l'adresse
	string path(uri.getPathAndQuery() + '?');

	
	int i = 0;
	double moy = 0;
	double x = 0;
	string str;
	for (RSSISample sample: samples)
	{
		str = sample.mac_address;
		x = pow(10.0,(sample.rssi/10.0));
		avg += x;
		i++;
	}

		avg = (avg/i);
		avg = 10*log10(avg);

		string str2 = "ap=" +ap_mac_addr+"," + s + "=" + to_string(avg);
		path += str2;

 
	if (path.empty()) path = "/"; 
	HTTPClientSession session(uri.getHost(), uri.getPort()); 
	HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
	HTTPResponse response;


}

