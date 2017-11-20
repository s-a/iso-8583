# iso-8583
A native Node module to process ISO 8583 Financial transaction card originated messages

[![NPM Version](http://img.shields.io/npm/v/iso-8583.svg)](https://www.npmjs.org/package/iso-8583)
[![Build Status](https://travis-ci.org/s-a/iso-8583.svg)](https://travis-ci.org/s-a/iso-8583)
[![Dependency Status](https://david-dm.org/s-a/iso-8583.svg)](https://david-dm.org/s-a/iso-8583)
[![devDependency Status](https://david-dm.org/s-a/iso-8583/dev-status.svg)](https://david-dm.org/s-a/iso-8583#info=devDependencies)
[![NPM Downloads](https://img.shields.io/npm/dm/iso-8583.svg)](https://www.npmjs.org/package/iso-8583)
[![Massachusetts Institute of Technology (MIT)](https://s-a.github.io/license/img/mit.svg)](/LICENSE.md#mit)
[![Donate](http://s-a.github.io/donate/donate.svg)](http://s-a.github.io/donate/)


[ISO 8583 Financial transaction card originated messages](https://en.wikipedia.org/wiki/ISO_8583) — Interchange message specifications is the International Organization for Standardization standard for systems that exchange electronic transactions made by cardholders using payment cards. 


## Installation
```bash
npm install iso-8583
```

## API
The main namespace
```javascript
	var ISO8583 = require('iso-8583');
```

### ISO8583 Message Constructor
```javascript
ISO8583.Message()
```

### Methods of ISO8583.Message

#### ISO8583.Message().packSync()
_A native method to pack a message._ 

***Example:***
```javascript
	var msg = [
		[0, "1234"],
		[2, "1234567890123456"],
		[4, "000000005699"],
		[11, "000234"],
		[39, "004"],
		[41, "12345"],
		[42, "678901234"],
		[125, "BLAH BLAH"]
	];

	var message = new ISO8583.Message();
	var packedMessage = message.packSync(msg);
	console.log(packedMessage);
```

***Yields:***
```javascript
[
	"12", "34", "D0", "20", "00", "00", "02", "C0", "00", "00", "00", "00", "00", "00", "00", "00", "00", "08", "16", "12", "34", "56", "78", "90", "12", "34", "56", "00", "00", "00", "00", "56", "99", "00", "02", "34", "00", "04", "31", "32", "33", "34", "35", "20", "20", "20", "36", "37", "38", "39", "30", "31", "32", "33", "34", "20", "20", "20", "20", "20", "20", "00", "09", "42", "4C", "41", "48", "20", "42", "4C", "41", "48"
]
```

#### ISO8583.Message().unpackSync()
_A native method to parse and unpack a message._ 

***Example:***
```javascript
	var message = new ISO8583.Message();
	var data = "1234D020000002C000000000000000000008161234567890123456000000005699000234000431323334352020203637383930313233342020202020200009424C414820424C4148";
	var msg = new Buffer(data, "hex");
	var len = msg.length;
  	var unpackedMessage = message.unpackSync(msg, len);
  	console.log(unpackedMessage);
```

***Yields:***
```javascript
[
	[0, "1234"],
	[2, "1234567890123456"],
	[4, "000000005699"],
	[11, "000234"],
	[39, "004"],
	[41, "12345"],
	[42, "678901234"],
	[125, "BLAH BLAH"]
]
```

#### ISO8583.Message().parseSync()
_A JavaScript wrapper helper method calling ```unpackSync()```_ 

***Example:***
```javascript
	var message = new ISO8583.Message();
	var data = "1234D020000002C000000000000000000008161234567890123456000000005699000234000431323334352020203637383930313233342020202020200009424C414820424C4148";
  	var unpackedMessage = message.parseSync(data);
  	console.log(unpackedMessage);
```

***Yields:***
```javascript
[
	[0, "1234"],
	[2, "1234567890123456"],
	[4, "000000005699"],
	[11, "000234"],
	[39, "004"],
	[41, "12345"],
	[42, "678901234"],
	[125, "BLAH BLAH"]
]
```


## Usage
This section need improvements. So far please take a look at the [unit tests](/test) to learn how to use this module.

## Background
***This module is based on the efforts of [Oscar Sanderson](http://www.oscarsanderson.com).***

> ISO 8583 is a messaging standard used for payment card originated financial transactions. At present there are three different versions of the standard, as follows:
> 
> 1987 (used by Visa / MasterCard)  
> 1993 (used by Amex)  
> 2003  
> Each version is named based on the year that it was published.
> 
> Whilst ISO 8583 is an official standard, it is important to note that most implementations are derivatives and do not fully comply with the published standard, however the variations are typically minor and it is generally quite a trivial task to modify an existing ISO 8583 version handler to accommodate such variations.

*Source: http://www.oscarsanderson.com/iso-8583/*

## More details
List of ISO 8583-like specifications used by Card Payment System associations:

- AMEX / American Express Payment Services Ltd.:
  - Global Credit Authorization Guide (GCAG)
  - Global Electronic Data Capture (GEDC) Terminal Message Specification
  - Plural Interface Processing (PIP) Terminal Interface
- CUP / China UnionPay / UnionPay International:
  - CUP Online Message.
- DCI / Diners Club International Ltd.:
  - Diners Club International Service Centre (DCISC) Relay Authorisation System
  - Diners Club International Relay Xpress Authorization System.
- DFS / Discover Financial Services:
  - Authorization Interface Technical Specifications.
- JCB / JCB International:
  - J-Link Online Interface Guide.
- MasterCard International:
  - Customer Interface Specification (CIS)
  - Global Clearing Management System (GCMS) Reference
  - Integrated Product Messages (IPM) Clearing Formats
  - MasterCard Debit Switch (MDS) Online Specifications
  - Single Message System (SMS) Specifications.
- Visa International / Visa Europe:
  - BASE I Technical Specifications
  - Single Message System (SMS) ATM/POS Technical Specifications
  - Dual Message System Authorization (DMSA) Technical Specifications.
  
A huge list of different acquirers national and domestic payment services which also used ISO 8583 messages are not in this list.  
Specifications available for the members of these associations. They are confidential usually but could be found in the web if you know which specification to search.  
The services around payments industry like [iso8583.info](http://iso8583.info) providing details about the messages content with data parsing and test hosts.
