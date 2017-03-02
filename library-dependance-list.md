	All those library have to be downloaded and put on arduino library folder;


NitrofMtl reposytory Library:

	#include <weeklyAlarm.h>	https://github.com/NitrofMtl/weeklyAlarm
	#include <TimeOut.h>		https://github.com/NitrofMtl/TimeOut
	#include <uHTTP.h>			https://github.com/NitrofMtl/uHTTP
	#include <snippets.h>		https://github.com/NitrofMtl/snippets
	#include <RTD10K.h>			https://github.com/NitrofMtl/RTD10k-temp-sensor
	#include <IOctrl.h>			https://github.com/NitrofMtl/IOCtrl
	#include <ADC_SEQR.h>		https://github.com/NitrofMtl/ADC_SEQR


Other code library;

	#include <TimeLib.h>		https://github.com/PaulStoffregen/Time
	#include <DueTimer.h>		https://github.com/ivanseidel/DueTimer
	#include <ArduinoJson.h>	https://github.com/bblanchon/ArduinoJson
	#include <Streaming.h>		http://arduiniana.org/libraries/streaming/

Now, Arduino home thermostat run with arduino internet shield 2. But if you wnat to use shield version 1, change all include of  

	<Ethernet2.h> and <EthernetUdp2.h>   to   <Ethernet.h> and <EthernetUdp.h>,


in sketch and snippets library.