#include <stdbool.h>
#include <stddef.h>

#include "variables.h"
#include "String.hpp"

#include "IPAddress.h"
#include "wifi_client.h"

WiFiClient client;

bool WiFiClient::connected(void) {
	return -1;
}

bool WiFiClient::connect(IPAddress ip, int port) {
	return -1;
}

bool WiFiClient::connect(String URL, int port) {
	return -1;
}

size_t WiFiClient::write(byte data) {
	return -1;
}

size_t WiFiClient::write(char data) {
	return -1;
}

/*byte WiFiClient::write(byte data) {
	return (byte) this->write(data);
}*/

/*byte WiFiClient::write(char data) {
	return (byte) this->write(data);
}*/

byte WiFiClient::print(char data) {
	return -1;
}

byte WiFiClient::print(byte data) {
	return -1;
}

byte WiFiClient::print(int data) {
	return -1;
}

byte WiFiClient::print(long data) {
	return -1;
}

byte WiFiClient::print(String data) {
	return -1;
}

byte WiFiClient::print(char data, int BASE) {
	return -1;
} 

byte WiFiClient::print(byte data, int BASE) {
	return -1;
}

byte WiFiClient::print(int data, int BASE) {
	return -1;
}

byte WiFiClient::print(long data, int BASE) {
	return -1;
}

byte WiFiClient::print(String data, int BASE) {
	return -1;
}

byte WiFiClient::println(void) {
	return -1;
}

byte WiFiClient::println(char data) {
	return -1;
}

byte WiFiClient::println(byte data) {
	return -1;
}

byte WiFiClient::println(int data) {
	return -1;
}

byte WiFiClient::println(long data) {
	return -1;
}

byte WiFiClient::println(String data) {
	return -1;
}

byte WiFiClient::println(char data, int BASE) {
	return -1;
}

byte WiFiClient::println(byte data, int BASE) {
	return -1;
}

byte WiFiClient::println(int data, int BASE) {
	return -1;
}

byte WiFiClient::println(long data, int BASE) {
	return -1;
}

byte WiFiClient::println(String data, int BASE) {
	return -1;
}

void WiFiClient::stop(void) {

}