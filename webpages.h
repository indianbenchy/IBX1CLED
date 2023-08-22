#ifndef WEBPAGES_H
#define WEBPAGES_H

const char* setuppagehtml = "\
<head>\
    <style>\
        body {\
            background-color: #f1f1f1;\
            font-family: Arial, sans-serif;\
        }\
        .controller {\
            display: flex;\
            justify-content: center;\
            align-items: center;\
            height: 100vh;\
        }\
        .form-controller {\
            background-color: white;\
            padding: 20px;\
            border-radius: 10px;\
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.3);\
            width: 50%;\
        }\
        .center {\
              margin: auto;\
              width: 100%;\
              padding: 10px;\
              text-align: center;\
            }\
        label {\
            display: block;\
            width: 100%;\
            margin-bottom: 5px;\
            font-weight: bold;\
            font-size: 16px;\
            color: #333;\
        }\
        input[type='text'] {\
            width: 100%;\
            padding: 10px;\
            border-radius: 5px;\
            border: 1px solid #ccc;\
            margin-bottom: 20px;\
            font-size: 16px;\
        }\
        input[type='submit'] {\
            background-color: #4CAF50;\
            color: white;\
            padding: 10px;\
            border-radius: 5px;\
            border: none;\
            cursor: pointer;\
            margin-top: 20px;\
            font-size: 16px;\
            font-weight: bold;\
        }\
        form {\
            text-align: left;\
        }\
    </style>\
</head>\
<body>\
    <div class='controller'>\
            <div class='form-controller'>\
                <h1 class='center'>BambuLab LED Controller Setup Page</h1>\
                <p class='center'> Configure your Bambulab LED controller to connect it to the MQTT of your Bambu Lab printer.</p>\
                <form method='POST' action='/setupmqtt'>\
                    <label>IP:</label>\
                    <input type='text' name='ip' title='Enter the IP address of your Printer' value='ipinputvalue'><br>\
                    <label>Access Code:</label>\
                    <input type='text' name='code' title='Enter the access code for your Printer' value='codeinputvalue'><br>\
                    <label>Serial ID:</label>\
                    <input type='text' name='id' title='Enter the serial ID of your Printer' value='idinputvalue'><br>\
                    <input  class='center' type='submit' value='Save'>\
                </form>\
            </div>\
        </div>\
</body>";
const char* finishedpagehtml = "\
<html>\
<head>\
	<title>BambuLab LED Controller</title>\
	<style>\
		body {\
			background-color: #f1f1f1;\
			font-family: Arial, Helvetica, sans-serif;\
		}\
		.controller {\
			background-color: #ffffff;\
			border-radius: 10px;\
			box-shadow: 0px 0px 10px 5px rgba(0,0,0,0.2);\
			padding: 20px;\
			margin-top: 50px;\
			margin-left: auto;\
			margin-right: auto;\
			max-width: 500px;\
			text-align: center;\
		}\
		h1 {\
			font-size: 36px;\
			margin-bottom: 20px;\
		}\
		p {\
			font-size: 18px;\
			margin-bottom: 20px;\
		}\
		button {\
			background-color: #4CAF50;\
			color: white;\
			padding: 10px;\
			border-radius: 5px;\
			border: none;\
			cursor: pointer;\
			margin-top: 20px;\
			font-size: 18px;\
		}\
		button:hover {\
			background-color: #3e8e41;\
		}\
	</style>\
</head>\
<body>\
	<div class='controller'>\
		<h1>Configuration Saved Successfully</h1>\
		<p>Your Configurations have been saved. You can now close this window.</p>\
	</div>\
</body>\
</html>";
#endif