#include "functions.h"

extern "C"{
    #include "lib/dl_iso8583.h"
    #include "lib/dl_iso8583_defs_1993.h"
    #include "lib/dl_output.h" // for 'DL_OUTPUT_Hex'
}


/*
NAN_METHOD(nothing) {
}

NAN_METHOD(aString) {
    info.GetReturnValue().Set(Nan::New("This is a thing.").ToLocalChecked());
}

NAN_METHOD(aBoolean) {
    info.GetReturnValue().Set(false);
}

NAN_METHOD(aNumber) {
    info.GetReturnValue().Set(1.75);
}

NAN_METHOD(anObject) {
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Nan::Set(obj, Nan::New("key").ToLocalChecked(), Nan::New("value").ToLocalChecked());
    info.GetReturnValue().Set(obj);
}

NAN_METHOD(anArray) {
    v8::Local<v8::Array> arr = Nan::New<v8::Array>(3);
    Nan::Set(arr, 0, Nan::New(1));
    Nan::Set(arr, 1, Nan::New(2));
    Nan::Set(arr, 2, Nan::New(3));

    info.GetReturnValue().Set(arr);
}

NAN_METHOD(callback) {
    v8::Local<v8::Function> callbackHandle = info[0].As<v8::Function>();
    Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callbackHandle, 0, 0);
}
*/
// Wrapper Impl

Nan::Persistent<v8::Function> Message::constructor;

NAN_MODULE_INIT(Message::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Message").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "pack", pack);
  Nan::SetPrototypeMethod(tpl, "unpack", unpack);
  Nan::SetPrototypeMethod(tpl, "test", test);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Message").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Message::Message(double value) : value_(value) {
}

Message::~Message() {
}

NAN_METHOD(Message::New) {
  if (info.IsConstructCall()) {
    double value = info[0]->IsUndefined() ? 0 : Nan::To<double>(info[0]).FromJust();
    Message *obj = new Message(value);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1; 
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}
 
v8::Local<v8::Array> stringToHexArray(DL_UINT8 packBuf[1000], DL_UINT16 iNumBytes){
  char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};
  v8::Local<v8::Array> result = Nan::New<v8::Array>();
  std::string str;
  int idx = 0;
  for (int i = 0; i < iNumBytes; ++i) {
    const char ch = packBuf[i];
    str = "";
    str.append(&hex[(ch  & 0xF0) >> 4], 1);
    str.append(&hex[ch & 0xF], 1);
    Nan::Set(result, idx, Nan::New(str).ToLocalChecked() );
    idx++;
  }
  return result;
};
 
NAN_METHOD(Message::pack) {
  Message* obj = Nan::ObjectWrap::Unwrap<Message>(info.This());


  DL_ISO8583_HANDLER isoHandler;
  DL_ISO8583_MSG     isoMsg;
  DL_UINT8           packBuf[1000];
  DL_UINT16          packedSize;

  /* get ISO-8583 1993 handler */
  DL_ISO8583_DEFS_1993_GetHandler(&isoHandler);

  //
  // Populate/Pack message
  //

  // initialise ISO message 
  DL_ISO8583_MSG_Init(NULL, 0, &isoMsg);

  // set ISO message fields  
  if (info[0]->IsArray()) {
      v8::Handle<v8::Array> messageFields = v8::Handle<v8::Array>::Cast(info[0]);
      for (unsigned int i = 0; i < messageFields->Length(); i++) {
        v8::Handle<v8::Array> messageField = v8::Handle<v8::Array>::Cast(messageFields->Get(i));
       
        DL_UINT16 messageFieldPosition = (DL_UINT16)messageField->Get(0)->Uint32Value();
        
        // convert it to C string
        v8::String::Utf8Value _messageFieldValue(messageField->Get(1)->ToString());
        std::string messageFieldValue = std::string(*_messageFieldValue);
        const char * c = messageFieldValue.c_str();

        //(DL_UINT16 iField, const DL_UINT8 *iDataStr)
        (void)DL_ISO8583_MSG_SetField_Str( messageFieldPosition, (DL_UINT8 *)c, &isoMsg);
      }
  }


  // output ISO message content 
  //DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);

  // pack ISO message 
  (void)DL_ISO8583_MSG_Pack(&isoHandler,&isoMsg,packBuf,&packedSize);

  // free ISO message 
  DL_ISO8583_MSG_Free(&isoMsg);

  // output packed message (in hex) 
  // DL_OUTPUT_Hex(stdout,NULL,packBuf,packedSize);

  v8::Local<v8::Array> result = stringToHexArray(packBuf, packedSize);
  info.GetReturnValue().Set(result);
}




NAN_METHOD(Message::unpack) {
  Message* obj = Nan::ObjectWrap::Unwrap<Message>(info.This());

/*
you must convert your msg from hex format to binary. like this

binMsg[0]=0x02
binMsg[1]=0x10
binMsg[2]=0x32
binMsg[3]=0x3A
binMsg[4]=0x40
untel last bites.

and use :

(void)DL_ISO8583_MSG_Unpack(&isoHandler,binMsg,strlen(msg)/2,&isoMsg);
*/

/*
    DL_ISO8583_HANDLER isoHandler;
    DL_ISO8583_MSG     isoMsg;
    DL_UINT8           packBuf[1000];
    DL_UINT16          packedSize;
    char msg[]="0210323A40010A4180103800000000000000000420050805011392120805042004225132072000001000000115604000800411 163011511463331563GBAAASDD ERRR 1300101B54391001000017654350000000000090300000268410000000300000000000000898100009431000000000000000000 000000000000000036000299";

    unsigned short iso_req_size,iso_resp_size;
    char iso_header[ISOHEADER+1];
     char iso_req_buf[10000];
     char iso_resp_buf[10000];

    // get ISO-8583 1993 handler 
    DL_ISO8583_DEFS_1993_GetHandler(&isoHandler);
    DL_ISO8583_MSG_Init(NULL,0,&isoMsg);

    // Unpack message
    DL_ISO8583_MSG_Init(NULL,0,&isoMsg);

    (void)DL_ISO8583_MSG_Unpack(&isoHandler,msg,strlen(msg),&isoMsg);
    DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);
    DL_ISO8583_MSG_Free(&isoMsg);

    return 0;
*/
  obj->value_ += 1;
  info.GetReturnValue().Set(obj->value_);
}


NAN_METHOD(Message::test) {
  Message* obj = Nan::ObjectWrap::Unwrap<Message>(info.This());

  v8::Local<v8::Array> result = Nan::New<v8::Array>();

  if (info[0]->IsArray()) {
      v8::Handle<v8::Array> messageFields = v8::Handle<v8::Array>::Cast(info[0]);
      for (unsigned int i = 0; i < messageFields->Length(); i++) {
        Nan::Set(result, i, messageFields->Get(i));
      }
  }

  info.GetReturnValue().Set(result);
}
