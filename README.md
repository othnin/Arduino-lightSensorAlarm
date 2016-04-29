Arduino with wireless bridge and a light sensor hooked up to provide an alarm in a dark environment.
Sends out an email and SMS to the recepient whemn the ldr threshold value is tripped.
I had to play around to get the correct value for the sensor. You may have to do the same.
The email and SMS functionality comes from Temboo.
Go to temboo at https//temboo.com/arduino/send-an-email
and there is another page for SMS. This will require an account with Twilio.
The easy part is that temboo will autogenerate all the code. All you need to do is provide the glue to the light
sensor code and include the header file that temboo provides.