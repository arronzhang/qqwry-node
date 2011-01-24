#include "qqwry.h"

using namespace node;
using namespace v8;

#define LOC_MAX_LEN 255

int code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	iconv_t cd;
	//int rc;
	//char **pin = &inbuf;
	//char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	//if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
	if (iconv(cd, &inbuf, &inlen, &outbuf, &outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
}

int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}


Persistent<FunctionTemplate> qqwry::constructor_template;

void qqwry::Initialize (Handle<Object> target)
{

	HandleScope scope;

	Local<FunctionTemplate> t = FunctionTemplate::New(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	NODE_SET_PROTOTYPE_METHOD(t, "query", Query);
	NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
	target->Set(String::NewSymbol("qqwry"), t->GetFunction());
}

Handle<Value> qqwry::New (const Arguments& args)
{
	HandleScope scope;
	if (args.Length() == 0 || !args[0]->IsString()) {
		return ThrowException(Exception::Error(
					String::New("Must give daemonname string as argument")));
	}
	char path[1024];
	args[0]->ToString()->WriteAscii((char*) &path);
	init( path );
	return Undefined();
	//return args.This();
}	

Handle<Value> qqwry::Query (const Arguments& args)
{
	HandleScope scope;
	char ip[1024];
	char loc[LOC_MAX_LEN];
	args[0]->ToString()->WriteAscii((char*) &ip);
	query(ip, loc);
	return String::New(loc);
}	

Handle<Value> qqwry::Close (const Arguments& args)
{
	HandleScope scope;
	destroy();
	return Undefined();
}	

static uint32_t ip_atoi(const char *ip_str) {
	const char *p = ip_str;
	uint32_t parts[4], i;

	for (i=0; i<4; i++) {
		parts[i] = atoi(p);
		while (*p != '.' && *p) p++;

		/* skip dot */
		if (*p == '.') p++;
	}

	return parts[3] | parts[2] << 8 | parts[1] << 16 | parts[0] << 24;
}

qqwry_data_t data; 

int qqwry::init(char *path)
{
	if (qqwry_init(path, &data) != 0) {
		perror(path);
		return 1;
	}
	return 0;
}

int qqwry::query(char *f_ip, /* OUT */ char *loc)
{
	uint32_t ip = ip_atoi(f_ip);
	qqwry_result_t result;
	qqwry_query(data, ip, &result);
	//return snprintf(loc, LOC_MAX_LEN, "%s | %s", result.country, result.area);
	//TO UTF8
	char str[LOC_MAX_LEN];
	snprintf(str, LOC_MAX_LEN, "%s | %s", result.country, result.area);
	return g2u(str, strlen(str), loc, LOC_MAX_LEN);
}

void qqwry::destroy()
{
	qqwry_clean(data);
}

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	qqwry::Initialize(target);
}

