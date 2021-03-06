/*
	This sample publishes a message to an Amazon SNS target (topic or endpoint).
*/

#include "application.h"
#include "SparkCore_Arduino_AmazonSNS_Sample/SparkCore_Arduino_AmazonSNS_Sample.h"
#include "SparkCore_Arduino_AmazonSNS_Sample/SparkAWSImplementations.h"
#include "SparkCore_Arduino_AmazonSNS_Sample/AWSFoundationalTypes.h"
#include "SparkCore_Arduino_AmazonSNS_Sample/keys.h"

/* Constants for connecting to Amazon SNS. */
static const char* TARGET_ARN = "SNSTopicARN";  // replace each ':' with '%3A'
static const char* AWS_REGION = "ap-southeast-1";  // us-west-2 etc
static const char* AWS_ENDPOINT = "amazonaws.com";

/* Light the LED while a message is transmitted. */
int led = 13;

/* Device independent implementations required for AmazonSNSClient to function. */
SparkHttpClient httpClient;
SparkDateTimeProvider dateTimeProvider;

/* AWS objects. */
AmazonSNSClient snsClient;
PublishInput publishInput;
ActionError actionError;

void setup() {
    /* Begin serial communication. */
    Serial.begin(9600);
    delay(5000);

    pinMode(led, OUTPUT);

	/* Initialize Amazon SNS client. */
	snsClient.setAWSRegion(AWS_REGION);
	snsClient.setAWSEndpoint(AWS_ENDPOINT);
	snsClient.setAWSSecretKey(awsSecKey);
	snsClient.setAWSKeyID(awsKeyID);
	snsClient.setHttpClient(&httpClient);
	snsClient.setDateTimeProvider(&dateTimeProvider);
}

void loop() {
	/* Repeat SNS attempt every 60s. */
	delay(60000);

	/* Light up the LED. */
	digitalWrite(led, HIGH);
	Serial.println("Start Loop");

	MinimalString dateTime(dateTimeProvider.getDateTime());
	dateTimeProvider.sync(dateTime.getCStr());

	/* Delivery attempt to Amazon SNS. */
	publishInput.setMessage(dateTime);
	publishInput.setTargetArn(TARGET_ARN);
	PublishOutput publishOutput = snsClient.publish(publishInput, actionError);
	Serial.println(publishOutput.getMessageId().getCStr());

	/* Finish loop and turn off the LED. */
	Serial.println(dateTime.getCStr());
	Serial.println("Finished loop");
	delay(1000);
	digitalWrite(led, LOW);
}
